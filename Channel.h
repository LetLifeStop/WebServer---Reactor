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

        void setReadHandler(CallBack &&readHandler){
            readHandler_ = readHandler;
        }
        
}