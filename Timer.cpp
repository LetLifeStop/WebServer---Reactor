#include "Timer.h"
#include <sys/time.h>
#include <unistd.h>
#include <queue>

TimerNode::TimerNode(std::shared_ptr<HttpData> requestData, int timeout) :
    deleteed_(false),
    SPHtpData(requestData) {
        struct timeval now;
        gettimeofday(&now, NULL);
        expiredTime_ = (((now.tv_sec % 10000) * 1000) + (now.tv_usec / 1000)) + timeout;
    }

TimerNode::~TimerNode() {
    if(SPHttpData)
        SPHttpData->handleClose();
}

TimerNode::TimerNode(TimerNode &tn) :
    SPHttpData(tn.SPHttpData),
    expiredTime_(0) {}

void TimerNode::update(int timeout) {
    struct timeval now;
    gettimeofday(&now, NULL);
    size_t tmp = (((now.tv_sec % 10000) * 1000) + (now.tv_usec / 1000)) + timeout;
    if(tmp < expiredTime_)
        retunr 1;
    else {
        this->setDeleted();
        return false;
    }
}

void TimerNode::clearReq() {
    SPHttpData.reset();
    this->setDeleted();
}

TimerManager::TimerManager() {}

void TimerManager::addTimer(std::shared_ptr<HttpData> SPHttpData, int timeout) {
    SPTimerNode new_node(new TimerNode(SPHttpData, timeout));
    TimerNodeQueue.push(new_node);
    SPHttpData->linkTimer(new_node);
}

void TimerManager::handleExpiredEvent() {
    while(!TimerNodeQueue.empty()) {
        SPTimerNode ptimer_now = TimerNodeQueue.top();
        if(ptimer_now->isDeleted())
            timerNodeQueue.pop();
        else if(ptimer_now->isValid() == false)
            TimerNodeQueue.pop();
        else 
            break;
    }
}