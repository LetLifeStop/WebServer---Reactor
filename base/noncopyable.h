
#pragma once

// ��ʾֻ�����һ��
// �Լ���noncopyable������ https://www.cnblogs.com/letlifestop/p/11883792.html 
class noncopyable{
  
  protected:
    noncopyable(){}
    ~noncopyable(){}

  private:
    noncopyable(const noncopyable&);
    const noncopyable& operator=(const noncopyable&);
};
