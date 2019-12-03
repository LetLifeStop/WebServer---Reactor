 #include "EventLoopThreadPool.h"

EventLoopThreadPool::EventLoopThreadPool(EventLoop *baseLoop, int numThread):
    : baseLoop_(baseLoop), started_(false), numThreads_(numThreads), next_(0) {
        if(numThreads_ <= 0){
            LOG << "numThreads_ <= 0";
            abort();
        }
}

void EventLoopThreadPool::start() {
    baseLoop_->assertInLoopThread();
    // 判断基本的EventLoop是否还存在
    started_ = true;
    int i;
    for(i = 0; i < numThreads_; i++){
        std::shared_ptr<EventLoopThread> t(new EventLoopThread());
        threads_.push_back(t);
        loops.push_back(t->startLoop());
    }
}

EventLoop *EventLoopThreadPool::getNextLoop() {
  // 得到下一个loop ?
  baseLoop_->assertInLoopThread();
  assert(started_);
  EventLoop *loop = baseLoop_;
  if(!loops_.empty()){
      loop = loops_[next_];
      next_ = (next_ + 1) % numThreads_;
  }
  return loop;
}



