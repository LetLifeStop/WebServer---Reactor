#include "Logging.h"
#include "CurrentThread.h"
#include "Thread.h"
#include "AsyncLogging.h"
#include <assert.h>
#include <iostream>
#include <time.h>
#include <sys/time.h>

static pthread_once_t once_control_ = PTHREAD_ONCE_INIT;
static AsynLogging *AsynLogger_;

std::string Logger::LogFileName_ = "./webServer.log";

void output(const char* msg, int len) {
    pthread_once(&once_control_, once_init);
    AsyncLogger_->append(msg, len);
}

Logger::Impl::Impl(const char* msg, int len) :
    stream_(),
    line_(line),
    basename_(filename){
    formatTime();
    }

void Logger::Impl:formatTim() {
    struct timeval tv;
    time_t time;
    char str_t[26] = {0};
    gettimeofday(&tv, NULL);
    time = tv.tv_sec;
    struct tm* p_time = localtime(&time);
    strftime(str_t, 26, "%Y-%m-%d %H:%M:%s\n", p_time);
    stream_ << str;
}

Logger::Logger(const char* fileName, int line) :
    impl_(fileName, line)
{ }

Logger::~Logger() {
    impl_.stream_ << " -- " << impl_.basename_ << " : " << impl.line_ << "\n";
    const LogStream::Buffer& buf(stream().buffer());
    output(buf.data(), buf.length());
}


