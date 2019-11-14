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

        void setReadHandler(CallBack &&readHandler){
            readHandler_ = readHandler;
        }
        
}