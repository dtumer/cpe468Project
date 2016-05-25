CPP = g++
CC = gcc
CFLAGS = -Wall -g
CPPFLAGS=-fPIC -O3 -std=c++0x
LIBS  = libs/hashmap.c libs/TFS468/libDisk.o libs/TFS468/libTinyFS.o
ParserSRC= libs/FLOPPY/FLOPPY_statements/FLOPPYCreateTableStatement.cpp libs/FLOPPY/FLOPPY_statements/FLOPPYCreateColumn.cpp libs/FLOPPY/FLOPPY_statements/FLOPPYPrimaryKey.cpp libs/FLOPPY/FLOPPY_statements/FLOPPYForeignKey.cpp libs/FLOPPY/FLOPPY_statements/FLOPPYDropTableStatement.cpp libs/FLOPPY/FLOPPY_statements/FLOPPYCreateIndexStatement.cpp libs/FLOPPY/FLOPPY_statements/FLOPPYDropIndexStatement.cpp libs/FLOPPY/FLOPPY_statements/FLOPPYInsertStatement.cpp libs/FLOPPY/FLOPPY_statements/FLOPPYValue.cpp libs/FLOPPY/FLOPPY_statements/FLOPPYDeleteStatement.cpp libs/FLOPPY/FLOPPY_statements/FLOPPYNode.cpp libs/FLOPPY/FLOPPY_statements/FLOPPYUpdateStatement.cpp libs/FLOPPY/FLOPPY_statements/FLOPPYTableSpec.cpp libs/FLOPPY/FLOPPY_statements/FLOPPYSelectItem.cpp libs/FLOPPY/FLOPPY_statements/FLOPPYSelectStatement.cpp libs/FLOPPY/lex_parse/FLOPPY_parser.cpp libs/FLOPPY/lex_parse/FLOPPY_lexer.cpp libs/FLOPPY/FLOPPYParser.cpp libs/FLOPPY/FLOPPYOutput.cpp


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


TestHarness.out: TestHarness.c BufferManager.o $(LIBS)
	$(CC) $(CFLAGS) -o $@ $^

TestReadWrite.out: TestReadWrite.c ReadWrite.o BufferManager.o $(LIBS)
	$(CC) $(CFLAGS) -o $@ $^

TestHeapFile.out: TestHeapFile.c HeapFile.o FileManager.o ReadWrite.o BufferManager.o $(LIBS)
	$(CC) $(CFLAGS) -o $@ $^

TestParser.out: TestParser.cpp ParserDebug.cpp $(ParserSRC) $(LIBS)
	$(CPP) $(CPPFLAGS) -o $@ $^

runTestHarness: TestHarness.out
	-@rm *.disk
	./TestHarness.out ./test_files/TestHarness1.txt

runTestHeapFile: TestHeapFile.out
	-@rm *.disk
	./TestHeapFile.out ./test_files/TestHeapFile1.txt

runTestParser: TestParser.out
	./TestParser.out


handin:
	handin dekhtyar 468-lab3 *.c *.h Makefile README

clean:
	rm -f *.out
	rm -f *.o
	rm -f *.disk
	rm -f *.so
