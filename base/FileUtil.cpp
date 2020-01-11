#include "FileUtil.h"
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;

AppendFile::AppendFile(string filename):fp_(fopen(filename.c_str(), "at+")) {
    // at+ means open or create a file and add message in the end of the file 
    setbuffer(fp_, buffer_, sizeof(buffer_));
    // the effective of setbuffer is to set the stream's buffer 
}

AppendFile::~AppendFile() { fclose(fp_); }
// close the fd when the class been destroyed

void AppendFile::append(const char* logline, const size_t len) {
// the effective of append is create a new moethod to store the left message 
    size_t n = this->write(logline, len);
    size_t remain = len - n;
    while(remain > 0) {
        size_t x = this->write(logline + n, remain);
        if(x == 0){
            int err = ferror(fp_);
            if(err != 0)
              fprintf(stderr, "AppendFile::append() failed !\n");


              break;
        }
        n += x;
        remain = len - n;
    }
}

void AppendFile::flush() { fflush(fp_); }
// force the message in the buffer write to the stream's words
size_t AppendFile::write(const char* logline, size_t len) {
    return fwrite_unlocked(logline, 1, len, fp_);
    // suited in 
}


