#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <iostream>

using namespace std;

# define MAXSIZE 1024
# define IPADDRESS "127.0.0.1"
# define SERV_PORT 8888
# define FDSIZE 1024
# define EPOLLEVENTS 20

int setSocketNonBlocking1(int fd) {
    int flag = fcntl(fd, F_GETFL, 0);
    if(flag == -1)
        return -1;
    
    flag |= O_NONBLOCK;
    if(fcntl(fd, F_SETFD, flag) == -1)
        return -1;
    return 0;

}
int main(int argc, char* argv[]) {
    int sockfd;
    struct sockaddr_in servaddr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, IPADDRESS, &servaddr.sin_addr);
    char buff[4096];
    buff[0] = '\0';
    // test for GET  
    const char *p = "GET";
    if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == 0) {
        setSocketNonBlocking1(sockfd);
        cout << "1:" << endl;
        ssize_t n = write(sockfd, p, strlen(p));
        cout << "strlen(p) = " << strlen(p) << endl;
        sleep(1);

        n = read(sockfd, buff, 4096);
        cout << "n = " << n << endl;
        printf("%s", buff);
        close(sockfd);
    }
    else {
        perror("err1");
    }

    //test for a tot apply 
    // GET  HTTP/1.1
    // Host: 127.7.7.1:8888
    // Content-Type: application/x-www-form-urlencoded
    // Connection: Keep-Alive
    p = "GET / HTTP/1.1\r\nHost: 127.0.0.1:8888\r\nContent-Type: ";
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == 0) {
        setSocketNonBlocking1(sockfd);
        cout << "2:" << endl;
        ssize_t n = write(sockfd, p, strlen(p));
        cout << "strlen(p) = " << strlen(p) << endl;
        sleep(1);
        n = read(sockfd, buff, 4096);
        cout << "n = " << n << endl;
        printf("%s\n", buff);
        close(sockfd);
    }
    else {
        perror("err2");
    }
        
    return 0;
}

