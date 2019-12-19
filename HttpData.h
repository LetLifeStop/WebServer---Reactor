#pragma once
#include <sys/epoll.h>
#include <unistd.h>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include "Timer.h"

class EventLoop;
class TimerNode;
class Channel;

enum ProcessState{
    STATE_PARSE_URI = 1,
    STATE_PARSE_HEADERS,
    STATE_RECV_BODY,
    STATE_ANALYSIS,
    STATE_FINSH
};

enum URIState{
    PARSE_URI_AGAIN = 1,
    PARSE_URI_ERROR,
    PARSE_URI_SUCCESS
};

enum HeaderState {
    PARSE_HEADER_SUCCESS = 1,
    PARSE_HEADER_AGAIN,
    PARSE_HEADER_ERROR
};

enum AnalysisState{
    ANALYSIS_SUCCESS = 1,
    ANALYSIS_ERROR
};

enum ParseState{
    H_START = 0,
    H_KEY,
    H_COLON,
    H_SPACES_AFTER_COLON,
    H_VALUE,
    H_CR,
    H_LF,
    H_END_CR,
    H_END_LF
};

enum ConnectionState{
    H_CONNECTED = 0, 
    H_DISCONNECTING, 
    H_DISCONNECTED 
};

enum HttpMethod{
    METHOD_POST = 1,
    METHOD_GET,
    METHOD_HEAD
};

enum HttpVersion{
    HTTP_10 = 1,
    HTTP_11
};

class MimeType{
  privater:
    static void init();
    static std::unordered_map<std::string, std::string> mime;
    MimeType();
    MineType(const MimeType &m);

  public:
    static std::string getMime(const std::string &suffix);

  private:
  //pthread_once_t 能保证定义的变量只会初始化一次
    static pthread_once_t once_control;
  
};
// template<class T>enable_shared_from_this
// 如果一个类继承std::enable_shared_from_this，会给该类提供成员函数，当T类型对象t被一个名称为pt的shared_ptr<T>类对象管理的时候，调用T::shared_from_this成员函数，将会返回一个新的shared_ptr指针
class HttpData : public std::enable_shared_from_this<HttpData> {
    public:
      HttpData(EventLoop *loop, int connfd);
      ~HttpData(){ close(fd_); }
      void reset();
      void seperateTimer();
      void linkTimer(std::shared_ptr<TimerNode> mtimer){
          timer_ = mtimer;
      }
      std::shared_ptr<Channel>getChannel(){ return channel_; }
      EventLop *getLoop() { return loop_; }
      void handleClose();
      void newEvent();

    private:
    EventLoop *loop_;
    std::shared_ptr<Channel> channel_;
    int fd_;
    std::string inBuffer_;
    std::string outBuffer_;
    bool error_;
    ConnectionState connectionState_;

    HttpMethod method_;
    HttpVersion HTTPVersion_;
    std::string fileName_;
    int nowReadPos_;
    ProcessState state_;
    ParseState hState_;
    bool keepAlive_;
    std::map<std::string, std::string> headers_;
    std::weak_ptr<TimerNode> timer_;

    void handleRead();
    void handleWrite();
    void handleError(int fd, int err_num, std::string short_msg);
    URIState parseURI();
    HeaderState parseHeaders();
    AnalysisState analysisRequest();
};




