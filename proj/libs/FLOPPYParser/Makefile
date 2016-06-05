CC=g++
CPPFLAGS=-O3 -std=c++0x -Wno-deprecated-register
FLOPPYSRC=FLOPPY_statements/FLOPPYCreateTableStatement.cpp FLOPPY_statements/FLOPPYCreateColumn.cpp FLOPPY_statements/FLOPPYPrimaryKey.cpp FLOPPY_statements/FLOPPYForeignKey.cpp FLOPPY_statements/FLOPPYDropTableStatement.cpp FLOPPY_statements/FLOPPYCreateIndexStatement.cpp FLOPPY_statements/FLOPPYDropIndexStatement.cpp FLOPPY_statements/FLOPPYInsertStatement.cpp FLOPPY_statements/FLOPPYValue.cpp FLOPPY_statements/FLOPPYDeleteStatement.cpp FLOPPY_statements/FLOPPYNode.cpp FLOPPY_statements/FLOPPYUpdateStatement.cpp FLOPPY_statements/FLOPPYTableSpec.cpp FLOPPY_statements/FLOPPYSelectItem.cpp FLOPPY_statements/FLOPPYSelectStatement.cpp
PARSERSRC=lex_parse/FLOPPY_parser.cpp lex_parse/FLOPPY_lexer.cpp
SRC=$(FLOPPYSRC) $(PARSERSRC) FLOPPYParser.cpp FLOPPYOutput.cpp main.cpp

all: release

release:
	$(CC) $(CPPFLAGS) $(SRC)

clean:
	rm a.out
