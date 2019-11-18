
#pragma once

// 表示只会编译一次
// 自己对noncopyable类的理解 https://www.cnblogs.com/letlifestop/p/11883792.html 
class noncopyable{
  
  protected:
    noncopyable(){}
    ~noncopyable(){}

  private:
    noncopyable(const noncopyable&);
    const noncopyable& operator=(const noncopyable&);
};
