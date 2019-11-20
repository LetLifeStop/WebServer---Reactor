第一，关于竞态条件的讨论

当前有如下情景，同一个类对象tmp，线程A打算调用析构函数将tmp销毁掉，线程B打算通过tmp来实现类的一些功能

```c++
class Poll{
  public:
    Poll(){}
    ~Poll(){}
    void f();
};
```

如果线程A正在销毁tmp，而这个时候线程B却正在通过tmp实现类的一些功能，这个程序就很有可能死掉。

如果通过互斥锁的形式的话

```c++
// thread A
Poll::~Poll(){
  MutexLockGuard lock(mutex_);
  // delete tmp;
}
// thread B
if(tmp){
  MutexlockGuard lock(mutex_);
  // achieve some function
}
```

如果线程A拿到了锁，并且准备进行销毁，而此时线程B通过了if检验，这个时候程序也会死掉，从这里可以看出，**互斥量也不能保护析构函数**，那条件变量可以吗？应该还是不行

应该采用什么方式使得线程A和线程B能正确运行，并且程序不会死掉？

 书中提到了使用对象池，简单理解就是指创建对象但是不销毁对象，如果申请新的时候，如果之前存在过，就进行重复利用。

  但是这样会产生比较多的副作用，怎么样使得所有的线程都知道对象是否还存活？只申请但是不进行对象的销毁，会造成资源的浪费

 接下来引入了智能指针，主要介绍的是shared_ptr，weak_ptr，scoped_ptr。

  shared_ptr是可以控制对象的生命周期的，只要一个对象所对应的shared_ptr数目不为0，这个对象就不会被销毁。当这个对象对应的shared_ptr数目为0的时候，这个对象就会自动销毁。（在使用智能指针的前提下，new和delete的个数可以不相同，甚至可以只有new，没有delete）

  weak_ptr （ https://zh.cppreference.com/w/cpp/memory/weak_ptr ）是用来给shared_ptr 当小弟的，可以判断对象是否还存活，如果对象还存活，就可以提升为shared_ptr。还有一个作用就是可以防止shared_ptr造成的死锁问题。

  scoped_ptr，在对象析构的时候自动释放内存，能保证方在离开作用域之后他所管理的对象能被自动释放。



 





