#include "CountDownLatch.h"
// 作用是等待其他线程执行完毕之后唤醒等待池的所有线程
CountDownLatch::CountDownLatch(int count):
    mutex_(), condition_(mutex_), count_(count) {}

void CountDownLatch::wait() {
    MutexLockGuard lock(mutex_);
    while( count_ > 0)
        condition_.wait();
}

void CoundDownLatch::CoundDown() {
    MutexLockGuard lock(mutex_);
    --count__;
    if(count_ == 0) condition_.notifyAll();
    // notify是随机唤醒对象的等待池中的一个线程
    // notifyAll是唤醒对象等待池的所有线程
}

