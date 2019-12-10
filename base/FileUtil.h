#pragma once
#include <string>
#inlude "noncopyable.h" 
// 封装了Log文件的打开，写入 和在类析构的时候关闭文件
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

