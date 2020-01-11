#pragma once
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <cstdint>
#include "MutexLock.h"
#include "noncopyable.h"

// 如果是class的话，默认的继承方式为private；struct的话，默认的继承方式为public
class Condition : noncopyable {

  public:
    // explicit 关键词，不能发生隐式的自动转换
    explicit Condition(MutexLock &_mutex) : mutex(_mutex) {
        pthread_cond_init(&cond, NULL);
    }   
    ~Condition() { pthread_cond_destroy(&cond); }
    void wait() { pthread_cond_wait(&cond, mutex.get()); }
    // send signal to one thread
    void notify() { pthread_cond_signal(&cond); }
    // send signal to all thread
    void notifyAll() { 
     pthread_cond_broadcast(&cond); 
    }
    bool waitForSeconds(int seconds){
        // struct timespec{
        //   time_t tv_sec;
        //   long tv_nsec;
        // }
        struct timespec abstime;
        //CLOCK_REALTIME 系统的实时时间
        clock_gettime(CLOCK_REALTIME, &abstime);
        abstime.tv_sec += static_cast<time_t>(seconds);
        // ETIMEOUT 连接超时
        // pthread_cond_timedwait 首先放开mutex， 如果在abstime中，没有获取到cond信号，收回abstime，会返回ETIMEDOUT 
        return ETIMEDOUT == pthread_cond_timedwait(&cond, mutex.get(), &abstime);
    }

  private:
    MutexLock &mutex;
    pthread_cond_t cond;  
};



