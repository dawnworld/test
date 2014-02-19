.SUFFIXES: .c .o
CC=gcc
AR = ar
FLAGS = -g -Wall
LDFLAGS = 
INCLUDES = 

MAIN_FILE = test_app
#LIB_NAME=libapplink
#APP_NAME=app_test

%.o : %.c
	$(CC) -c $(FLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $<

all: $(MAIN_FILE)

$(MAIN_FILE) : batch_edit.c
	$(CC) $(FLAGS) $(INCLUDES) $(LDFLAGS) $^ -o $@
#main : solibfile staticlib main.o
#	$(CC) $(FLAGS) $(LDFLAGS) -o $(APP_NAME) main.o
#
#solibfile : app_socket_link.c
#	$(CC) -fPIC --shared $(FLAGS) -o $(LIB_NAME).so $?
#
#staticlib : app_socket_link.o
#	$(AR) -crv -o $(LIB_NAME).a $<

clean:
	rm -rf *.o *.so *.a *.o.dSYM $(MAIN_FILE)

