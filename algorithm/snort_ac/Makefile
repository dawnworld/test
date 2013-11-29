SRCS:=$(wildcard *.c) $(wildcard *.cc) $(wildcard *.cpp)
OBJECT:=$(patsubst %.cpp,%.o,$(patsubst %.c, %.o,$(patsubst %.cc, %.o, $(SRCS))))
EXECUTABLE:=dfaTest
GCC=gcc
CC=g++
USR_OPTION=-g -std=c99
#MACROS=-DACSMX2S_MAIN
#-D_S1APDEBUG -D_S1APJSONLIB"
#INCLUDES=-I/usr/include/glib-2.0
#LIBPATH=.
#LIB=-lglib-2.0 -lpthread
.SUFFIXES:
.SUFFIXES: .cpp .o .h .hh .cc  .c .cpp
.cpp.o:
	$(CC) $(USR_OPTION) $(MACROS) -c $< $(INCLUDES)
.c.o:
	$(GCC) $(USR_OPTION) $(MACROS) -c $< $(INCLUDES)

.cc.o:
	$(CC) $(USR_OPTION) $(MACROS) -c $< $(INCLUDES)

all:$(OBJECT)
	$(CC) -o $(EXECUTABLE) $(OBJECT) $(LIBPATH) $(LIB)

clean:
	-rm -f $(OBJECT) $(EXECUTABLE)
explain:
	@echo "Source files:	$(SRCS)"
	@echo "Object files:	$(OBJECT)"
