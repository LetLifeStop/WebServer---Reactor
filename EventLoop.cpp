#include "EventLoop.h"
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <iostream>
#include "Util.h"
#include "base/Logging.h"

using namespace std;

__thread EventLoop* t_loopInthisThread = 0;

int createEventfd() {
    int evtfd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if(evtfd < 0){
        LOG << "Failed in eventfd";
        abort();
        // 终止程序运行
    }
    return evtfd;
}

EventLoop::EventLoop ():
    looping_(false),
    poller_(new Epoll()),
    wakeupFd_(createEventfd()),
    quit_(false),
    eventhandling_(false),
    callingPendingFunctors_(false),
    threadId_(CurrentThread::tid()),
    pwakeupChannel_(new Channel(this, wakeupFd_)) {
      if(t_loopInthisThread) {
      LOG << "Another EventLoop" << t_loopInthisThread << "exists "; 
    }
      else 
    t_loopInthisThread = this;
    }

    pwakeupChannel_->setEvents(EPOLLIN | EPOLLET);
    pwakeupChannel_->setReadHandler(bind(&EventLoop::handleRead, this));
    pwakeupChannel_->setConnhandler(bind(&EventLoop::handleConn, this));
    poller_->epoll_add(pwakeupChannel_, 0);
    










    