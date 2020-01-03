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

typedef shared_ptr<Channel> SP_Channel;

Epoll::Epoll() : epollFd_(epoll_create1(EPOLL_CLOEXEC)), events_(EVENTSUM) {
    assert(epollFd_ > 0);
    // EPOLL_CLOEXEC ??? close-on-exec状态为0的时候，调用exec的时候，fd不会被关???
    // 状态非零的时候，文件描述符会被关闭， 防止fd泄露给exec后的进程
}

Epoll::~Epoll() {}

void Epoll::epoll_add(SP_Channel request, int timeout) {
    int fd = request->getFd();
    if(timeout > 0){
        add_timer(request, timeout);
        fd2http_[fd] = request->getHolder();
    }
    struct epoll_event event;
    event.data.fd = fd;
    event.events = request->getEvents();

    request->EqualAndUpdateLastEvents();

    fd2chan_[fd] = request;
    if(epoll_ctl(epollFd_, EPOLL_CTL_ADD, fd, &event) < 0) {
        perror("epoll_add error");
        fd2chan_[fd].reset();
        // shared_ptr指向的内容引用数 - 1
    }
}

void Epoll::epoll_mod(SP_Channel request, int timeout) {
    int fd = request->getFd();
    if(timeout > 0)
      add_timer(request, timeout);
    if(!request->EqualAndUpdateLastEvents()) {
    // 为什么是不相等的时候才进行更新 ???
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

void Epoll::epoll_del(SP_Channel request) {
    int fd = request->getFd();
    struct epoll_event event;
    event.data.fd = fd;
    event.events = request->getLastEvents();

    if(epoll_ctl(epollFd_, EPOLL_CTL_DEL, fd, &event) < 0) {
        perror("Epoll_del error");
    }
    fd2chan_[fd].reset();
    fd2http_[fd].reset();
}

std::vector<SP_Channel>Epoll::poll() {
    while(1){
      int event_count = epoll_wait(epollFd_, &*events_.begin(), events_.size(), EPOLLWAIT_TIME);
      if(event_count < 0){
          perror("epoll wait error");
      }
      std::vector<SP_Channel> req_data = getEventsRequest(event_count);
      if(req_data.size() > 0)
        return req_data;
    }
}

std::vector<SP_Channel> Epoll::getEventsRequest(int events_num) {
    std::vector<SP_Channel> req_data;
    int i;
    for(i = 0; i < events_num; i++){
        int fd = events_[i].data.fd;
        SP_Channel cur_req = fd2chan_[fd];

        if(cur_req){
            cur_req->setRevents(events_[i].events);
            cur_req->setEvents(0);
            req_data.push_back(cur_req);
        }
        else {
            LOG << "SP cur_req is invalid";
        }
    }
    return req_data;
}

void Epoll::add_timer(SP_Channel request_data, int timeout) {
    shared_ptr<HttpData> t = request_data->getHolder();
    if(t)
      timerManager_.addTimer(t, timeout);
    else 
      LOG << "timer add fail";
}

