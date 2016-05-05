CC = gcc
CFLAGS = -Wall -g
LIBS  = libs/hashmap.c libs/smartalloc.c libs/TFS468/libDisk.o libs/TFS468/libTinyFS.o

all: clean

TFS468:
	wget -P ./libs/TFS468/ -N http://users.csc.calpoly.edu/~foaad/class/468/TFS468/libTinyFS.h
	wget -P ./libs/TFS468/ -N http://users.csc.calpoly.edu/~foaad/class/468/TFS468/libTinyFS.o
	wget -P ./libs/TFS468/ -N http://users.csc.calpoly.edu/~foaad/class/468/TFS468/libDisk.h
	wget -P ./libs/TFS468/ -N http://users.csc.calpoly.edu/~foaad/class/468/TFS468/libDisk.o
	wget -P ./libs/TFS468/ -N http://users.csc.calpoly.edu/~foaad/class/468/TFS468/tinyFS.h
	wget -P ./libs/TFS468/ -N http://users.csc.calpoly.edu/~foaad/class/468/TFS468/README

handinPostConfig: TFS468
	mv hashmap.* ./libs
	mv smartalloc.* ./libs

BufferManager.o: BufferManager.c
	$(CC) $(CFLAGS) -c -o $@ $^

ReadWrite.o: ReadWrite.c
	$(CC) $(CFLAGS) -c -o $@ $^

TestHarness: TestHarness.c BufferManager.o $(LIBS)
	$(CC) $(CFLAGS) -o $@ $^

TestReadWrite: TestReadWrite.c ReadWrite.o BufferManager.o $(LIBS)
	$(CC) $(CFLAGS) -o $@ $^

runTestHarness: TestHarness
	-@rm *.disk
	./TestHarness ./test_files/TestHarness1.txt

handin:
	handin dekhtyar 468-lab3 *.c *.h Makefile README

clean:
	rm -f TestHarness
	rm -f TestReadWrite
	rm -f *.o
	rm -f *.disk