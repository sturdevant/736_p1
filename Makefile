CC = gcc
CFLAGS = -Wall
CFLAGS += -lrt

all: asmtest forktest cachedTimer sigtest

asmtest:
	$(CC) -c -o asmtest.o asmtest.c $(CFLAGS)

forktest:
	$(CC) -c -o forktest.o forktest.c $(CFLAGS)

cachedTimer:
	$(CC) -c -o cachedTimer.o cachedTimer.c $(CFLAGS)

clean:
	rm -rf *.o
