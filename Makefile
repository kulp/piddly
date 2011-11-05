CFLAGS += -g
CFLAGS += -std=c89 -ansi -pedantic-errors
CFLAGS += -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-function
#CFLAGS += -O3 -fomit-frame-pointer

all: main

main: piddly.o

piddly.o main.o: piddly.h

clean:
	rm *.o main
	rm -rf main.dSYM/

