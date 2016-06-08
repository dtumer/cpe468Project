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
    FLOPPYBufferManager *buf;
public:
    FLOPPY_DBMS(std::string diskName, int nPersistentBlocks, int nVolatileBlocks); //constructor
    ~FLOPPY_DBMS(); //destructor
    
    
    FLOPPYResult * execute(std::string sql);
    
    
    FLOPPYResult * createTable(FLOPPYCreateTableStatement *statement);
    FLOPPYResult * insertRecord(FLOPPYInsertStatement *statement);
    
};

#endif /* FLOPPY_DBMS_h */
