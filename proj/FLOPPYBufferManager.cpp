//
//  FLOPPYBufferManager.cpp
//

#include "FLOPPYBufferManager.h"

#include <limits.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string>
#include <string.h>


#include <iostream>

/**
 * commence is called once at the beginning of a program that uses the buffer.
 * \param Database  name of the tinyFS disk file
 * \param buf       The buffer to create/initialize
 * \param nPersistentBlocks   how many buffer slots to create in the buffer
 *
 * Open or create a TinyFS filesystem using tfs_mount/tfs_mkfs,
 * then initialize the buffer with nPersistentBlocks buffer slots.
 * Load any pages that might be needed into the buffer.
 * In our case, pages needed are:
 *
 * Commence requires that the buffer be a valid pointer (i.e. retrieved from malloc or
 *  the address of a local variable).
 *
 * Commence returns BFMG_OK upon success, BFMG_ERR upon other failure
 * If more error codes are needed, feel free to #define them
 */
FLOPPYBufferManager::FLOPPYBufferManager(std::string database, int nPersistentBlocks, int nVolatileBlocks) {
    int tfsErr;
    
    //open disks
    char *diskNameC = new char[database.length() + 1];
    strcpy(diskNameC, database.c_str());
    
    tfsErr = tfs_mount(diskNameC);
    
    if (tfsErr != 0) {
        tfs_mkfs(diskNameC, DEFAULT_DISK_SIZE);
        tfsErr = tfs_mount(diskNameC);
    }
    delete diskNameC;
    
    //copy database name over
    this->database = database;
    
    
    //buffer
    ops = 0; //set initial op count to 0
    
    this->nPersistentBlocks = nPersistentBlocks;
    /* allocate the page arrays for nPersistentBlocks pages */
    persistentPages = (Block **)calloc(nPersistentBlocks, sizeof(Block*));
    persistentTimestamp = (unsigned long *)calloc(nPersistentBlocks, sizeof(unsigned long));
    pin = (char *)calloc(nPersistentBlocks, sizeof(char));
    dirty = (char *)calloc(nPersistentBlocks, sizeof(char));
    isVolatile = (char *)calloc(nPersistentBlocks, sizeof(char));
    /* a new buffer has no pages occupied */
    numPersistentOccupied = 0;
    
    //volatile
    volatilePages = (Block **)calloc(nVolatileBlocks, sizeof(Block*));
    volatileTimestamp = (unsigned long *)calloc(nVolatileBlocks, sizeof(unsigned long));
    this->nVolatileBlocks = nVolatileBlocks;
    numVolatileOccupied = 0;
    
    /* initialize the hashmaps */
    persistentMap = hashmap_new();
    volatileMap = hashmap_new();
    openFileMap = hashmap_new();
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
FLOPPYBufferManager::~FLOPPYBufferManager() {
    int tfsErr, i;
    Block *pageBlock;
    
    //clears all Persistent slots
    for (i = 0; i < nPersistentBlocks; i++) {
        pageBlock = persistentPages[i];
        
        if (pageBlock != NULL) {
            //unpin page
            if (pin[i] == 'T') {
                pin[i] = 'F';
            }
            
            //flush if dirty
            if (dirty[i] == 'T') {
                tfs_writePage(pageBlock->diskAddress.FD, pageBlock->diskAddress.pageId, pageBlock->block);
                dirty[i] = 'F';
            }
            
            //free the page
            free(pageBlock);
            persistentPages[i] = NULL;
        }
    }
    
    //clears all Volatile slots
    for (i = 0; i < nVolatileBlocks; i++) {
        pageBlock = volatilePages[i];
        
        if (pageBlock != NULL) {
            //free the page
            free(pageBlock);
            volatilePages[i] = NULL;
        }
    }
    
    //free hashmaps
    hashmap_free(persistentMap);
    hashmap_free(volatileMap);
    hashmap_free(openFileMap);
    
    //free buffer
    //free(database);
    free(persistentPages);
    free(volatilePages);
    free(persistentTimestamp);
    free(isVolatile);
    free(volatileTimestamp);
    free(pin);
    free(dirty);
    
    //closes tinyFS disk associated with buffer
    tfsErr = tfs_unmount();
}

/* Helper Functions */

//returns the number of digits in a number
int FLOPPYBufferManager::numDigits(int n) {
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

void FLOPPYBufferManager::printHashmapError(int errorCode) {
    if (errorCode == MAP_OMEM) {
        printf("ERROR: Map is out of memory!\n");
    }
    else if (errorCode == MAP_FULL) {
        printf("ERROR: Map is full!\n");
    }
    else if (errorCode == MAP_MISSING) {
        printf("ERROR: There is no such element in the Map!\n");
    }
}

/**
 * get the FD by filename
 * if it does not exist it create it
 * if it exists but it is not open, open it
 * if it is not open but exists it return it
 */
fileDescriptor FLOPPYBufferManager::getFileDescriptor(char *fileName) {
    fileDescriptor fd = -1;
    int error = hashmap_get(openFileMap, fileName, &fd);
    
    //print error if the map returned an error
    if (error == MAP_MISSING) {
        fd = tfs_openFile(fileName);
        error = hashmap_put(openFileMap, fileName, fd);
    }
    
    if (error != MAP_OK) {
        printHashmapError(error);
        return BFMG_ERR;
    }
    return fd;
}

//converts a disk address to a string value
char* FLOPPYBufferManager::diskAddressToString(DiskAddress diskAdd) {
    int lenFD = numDigits(diskAdd.FD);
    int lenPageId = numDigits(diskAdd.pageId);
    char *str = (char *)calloc(lenFD + 1 + lenPageId + 2, sizeof(char));
    
    sprintf(str, "%d,%d", diskAdd.FD, diskAdd.pageId);
    
    return str;
}

/* This function wraps the hashmap get function for ease of use in this file
 Used for looking up the index in buffer of a given page */
int FLOPPYBufferManager::getIndex(map_t map, DiskAddress diskAdd) {
    char *diskStr;
    int retValue, error;
    
    diskStr = diskAddressToString(diskAdd);
    error = hashmap_get(map, diskStr, &retValue);
    free(diskStr);
    
    //print error if the map returned an error
    if (error == MAP_MISSING) {
        return BFMG_ERR;
    } else if (error != MAP_OK) {
        printHashmapError(error);
        return BFMG_ERR;
    } else {
        return retValue;
    }
}

//This function wraps the hashmap put function for ease of use in this file
int FLOPPYBufferManager::putIndex(map_t map, DiskAddress diskAdd, int index) {
    char *diskStr;
    int error;
    
    diskStr = diskAddressToString(diskAdd);
    error = hashmap_put(map, diskStr, index);
    free(diskStr);
    
    //print error if there is one, return 0 is no error
    if (error != MAP_OK) {
        printHashmapError(error);
        return BFMG_ERR;
    } else {
        return BFMG_OK;
    }
}

/* remove a key from the hashmap */
int FLOPPYBufferManager::removeIndex(map_t map, DiskAddress diskAdd) {
    char *diskStr;
    int error;
    
    diskStr = diskAddressToString(diskAdd);
    error = hashmap_remove(map, diskStr);
    free(diskStr);
    
    /* print error if there is one */
    if (error != MAP_OK) {
        printHashmapError(error);
        return BFMG_ERR;
    } else {
        return BFMG_OK;
    }
}

//finds whether or not there is a Block in the page location specified in the buffer
Block* FLOPPYBufferManager::findPageInBuffer(int index) {
    if (index >= 0 && index < nPersistentBlocks) {
        return persistentPages[index];
    } else {
        return NULL;
    }
}

/**
 * evictPage takes a page to load into the buffer, uses the
 * eviction algorithm to find and remove a page (flushing it if necessary),
 * and places the input page at the correct spot.
 * It also free()s the page if one is evicted.
 *
 * if there are empty buffer slots, put the page in the first empty one.
 * if not, ask the eviction policy.
 */
int FLOPPYBufferManager::placePageInBuffer(Block *newBlock) {
    int toEvict;
    
    int insertNdx = getIndex(persistentMap, newBlock->diskAddress);
    
    if (insertNdx == -1) { //need to fetch it
        //check for open page
        if (numPersistentOccupied < nPersistentBlocks) {
            insertNdx = numPersistentOccupied;
            numPersistentOccupied++;
        } else {
            toEvict = persistentEvictionPolicy();
            if (dirty[toEvict] == 'T') {
                if (flushPage(persistentPages[toEvict]->diskAddress) == BFMG_ERR) {
                    fprintf(stderr, "placePageInBuffer: failed to flush dirty page\n");
                    return BFMG_ERR;
                }
            }
            removeIndex(persistentMap, persistentPages[toEvict]->diskAddress);
            fprintf(stderr, "info: freeing the page at %d\n", toEvict);
            free(persistentPages[toEvict]);
            
            insertNdx = toEvict;
        }
        
        persistentPages[insertNdx] = newBlock;
        pin[insertNdx] = 'F';
        dirty[insertNdx] = 'F';
        isVolatile[insertNdx] = 'F';
        persistentTimestamp[insertNdx] = ops++;
        putIndex(persistentMap, persistentPages[insertNdx]->diskAddress, insertNdx);
        
    }
    return insertNdx;
}


/* External Functions */


/**
 * This function provides access to the specified block of data in the persistent buffer.
 * \param buf       the currently used buffer struct
 * \param diskPage  a TinyFS identifier for the disk page needed
 *
 * If the data is not in the persistent buffer it will load it from disk using the
 * buffer replacement policy
 *
 * returns the index of the requested page, or BFMG_ERR if something is wrong.
 * on error, errno is also set
 */
int FLOPPYBufferManager::loadPersistentPage(DiskAddress diskPage) {
    int result = BFMG_OK;
    Block *newBlock;
    int existingIndex = getIndex(persistentMap, diskPage);
    
    if (existingIndex == -1) {
        newBlock = (Block *)malloc(sizeof(Block));
        newBlock->diskAddress = diskPage;
        
        result = tfs_readPage(diskPage.FD, diskPage.pageId,
                              newBlock->block);
        if (result != 0) {
            fprintf(stderr, "tfs_readPage returned %d\n", result);
        }
        
        result = placePageInBuffer(newBlock);
    } else {
        persistentTimestamp[existingIndex] = ops++;
    }
    
    return result;
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
int FLOPPYBufferManager::writePage(DiskAddress diskPage) {
    int index = getIndex(persistentMap, diskPage);
    Block *pageBlock = findPageInBuffer(index);
    
    if (pageBlock) {
        dirty[index] = 'T';
        persistentTimestamp[index] = ops++;
        return BFMG_OK;
    } else {
        return BFMG_ERR;
    }
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
int FLOPPYBufferManager::flushPage(DiskAddress diskPage) {
    int index = getIndex(persistentMap, diskPage);
    Block *pageBlock = findPageInBuffer(index);
    
    //Exits if page is not in the buffer
    if (!pageBlock) {
        return BFMG_ERR;
    }
    
    //handle if it is volatile
    if(isVolatile[index] == 'T')
    {
        //need to create file?
    }
    
    //Writes the selected page's data to disk
    tfs_writePage(pageBlock->diskAddress.FD, pageBlock->diskAddress.pageId, pageBlock->block);
    
    //Unset dirty flag
    dirty[index] = 'F';
    
    return BFMG_OK;
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
int FLOPPYBufferManager::pinPage(DiskAddress diskPage) {
    int index = getIndex(persistentMap, diskPage);
    Block *pageBlock = findPageInBuffer(index);
    
    if (pageBlock) {
        pin[index] = 'T';
        return BFMG_OK;
    } else {
        return BFMG_ERR;
    }
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
int FLOPPYBufferManager::unPinPage(DiskAddress diskPage) {
    int index = getIndex(persistentMap, diskPage);
    Block *pageBlock = findPageInBuffer(index);
    
    if (pageBlock) {
        pin[index] = 'F';
        return BFMG_OK;
    } else {
        return BFMG_ERR;
    }
}

/**
 * This function creates a new disk page on the tinyFS disk.
 * \param buf       the buffer structure
 * \param FD        the descriptor of the file the new page belongs to
 * \param diskPage  tinyFS id of the disk page to be flushed
 *
 * This function discovers the next "open" tinyFS page Id in a given file and
 * creates a tinyFS page with this ID on the tinyFS disk.
 *
 * returns BFMG_OK if there are no errors and BFMG_ERR if there is an error.
 * on error, errno is also set
 */
int FLOPPYBufferManager::newPage(DiskAddress diskPage) {
    int result;
    Block *pageBlock = (Block *)calloc(1, sizeof(Block));
    
    pageBlock->diskAddress = diskPage;
    
    tfs_writePage(diskPage.FD, diskPage.pageId, pageBlock->block);
    
    //do eviction
    result = placePageInBuffer(pageBlock);
    
    return BFMG_OK;
}


int FLOPPYBufferManager::allocateCachePage(DiskAddress diskPage) {
    int i, bufNdx, index, insertNdx = -1;
    Block *newBlock, *tempBlock;
    
    //check not already in volatile storage
    index = getIndex(volatileMap, diskPage);
    if(index != -1) {
        fprintf(stderr, "INFO: page already in Cache: %d\n", index);
        return BFMG_ERR;
    }
    
    //find first open slot in volatile storage
    if(numVolatileOccupied < nVolatileBlocks) {
        for(i=0; i<nVolatileBlocks; i++) {
            if(volatilePages[i] == NULL) {
                insertNdx = i;
                break;
            }
        }
    }
    
    //evict from volatile storage to persistent storage if
    //volatile storage is full
    if(insertNdx == -1) {
        insertNdx = volatileEvictionPolicy();
        tempBlock = volatilePages[insertNdx];
        
        removeIndex(volatileMap, tempBlock->diskAddress);
        
        bufNdx = placePageInBuffer(tempBlock);
        if(bufNdx == BFMG_ERR) {
            fprintf(stderr, "ERROR: failed to move page from Cache to Buffer\n");
            return BFMG_ERR;
        } else {
            isVolatile[bufNdx] = 'T';
        }
    }
    
    //create new block
    newBlock = (Block *)calloc(1, sizeof(Block));
    newBlock->diskAddress = diskPage;
    
    //write block to volatile storage
    volatilePages[insertNdx] = newBlock;
    volatileTimestamp[insertNdx] = ops++;
    numVolatileOccupied += numVolatileOccupied < nVolatileBlocks ? 1 : 0;
    putIndex(volatileMap, newBlock->diskAddress, insertNdx);
    
    return BFMG_ERR;
}


int FLOPPYBufferManager::removeCachePage(DiskAddress diskPage) {
    Block *volatilePage;
    int result;
    
    //check volatile storage
    result = getIndex(volatileMap, diskPage);
    if (result != -1) {
        // page is in volatile storage
        volatilePage = volatilePages[result];
        free(volatilePage);
        removeIndex(volatileMap, diskPage);
        numVolatileOccupied--;
        volatilePages[result] = NULL;
    } else {
        // in buffer or on disk
        result = getIndex(persistentMap, diskPage);
        
        if (result != -1) {
            //drop the volatile page out of the volatile storage
            volatilePage = persistentPages[result];
            free(volatilePage);
            removeIndex(persistentMap, diskPage);
            
            //move the last entry in the buffer to the vacated slot
            persistentPages[result] = persistentPages[numPersistentOccupied - 1];
            removeIndex(persistentMap, persistentPages[numPersistentOccupied - 1]->diskAddress);
            putIndex(persistentMap, persistentPages[result]->diskAddress, result);
            persistentPages[numPersistentOccupied - 1] = NULL;
            numPersistentOccupied--;
        } else {
            /* the page was evicted to disk from the buffer, or it never existed*/
            /* what do? */
        }
    }
    return BFMG_OK;
}

/**
 * This function provides access to the specified block of data in the persistent buffer.
 * \param buf       the currently used buffer struct
 * \param diskPage  a TinyFS identifier for the disk page needed
 *
 * If the data is not in the persistent buffer it will load it from disk using the
 * buffer replacement policy
 *
 * returns the index of the requested page, or BFMG_ERR if something is wrong.
 * on error, errno is also set
 */
int FLOPPYBufferManager::loadVolatilePage(DiskAddress diskPage) {
    int existingIndex = getIndex(volatileMap, diskPage);
    
    if (existingIndex > -1) {
        volatileTimestamp[existingIndex] = ops++;
    }
    
    return existingIndex;
}

//return the byte array with the contents of entire page
unsigned char * FLOPPYBufferManager::getPage(DiskAddress page) {
    return buf_read(page, 0, BLOCKSIZE);
}

//make data the contents of a disk page
int FLOPPYBufferManager::putPage(DiskAddress page, char * data) {
    return buf_write(page, 0, BLOCKSIZE, data);
}

//return a portion of the disk page
unsigned char* FLOPPYBufferManager::buf_read(DiskAddress page, int startOffset, int nBytes)
{
    int index;
    unsigned char *ptr = NULL, *data = NULL;
    
    if(nBytes > BLOCKSIZE || startOffset > BLOCKSIZE) {
        return NULL;
    }
    
    //check volatile storage
    index = loadVolatilePage(page);
    
    if(index >= 0)
    {
        ptr = volatilePages[index]->block;
        ptr = ptr + startOffset;
    }
    else
    {
        index = loadPersistentPage(page);
        if(index < 0)
        {
            return NULL;
        }
        else
        {
            ptr = persistentPages[index]->block;
            ptr = ptr + startOffset;
        }
    }
    
    
    data = (unsigned char *) calloc(nBytes, sizeof(char));
    
    if(startOffset - 1 + nBytes > BLOCKSIZE) {
        memcpy(data, ptr, (BLOCKSIZE - startOffset));
    } else {
        memcpy(data, ptr, nBytes);
    }
    
    return data;
}

//write data to disk page
int FLOPPYBufferManager::buf_write(DiskAddress page, int startOffset, int nBytes, char * data) {
    if(startOffset - 1 + nBytes > BLOCKSIZE) {
        return 1;
    }
    int index;
    unsigned char *ptr = NULL;
    
    //check volatile storage
    index = loadVolatilePage(page);
    
    if(index >= 0)
    {
        ptr = volatilePages[index]->block;
        ptr = ptr + startOffset;
    }
    else
    {
        index = loadPersistentPage(page);
        if(index < 0)
        {
            return -1;
        }
        else
        {
            ptr = persistentPages[index]->block;
            ptr = ptr + startOffset;
        }
    }
    
    memcpy(ptr, data, nBytes);
    
    return 0;
}

//read from volatile storage
unsigned char * FLOPPYBufferManager::readVolatile(DiskAddress page, int startOffset, int nBytes) {
    int index = loadVolatilePage(page);
    unsigned char *ptr = NULL, *data = NULL;
    
    if(index >= 0)
    {
        ptr = volatilePages[index]->block;
    }
    
    if(ptr != NULL)
    {
        ptr = ptr + startOffset;
        data = (unsigned char *) calloc(nBytes, sizeof(char));
        
        memcpy(data, ptr, nBytes);
    }
    
    return data;
}

//write to volatile storage
int FLOPPYBufferManager::writeVolatile(DiskAddress page, int startOffset, int nBytes, char * data) {
    int index = loadVolatilePage(page);
    unsigned char *ptr = NULL;
    
    if(index >= 0)
    {
        ptr = volatilePages[index]->block;
        ptr = ptr + startOffset;
        
        memcpy(ptr, data, nBytes);
    } else {
        return -1;
    }
    
    return 0;
}

/* Test Functions */

/**
 * This function prints the current state of the buffer.
 * \param buf       the buffer structure
 *
 * Prints general information about the buffer is printed:
 *    what disk it is associated with
 *    slot ussage counts
 *
 * For each disk slot the following is printed:
 *    tinyFS blockID (indicates if empty)
 *    persistentTimestamps for the block
 *    pin flag
 *    dirty flag
 *    any other attributes about the slot
 */
void FLOPPYBufferManager::checkpoint() {
    int i;
    
    printf("Disk: %s\n", this->database.c_str());
    printf("Buffer Slots Occupied: %d\n", numPersistentOccupied);
    
    for(i=0; i < nPersistentBlocks; i++) {
        if(i >= numPersistentOccupied) {
            printf("Buffer Slot %d is empty\n", i);
        } else {
            printf("Buffer Slot %d:\n", i);
            printf("\ttinyFS FD: %d\n", persistentPages[i]->diskAddress.FD);
            printf("\ttinyFS blockID: %d\n", persistentPages[i]->diskAddress.pageId);
            printf("\tpersistentTimestamps for the block: %ld\n", persistentTimestamp[i]);
            printf("\tpin flag: %d\n", pin[i]);
            printf("\tdirty flag: %d\n", dirty[i]);
        }
    }
    
    printf("Cache Slots Occupied: %d\n", numVolatileOccupied);
    
    for(i=0; i < nVolatileBlocks; i++) {
        if(volatilePages[i] == NULL) {
            printf("Cache Slot %d is empty\n", i);
        } else {
            printf("Cache Slot %d:\n", i);
            printf("\ttinyFS FD: %d\n", volatilePages[i]->diskAddress.FD);
            printf("\ttinyFS blockID: %d\n", volatilePages[i]->diskAddress.pageId);
        }
    }
}

/**
 * This function prints the of contents of a page at the specified buffer slot.
 * \param buf       the buffer structure
 * \param diskPage  the index of a buffer slot
 *
 * returns BFMG_OK if the slot exists and BFMG_ERR if it does not.
 */
int FLOPPYBufferManager::pageDump(int index) {
    if (index >= 0 && index < nPersistentBlocks) {
        fwrite(&(persistentPages[index]), BLOCKSIZE, 1, stdout);
        return BFMG_OK;
    }
    
    return BFMG_ERR;
}

/**
 * This function prints the of contents of the specified diskPage.
 * \param buf       the buffer structure
 * \param index     tinyFS id of the disk page to print
 *
 * if page is in buffer it is printed
 * if page does not exist an error is printed
 * if page exits but it's not in the buffer an error is printed
 *
 * returns BFMG_OK if there are no errors and BFMG_ERR if there is an error.
 */
int FLOPPYBufferManager::printPage(DiskAddress diskPage) {
    int index = getIndex(persistentMap, diskPage);
    int numpages = tfs_numPages(diskPage.FD);
    
    if(diskPage.pageId > numpages) {
        printf("Page does not exist on disk");
        return BFMG_ERR;
    }
    
    if(index == -1) {
        printf("Page not found in buffer");
        return BFMG_ERR;
    }
    
    Block *pageBlock = findPageInBuffer(index);
    fwrite(pageBlock, BLOCKSIZE, 1, stdout);
    
    return BFMG_OK;
}

/**
 * This function prints the of contents of the specified diskPage directly from the tinyFS disk.
 * \param buf       the buffer structure
 * \param index     tinyFS id of the disk page to print
 *
 * returns BFMG_OK if there are no errors and BFMG_ERR if there is an error.
 */
int FLOPPYBufferManager::printBlock(DiskAddress diskPage) {
    unsigned char block[BLOCKSIZE];
    
    tfs_readPage(diskPage.FD, diskPage.pageId, block);
    fwrite(block, BLOCKSIZE, 1, stdout);
    
    return BFMG_OK;
}

/*
 * lru_persistentEvict analyzes the state of the input buffer and returns the slot which should be flushed/dropped,
 * and returns that. Called by loadPersistentPage
 * loadPersistentPage and writePage update the timestamp for
 * blocks used.
 * Least Recently Used algorithm finds the oldest
 * timestamp (i.e. the block which has been inactive
 * for longest) and returns that index.
 *
 * Priority:
 * 1. Unpinned dirty pages (flush and replace)
 * 2. pick from the unpinned, un-dirty pages.
 * 3. volatile pages
 */
int FLOPPYBufferManager::persistentEvictionPolicy() {
    int i, oldestDirtyIndex = -1, oldestCleanIndex = -1, oldestVolIndex = -1;
    //oldest clean/dirty pages- start from current operation count
    unsigned long oldestDirtyPage = ops + 1, oldestCleanPage = ops + 1, oldestVolPage = ops + 1;;
    
    if (numPersistentOccupied < nPersistentBlocks) {
        fprintf(stderr, "WARNING: lru_persistentEvict called on a non-full buffer\n");
    }
    
    //find the oldest pages by iterating through timestamps
    for (i = 0; i < nPersistentBlocks; i++) {
        if (pin[i] == 'F') {
            if (isVolatile[i] == 'T') {
                if (persistentTimestamp[i] < oldestVolPage) {
                    oldestVolPage = persistentTimestamp[i];
                    oldestVolIndex = i;
                }
            } else if (dirty[i] == 'T') {
                if (persistentTimestamp[i] < oldestDirtyPage) {
                    oldestDirtyPage = persistentTimestamp[i];
                    oldestDirtyIndex = i;
                }
            } else {
                if(persistentTimestamp[i] < oldestCleanPage) {
                    oldestCleanPage = persistentTimestamp[i];
                    oldestCleanIndex = i;
                }
            }
        }
    }
    
    //if there is an unpinned dirty page
    if (oldestDirtyIndex != -1) {
        printf("oldest slot: %d (dirty), with timestamp: %lu\n",
               oldestDirtyIndex, oldestDirtyPage);
        return oldestDirtyIndex;
    } else if(oldestCleanIndex != -1) {
        printf("oldest slot: %d (clean), with timestamp: %lu\n",
               oldestCleanIndex, oldestCleanPage);
        return oldestCleanIndex;
    } else {
        printf("oldest slot: %d (clean), with timestamp: %lu\n",
               oldestVolIndex, oldestVolPage);
        return oldestVolIndex;
    }
}

/*
 * lru_volatileEvict analyzes the state of the volatile input volatile and returns the slot which should be
 * evicted and returns that index.
 * Least Recently Used algorithm finds the oldest
 * timestamp (i.e. the block which has been inactive
 * for longest) and returns that index.
 */
int FLOPPYBufferManager::volatileEvictionPolicy() {
    int i, oldestIndex = -1;
    //oldest clean/dirty pages- start from current operation count
    unsigned long oldestPage = ops + 1;
    
    //find the oldest pages by iterating through timestamps
    for (i = 0; i < nVolatileBlocks; i++) {
        if (volatileTimestamp[i] < oldestPage) {
            oldestPage = volatileTimestamp[i];
            oldestIndex = i;
        }
    }
    
    if (numVolatileOccupied < nVolatileBlocks) {
        fprintf(stderr, "WARNING: lru_volatileEvict called on non-full volatile storage\n");
    }
    
    printf("oldest slot: %d, with timestamp: %lu\n", oldestIndex, oldestPage);
    return oldestIndex;
}



