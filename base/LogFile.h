#pragma once
#include <memory>
#include <string>
#include "FileUtil.h"
#include "MutexLock.h"
#include "noncopyable.h"
// package the FileUtil further and set a const number that flush the buffer when the number of append greater than or equal the const number
class LogFile : noncopyable {
  
  public:
    LogFile(cosnt std::string& basename, int flushEveryN = 1024);
    ~logFile();
    void append(const char* logline, int len);
    void flush();
    bool rollFile();

  private:
    void append_unlocked(const char* logline, int len);
    const std::string basename_;
    const int flushEveryN_;
    int count_;
    std::unique_ptr<Mutex_lock> mutex_;
    std::unique_ptr<AppendFile> file_;
};

