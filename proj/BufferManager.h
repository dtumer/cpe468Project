#include "libs/TFS468/tinyFS.h"
#include "libs/TFS468/libTinyFS.h"
#include "libs/TFS468/libDisk.h"



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
	int nBufferBlocks;
    int nCacheBlocks;
	Block **persistentPages;
    Block **volatilePages;
	unsigned long *timestamp;
    unsigned long *volatileTimestamp;
	char *pin;
	char *dirty;
    char *isVolatile;
	int numBufferOccupied;
    int numCacheOccupied;
} Buffer;

/* function type for eviction policy */
typedef int (*evictFn)(Buffer *);

int commence(char *database, Buffer *buf, int nBufferBlocks, int nCacheBlocks);
int squash(Buffer *buf);
int loadPage(Buffer *buf, DiskAddress diskPage);
int readPage(Buffer *buf, DiskAddress diskPage);
int writePage(Buffer *buf, DiskAddress diskPage);
int flushPage(Buffer *buf, DiskAddress diskPage);
int pinPage(Buffer *buf, DiskAddress diskPage);
int unPinPage(Buffer *buf, DiskAddress diskPage);
int newPage(Buffer *buf, DiskAddress diskPage);
int allocateCachePage(Buffer *buf, DiskAddress diskPage);
int removeCachePage(Buffer *buf, DiskAddress diskPage);

//test functions
void checkpoint(Buffer * buf);
int pageDump(Buffer *buf, int index);
int printPage(Buffer *buf, DiskAddress diskPage);
int printBlock(Buffer *buf, DiskAddress diskPage);
