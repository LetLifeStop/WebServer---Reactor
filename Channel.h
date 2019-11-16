// ͨ��
#pragma once
#include <sys/epoll.h>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include "Timer.h"

class EventLoop;
class HttpData;

//std::function �������¶Ը��ֿɵ���ʵ��ķ�װ�����Դ���ͬ������������


class Channel{
  private:
    // ����ص�����
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
            // ���expiredΪ 0�Ļ�������һ��ָ��holder�Ķ����shared_ptrָ��
            std::shared_ptr<HttpData> ret(holder_.lock());
            return ret;
        }
        // ��ֵ����
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
            // ��Ӧ���ļ����������Ҷϣ���Ӧ���ļ����������������Զ�
            if((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN)) {
                events_ = 0;
                return ;
            }
            // �����ӻ����������٣� ��Ӧ���ļ��������н��������ݿɶ�
            // �� EPOLLHUP��EPOLLRDHUP ��һ���Ա� https://codeday.me/bug/20190228/692013.html            
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
