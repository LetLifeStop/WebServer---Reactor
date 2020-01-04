#pragma once
#include <functional>
#include <string>
#include <vector>
#include "CountDownLatch.h"
#include "LogStream.h"
#include "MutexLock.h"
#include "Thread.h"
#include "noncopyable.h"

class AsyncLogging : noncopyable {
    public:
      AsyncLogging(const std::string basename, int flushInterval = 2);
      ~AsyncLogging() {
          if(running_) stop();
      }

      void append(const char* logline, int len);

      void start() {
          running_ = 1;
          thread_.start();
          latch_.wait();
      }

      void stop() {
          running_ = false;
          cond_.notify();
          thread_.join();
      }

      private:
        void threadFunc();
        typedef FixedBuffer<kLargeBuffer> Buffer;
        // FixedBuffer是在LogStream中声明的
        typedef std::vector< std::shared_ptr<Buffer> > BufferVector;
        typedef std::shared_ptr<Buffer> BufferPtr;
        const int flushInterval_;
        bool running_;
        std::string basename_;
        Thread thread_;
        MutexLock mutex_;
        Condition cond_;
        BufferPtr currentBuffer_;
        BufferPtr nextBuffer_;
        BufferVector buffers_;
        CountDownLatch latch_;
};
