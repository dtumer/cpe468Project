#ifndef BufferManager_h
#define BufferManager_h

#include "libs/TFS468/tinyFS.h"
#include "libs/TFS468/libTinyFS.h"
#include "libs/TFS468/libDisk.h"
#include "libs/hashmap.h"
#include "libs/smartalloc.h"

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

//struct defining the actual buffer
typedef struct Buffer {
	char *database;
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
} Buffer;

/* function type for eviction policy */
typedef int (*evictFn)(Buffer *);

fileDescriptor getFileDescriptor(Buffer *buf, char *fileName);

int commence(char *database, Buffer *buf, int nPersistentBlocks, int nVolatileBlocks);
int squash(Buffer *buf);
int loadPersistentPage(Buffer *buf, DiskAddress diskPage);
int writePage(Buffer *buf, DiskAddress diskPage);
int flushPage(Buffer *buf, DiskAddress diskPage);
int pinPage(Buffer *buf, DiskAddress diskPage);
int unPinPage(Buffer *buf, DiskAddress diskPage);
int newPage(Buffer *buf, DiskAddress diskPage);

int allocateCachePage(Buffer *buf, DiskAddress diskPage);
int removeCachePage(Buffer *buf, DiskAddress diskPage);
int loadVolatilePage(Buffer *buf, DiskAddress diskPage);

//test functions
void checkpoint(Buffer * buf);
int pageDump(Buffer *buf, int index);
int printPage(Buffer *buf, DiskAddress diskPage);
int printBlock(Buffer *buf, DiskAddress diskPage);

#endif
