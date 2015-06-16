# Suffixs
%.o : %.m
	$(CC) $(CCFLAGS) $(CCOBJFLAGS) $^ -o $@

# Macros
CC = clang
#CCFLAGS = -g
CCOBJFLAGS = -framework Foundation
SOURCES = $(wildcard *.m)
OBJECTS = $(SOURCES:.m=.o)

all : $(OBJECTS)
clean:
	rm -f $(OBJECTS)
