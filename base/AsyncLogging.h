#pragma once
#include <functional>
#include <string>
#include <vector>
#include "CountDownLatch.h"
#include "LogStream.h"
#include "MutexLock.h"
#include "Thread.h"
#include "noncopyable.h"
//  start a log thread ,write the data in the buffer into the LogFile and set two buffer size

class AsynLogging : noncopyable {
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

      private:
        void threadFunc();
        typedef FixedBuffer<kLargeBuffer> Buffer;
        // FixedBuffer����LogStream��������
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
        CountDownLatch latch;
};
