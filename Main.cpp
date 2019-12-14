#include <getopt.h>
#include <string>
#include "EventLoop.h"
#include "Server.h"
#include "base/Logging.h"

int main(int argc, char* argv[]) {

    int threadNum = 4;
    int port = 80;
    std::string logPath = "./WebServer.log";

    int opt;
    const char *str = "t:l:p";
    while((opt = getopt(argc, argv, str) != -1)) {
        switch(opt) {
            cast 't': {
                threadNum = atoi(optar);
                break;
            }
            case 'l': {
                logPath = optarg;
                if(logPath.size() < 2 || optarg[0] != '/') {
                    printf("logPath should start with \"/\"\n");
                    abort();
                }
                break;
            }
            case 'p': {
                port = atoi(optarg);
                break;
            }
            default:
              break;
        }
    }
    Logger::setLogFileName(logPath);

#ifndef _PTREADS
    LOG << ")PTHREAD is not defined !";
#endif
    EventLoop mainLoop;
    Server myHTTPServer(&maininLoop, threadNum, port);
    myHTTPServer.start();
    mainLoop.loop();
    return 0;
}