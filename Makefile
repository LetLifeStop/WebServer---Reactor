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
# 声明编译的时候引用哪个静态库,头文件搜索目录
CFLAGS	:= -std=c++11 -g -Wall -D_PTHREADS 
# 添加编译器，添加gdb调试选项， 编译后显示所有警告， 检查_PTHREADS是否被定义
CXXFLAGS:= $(CFLAGS)
# 将CFLANGS对应的变量们赋值给CXXFLAGS


SUBTARGET1 := Logging
SUBTARGET2 := HTTPClient

#.PHONY 作用是避免和同名文件冲突，如下所示
# clean:
#	rm *.o tmp
#首先，rm命令并不会产生clean文件，如果通过make执行clean文件的话，并没有什么问题
#如果目录中出现了clean为名的文件，这个命令就永远不会执行；
#而使用了.PHONY之后，make clean 就会无视clean文件存在与否，上一次更新的时间。每次clean都会执行该命令

.PHONY := objs clean veryclean rebuild all tests debug
all : $(TARGET) $(SUBTARGET1) $(SUBTARGET2)
objs : $(OBJS)
rebuild : veryclean all
tests : $(SUBTARGET1) $(SUBTARGET2)
clean : 
		find . -name '*.o' | xargs rm -f
		# 找出以.o为后缀的文件，然后通过行显示出来，再删除
veryclean :
		find . -name '*,o' | xargs rm -f
		find . -name $(TARGET) | xargs rm -f
		find . -name $(SUBTARGET1) | xargs rm -f
		find . -name $(SUBTARGET2) | xargs rm -f

debug :
		@echo $(SOURCE)
		# 如果不带@，会打印SOURCE对应的文件；如果带@，就不会打印

$(TARGET) : $(OBJS) Main.o
# 如果需要获取WebServer这个cpp文件，需要头文件中对应的.o文件还有Main.o
		$(CC) $(CFLAGS) -o  $@ $^ $(LIBS) 
		# $@ 代表目标文件，在这里代表 TARGET
		# $^ 代表所有的依赖文件
$(SUBTARGET1) : $(OBJS) base/tests/Logging.o
		$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
$(SUBTARGET2) : $(OBJS) tests/HTTPClient.o
		$(cc) $(CFLAGS) -o $@ $^ $(LIBS)



