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
    return buf_read(buf, page, 0, PAGE_SIZE);
}

//make data the contents of a disk page
int putPage(Buffer buf, DiskAddress page, char * data) {
    return buf_write(buf, page, 0, PAGE_SIZE, data);
}

//return a portion of the disk page
char *buf_read(Buffer * buf, DiskAddress page, int startOffset, int nBytes)
{
    int index;
    char *ptr = NULL, *data = NULL;
    char *data = calloc(nBytes, sizeof(char));
    
    //check cache
    index = getIndex(volatileMap, page);
    
    if(index >= 0)
    {
        ptr = cache[index]->block;
        ptr = prt + startOffset;
        
        data = calloc(nBytes, sizeof(char));
        memcpy(data, ptr, nBytes);
    }
    else
    {
        index = readPage(buf, page);
        if(index < 0)
        {
            return NULL;
        }
        else
        {
            ptr = persistentPages[index]->block;
            ptr = prt + startOffset;
        }
        
    }
    
    data = calloc(nBytes, sizeof(char));
    memcpy(data, ptr, nBytes);
    
    return data;
}

//write data to disk page
int buf_write(Buffer * buf, DiskAddress page, int startOffset, int nBytes, char * data) {
    
    return 0;
}

//read from volatile storage
char * readVolatile(Buffer * buf, DiskAddress page, int startOffset, int nBytes) {
    int index = getIndex(volatileMap, page);
    char *ptr = NULL, *data = NULL;
    
    if(index >= 0)
    {
        ptr = buf->volatilePages[index]->block;
    }
    
    if(ptr != NULL)
    {
    	ptr = ptr + startOffset;
    	data = calloc(nBytes, sizeof(char));
    	
    	memcpy(data, ptr, nBytes);
    }
    
    return data;
}

//write to volatile storage
int writeVolatile(Buffer* buf, DiskAddress page, int startOffset, int nBytes, char * data) {
    char *ptr = getVolPage(buf, page);
    
    return 0;
}
