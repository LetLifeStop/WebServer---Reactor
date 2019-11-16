// 通道
#pragma once
#include <sys/epoll.h>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include "Timer.h"

class EventLoop;
class HttpData;

//std::function 可以是新对各种可调用实体的封装，可以处理不同函数对象类型


class Channel{
  private:
    // 定义回调函数
    typedef std::function<void()> CallBack;
    EventLoop *loop_;
    int fd_;
    _uint32_t events_;
    _uint32_t revents_;
    _uint32_t   lastEvents_;

    std::weak_ptr<HttpData> holder_;

    private:
      int parse_URI();
      int parse_Headers();
      int analysisRequest();

      CallBack readHandler_;
      CallBack writeHandler_;
      CallBack errorHandler_;
      CallBack connHandler_;

      public:
        Channel(EventLoop *loop);
        Channel(EventLoop *Loop, int fd);
        ~Channel();
        int getFd();
        void setFd(int fd);

        void setHolder(std::shared_ptr<HttpData> holder) { holder_ = hoder; }
        std::shared_ptr<HttpData> getHolder(){
            // 如果expired为 0的话，返回一个指向holder的对象的shared_ptr指针
            std::shared_ptr<HttpData> ret(holder_.lock());
            return ret;
        }
        // 右值引用
        void setReadHandler(CallBack &&readHandler){
            readHandler_ = readHandler;
        }

        void setErrorHandler(CallBack &&writeHandler){
            writeHandler_ = writeHandler;
        }

        void setConnHandler(CallBack &&connHandler){
            connHandler_ = connHandler;
        }

        void handleEvents(){
            
            events_ = 0;
            // 对应的文件描述符被挂断；对应的文件描述符（不）可以读
            if((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN)) {
                events_ = 0;
                return ;
            }
            // 有连接或者数据来临， 对应的文件描述符有紧急的数据可读
            // 对 EPOLLHUP和EPOLLRDHUP 的一个对比 https://codeday.me/bug/20190228/692013.html            
            if(revents_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP)){
                handleRead();
            }

            if(revents_ & EPOLLOUT){
                handleWrite();
            }

            hanleConn();
        }

        void handleRead();
        void handleWrite();
        void handleConn();
        void handleError(int fd, int err_num, std::string short_msg);

        void setRevents(_uint32_t ev){
            revents_ = ev;
        }

        void setEvents(_uint32_t ev){
            events_ = ev;
        }

        bool EqyalAndUpdataLastEvents(){
            bool ret = (lastEvents_ == events_);
            lastEvents_ = events_;
            return ret;
        }

        _uint32_t getLastEcvents(){
            return lastEvents_;
        }
            return ;
        };
}

typedef std::shared_ptr<Channel> SP_Channel;
