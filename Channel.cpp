#include "Channel.h"
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <queue>
#include "Epoll.h"
#include "EventLoop.h"
#include "Util.h"

using namespace std;

Channel::Channel(EventLoop *loop)
    : loop_(loop), events_(0), lastEvents_(0), fd_(0) {}

Channel::Channel(EventLoop *loop, int fd)
    : loop_(loop), fd_(fd), events_(0), lastEvents_(0) {}

Channel::~Channel() { }
 
int Channel::getFd() { return fd_; }

void Channel::setFd(int fd) { fd_ = fd; }

void Channel::handleRead() {
    if(readHandler_) {
        readHandler_();
    }
}
// 如果可读的回调函数存在的话，调用该函数

void Channel::handleWrite() {
    if(writeHandler_) {
        writeHandler_();
    }
}
// 如果可写的回调函数存在的话，调用该函数
void Channel::handleConn() {
    if(connHandler_) {
        connHandler_();
    }
}
// 如果连接的回调函数存在的话，调用该函数
