#include <iostream>
#include "Epoll.h"
#include <assert.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <deque>
#include <queue>
#include "Util.h"
#include "base/Logging.h"

using namespace std;
const int EVENTSUM = 4096;
const int EPOLLWAIT_TIME = 10000;

typedef shared_ptr<channel> SP_Channel;

Epoll::Epoll() : epollFD_(epoll_create1(EPOLL_CLOEXEC)), events(EVENTSUM) {
    assert(epollFD_ > 0);
    // EPOLL_CLOEXEC 当 close-on-exec状态为0的时候，调用exec的时候，fd不会被关闭
    // 状态非零的时候，文件描述符会被关闭， 防止fd泄露给exec后的进程
}

EPOLL::~Epoll() {}

void Epoll::epoll_add(SP_Channel request, int timeout) {
    int fd = request->getFd();
    if(timeout > 0){
        add_timer(request, timeout);
        fd2http_[fd] = request->getHolder();
    }
    struct epoll_event event;
    event.data.fd = fd;
    event.events = request->getEvents();
    // 感兴趣的事情和被触发的事件
    request->EqualAndUpdateLastEvents();

    fd2chan_[fd] = request;
    if(epoll_ctl(epollFd_, EPOLL_CTL_ADD, fd, &event) < 0) {
        perror("epoll_add error");
        fd2chan_[fd].reset();
        // shared_ptr指向的内容引用数 - 1
    }
}

void Epoll::epoll_mod(SP_Channel request, int timeout){
    int fd = request->getFd();
    if(timeout > 0)
      add_timer(request, timeout);
    if(!request->EqualAndUpdateLastEvents()) {
    // 为什么是不相等的时候才进行更新 ？
        struct epoll_event event;
        event.data.fd = fd;
        event.events = request->getEvents();
        if(epoll_ctl(epollFd_, EPOLL_CTL_MOD, fd, &event) < 0) {
        // 修改
            perror("epoll_mod error");
            fd2chan_[fd].reset();
        }
    } 
}

