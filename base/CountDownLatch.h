#pragma once 
#include "Condition.h"
#include "MutexLock.h"
#include "noncopyable.h"

// ȷ��thread�д���ȥ��func���������֮��start�ŷ���

class CountDownLatch:noncopyable {
    public:
      explicit CountDownLatch(int count);
      void wait();
      void CountDown();
    private:
      mutable MutexLock mutex_;
      Condition condition_;
      int count_;
};


