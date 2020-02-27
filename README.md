# WebServer---Reactor

项目基于: https://github.com/linyacool/WebServer

* Util

  调整文件描述符为ET模式，定义read，write函数支持string和char*类型输入和输出，对SIGPIPE信号进行忽略处理，禁止使用Nagle算法，允许小包的发送；对socket,bind, listen进行封装。对linger的调整，具体用处是对TIME_WAIT所对应的时间进行重新设置

  SIGPIPE信号产生条件：

  在四次挥手的过程中，当服务器端已经关闭对客户端的通道，客户端向服务器端进行write[此时client是可以向缓冲区写入的]，此时服务器端会返回一个RST报文，当客户端再次向server发送数据的时候，此时系统会对进行发送SIGPIPE信号，导致客户端进程退出。

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
  EPOLLIN 表示连接到达或者有数据来临
  EPOLLOUT 表示有数据要写
  EPOLLET 表示边缘触发模式
  EPOLLPRI 表示对应的文件描述符有紧急的数据可读
  EPOLLRDHUP 表示对端断开连接
  对于MainReactor来说，只需要设置处理新的连接和对文件描述符的状态更新。
  在处理完事件之后，为什么是先处理文件，再去通过handleConn进行文件描述符的更新？？

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

* 具体运行过程

  * 首先输入端口，日志文件名，初始的线程数目
  * 开启一个mainLoop，用来接收新的连接。同时在eventLoopThreadPool下创建threadNum个线程
  * 




调试过程

* 如果有一个类需要在a.h中提前声明，在b.h中对类进行定义的时候，这个时候会报重复定义的错误，这个时候我们可以在a.h中加上 pragma once,这个能保证头文件只被编译一次

* 当有新的客户端请求建立连接，为何在server.cpp下的handNewconn（）下面测试，不会输出内容？

  思路：

  排查原因是Server的start函数中的EventLoopThreadPool->start函数出现了问题，这个函数的作用是开启threadnum个线程来处理IO请求。 然后这threadnum个线程要储存在EventloopThreadPool中，通过vector的形式进行存储。在EventloopThreadPool中，通过thread.cpp创建线程。然后创建线程成功之后，通过CountDownLatch对线程进行上锁，当有任务来临的时候，通过CountDown来进行线程的解锁。

  在condition.h中，会调用pthread_cond_wait, 这个时候会先放开自己拥有的锁，然后等待一个cond信号。这个cond信号应该是通过CoundDownLatch中的countDown信号来处理。大体是这个思路，明天再改，今天没空了

  **bug已解决**，具体原因是自解锁MutexGuard的析构函数编写出错，析构函数应该是unlock，原来写成了lock

* ```c++
  EventLoop.cpp:88: void EventLoop::loop(): Assertion `!isInLoopThread()' failed.
  ```

  思路：

  猜测造成这个的具体原因是竞态条件。应该是MutexLock和MutexLockGuard是否在哪个地方有遗漏

   EventLoopThreadPool中，先创建threadnum个线程，然后需要将每一个EventLopThread通过vector的形式保存起来。在startLoop中，会有线程的创建。线程创建的回调函数是startThread，在startthread中，判断当前线程的前面是否有其他线程。如果有，就阻塞等待其他线程执行完各自的任务，然后再从死锁的情况下解放出来。然后再去执行自己的func，func是从EventLoopThread中赋值过来的。fuc的作用就是，通过loop调用poller收集哪些fd有事件发生。

  解决方法：将 assert(!isInLoopThread())改成assert(isInLoopThread())。

  isInLoopThread的作用是判断这个函数所属的线程和当前的线程是否一样。如果一样就返回True否则返回false。assert(1)会继续执行，assert(0) 会报错，所以；如果不是在当前的线程，是会报错的。

* ```c++
  wget localhost:8888/hello 返回No data received
  ```

  怀疑是HttpData中的handleRead和Server中的HandleRead赋值出现了问题

  应该是Server的handleRead是用来处理新的连接的，而其余的线程中的handleRead是用来进行信息解析的，现在的任务是找出Server的handleRead和wakeupChannel的handleRead分别是再哪里赋值的。

  开始一步步排查，真正wakeupchannel中的handleRead应该是HttpData中的handleRead。第一次使用httpData是在server的handleNewConn中。对于当前的client_addr，我们需要先找到一个EventLoop来管理它，然后new一个httpData用于实现该fd的读取，写入，解析报文等操作。然后接下来就是处理该fd对应的事件

  疑点：发现正确的版本没有使用HttpData中的handleEvents，也就是说handleEvents有很大可能是在epoll中进行调用的。

  修改bug，Util.cpp中read函数和write函数编写错误

  整个设计框架，应该是MainReactor和SubReactor都在不停的loop，只不过handleEvent之后，handleRead和handleWrite等对应的实现功能不同。MainReactor是为了处理新的连接，SubReactor进行事件的分析。

* 时间问题

* 

* 为什么要有CountDownLatch这个类？





注释
-t  指定初始线程数目
-l  指定PathLog的路径
-p  指定端口









