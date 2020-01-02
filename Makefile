MAINSOURCE := Main.cpp base/tests/Logging.cpp tests/HTTPClient.cpp
SOURCE 	:= $(wildcard *.cpp base/*.cpp tests/HTTPClient.cpp)
# wildcard的作用是获取这三个目录下的所有的cpp文件
override SOURCE := $(filter-out $(MAINSOURCE),$(SOURCE))	
# override的作用是防止SOURCE对应的变量值被命令行修改
# fileter-out能从SOURCE中过滤出MAINSOURCE所包含的变量值
# 也就是说此时SOURCE对应的是 去除测试代码之后的变量值们
OBJS	:= $(patsubst %.cpp,%.o,$(SOURCE))
#把$(SOURCE)中的变量符合后缀含有.cpp的变量替换为.o

#MAINSOURCE 用来存储主函数，日志测试函数，客户端测试函数
#SOURCE 用来存储 除了MAINSOURCE中的三个cpp 以外的cpp文件
#OBJS s用来存储SOURCE对应的cpp文件的.o文件

TARGET	:= WebServer
# 最终生成的目标
CC		:= g++
# 采用的编译方法
LIBS	:= -lpthread
# 有线程的存在
INCLUDE	:= -I./usr/local/lib
# 声明编译的时候引用哪个静态库
CFLAGS	:= -std=c++11 -g -Wall -D_PTHREADS 
# 添加编译器，添加gdb调试选项， 编译后显示所有警告， 编译带有线程的程序
CXXFLAGS:= $(CFLAGS)


