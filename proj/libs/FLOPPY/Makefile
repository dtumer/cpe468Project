CC=g++
CPPFLAGS=-O3 -std=c++0x -Wno-deprecated-register
FLOPPYSRC=FLOPPY_statements/FLOPPYCreateTableStatement.cpp FLOPPY_statements/FLOPPYCreateColumn.cpp FLOPPY_statements/FLOPPYPrimaryKey.cpp FLOPPY_statements/FLOPPYForeignKey.cpp
PARSERSRC=lex_parse/FLOPPY_parser.cpp lex_parse/FLOPPY_lexer.cpp
SRC=$(FLOPPYSRC) $(PARSERSRC) FLOPPYParser.cpp FLOPPYOutput.cpp main.cpp

all: release

release:
	$(CC) $(CPPFLAGS) $(SRC)

clean:
	rm a.out
