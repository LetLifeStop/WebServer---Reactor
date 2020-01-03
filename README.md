# WebServer---Reactor



* Util

  调整文件描述符为ET模式，定义read，write函数支持string和char*类型输入和输出，对SIGPIPE信号进行忽略处理，禁止使用Nagle算法，允许小包的发送；对socket,bind, listen进行封装。对linger的调整，具体用处是对TIME_WAIT所对应的时间进行重新设置

  SIGPIPE信号产生条件：

  在四次握手的过程中，当服务器端已经关闭对客户端的通道，客户端向服务器端进行write[此时client是可以向缓冲区写入的]，此时服务器端会返回一个RST报文，当客户端再次向server发送数据的时候，此时系统会对进行发送SIGPIPE信号，导致客户端进程退出。

* noncopyable

  在保证类中数据安全的前提下，禁止派生类编译器自动生成构造函数和构造赋值函数

* Condition

  封装了条件变量的init，signal，broadcast，timewait

* CountDownLatch

   等待其他线程执行完毕之后，实行条件变量的唤醒。具体实现通过计数的形式，记录前面还有多少线程要执行，当前面等待线程数目为0时，实行条件变量的唤醒, 其实这个项目中主要是运用了这个思想。

* MutexLock

  封装了锁的基本使用，初始化，上锁，解锁

* thread

  实现了线程的创建，执行，销毁，回收。线程回调函数中执行startThread函数，通过修改线程名称显示当前的状态， 当Thread要析构的时候，进行线程分离，继续执行任务。
  
* Httpdata

   实现了对Http请求的分析[请求行（请求方法，URL，协议版本），请求头部，请求数据]，以及响应消息。这个版本的HttpData中，只对GET和HEAD进行了分析

* Timer

   包含了两个类，TimerNode和TimerManager,每个TimerNode管理一个HttpData,包含这个请求节点所占用的时间，这个节点是否被删除。TimerManager是通过一个优先队列对请求节点进行管理，优先队列通过节点存活的时间进行递减排序，在该项目中，允许删除的情况是当头节点超时的时候才可以对节点进行删除，如果第二个节点没有超时，但是第三个节点并没有超时，是不允许第三个进行进行删除操作的。之所以这样实现的原因：

   * 不用遍历优先队列，节省时间
   * 不允许节点的随意删除，避免了随意删除节点后导致的优先队列结构重新调整
   * 如果一个中间节点已经超出自己的存活时间，并且没有达到超时的下限（如果达到了就会实行删除操作），在这个时间中，如果这个HttpData又重新发送过来请求，就可以避免节点的重新申请，节省资源。 

* Channel

   一个channel对应一个文件描述符，主要是实现文件描述符的读写事件，以及对连接的分析[请求行，请求头部，请求数据]

* Epoll

   封装了epoll多路复用的基本用法，对文件描述符的增删改查;通过调用epoll_wait收集哪些fd上有时事件发生，然后将这一些fd转移到channel中进行处理

* CurrentThread

   t_cachedTid为线程的真实id，t_tidString为将tid通过字符串的形式进行保存，便于输出日志，t_tidStringLength为线程的名称的长度，t_threadName为线程的名称。

   线程的真实Id和pthread_self并不同，pthread_self返回的是当前线程的标识符，类型为结构体。而线程的真实id是通过系统调用生成的，类型为整数，为了减少系统调用的次数，将线程的真实id通过t_cachedTid储存起来

* EventLoop

    事件循环，每次通过loop调用epoll中的poll获取活跃的事件列表，然后通知channel去处理。还实现了对poll所管理的fd的增加，删除，更新

    注意runInloop函数，这个函数借鉴的是mudo库的一个设计思想，可以实现在不用锁的情况下保证线程的安全性。当我们要执行一个函数的时候，判断该函数是否在他属于的IO线程中，如果在的话，执行即可。如果不在的花，执行queueInLoop函数，通过wake唤醒对应的IO线程来领取人物

* EventLoopThread

   新建一个线程来实现EventLoop的loop功能

* EventLoopThreadPool

   一次性开启numthread_个EventLoopThread线程，通过线程池的形式管理EventLoopThread 

* Server

   调用accept等待客户端发送请求建立连接，然后通过handeNewconn函数，进行channel的绑定，寻找一个EventLoop来认领这个channel,loop的绑定

* Main

   

## Log 日志记录

  具体设计思想借鉴了mudo的设计方法，分为前端和后端，前端负责将数据写入到Log线程中，后端负责将Log线程中的内容写到磁盘中。一次设置两个缓冲区，每当第一个缓冲区写满，两个缓冲区互换位置，前端继续向新的缓冲区写，替换下来的缓冲区向磁盘写入，实现循环。在这个项目中，为了防止第一个缓冲区写满没有地方写，设置一组两个缓冲区，每次交换两个缓冲区对应的指针。好处就是，较少了线程的唤醒频率，降低了开销

* LogStream

  重载了运算符 "<<", 按照自己规定的格式，写入自己创建的缓冲区中。自己创建的缓冲区主要是为了保存多个 "<<" 连起来的内容

* Logging

  对外接口，将Logstream进一步封装，加上固定的格式化信息

* FileUtil

  封装了底层操作，直接向文件中写，封装了一组缓冲区，以及文件的打开，文件的写入，文件的关闭

* LogFile

  封装了FileUtil，设置一个常数，当写入的次数大于等于这个常数的时候将缓冲中的数据写入到文件中

* AsyncLogging

  主要函数有两个，append和logline
  
  * append函数的作用是，向缓冲区中写入logline字符串
  
    如果说currentbuffer剩余的空间无法存储当前的信息，首先将当前的信息写入到buffer中，然后通过第二块缓冲区进行存储，然后再将第二块缓冲区的内容写入到buffer中去
  
  * threadFuc的作用是执行日志从缓冲区到文件的写入
  
    每隔flushInterval_的时间会进行一次从缓冲区到文件的写入


调试过程
* 如果有一个类需要在a.h中提前声明，在b.h中对类进行定义的时候，这个时候会报重复定义的错误，这个时候我们可以在a.h中加上 pragma once,这个能保证头文件只被编译一次





