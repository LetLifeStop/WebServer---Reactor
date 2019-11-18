#pragma once
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include "LogStream.h"

class AsyncLogging;

class Logger {
  
  public:
    Logger(const char *fileName, int line);
    ~Logger();
    LogStream &stream() { return impl_.stream_; }


  private:
    class Impl {
      public:
      Impl(const char* fileName, int line);
      void formatTime();

      LogStream stream_;
      int line_;
      std::string basename_;
    };
    Impl impl_;
    static std::string logFileName_;
};
// __FILE__ 返回所在的文件名, __LINE__ 返回所在行数， __func__ 返回所在的函数名
# define LOG logger(__FILE__, __LINE__).stream()
