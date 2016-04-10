#include <limits.h>
#include <stdio.h>
#include <time.h>

#include "BufferManager.h"
#include "hashmap.h"
#include "smartalloc.h"

/* type for a replacement policy function 
   It returns the buffer index of the page that should be evicted */
//typedef int replacement();

//global map variable for holding references to disk addresses and their location in the buffer
map_t diskMap;

//returns the number of digits in a number
int numDigits(int n) {
   if (n < 0) n = (n == INT_MIN) ? INT_MAX : -n;
   if (n < 10) return 1;
   if (n < 100) return 2;
   if (n < 1000) return 3;
   if (n < 10000) return 4;
   if (n < 100000) return 5;
   if (n < 1000000) return 6;
   if (n < 10000000) return 7;
   if (n < 100000000) return 8;
   if (n < 1000000000) return 9;
    
   return 10;
}

void printHashmapError(int errorCode) {
    if (errorCode == -1) {
        printf("ERROR: Map is out of memory!\n");
    }
    else if (errorCode == -2) {
        printf("ERROR: Map is full!\n");
    }
    else if (errorCode == -3) {
        printf("ERROR: There is no such element in the Map!");
    }
}

//converts a disk address to a string value
char* diskAddressToString(DiskAddress diskAdd) {
    int lenFD = numDigits(diskAdd.FD);
    int lenPageId = numDigits(diskAdd.pageId);
    
    char *str = calloc(lenFD + 1 + lenPageId + 1, sizeof(char));

    sprintf(str, "%d,%d", diskAdd.FD, diskAdd.pageId);

    return str;
}

//This function wraps the hashmap get function for ease of use in this file
int getIndex(DiskAddress diskAdd) {
     char *diskStr;
     int *retValue, error;
    
     diskStr = diskAddressToString(diskAdd);
     error = hashmap_put(diskMap, diskStr, (void**)&retValue);
     free(diskStr);
    
     //print error if the map returned an error
     if (error != MAP_OK) {
         printHashmapError(error);
         return -1;
     }
     else {
         return *retValue;
    }
}

//This function wraps the hashmap put function for ease of use in this file
int putIndex(DiskAddress diskAdd, int index) {
    char *diskStr;
    int error;
    
    diskStr = diskAddressToString(diskAdd);
    error = hashmap_put(diskMap, diskStr, &index);
    free(diskStr);
    
    //print error if there is one, return 0 is no error
    if (error != MAP_OK) {
        printHashmapError(error);
        return -1;
    }
    else {
        return 0;
    }
}

//finds whether or not there is a Block in the page location specified in the buffer
int findPageInBuffer(Buffer *buf, int index, Block *retBlock) {
    if (index >= 0 && index < BUFFER_SIZE) {
        retBlock = &(buf->pages[index]);
        return 0;
    }
    else {
        return -1;
    }
}

/**
 * commence is called once at the beginning of a program that uses the buffer.
 * \param Database  name of the tinyFS disk file
 * \param buf       The buffer to create/initialize
 * \param nBlocks   how many buffer slots to create in the buffer
 *
 * Open or create a TinyFS filesystem using tfs_mount/tfs_mkfs,
 * then initialize the buffer with nBlocks buffer slots.
 * Load any pages that might be needed into the buffer. 
 * In our case, pages needed are:
 *
 * Commence returns BFMG_OK upon success, BFMG_ERR upon other failure
 * If more error codes are needed, feel free to #define them
 */
int commence(char *Database, Buffer *buf, int nBlocks) {
    return 0;
}

/**
 * squash is called exactly once at the end of the buffer manager's work.
 * It closes the buffer and finishes anything else that needs to be shut down.
 * \param buf   the buffer to destroy
 * 
 * Unpins all pinned pages, flushes remaining dirty pages to disk,
 * clears out the buffer slots, and closes the tinyFS disk. Also free()s the 
 * buffer.
 * Squash returns BFMG_OK upon success and BFMG_ERR upon failure.
 *    On failure, errno is set.
 */
int squash(Buffer *buf) {
    return BFMG_OK;
}

/**
 * This function provides read access to the specified block of the disk.
 * \param buf       the currently used buffer struct
 * \param diskPage  a TinyFS identifier for the disk page needed
 *
 * readPage guarantees that after its return, the parameter page will be in the 
 * buffer. Further calls can use the other map-access function (not yet defined)
 * to access the page read by this function.
 * returns BFMG_OK if there are no errors and BFMG_ERR if something is wrong.
 * on error, errno is also set
 */
