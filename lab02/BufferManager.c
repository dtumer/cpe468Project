#include <limits.h>
#include <stdio.h>

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
    int *retValue;
    
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
   return BFMG_OK;
}

