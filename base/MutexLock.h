// 将互斥量相关的操作封装进类中
#pragma once
#include <pthread.h>
#include <cstdio>
#include "noncopyable.h"

class MutexLock: noncopyable{
    public:
    MutexLock(){pthread_mutex_init(&mutex, NULL);}
    ~MutexLock(){
        pthread_mutex_lock(&mutex);
        pthread_mutex_destroy(&mutex);
    }
    void lock(){ pthred_mutex_lock(&mutex); }
    void unlock{ pthread_mutex_unlock(&mutex); }
    pthread_mutex_t *get(){
        return &mutex;
    }

    private:
      pthread_mutex_t mutex;

}
