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

int main(int argc, char* argv[]) {
    int sockfd;
    struct sockaddr_in servddr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&serveraddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, IPADDRESS, &servaddr.sin_addr);
    char buff[4096];
    buff[0] = '\0';

    const char *p = " ";

    
    return 0;
}

