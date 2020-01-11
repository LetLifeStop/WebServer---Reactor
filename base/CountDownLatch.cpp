#include "CountDownLatch.h"
// 作用是等待其他线程执行完毕之后唤醒等待池的所有线程
CountDownLatch::CountDownLatch(int count):
    mutex_(), condition_(mutex_), count_(count) {}

void CountDownLatch::wait() {
    MutexLockGuard lock(mutex_);
    while(count_ > 0) {
    //    printf("121112121213\n");
        condition_.wait(); 
    }
}

void CountDownLatch::CountDown() {
    MutexLockGuard lock(mutex_);
//    printf("CountDown and the count_ is %d\n", count_);
    count_--;
//    printf("CoundDown after the count_ is %d\n", count_);
    if(count_ == 0) {
        condition_.notifyAll();
    }
    // notifyAll是唤醒对象等待池的所有线程
}

