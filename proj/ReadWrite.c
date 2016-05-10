//
//  ReadWrite.c
//
//

#include <limits.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>

#include "ReadWrite.h"

//return the byte array with the contents of entire page
unsigned char * getPage(Buffer * buf, DiskAddress page) {
    return buf_read(buf, page, 0, PAGE_SIZE);
}

//make data the contents of a disk page
int putPage(Buffer * buf, DiskAddress page, char * data) {
    return buf_write(buf, page, 0, PAGE_SIZE, data);
}

//return a portion of the disk page
unsigned char *buf_read(Buffer * buf, DiskAddress page, int startOffset, int nBytes)
{
    int index;
    unsigned char *ptr = NULL, *data = NULL;
    
    if(nBytes > PAGE_SIZE || startOffset > PAGE_SIZE) {
        return NULL;
    }
    
    //check volatile storage
    index = loadVolatilePage(buf, page);
    
    if(index >= 0)
    {
        ptr = buf->volatilePages[index]->block;
        ptr = ptr + startOffset;
    }
    else
    {
        index = loadPersistentPage(buf, page);
        if(index < 0)
        {
            return NULL;
        }
        else
        {
            ptr = buf->persistentPages[index]->block;
            ptr = ptr + startOffset;
        }
    }
    
    
    data = calloc(nBytes, sizeof(char));
    
    if(startOffset - 1 + nBytes > PAGE_SIZE) {
        memcpy(data, ptr, (PAGE_SIZE - startOffset));
    } else {
        memcpy(data, ptr, nBytes);
    }
    
    return data;
}

//write data to disk page
int buf_write(Buffer * buf, DiskAddress page, int startOffset, int nBytes, char * data) {
    if(startOffset - 1 + nBytes > PAGE_SIZE) {
        return 1;
    }
    int index;
    unsigned char *ptr = NULL;
    
    //check volatile storage
    index = loadVolatilePage(buf, page);
    
    if(index >= 0)
    {
        ptr = buf->volatilePages[index]->block;
        ptr = ptr + startOffset;
    }
    else
    {
        index = loadPersistentPage(buf, page);
        if(index < 0)
        {
            return -1;
        }
        else
        {
            ptr = buf->persistentPages[index]->block;
            ptr = ptr + startOffset;
        }
        
    }
    
    memcpy(ptr, data, nBytes);
    
    return 0;
}

//read from volatile storage
unsigned char * readVolatile(Buffer * buf, DiskAddress page, int startOffset, int nBytes) {
    int index = loadVolatilePage(buf, page);
    unsigned char *ptr = NULL, *data = NULL;
    
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
    int index = loadVolatilePage(buf, page);
    unsigned char *ptr = NULL;
    
    if(index >= 0)
    {
        ptr = buf->volatilePages[index]->block;
    	ptr = ptr + startOffset;
    	
        memcpy(ptr, data, nBytes);
    } else {
        return -1;
    }
    
    return 0;
}
