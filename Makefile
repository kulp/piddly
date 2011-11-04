CFLAGS += -g
CFLAGS += -std=c89 -ansi -pedantic

all: main

main: piddle.o

clean:
	rm *.o main

