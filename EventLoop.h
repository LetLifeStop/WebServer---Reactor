// Event为事件循环，每次从poller中拿取活跃的事件，然后分发到channel中进行分发处理
#include <functional>
#include <memory>
#include <vector>
#include "Channel.h"
#include "Epoll.h"
#include "Util.h"
#include "base/CurrentThread.h"
#include "base/Logging.h"
#include "base/Thread.h"
using namespace std;

class EventLoop {

  public:
    typedef std::function<void()> Functor; // 回调函数
    EventLoop();
    ~EventLoop();
    void loop();  // 核心函数，当创建一个EventLoop实体的时候，调用这个函数，启动事件循环
    void quit(); 
    void runInLoop(Functor&& cb);
    void queueInLoop(Functor&& cb);
    // the design style in mudo 
    bool isInLoopThread const {
        return threadId_ == CurrentThread::tid();  
    }
    // 如果条件返回错误，终止程序的执行
    bool assertInLoopThread() {
        assert(isInLoopThread());
    }
    // 禁止在一个套接口上进行读写操作
    void shutdown(shared_ptr<Channel> channel){   
        shutDownWR(channel->getFd());
    }
    // 从就绪事件中取走对应channel
    void removeFromPoller(shared_ptr<Channel> channel){
        poller_->epoll_del(channel);
    }
    // 更新就绪事件
    void updatePoller(shared_ptr<Channel> channel, int timeout = 0){
        poller_->epoll_mod(channel, timeout);
    }
    // 向就绪事件中添加channel
    void addToPoller(shared_ptr<Channel> channel, int timeout = 0){
        poller_->epoll_add(channel, timeout);
    }
    
  private:
    bool looping_;
    // whether the EventLoop running 
    shared_ptr<Epoll> poller_;  
    int wakeupFd_;  
    bool quit_;
    bool eventHandling_;
    // 被mutable修饰的变量，将永远处于可变的状态 
    mutable MutexLock mutex_;
    std::vector<Functor> pendingFunctors_;
    bool callingPendingFunctors_;
    const pid_t threadId_;
    shared_ptr<Channel> pwakeupChannel_;
    
    void wakeup();
    void handleRead();
    void doPendingFunctors();
    void handleConn();
};

