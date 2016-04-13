CC = gcc
CFLAGS = -Wall -g

all: tests

TFS468:
	wget -N http://users.csc.calpoly.edu/~foaad/class/468/TFS468/libTinyFS.h
	wget -N http://users.csc.calpoly.edu/~foaad/class/468/TFS468/libTinyFS.o 
	wget -N http://users.csc.calpoly.edu/~foaad/class/468/TFS468/libDisk.h 
	wget -N http://users.csc.calpoly.edu/~foaad/class/468/TFS468/libDisk.o 
	wget -N http://users.csc.calpoly.edu/~foaad/class/468/TFS468/tinyFS.h
	wget -N http://users.csc.calpoly.edu/~foaad/class/468/TFS468/tests.c
	wget -N http://users.csc.calpoly.edu/~foaad/class/468/TFS468/README

cleanTFS468:
	rm libTinyFS.h libTinyFS.o libDisk.h libDisk.o tinyFS.h tests.c README
	rm *.1

tests: tests.c 
	$(CC) $(CFLAGS) -o tests tests.c libDisk.o libTinyFS.o

TestHarness: TestHarness.c BufferManager.c
	$(CC) $(CFLAGS) -o TestHarness TestHarness.c BufferManager.c hashmap.c smartalloc.c

testBM: testBufferManager.c BufferManager.c
	$(CC) $(CFLAGS) -o testBM BufferManager.c testBufferManager.c hashmap.c smartalloc.c libDisk.o libTinyFS.o

testHashmap: testHashmap.c
	$(CC) $(CFLAGS) -o testHashmap hashmap.c testHashmap.c smartalloc.c

cleanTestHarness:
	rm TestHarness 

clean: cleanTestHarness