int readPage(Buffer *buf, DiskAddress diskPage) {
    int index = getIndex(diskPage), error, retValue;
    Block *pageBlock = calloc(1, sizeof(Block));
    
    error = findPageInBuffer(buf, index, pageBlock);
    
    //check if theres a page block in the specified location
    if (error) {
        retValue = BFMG_OK;
    }
    else {
        retValue = BFMG_ERR;
    }
    
    //free temp block
    free(pageBlock);
    return retValue;
}

/**
 * This function provides write access to the specified tinyFS block.
 * \param buf       the buffer structure
 * \param diskPage  tinyFS id o the disk page to be written to
 *
 * Will not actually change the content of the disk page. Instead, it simply
 * performs the activities necessary to let the buffer manager know that a write
 * operation to the contents of a disk page has been performed.
 *
 * returns BFMG_OK if there are no errors and BFMG_ERR if there is an error.
 * on error, errno is also set
 */
int writePage(Buffer *buf, DiskAddress diskPage) {
    int index = getIndex(diskPage), error, retValue;
    Block *pageBlock = calloc(1, sizeof(Block));
    
    error = findPageInBuffer(buf, index, pageBlock);
    
    if (error) {
        buf->dirty[index] = 'T';
        buf->timestamp[index] = time(NULL);
        retValue = BFMG_OK;
    }
    else {
        retValue = BFMG_ERR;
    }
    
    //free temp block
    free(pageBlock);
    return retValue;
}

/**
 * This function flushes the given page back to disk.
 * \param buf       the buffer structure
 * \param diskPage  tinyFS id of the disk page to be flushed
 *
 * Finds a page and writes the page to disk using the tinyFS API. Also unsets the dirty
 * flag associated with the page.
 *
 * returns BFMG_OK if there are no errors and BFMG_ERR if there is an error.
 * on error, errno is also set
 */
int flushPage(Buffer *buf, DiskAddress diskPage) {
    return 0;
}

/**
 * This function pins the given page on the buffer.
 * \param buf       the buffer structure
 * \param diskPage  tinyFS id of the disk page to be flushed
 *
 * Finds a page and sets its pin flag to true. This makes it so that the buffer
 * replacement policies cannot remove this page from the buffer.
 *
 * returns BFMG_OK if there are no errors and BFMG_ERR if there is an error.
 * on error, errno is also set
 */
int pinPage(Buffer *buf, DiskAddress diskPage) {
    int index = getIndex(diskPage), error, retValue;
    Block *pageBlock = calloc(1, sizeof(Block));
    
    error = findPageInBuffer(buf, index, pageBlock);
    
    if (error) {
        buf->pin[index] = 'T';
        retValue = BFMG_OK;
    }
    else {
        retValue = BFMG_ERR;
    }
    
    free(pageBlock);
    return retValue;
}

/**
 * This function unpins the given page on the buffer.
 * \param buf       the buffer structure
 * \param diskPage  tinyFS id of the disk page to be flushed
 *
 * Finds a page and sets its pin flag to false. This allows the buffer replacement
 * policies to remove the page if necessary from the buffer.
 *
 * returns BFMG_OK if there are no errors and BFMG_ERR if there is an error.
 * on error, errno is also set
 */
int unPinPage(Buffer *buf, DiskAddress diskPage) {
    int index = getIndex(diskPage), error, retValue;
    Block *pageBlock = calloc(1, sizeof(Block));
    
    error = findPageInBuffer(buf, index, pageBlock);
    
    if (error) {
        buf->pin[index] = 'F';
        retValue = BFMG_OK;
    }
    else {
        retValue = BFMG_ERR;
    }
    
    free(pageBlock);
    return retValue;
}

/**
 * This function creates a new disk page on the tinyFS disk.
 * \param buf       the buffer structure
 * \param FD        the descriptor of the file the new page belongs to
 * \param diskPage  tinyFS id of the disk page to be flushed
 *
 * This functoin discovers the next "open" tinyFS page Id in a given file and
 * creates a tinyFS page with this ID on the tinyFS disk.
 *
 * returns BFMG_OK if there are no errors and BFMG_ERR if there is an error.
 * on error, errno is also set
 */
int newPage(Buffer *buf, fileDescriptor FD, DiskAddress *diskPage) {
    return 0;
}