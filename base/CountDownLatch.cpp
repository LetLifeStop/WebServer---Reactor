#include "CountDownLatch.h"
// �����ǵȴ������߳�ִ�����֮���ѵȴ��ص������߳�
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
    // notify��������Ѷ���ĵȴ����е�һ���߳�
    // notifyAll�ǻ��Ѷ���ȴ��ص������߳�
}

