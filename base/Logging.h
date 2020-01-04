#pragma once
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include "LogStream.h"

class AsyncLogging;
// set port and 
class Logger {
  
  public:
    Logger(const char *fileName, int line);
    ~Logger();
    LogStream &stream() { return impl_.stream_; }
    static void setLogFileName(std::string fileName) {
       LogFileName_ = fileName;
    }
    static std::string getLogFileName() {
        return LogFileName_;
    }

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
    static std::string LogFileName_;
};
// __FILE__ �������ڵ��ļ���, __LINE__ �������������� __func__ �������ڵĺ�����
# define LOG Logger(__FILE__, __LINE__).stream()
