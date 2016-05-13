CPP = g++
CC = gcc
CFLAGS = -Wall -g
CPPFLAGS=-fPIC -O3 -std=c++0x -Wno-deprecated-register
LIBS  = libs/hashmap.c libs/TFS468/libDisk.o libs/TFS468/libTinyFS.o
ParserSRC= libs/FLOPPY/FLOPPY_statements/FLOPPYCreateTableStatement.cpp libs/FLOPPY/FLOPPY_statements/FLOPPYCreateColumn.cpp libs/FLOPPY/FLOPPY_statements/FLOPPYPrimaryKey.cpp libs/FLOPPY/FLOPPY_statements/FLOPPYForeignKey.cpp libs/FLOPPY/lex_parse/FLOPPY_parser.cpp libs/FLOPPY/lex_parse/FLOPPY_lexer.cpp libs/FLOPPY/FLOPPYParser.cpp libs/FLOPPY/FLOPPYOutput.cpp


all: clean

TFS468:
	wget -P ./libs/TFS468/ -N http://users.csc.calpoly.edu/~foaad/class/468/TFS468/libTinyFS.h
	wget -P ./libs/TFS468/ -N http://users.csc.calpoly.edu/~foaad/class/468/TFS468/libTinyFS.o
	wget -P ./libs/TFS468/ -N http://users.csc.calpoly.edu/~foaad/class/468/TFS468/libTinyFS32.o
	wget -P ./libs/TFS468/ -N http://users.csc.calpoly.edu/~foaad/class/468/TFS468/libDisk.h
	wget -P ./libs/TFS468/ -N http://users.csc.calpoly.edu/~foaad/class/468/TFS468/libDisk.o
	wget -P ./libs/TFS468/ -N http://users.csc.calpoly.edu/~foaad/class/468/TFS468/libDisk32.o
	wget -P ./libs/TFS468/ -N http://users.csc.calpoly.edu/~foaad/class/468/TFS468/tinyFS.h

handinPostConfig: TFS468
	mv hashmap.* ./libs
	mv smartalloc.* ./libs

BufferManager.o: BufferManager.c
	$(CC) $(CFLAGS) -c -o $@ $^

ReadWrite.o: ReadWrite.c
	$(CC) $(CFLAGS) -c -o $@ $^

HeapFile.o: HeapFile.c
	$(CC) $(CFLAGS) -c -o $@ $^

FileManager.o: FileManager.c
	$(CC) $(CFLAGS) -c -o $@ $^

libParser.so:
	$(CPP) $(CPPFLAGS) $(ParserSRC) -shared -o $@

TestHarness.out: TestHarness.c BufferManager.o $(LIBS)
	$(CC) $(CFLAGS) -o $@ $^

TestReadWrite.out: TestReadWrite.c ReadWrite.o BufferManager.o $(LIBS)
	$(CC) $(CFLAGS) -o $@ $^

TestHeapFile.out: TestHeapFile.c HeapFile.o FileManager.o ReadWrite.o BufferManager.o $(LIBS)
	$(CC) $(CFLAGS) -o $@ $^

runTestHarness: TestHarness.out
	-@rm *.disk
	./TestHarness.out ./test_files/TestHarness1.txt

runTestHeapFile: TestHeapFile.out
	-@rm *.disk
	./TestHeapFile.out ./test_files/TestHeapFile1.txt

handin:
	handin dekhtyar 468-lab3 *.c *.h Makefile README

clean:
	rm -f *.out
	rm -f *.o
	rm -f *.disk
	rm -f *.so
