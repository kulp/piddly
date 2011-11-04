CFLAGS += -g
CFLAGS += -std=c89 -ansi -pedantic

all: main

main: piddly.o

clean:
	rm *.o main

