//
//  ReadWrite.c
//
//

#include <limits.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>

#include "ReadWrite.h"
#include "libs/hashmap.h"

//return the byte array with the contents of entire page
char * getPage(Buffer * buf, DiskAddress page) {
    
    return NULL;
}

//make data the contents of a disk page
int putPage(Buffer buf, DiskAddress page, char * data) {
    
    return 0;
}

//return a portion of the disk page
char * read(Buffer * buf, DiskAddress page, int startOffset, int nBytes)
{
    
    return NULL;
}

//write data to disk page
int write(Buffer * buf, DiskAddress page, int startOffset, int nBytes, char * data) {
    
    return 0;
}

//read from volatile storage
char * readVolatile(Buffer * buf, DiskAddress page, int startOffset, int nBytes) {
    
    return NULL;
}

//write to volatile storage
int writeVolatile(Buffer* buf, DiskAddress page, int startOffset, int nBytes, char * data) {
    
    return 0;
}