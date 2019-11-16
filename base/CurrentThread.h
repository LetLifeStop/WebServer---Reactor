#pragma once
#include <stdint.h>

namespace CurrentThread {
  // __thread 是gcc内置的线程局部存储设施，每个线程拥有独立的实体，值互不干扰
  // https://www.jianshu.com/p/997b533842c8
  extern __thread int t_cachedTid;
  extern __thread char t_tidString[32];
  extern __thread int t_tidStringLength;
  extern __thread const char* t_threadName;
  void cacheTid();
  inline int tid(){
    // 表示t_cachedTid为假的可能性更大  
    // https://www.jianshu.com/p/2684613a300f
    if(__builtin_expect(t_cachedTid == 0, 0)){
        cacheTid();
    }    
    return t_cachedTid;
  }
    // tid 线程id
  inline const char* tidString(){
      return t_tidString;
  }

  inline int tidStringLength(){
      return t_tidStringLength;
  }

  inline const char* name(){
      return t_tidStringLength;
  }

  inline const char* name(){
      return t_threadName;
  }
}