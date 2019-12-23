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

//����fd����Ҫ������events���Լ����ֻص�����
class Channel{
  private:
    // ����ص�����
    typedef std::function<void()> CallBack;
    // ������EventLoop
    EventLoop *loop_;
    // �ļ�������
    int fd_;
    _uint32_t events_;
    // poll/epoll ���ص��¼�
    _uint32_t revents_;
    _uint32_t   lastEvents_;

    std::weak_ptr<HttpData> holder_;

    private:
      int parse_URI();
      int parse_Headers();
      int analysisRequest();
      // �ɶ��ص�����
      CallBack readHandler_;
      // ��д�ص�����
      CallBack writeHandler_;
      // ���������
      CallBack errorHandler_;
      // �رպ���
      CallBack connHandler_;

      public:
        Channel(EventLoop *loop);
        Channel(EventLoop *Loop, int fd);
        ~Channel();
        int getFd();
        void setFd(int fd);

        void setHolder(std::shared_ptr<HttpData> holder) { holder_ = holder; }
        std::shared_ptr<HttpData> getHolder() {
            // ���expiredΪ0�Ļ�������һ��ָ��holder�Ķ����shared_ptrָ��
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
            //�����¼����ͽ��зַ�
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
            // ������ͨ���ļ�������д��
            if(revents_ & EPOLLOUT){
                handleWrite();
            }

            handleConn();
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

        bool EqualAndUpdataLastEvents(){
            bool ret = (lastEvents_ == events_);
            lastEvents_ = events_;
            return ret;
        }

        _uint32_t getLastEvents(){
            return lastEvents_;
        }
            return ;
        };
}

typedef std::shared_ptr<Channel> SP_Channel;
