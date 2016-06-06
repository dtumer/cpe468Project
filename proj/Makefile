CPP = g++
CC = gcc
CFLAGS = -Wall -g -fPIC
CPPFLAGS=-O3 -std=c++0x -Wno-deprecated-register

#Directories
FLOPPY_PARSER_DIR = libs/FLOPPYParser
FLOPPY_STATEMENTS_DIR = $(FLOPPY_PARSER_DIR)/FLOPPY_statements
FLOPPY_LEX_PARSER_DIR = $(FLOPPY_PARSER_DIR)/lex_parse

LIBS  = libs/hashmap.c libs/TFS468/libDisk.o libs/TFS468/libTinyFS.o
ParserSRC = $(FLOPPY_PARSER_DIR)/libFLOPPYParser.a

all: clean

$(FLOPPY_PARSER_DIR)/%.o: $(FLOPPY_PARSER_DIR)/%.cpp $(FLOPPY_PARSER_DIR)/%.h
	$(CPP) $(CPPFLAGS) -c -o $@ $<

$(FLOPPY_STATEMENTS_DIR)/%.o: $(FLOPPY_STATEMENTS_DIR)/%.cpp $(FLOPPY_STATEMENTS_DIR)/%.h
	$(CPP) $(CPPFLAGS) -c -o $@ $<

$(FLOPPY_LEX_PARSER_DIR)/%.o: $(FLOPPY_LEX_PARSER_DIR)/%.cpp $(FLOPPY_LEX_PARSER_DIR)/%.h
	$(CPP) $(CPPFLAGS) -c -o $@ $<

$(FLOPPY_PARSER_DIR)/libFLOPPYParser.a: $(patsubst %.cpp, %.o, $(wildcard $(FLOPPY_PARSER_DIR)/*.cpp)) $(patsubst %.cpp, %.o, $(wildcard $(FLOPPY_STATEMENTS_DIR)/*.cpp)) $(patsubst %.cpp, %.o, $(wildcard $(FLOPPY_LEX_PARSER_DIR)/*.cpp))
	ar -rcs $@ $^

%.o: %.cpp %.h
	$(CPP) $(CPPFLAGS) -c -o $@ $<

HeapFile.o: HeapFile.c
	$(CC) $(CFLAGS) -c -o $@ $^

FileManager.o: FileManager.c
	$(CC) $(CFLAGS) -c -o $@ $^

TestHarness.out: TestHarness.cpp FLOPPYBufferManager.o $(LIBS)
	$(CPP) $(CPPFLAGS) -o $@ $^

TestHeapFile.out: TestHeapFile.cpp FLOPPYHeapFile.cpp FLOPPYFileManager.o FLOPPYBufferManager.cpp FLOPPYBitmap.o RecordDescription.o $(ParserSRC) $(LIBS)
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

clean:
	rm -f *.out
	rm -f *.o
	rm -f $(FLOPPY_PARSER_DIR)/*.o
	rm -f $(FLOPPY_STATEMENTS_DIR)/*.o
	rm -f $(FLOPPY_LEX_PARSER_DIR)/*.o
	rm -f *.disk
	rm -f $(FLOPPY_PARSER_DIR)/libFLOPPYParser.a
