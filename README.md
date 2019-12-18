# WebServer---Reactor

#### 

* noncopyable

  在保证类中数据安全的前提下，禁止派生类编译器自动生成构造函数和构造赋值函数

* Condition

  封装了条件变量的init，signal，broadcast，timewait

* CountDownLatch

   等待其他线程执行完毕之后，实行条件变量的唤醒。具体实现通过计数的形式，记录前面还有多少线程要执行，当前面等待线程数目为0时，实行条件变量的唤醒。

* MutexLock

  封装了锁的基本使用，初始化，上锁，解锁

* thread

  实现了线程的创建，执行，销毁，回收。线程回调函数中执行startThread函数，通过修改线程名称显示当前的状态， 当Thread要析构的时候，进行线程分离，继续执行任务。



## Log 日志记录

  具体设计思想借鉴了mudo的设计方法，分为前端和后端，前端负责将数据写入到Log线程中，后端负责将Log线程中的内容写到磁盘中。一次设置两个缓冲区，每当第一个缓冲区写满，两个缓冲区互换位置，前端继续向新的缓冲区写，替换下来的缓冲区向磁盘写入，实现循环。在这个项目中，为了防止第一个缓冲区写满没有地方写，设置一组两个缓冲区，每次交换两个缓冲区对应的指针。好处就是，较少了线程的唤醒频率，降低了开销

* LogStream

  重载了运算符 "<<", 按照自己规定的格式，写入自己创建的缓冲区中。自己创建的缓冲区主要是为了保存多个 "<<" 连起来的内容

* Logging

  对外接口，将Logstream进一步封装，加上固定的格式化信息

* LogFile

  封装了FileUtil，设置一个常数，当写入的次数大于等于这个常数的时候将缓冲中的数据写入到文件中

* FileUtil

  封装了底层操作，直接向文件中写，封装了一组缓冲区，以及文件的打开，文件的写入，文件的关闭

* AsynLogging

  主要函数有两个，append和logline
  
  * append函数的作用是，向缓冲区中写入logline字符串
  
    如果说currentbuffer无法存储当前的信息，首先写入到buffer中，然后通过第二块缓冲区进行存储，然后再将第二块缓冲区的内容写入到buffer中去
  
  * threadFuc的作用是执行日志从缓冲区到文件的写入
  
    每隔flushInterval_的时间会进行一次从缓冲区到文件的写入

