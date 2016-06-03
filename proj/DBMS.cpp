#include "DBMS.h"

DBMS::DBMS (std::string diskName, int nPersistentBlocks, int nVolatileBlocks) {
    buf = (Buffer *)calloc(1, sizeof(Buffer)); //buffer object for page storage
    
    //create temp char*
    char *diskNameC = new char[diskName.length() + 1];
    strcpy(diskNameC, diskName.c_str());
    
    int ret = commence(diskNameC, buf, nPersistentBlocks, nVolatileBlocks);
    
    //free memory
    delete [] diskNameC;
}

void DBMS::execute(std::string sql) {
    //FLOPPYOutput *result = FLOPPYParser::parseFLOPPYString(sql);
    
    TestSQLStatement(sql.c_str());
}

DBMS::~DBMS() {
    int ret = squash(buf);
}