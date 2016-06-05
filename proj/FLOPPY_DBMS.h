#ifndef FLOPPY_DBMS_h
#define FLOPPY_DBMS_h

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>

#include "libs/FLOPPY/FLOPPYParser.h"
#include "FLOPPYResult.h"
#include "ParserDebug.h"
#include "BufferManager.h"

#ifdef __cplusplus
extern "C" {
#endif
    

#ifdef __cplusplus
}
#endif


class FLOPPY_DBMS {
    Buffer *buf;
public:
    FLOPPY_DBMS(std::string diskName, int nPersistentBlocks, int nVolatileBlocks); //constructor
    FLOPPYResult * execute(std::string sql);
    ~FLOPPY_DBMS(); //destructor
    
};

#endif /* FLOPPY_DBMS_h */
