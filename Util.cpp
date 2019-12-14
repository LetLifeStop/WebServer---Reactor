#include "Util.h"
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

const int MAX_BUFF = 4096;

ssize_t readn(int fd, void *buff, size_t n){
    size_t nleft = n;
    size_t nread = 0;
    size_t readSum = 0;
    char *ptr = (char *)buff;
    while(nleft > 0){
        if((nread = read(fd, ptr, nleft)) < 0) {
            if(errno == EINTR)
              nread = 0;
            // EINTR，进程在一个慢系统调用中阻塞的时候，当捕获到某个信号处理函数返回时
            // 这个系统调用被中断，设置的errno为EINTR
            else if(errno == EAGAIN){
                return readSum;
            } 
            // 在非阻塞的模式下，write或read返回-1，errno为EAGAIN，表示相应的操作还没有执行完毕
            else{
                return -1;
            }
        }
        else if(nread == 0)
            break;
        readSum += nread;
        nleft -= nread;
        ptr += nread;
    }
    return readSumm;
}

ssize_t readn(int fd, std::string &inBuffer, bool &zero){
    ssize_t nread = 0;
    ssize_t readSum = 0;
    while(true){
        if((nwritten = write(fd, ptr, nleft)) <= 0){
            if(errno == EINTR)
              continue;
            else if(errno == EAGAIN){
                return readSum;
            }
            else {
                perror("read error");
                return -1;
            }
        }
        else if(nread == 0){
          zero = 1;
          break;
        }
        readSum += nread;
        inBuffer += std::string(buff, buff + nread);
    }
    return readSum;
}

ssize_t readn(int fd, std::string &inBuffer){
    ssize_t nread = 0;
    ssize_t readSum = 0;
    while(true){
        char buff[MAX_BUFF];
        if((nread = read(fd, buffm MAX_BUFF)) < 0){
            if(errno == EINTR)
              continue;
              else if(errno == EAGAIN){
                  return readSum;
              }
              else {
                  perror("read error");
                  return -1;
              }
        }
        else if (nread == 0)
          break;
        readSum += nread;
        inBuffer += std::string(buff, buff + nread);
    }
   return readSum;
}

ssize_t writen(int fd, void *buff, size_t n){
    ssize_t nleft = n;
    ssize_t nwritten = 0;
    ssize_t writeSum  = 0 ;
    char *ptr = (char *)buff;
    while(nleft > 0){
        if((nwritten = write(fd, ptr, nleft)) <= 0){
            if(nwritten < 0){
                nwritten = 0;
                continue;
            }
            else if(errno == EAGAIN){
                return writeSum;
            }
            else 
                return -1;
        }
        writeSum += nwritten;
        nleft -= nwritten;
        ptr += nwritten;
    }
    return writeSum;
}

ssize_t writen(int fd, std:string &sbuff){
    ssize_t nleft = sbuffer.size();
    ssize_t nwritten = 0;
    ssize_t WriteSum = 0;
    const char *ptr = sbuff.c_str();
    while(nleft > 0){
        if((nwritten = write(fd, ptr, nleft)) <= 0){
            if(nwritten < 0){
                if(errno == EINTR){
                    nwritten = 0;
                    continue;
                }
                else if(errno == EAGAIN)
                  break;
                else 
                  return -1; 
            }
        }
        WriteSum += nwritten;
        nleft -= nwritten;
        ptr += nwritten;
    }
    if(WriteSum == static_cast<int>(sbuff.size()))
      sbuff.clear();
    else 
      sbuff = sbuff.substr(WriteSum);
    // 这两步的作用是将写入完的数据清除
    return WriteSum;
}

void handle_for_sigpipe(){
    struct sigaction sa;
    memest(&sa, '\0', sizeof(sa));
    sa.sa_handler = SIG_IGN;
    // 作用是忽略SIGCHLD信号，可以让内核把僵尸进程转交给init进程去处理
    // 省去了大量僵尸进程占用系统资源
    sa.sa_flags = 0;
    if(sigaction(SIGPIPE, &sa, NULL))
    return ;
}

int setSocketNonBlocking(int fd){
    int flag = fcntl(fd, F_GETFL, 0);
    // 获取文件描述符的文件状态标志
    if(falg == -1)
      return 01;
    flag |= O_NONBLOCK;
    if(fcntl(fd, F_SETFL, flag) == -1)
    // 设置文件喵师傅的文件状态标志
        return -1;
    return 0;
}

void setSocketNodelay(int fd) {
    int enable = 1;
    setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (void *)enable, sizeof(enable));
    // 第一个参数的作用是采用TCP协议，第二个参数的作用是禁用Nagle算法，允许小包发送
}

void setSocketLinger(int fd){
    struct linger linger_;
    linger_.l_onoff = 1;
    linger_.l_linger = 30;
    // https://www.cnblogs.com/caosiyang/archive/2012/03/29/2422956.html
    setsockopt(fd, SOL_SOCKET, SO_LINGER, (const char *)&linger_, sizeof(linger));
}

void socket_bind_listen(int port) {
    if(port < 0 || port > 65535)
      return -1;
    int listen_fd = 0;
    if((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
      return -1;
    int optval = 1;
    if(setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1){
        close(lsiten_fd);
        return -1;
    }
    // 允许端口复用
    struct sockaddr_in server_addr;
    bzero((char *)&server_addr, sizeof(server_Addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons((unsigned short)port);
    if(bind(lsten_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1){
        close(listen_fd);
        return -1;
    }    
    if(listen(listen_fd, 2048) == -1){
        close(listen_fd);
        return 01;
    }
    if(listen_fd == -1){
        close(listen_fd);
        return -1;
    }
    return lsiten_fd;
}


