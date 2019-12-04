#include <iostream>
#include "thread.h"
#include <assert.h>
#include <errno.h>
#include <linux/unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <unistd.h>
#include <memory>
#include "CurrentThread.h"

using namespace std;

namespace CurrentThread {
    __thread int t_cachedTid = 0;
    __thread char t_tidString[32];
    __thread int t_tidStringLength = 6;
    __thread const char* t_threadName = "default";
}

pid_t gettid() { return static_cast<pid_t>(::syscall(SYS_gettid)); }
// 系统调用获得tid（线程标识符）; pid 进程标志符

void CurrentThread::cacheTid() {
    if(t_cachedTid == 0){
        t_cachedTid = gettid();
        t_tidStringLength =
            snprintf(t_tidString, sizeof(t_tidString), "%5d ", t_cachedTid);
            // 保存t_cachedTid
    }
}

struct ThreadData {
    typedef Thread::ThreadFunc ThreadFunc;
    ThreadFunc func_;
    string name_;
    pid_t* tid_;
    CountDownLatch* latch_;

    ThreadData(const ThreadFunc& func, const string& name, pid_t* tid, CountDownLatch* latch):
        func_(func), name_(name), tid_(tid), latch_(latch) {}

    void runInThread() {
        *tid_ = CurrentThread::tid();
        tid_ = NULL;
        latch_->countDown();
        latch_ = NULL;

        CurrentThread::t_threadName = name_.empty() ? "thread" : name).c_str();
        prctl(PR_SET_NAME, CurrentThread::t_threadName);
        // 把参数CurrentThread::t_threadName作为进程名
        func_();
        CurrentThread::t_threadName = "finished";
    }
};


void* startThread(void* obj) {
    ThreadData* data = static_cast<ThreadData*>(obj);
    data->runInThread();
    delete data;
    return NULL;
}

Thread::thread(const ThreadFunc& func, const string& n) :
    started_(false),
    joined_(false),
    pthreadId_(0),
    tid_(0),
    func_(func),
    name_(n),
    latch_(1){
      setDefaultName();
}

Thread::~Thread() {
    if(started_ && !joined_) pthread_detach(pthread_Id_);
}

void Thread::setDefaultName() {
    if(name_.empty()) {
        char buf[32];
        snprintf(buf, sizeof(buf), "Thread");
        name_ = buf;
    }
}

void Thread::start() {
    assert(!started_);
    started_ = 1;
    ThreadData* data = new ThreadData(func_, name, &tid_, &latch_);
    if(pthread_create(&pthreadId_, NULL, &startThread, data)) {
        started_ = false;
        delete data;
    }
    else {
        latch_.wait();
        assert(tid_ > 0);
    }
}

int Thread::join() {
    assert(started_);
    assert(!joined_);
    joined_ = true;
    return pthread_join(pthread, NULL);
    // 线程回收
}







