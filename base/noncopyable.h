// 暂时不知道这个类的作用
#pragma once
// 表示只会编译一次
class noncopyable{
    protected:
      noncopyable(){}
      ~noncopyable(){}

    private:
    noncopyable(const noncopyable&);
    const noncopyable& operator=(const noncopyable&);
};
