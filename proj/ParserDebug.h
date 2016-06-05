#ifndef ParserDebug_h
#define ParserDebug_h

#include <stdio.h>

#include "libs/FLOPPYParser/FLOPPYParser.h"

void printFLOPPYValue(FLOPPYValue *val);

void printFLOPPYAggregate(FLOPPYAggregateOperator op, FLOPPYValue *value);

void printFLOPPYNode(int tabLvl, FLOPPYNode *node);

void printCreateTableStatement(FLOPPYCreateTableStatement *statement);

void printInsertStatement(FLOPPYInsertStatement *statement);

void printDeleteStatement(FLOPPYDeleteStatement *statement);

void printUpdateStatement(FLOPPYUpdateStatement *statement);

void printSelectStatement(FLOPPYSelectStatement *statement);

void printDropIndexStatement(FLOPPYDropIndexStatement *statement);

void printDropTableStatement(FLOPPYDropTableStatement *statement);

void TestSQLStatement(const char* sql);


#endif /* ParserDebug_h */
