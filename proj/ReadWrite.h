//
//  ReadWrite.h
//
//

#ifndef ReadWrite_h
#define ReadWrite_h

#include "BufferManager.h"

#define PAGE_SIZE 1024

unsigned char * getPage(Buffer * buf, DiskAddress page);
int putPage(Buffer * buf, DiskAddress page, char * data);

unsigned char * buf_read(Buffer * buf, DiskAddress page, int startOffset, int nBytes);
int buf_write(Buffer * buf, DiskAddress page, int startOffset, int nBytes, char * data);

unsigned char * readVolatile(Buffer * buf, DiskAddress page, int startOffset, int nBytes);
int writeVolatile(Buffer* buf, DiskAddress page, int startOffset, int nBytes, char * data);


#endif
