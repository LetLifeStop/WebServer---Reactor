#include <getopt.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include "EventLoop.h"
#include "Server.h"
#include "base/Logging.h"

int main(int argc, char* argv[]) {

    int threadNum = 4;
    int port = 8888;
    std::string logPath = "WebServer.log";

    int opt;
    const char *str = "t:l:p";
    // t means the number of thread 
    // l means logPath
    // p means the port
    while((opt = getopt(argc, argv, str) != -1)) {
        switch(opt) {
            case 't': {
                threadNum = atoi(optarg);
                break;
            }
            case 'l': {
                logPath = optarg;
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
    // set the name of LogFile 
   Logger::setLogFileName(logPath);
    printf("threadNum = %d, port = %d\n", threadNum, port);
    EventLoop mainLoop;
   Server myHTTPServer(&mainLoop, threadNum, port);
//#ifndef _PTREADS
//    LOG << "PTHREAD is not defined !";
//#endif
    myHTTPServer.start();
    mainLoop.loop();
    return 0;
}
