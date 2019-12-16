# WebServer---Reactor



## Log 日志记录

  具体设计思想借鉴了mudo的设计方法，分为前端和后端，前端负责将数据写入到Log线程中，后端负责将Log线程中的内容写到磁盘中。一次设置两个缓冲区，每当第一个缓冲区写满，两个缓冲区互换位置，前端继续向新的缓冲区写，第一个缓冲区向磁盘写入，实现循环。在这个项目中，为了防止第一个缓冲区写满没有地方写，设置一组两个缓冲区，每次交换两个缓冲区对应的指针。

* LogStream

  重载了运算符 "<<", 按照自己规定的格式，写入自己创建的缓冲区中。自己创建的缓冲区主要是为了保存多个 "<<" 连起来的内容

* Logging

  对外接口，将Logstream进一步封装，加上固定的格式化信息

* LogFile

  封装了FileUtil，设置一个常数，当写入的次数大于等于这个常数的时候将缓冲中的数据写入到文件中

* FileUtil

  封装了底层操作，直接向文件中写，封装了一组缓冲区，以及文件的打开，文件的写入，文件的关闭

* AsynLogging

  对FileUtil，LogFile的进一步封装，开始一个线程，使用双缓冲的方法，通过对写入次数，缓冲区的数据大小进行判断，进行缓冲区智者你的交换，将已满缓冲区的数据写入到LogFile文件中



* Conditonn

  封装了条件变量的一些基本使用，初始化，线程之间的通知，定时等待

* CountDownLatch

  等待其他线程执行完毕之后，实行条件变量的唤醒

  