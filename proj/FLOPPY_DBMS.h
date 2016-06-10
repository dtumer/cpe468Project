#ifndef FLOPPY_DBMS_h
#define FLOPPY_DBMS_h

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>

#include "libs/FLOPPYParser/FLOPPYParser.h"
#include "FLOPPYResult.h"
#include "ParserDebug.h"
#include "FLOPPYBufferManager.h"
#include "FLOPPYHeapFile.h"

#ifdef __cplusplus
extern "C" {
#endif
    

#ifdef __cplusplus
}
#endif


class FLOPPY_DBMS {
private:
    FLOPPYBufferManager *buf;
public:
    FLOPPY_DBMS(std::string diskName, int nPersistentBlocks, int nVolatileBlocks); //constructor
    ~FLOPPY_DBMS(); //destructor
    
    
    FLOPPYResult * execute(std::string sql);
    
    
    FLOPPYResult * createTable(FLOPPYCreateTableStatement *statement);
    FLOPPYResult * dropTable(FLOPPYDropTableStatement *statement);
    
    FLOPPYResult * insertRecord(FLOPPYInsertStatement *statement);
    FLOPPYResult * selectRecords(FLOPPYSelectStatement *statement);
    FLOPPYResult * deleteRecords(FLOPPYDeleteStatement *statement);
    
private:
	std::vector<FLOPPYSelectItem *>* getAggregations(FLOPPYSelectStatement *statement);
	void getAggregationsFromHaving(FLOPPYNode *havingClause, std::vector<FLOPPYSelectItem *> **retAggregations);
};

#endif /* FLOPPY_DBMS_h */
