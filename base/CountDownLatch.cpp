#include "CountDownLatch.h"
// �����ǵȴ������߳�ִ�����֮���ѵȴ��ص������߳�
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
    // notifyAll�ǻ��Ѷ���ȴ��ص������߳�
}

