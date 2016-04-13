#include "tinyFS.h"
#include "libTinyFS.h"
#include "libDisk.h"

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
   char block[BLOCKSIZE];
   DiskAddress diskAddress;
} Block;

//struct defining the actual buffer
typedef struct Buffer {
   char *database;
   int nBlocks;
   Block *pages;
   long *timestamp;
   char *pin;
   char *dirty;
   int numOccupied;
} Buffer;

int commence(char *database, Buffer *buf, int nBlocks);
int squash(Buffer *buf);
int readPage(Buffer *buf, DiskAddress diskPage);
int writePage(Buffer *buf, DiskAddress diskPage);
int flushPage(Buffer *buf, DiskAddress diskPage);
int pinPage(Buffer *buf, DiskAddress diskPage);
int unPinPage(Buffer *buf, DiskAddress diskPage);
int newPage(Buffer *buf, fileDescriptor FD, DiskAddress *diskPage);
