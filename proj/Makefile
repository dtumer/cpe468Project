CPP = g++
CC = gcc
CFLAGS = -Wall -g -fPIC
CPPFLAGS=-O3 -std=c++0x -Wno-deprecated-register
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

FLOPPYBufferManager.o: FLOPPYBufferManager.cpp
	$(CPP) $(CPPFLAGS) -c -o $@ $^

HeapFile.o: HeapFile.c
	$(CC) $(CFLAGS) -c -o $@ $^

FileManager.o: FileManager.c
	$(CC) $(CFLAGS) -c -o $@ $^

FLOPPY_DBMS.o: FLOPPY_DBMS.cpp
	$(CPP) $(CPPFLAGS) -c -o $@ $^

TestHarness.out: TestHarness.cpp FLOPPYBufferManager.o $(LIBS)
	$(CPP) $(CPPFLAGS) -o $@ $^

TestHeapFile.out: TestHeapFile.cpp FLOPPYHeapFile.cpp FileManager.o FLOPPYBufferManager.cpp $(LIBS)
	$(CPP) $(CPPFLAGS) -o $@ $^

TestParser.out: TestParser.cpp ParserDebug.cpp $(ParserSRC) $(LIBS)
	$(CPP) $(CPPFLAGS) -o $@ $^

FLOPPY_CLI.out: FLOPPY_CLI.cpp ParserDebug.cpp FLOPPYResult.cpp FLOPPYBufferManager.o FLOPPY_DBMS.o $(ParserSRC) $(LIBS)
	$(CPP) $(CPPFLAGS) -o $@ $^

runTestHarness: TestHarness.out
	-@rm *.disk
	./TestHarness.out ./test_files/TestHarness1.txt

runTestHeapFile: TestHeapFile.out
	-@rm *.disk
	./TestHeapFile.out ./test_files/TestHeapFile1.txt

runTestParser: TestParser.out
	./TestParser.out

runFLOPPY_CLI: FLOPPY_CLI.out
	-@rm *.disk
	./FLOPPY_CLI.out

handin:
	handin dekhtyar 468-lab3 *.c *.h Makefile README

clean:
	rm -f *.out
	rm -f *.o
	rm -f *.disk
	rm -f *.so
