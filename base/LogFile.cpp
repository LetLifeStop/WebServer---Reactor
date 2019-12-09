#include "LogFile.h"
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include "FileUtil.h"

using namespace std;

LogFile::LogFile(const string& basename, int flushEveryN) :
    basename_(basename),
    flushEveryN_(flushEveryN),
    count_(0),
    mutex_(new MutexLock) {
        flie_.reset(new AppendFile(basename));
    }

LogFile::~LogFile() {}

void logFile::append(const char* logline, int len) { 
    MutexLockGuard lock(*mutex_);
    append_unlocked(logline, len);
}

void LogFile::flush() {
    MutexLockGuard lock(*mutex_);
    file_->flush();
}

void LogFile::append_unlocked(const char* logine, int len) {
    file_->append(logline, len);
    ++count_;
    // 每当append的次数大于等于 flushEveryN_的时候，就进行一次flush
    if(cout_ >= flushEveryN_) {
        count = 0;
        file_->flush();
    }
}

void 
