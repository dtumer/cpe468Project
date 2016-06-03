#ifndef DBMS_h
#define DBMS_h

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>

#include "libs/FLOPPY/FLOPPYParser.h"
#include "ParserDebug.h"
#include "BufferManager.h"

#ifdef __cplusplus
extern "C" {
#endif
    

#ifdef __cplusplus
}
#endif


class DBMS {
    Buffer *buf;
public:
    DBMS(std::string diskName, int nPersistentBlocks, int nVolatileBlocks); //constructor
    void execute(std::string sql);
    ~DBMS(); //destructor
    
};

#endif /* FLOPPY_DBMS_h */
