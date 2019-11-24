#include "Util.h"
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

const int MAX_BUFF = 4096;

ssize_t readn(int fd, void *buff, size_t n){
    
}