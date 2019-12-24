// EventΪ�¼�ѭ����ÿ�δ�poller����ȡ��Ծ���¼���Ȼ��ַ���channel�н��зַ�����
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
    typedef std::function<void()> Functor; // �ص�����
    EventLoop();
    ~EventLoop();
    void loop();  // ���ĺ�����������һ��EventLoopʵ���ʱ�򣬵�����������������¼�ѭ��
    void quit(); 
    void runInLoop(Functor&& cb);
    void queueInLoop(Functor&& cb);
    // the design style in mudo 
    bool isInLoopThread const {
        return threadId_ == CurrentThread::tid();  
    }
    // ����������ش�����ֹ�����ִ��
    bool assertInLoopThread() {
        assert(isInLoopThread());
    }
    // ��ֹ��һ���׽ӿ��Ͻ��ж�д����
    void shutdown(shared_ptr<Channel> channel){   
        shutDownWR(channel->getFd());
    }
    // �Ӿ����¼���ȡ�߶�Ӧchannel
    void removeFromPoller(shared_ptr<Channel> channel){
        poller_->epoll_del(channel);
    }
    // ���¾����¼�
    void updatePoller(shared_ptr<Channel> channel, int timeout = 0){
        poller_->epoll_mod(channel, timeout);
    }
    // ������¼������channel
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
    // ��mutable���εı���������Զ���ڿɱ��״̬ 
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

