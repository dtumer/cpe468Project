//
//  ReadWrite.h
//
//

#include "BufferManager.h"


char * getPage(Buffer * buf, DiskAddress page);
int putPage(Buffer buf, DiskAddress page, char * data);

char * buf_read(Buffer * buf, DiskAddress page, int startOffset, int nBytes);
int buf_write(Buffer * buf, DiskAddress page, int startOffset, int nBytes, char * data);

char * readVolatile(Buffer * buf, DiskAddress page, int startOffset, int nBytes);
int writeVolatile(Buffer* buf, DiskAddress page, int startOffset, int nBytes, char * data);