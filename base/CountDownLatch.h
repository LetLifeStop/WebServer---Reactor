#pragma once 
#include "Condition.h"
#include "MutexLock.h"
#include "noncopyable.h"

// 确保thread中传进去的func真的启动了之后，start才返回

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


