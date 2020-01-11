#pragma once
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <cstdint>
#include "MutexLock.h"
#include "noncopyable.h"

// �����class�Ļ���Ĭ�ϵļ̳з�ʽΪprivate��struct�Ļ���Ĭ�ϵļ̳з�ʽΪpublic
class Condition : noncopyable {

  public:
    // explicit �ؼ��ʣ����ܷ�����ʽ���Զ�ת��
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
        //CLOCK_REALTIME ϵͳ��ʵʱʱ��
        clock_gettime(CLOCK_REALTIME, &abstime);
        abstime.tv_sec += static_cast<time_t>(seconds);
        // ETIMEOUT ���ӳ�ʱ
        // pthread_cond_timedwait ���ȷſ�mutex�� �����abstime�У�û�л�ȡ��cond�źţ��ջ�abstime���᷵��ETIMEDOUT 
        return ETIMEDOUT == pthread_cond_timedwait(&cond, mutex.get(), &abstime);
    }

  private:
    MutexLock &mutex;
    pthread_cond_t cond;  
};



