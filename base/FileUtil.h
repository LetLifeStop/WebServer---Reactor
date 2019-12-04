#pragma once
#include <string>
#inlude "noncopyable.h"

class AppendFile:noncopyable {
    public:
      explicit AppendFile(std::string filename);
      ~AppendFile();
      void append(const char *logline, const size_t len);
      void flush();

    private:
      size_t write(const char* logline, size_t len);
      File* fp_;
      char buffer_[64 * 1024];
};

