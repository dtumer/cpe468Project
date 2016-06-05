//
//  FLOPPYBufferManager.h
//

#ifndef FLOPPYBufferManager_h
#define FLOPPYBufferManager_h

#ifdef __cplusplus
extern "C" {
#endif

#include "libs/TFS468/tinyFS.h"
#include "libs/TFS468/libTinyFS.h"
#include "libs/TFS468/libDisk.h"
#include "libs/hashmap.h"
    
#ifdef __cplusplus
}
#endif

#include <string>

#define MAX_BUFFER_SIZE 1024

/* Error code definitions */
#define BFMG_OK 0
#define BFMG_ERR -1

//structure defining a location for a page on disk
typedef struct DiskAddress {
    fileDescriptor FD;
    int pageId;
} DiskAddress;

//structure defining a block in the buffer
typedef struct Block {
    unsigned char block[BLOCKSIZE];
    DiskAddress diskAddress;
} Block;

class FLOPPYBufferManager {
public:
    FLOPPYBufferManager(std::string database, int nPersistentBlocks, int nVolatileBlocks);
    ~FLOPPYBufferManager();
    
    int loadPersistentPage(DiskAddress diskPage);
    int writePage(DiskAddress diskPage);
    int flushPage(DiskAddress diskPage);
    int pinPage(DiskAddress diskPage);
    int unPinPage(DiskAddress diskPage);
    int newPage(DiskAddress diskPage);
    
    int allocateCachePage(DiskAddress diskPage);
    int removeCachePage(DiskAddress diskPage);
    int loadVolatilePage(DiskAddress diskPage);
    
    //test functions
    void checkpoint();
    int pageDump(int index);
    int printPage(DiskAddress diskPage);
    int printBlock(DiskAddress diskPage);
    
    fileDescriptor getFileDescriptor(char *fileName);
    
    unsigned char * getPage(DiskAddress page);
    int putPage(DiskAddress page, char * data);
    
    unsigned char * buf_read(DiskAddress page, int startOffset, int nBytes);
    int buf_write(DiskAddress page, int startOffset, int nBytes, char * data);
    
    unsigned char * readVolatile(DiskAddress page, int startOffset, int nBytes);
    int writeVolatile(DiskAddress page, int startOffset, int nBytes, char * data);

    
private:
    int numDigits(int n);
    void printHashmapError(int errorCode);
    char* diskAddressToString(DiskAddress diskAdd);
    int getIndex(map_t map, DiskAddress diskAdd);
    int putIndex(map_t map, DiskAddress diskAdd, int index);
    int removeIndex(map_t map, DiskAddress diskAdd);
    Block* findPageInBuffer(int index);
    int placePageInBuffer(Block *newBlock);
    
    int persistentEvictionPolicy();
    int volatileEvictionPolicy();
    
    
    std::string database;
    int nPersistentBlocks;
    Block **persistentPages;
    unsigned long *persistentTimestamp;
    int numPersistentOccupied;
    char *pin;
    char *dirty;
    char *isVolatile;
    
    int nVolatileBlocks;
    Block **volatilePages;
    unsigned long *volatileTimestamp;
    int numVolatileOccupied;
    
    //global map variable for holding references to disk addresses and their location in the buffer
    map_t persistentMap;
    map_t volatileMap;
    
    map_t openFileMap;
    
    /* using an operation counter rather than a timestamp because it's simpler*/
    /* TODO? change to timevals, and alter the buffer struct accordingly?*/
    unsigned long ops;
};

#endif /* FLOPPYBufferManager_h */
