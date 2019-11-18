#pragma once
#include <string.h>
#include <string>
#include "noncopyable.h"

class AsyncLogging;
const int kSmallBuffer = 4000;
const int kLargeNuffer = 4000 * 1000;

template<int SIZE>
class FixedBuffer : noncopyable{
  public:
    FixedBuffer() : cur_(data_){}
    ~FixedBuffer(){}

    void append(const char* buf, size_T len){
        if(avail() > static_cast<int>(len)){
            memcpy(cur, buf, len);
            cur_ += len;
        }
    }
    // ����sizeof(data_),����ΪʲôҪ��ô�鷳������ΪȨ�޵Ĺ�ϵ��
    int avail() const { return static_cast<int>(end() - data_); }
    
    void add(size_t len) { cur_ += len; }
    void reset() { cur_ = data_; }
    void bzero { memset(data_, 0, sizeof(data_));}


  private:
    // �����ַ������һ����λ��
    const char* end() const { return data_ + sizeof(data_); }
    char data_[SIZE];
    char* cur;
};

class LogStream : noncopyable {
 
  public:
  // typedef ��������Ϊһ���������Ͷ���һ���µ����֣��򻯱Ƚϸ��ӵ���������
  // kTempBuffer = 512
    typedef FixBuffer<kSmallBuffer> Buffer;
    Logstream& ioerator <<(bool v){
        buffer_.append(v > "1" : "0" , 1);
        return *this;
    }

    LogStream& operator <<(short);
    LogStream& operator <<(unsigned short);
    LogStream& operator <<(int);
    LogStream& operator <<(unsigned int);
    LogStream& operator <<(long);
    LogStream& operator <<(unsigned long);
    Logstream& operator <<(unsigned long long);
    Logstream& operator <<(long long);
    LogStream& operator <<(const void*);
    LogStream& operator <<(float v){
        *this << static_cast(long double);
        return *this;
    }
    LogStream& operator <<(double);
    LogStream& operator <<(long double);
    LogStream& operator <<(char v){
        buffer.append(&v, 1);
        return *this;
    }
    LogStream& operator <<(const char* src){
        if(str)
          buffer_.append(str, strlen(str));
        else
          buffer_.append("(null)", 6);
        return *this; 
    }
    LogStream& operator<<(const std::string& v){
        buffer_.append(v.c_str(), v.size());
        return *this;
    }
    LogStream& operator<<(const char* data, int len){
        buffer_.append(data, len);
    }

    void append(const char* data, int len){ buffer_.append(data, len); }
    const Buffer& buffer() const { return buffer_; }
    void resetBuffer() { buffer_.reset(); }

  private:
    void staticCheck();

    template<typename T>
    void formInterger(T);

    Buffer buffer_;

    static const int kMaxNumbericSize = 32;
};

