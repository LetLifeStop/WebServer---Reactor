#ifndef _EVENTLOOP_H
#define _EVENTLOOP_H

#include <functional>
#include <memory>
#include <vector>
#include "Channel.h"
#include "Epoll.h"
#include "Util.h"
#include "base/CurrentThread.h"
#include "base/Logging.h"
#include "base/Thread.h"
using namespace std;

class EventLoop {

  public:
    typedef std::function<void()> Functor;
    EventLoop();
    ~EventLoop();
    void loop();
    void quit();
    void runInLoop(Functor&& cb);
    void queueInLoop(Finctor&& cb);
    bool isInLoopThread const {
        return threadId_ == CurrentThread::tid();  
    }
    // 如果条件返回错误，终止程序的执行
    bool assertInLoopThread() {
        assert(isInLoopThread());
    }

    void shutdown(shared_ptr<Channel> channel){
        // 禁止在一个套接口上进行读写操作
        shutDownWR(channel->getFd());
    }
    
    void removeFromPoller(shared_ptr<Channel> channel){
        poller_->epoll_del(channel);
    }

    void updatePoller(shared_ptr<Channel> channel, int timeout = 0){
        poller_->epoll_mod(channel, timeout);
    }

    void addToPoller(shared_ptr<Channel> channel, int timeout = 0){
        poller_->epoll_add(channel, timeout);
    }
    
  private:
    bool looping_;
    shared_ptr<Epoll> poller_;
    int wakeupFd_;
    bool quit_;
    bool eventHandling_;
    // 被mutable修饰的变量，将永远处于可变的状态 
    mutable MutexLock mutex_;
    std::vector<Functor> pendingFunctors_;
    bool callingPendingFunctors_;
    const pid_t threadId_;
    shared_ptr<Channel> pwakeupChannel_;

    void wakeup();
    void handleRead();
    void doPendingFunctors();
    void handleConn();
};



