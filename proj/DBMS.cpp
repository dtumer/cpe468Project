#include "DBMS.h"


DBMS::DBMS (std::string diskName, int nPersistentBlocks, int nVolatileBlocks) {
    buf = (Buffer *)calloc(1, sizeof(Buffer)); //buffer object for page storage
    
    //create temp char*
    char *diskNameC = (char *) malloc(diskName.length() + 1);
    strcpy(diskNameC, diskName.c_str());
    //int commence(char *database, Buffer *buf, int nPersistentBlocks, int nVolatileBlocks);

    //buf->persistentTimestamp++;
    
    printf("%ld\n", buf->persistentTimestamp);
    int ret = commence(diskNameC, buf, nPersistentBlocks, nVolatileBlocks);
    
    //free memory
    delete [] diskNameC;
}