#include <limits.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

#include "BufferManager.h"


int lru_persistentEvict(Buffer *buf);
int lru_volatileEvict(Buffer *buf);
/* type for a replacement policy function 
   It returns the buffer index of the page that should be evicted */
static evictFn persistentEvictionPolicy = lru_persistentEvict;
static evictFn volatileEvictionPolicy = lru_volatileEvict;

/* using an operation counter rather than a timestamp because it's simpler*/
/* TODO? change to timevals, and alter the buffer struct accordingly?*/
static unsigned long ops = 0;

/* Helper Functions */

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
fileDescriptor getFileDescriptor(Buffer *buf, char *fileName) {
    fileDescriptor fd = -1;
    int error = hashmap_get(buf->openFileMap, fileName, &fd);
    
    //print error if the map returned an error
    if (error == MAP_MISSING) {
        fd = tfs_openFile(fileName);
        error = hashmap_put(buf->openFileMap, fileName, fd);
    }
    
    if (error != MAP_OK) {
        printHashmapError(error);
        return BFMG_ERR;
    }
    return fd;
}

//converts a disk address to a string value
char* diskAddressToString(DiskAddress diskAdd) {
    int lenFD = numDigits(diskAdd.FD);
    int lenPageId = numDigits(diskAdd.pageId);
    char *str = (char *)calloc(lenFD + 1 + lenPageId + 2, sizeof(char));

    sprintf(str, "%d,%d", diskAdd.FD, diskAdd.pageId);
   
    return str;
}

/* This function wraps the hashmap get function for ease of use in this file
   Used for looking up the index in buffer of a given page */
int getIndex(map_t map, DiskAddress diskAdd) {
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
int putIndex(map_t map, DiskAddress diskAdd, int index) {
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
int removeIndex(map_t map, DiskAddress diskAdd) {
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
Block* findPageInBuffer(Buffer *buf, int index) {
    if (index >= 0 && index < buf->nPersistentBlocks) {
        return buf->persistentPages[index];
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
int placePageInBuffer(Buffer *buf, Block *newBlock) {
    int toEvict;
    
    int insertNdx = getIndex(buf->persistentMap, newBlock->diskAddress);
    
    if (insertNdx == -1) { //need to fetch it
        //check for open page
        if (buf->numPersistentOccupied < buf->nPersistentBlocks) {
            insertNdx = buf->numPersistentOccupied;
            buf->numPersistentOccupied++;
        } else {
            toEvict = persistentEvictionPolicy(buf);
            if (buf->dirty[toEvict] == 'T') {
                if (flushPage(buf, buf->persistentPages[toEvict]->diskAddress) == BFMG_ERR) {
                    fprintf(stderr, "placePageInBuffer: failed to flush dirty page\n");
                    return BFMG_ERR;
                }
            }
            removeIndex(buf->persistentMap, buf->persistentPages[toEvict]->diskAddress);
            fprintf(stderr, "info: freeing the page at %d\n", toEvict);
            free(buf->persistentPages[toEvict]);
            
            insertNdx = toEvict;
        }
        
        buf->persistentPages[insertNdx] = newBlock;
        buf->pin[insertNdx] = 'F';
        buf->dirty[insertNdx] = 'F';
        buf->isVolatile[insertNdx] = 'F';
        buf->persistentTimestamp[insertNdx] = ops++;
        putIndex(buf->persistentMap, buf->persistentPages[insertNdx]->diskAddress, insertNdx);
        
    }
    return insertNdx;
}


/* External Functions */

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
int commence(char *database, Buffer *buf, int nPersistentBlocks, int nVolatileBlocks) {
    int tfsErr;
    
    tfsErr = tfs_mount(database);
    
    if (tfsErr != 0) {
        tfs_mkfs(database, DEFAULT_DISK_SIZE);
        tfsErr = tfs_mount(database);
    }
    
    //copy database name over
    buf->database = (char *)calloc(strlen(database) + 1, sizeof(char));
    strcpy(buf->database, database);
    
    //buffer
    buf->nPersistentBlocks = nPersistentBlocks;
    /* allocate the page arrays for nPersistentBlocks pages */
    buf->persistentPages = (Block **)calloc(nPersistentBlocks, sizeof(Block*));
    buf->persistentTimestamp = (unsigned long *)calloc(nPersistentBlocks, sizeof(unsigned long));
    buf->pin = (char *)calloc(nPersistentBlocks, sizeof(char));
    buf->dirty = (char *)calloc(nPersistentBlocks, sizeof(char));
    buf->isVolatile = (char *)calloc(nPersistentBlocks, sizeof(char));
    /* a new buffer has no pages occupied */
    buf->numPersistentOccupied = 0;
    
    //volatile
    buf->volatilePages = (Block **)calloc(nVolatileBlocks, sizeof(Block*));
    buf->volatileTimestamp = (unsigned long *)calloc(nVolatileBlocks, sizeof(unsigned long));
    buf->nVolatileBlocks = nVolatileBlocks;
    buf->numVolatileOccupied = 0;
    
    /* initialize the hashmaps */
    buf->persistentMap = hashmap_new();
    buf->volatileMap = hashmap_new();
    buf->openFileMap = hashmap_new();
    
    return BFMG_OK;
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
    int tfsErr, i;
    Block *pageBlock;
    
    //clears all Persistent slots
    for (i = 0; i < buf->nPersistentBlocks; i++) {
        pageBlock = buf->persistentPages[i];
        
        if (pageBlock != NULL) {
            //unpin page
            if (buf->pin[i] == 'T') {
                buf->pin[i] = 'F';
            }
            
            //flush if dirty
            if (buf->dirty[i] == 'T') {
                tfs_writePage(pageBlock->diskAddress.FD, pageBlock->diskAddress.pageId, pageBlock->block);
                buf->dirty[i] = 'F';
            }
            
            //free the page
            free(pageBlock);
            buf->persistentPages[i] = NULL;
        }
    }
    
    //clears all Volatile slots
    for (i = 0; i < buf->nVolatileBlocks; i++) {
        pageBlock = buf->volatilePages[i];
        
        if (pageBlock != NULL) {
            //free the page
            free(pageBlock);
            buf->volatilePages[i] = NULL;
        }
    }
    
    //free hashmaps
    hashmap_free(buf->persistentMap);
    hashmap_free(buf->volatileMap);
    hashmap_free(buf->openFileMap);
    
    //free buffer
    free(buf->database);
    free(buf->persistentPages);
    free(buf->volatilePages);
    free(buf->persistentTimestamp);
    free(buf->isVolatile);
    free(buf->volatileTimestamp);
    free(buf->pin);
    free(buf->dirty);
    free(buf);
    
    //closes tinyFS disk associated with buffer
    tfsErr = tfs_unmount();
    
    if (tfsErr)
        return BFMG_ERR;
    
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
int loadPersistentPage(Buffer *buf, DiskAddress diskPage) {
    int result = BFMG_OK;
    Block *newBlock;
    int existingIndex = getIndex(buf->persistentMap, diskPage);
    
    if (existingIndex == -1) {
        newBlock = (Block *)malloc(sizeof(Block));
        newBlock->diskAddress = diskPage;
        
        result = tfs_readPage(diskPage.FD, diskPage.pageId,
                              newBlock->block);
        if (result != 0) {
            fprintf(stderr, "tfs_readPage returned %d\n", result);
        }
        
        result = placePageInBuffer(buf, newBlock);
    } else {
        buf->persistentTimestamp[existingIndex] = ops++;
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
int writePage(Buffer *buf, DiskAddress diskPage) {
    int index = getIndex(buf->persistentMap, diskPage);
    Block *pageBlock = findPageInBuffer(buf, index);
    
    if (pageBlock) {
        buf->dirty[index] = 'T';
        buf->persistentTimestamp[index] = ops++;
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
int flushPage(Buffer *buf, DiskAddress diskPage) {
    int index = getIndex(buf->persistentMap, diskPage);
    Block *pageBlock = findPageInBuffer(buf, index);
    
    //Exits if page is not in the buffer
    if (!pageBlock) {
        return BFMG_ERR;
    }
    
    //handle if it is volatile
    if(buf->isVolatile[index] == 'T')
    {
        //need to create file?
    }
    
    //Writes the selected page's data to disk
    tfs_writePage(pageBlock->diskAddress.FD, pageBlock->diskAddress.pageId, pageBlock->block);
    
    //Unset dirty flag
    buf->dirty[index] = 'F';
    
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
int pinPage(Buffer *buf, DiskAddress diskPage) {
    int index = getIndex(buf->persistentMap, diskPage);
    Block *pageBlock = findPageInBuffer(buf, index);
    
    if (pageBlock) {
        buf->pin[index] = 'T';
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
int unPinPage(Buffer *buf, DiskAddress diskPage) {
    int index = getIndex(buf->persistentMap, diskPage);
    Block *pageBlock = findPageInBuffer(buf, index);
    
    if (pageBlock) {
        buf->pin[index] = 'F';
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
int newPage(Buffer *buf, DiskAddress diskPage) {
    int result;
    Block *pageBlock = (Block *)calloc(1, sizeof(Block));
    
    pageBlock->diskAddress = diskPage;
    
    tfs_writePage(diskPage.FD, diskPage.pageId, pageBlock->block);
    
    //do eviction
    result = placePageInBuffer(buf, pageBlock);
    
    return BFMG_OK;
}


int allocateCachePage(Buffer *buf, DiskAddress diskPage) {
    int i, bufNdx, index, insertNdx = -1;
    Block *newBlock, *tempBlock;
    
    //check not already in volatile storage
    index = getIndex(buf->volatileMap, diskPage);
    if(index != -1) {
        fprintf(stderr, "INFO: page already in Cache: %d\n", index);
        return BFMG_ERR;
    }
    
    //find first open slot in volatile storage
    if(buf->numVolatileOccupied < buf->nVolatileBlocks) {
        for(i=0; i<buf->nVolatileBlocks; i++) {
            if(buf->volatilePages[i] == NULL) {
                insertNdx = i;
                break;
            }
        }
    }
    
    //evict from volatile storage to persistent storage if
    //volatile storage is full
    if(insertNdx == -1) {
        insertNdx = volatileEvictionPolicy(buf);
        tempBlock = buf->volatilePages[insertNdx];
        
        removeIndex(buf->volatileMap, tempBlock->diskAddress);
        
        bufNdx = placePageInBuffer(buf, tempBlock);
        if(bufNdx == BFMG_ERR) {
            fprintf(stderr, "ERROR: failed to move page from Cache to Buffer\n");
            return BFMG_ERR;
        } else {
            buf->isVolatile[bufNdx] = 'T';
        }
    }
    
    //create new block
    newBlock = (Block *)calloc(1, sizeof(Block));
    newBlock->diskAddress = diskPage;
    
    //write block to volatile storage
    buf->volatilePages[insertNdx] = newBlock;
    buf->volatileTimestamp[insertNdx] = ops++;
    buf->numVolatileOccupied += buf->numVolatileOccupied < buf->nVolatileBlocks ? 1 : 0;
    putIndex(buf->volatileMap, newBlock->diskAddress, insertNdx);
    
    return BFMG_ERR;
}


int removeCachePage(Buffer *buf, DiskAddress diskPage) {
    Block *volatilePage;
    int result;
    
    //check volatile storage
    result = getIndex(buf->volatileMap, diskPage);
    if (result != -1) {
        // page is in volatile storage
        volatilePage = buf->volatilePages[result];
        free(volatilePage);
        removeIndex(buf->volatileMap, diskPage);
        buf->numVolatileOccupied--;
        buf->volatilePages[result] = NULL;
    } else {
        // in buffer or on disk
        result = getIndex(buf->persistentMap, diskPage);
        
        if (result != -1) {
            //drop the volatile page out of the volatile storage
            volatilePage = buf->persistentPages[result];
            free(volatilePage);
            removeIndex(buf->persistentMap, diskPage);
            
            //move the last entry in the buffer to the vacated slot
            buf->persistentPages[result] = buf->persistentPages[buf->numPersistentOccupied - 1];
            removeIndex(buf->persistentMap, buf->persistentPages[buf->numPersistentOccupied - 1]->diskAddress);
            putIndex(buf->persistentMap, buf->persistentPages[result]->diskAddress, result);
            buf->persistentPages[buf->numPersistentOccupied - 1] = NULL;
            buf->numPersistentOccupied--;
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
int loadVolatilePage(Buffer *buf, DiskAddress diskPage) {
    int existingIndex = getIndex(buf->volatileMap, diskPage);
    
    if (existingIndex > -1) {
        buf->volatileTimestamp[existingIndex] = ops++;
    }
    
    return existingIndex;
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
void checkpoint(Buffer * buf) {
    int i;
    
    printf("Disk: %s\n", buf->database);
    printf("Buffer Slots Occupied: %d\n", buf->numPersistentOccupied);
    
    for(i=0; i < buf->nPersistentBlocks; i++) {
        if(i >= buf->numPersistentOccupied) {
            printf("Buffer Slot %d is empty\n", i);
        } else {
            printf("Buffer Slot %d:\n", i);
            printf("\ttinyFS FD: %d\n", buf->persistentPages[i]->diskAddress.FD);
            printf("\ttinyFS blockID: %d\n", buf->persistentPages[i]->diskAddress.pageId);
            printf("\tpersistentTimestamps for the block: %ld\n", buf->persistentTimestamp[i]);
            printf("\tpin flag: %d\n", buf->pin[i]);
            printf("\tdirty flag: %d\n", buf->dirty[i]);
        }
    }
    
    printf("Cache Slots Occupied: %d\n", buf->numVolatileOccupied);
    
    for(i=0; i < buf->nVolatileBlocks; i++) {
        if(buf->volatilePages[i] == NULL) {
            printf("Cache Slot %d is empty\n", i);
        } else {
            printf("Cache Slot %d:\n", i);
            printf("\ttinyFS FD: %d\n", buf->volatilePages[i]->diskAddress.FD);
            printf("\ttinyFS blockID: %d\n", buf->volatilePages[i]->diskAddress.pageId);
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
int pageDump(Buffer *buf, int index) {
    if (index >= 0 && index < buf->nPersistentBlocks) {
        fwrite(&(buf->persistentPages[index]), BLOCKSIZE, 1, stdout);
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
int printPage(Buffer *buf, DiskAddress diskPage) {
    int index = getIndex(buf->persistentMap, diskPage);
    int numpages = tfs_numPages(diskPage.FD);
    
    if(diskPage.pageId > numpages) {
        printf("Page does not exist on disk");
        return BFMG_ERR;
    }
    
    if(index == -1) {
        printf("Page not found in buffer");
        return BFMG_ERR;
    }
    
    Block *pageBlock = findPageInBuffer(buf, index);
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
int printBlock(Buffer *buf, DiskAddress diskPage) {
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
int lru_persistentEvict(Buffer *buf) {
    int i, oldestDirtyIndex = -1, oldestCleanIndex = -1, oldestVolIndex = -1;
    //oldest clean/dirty pages- start from current operation count
    unsigned long oldestDirtyPage = ops + 1, oldestCleanPage = ops + 1, oldestVolPage = ops + 1;;
    
    if (buf->numPersistentOccupied < buf->nPersistentBlocks) {
        fprintf(stderr, "WARNING: lru_persistentEvict called on a non-full buffer\n");
    }
    
    //find the oldest pages by iterating through timestamps
    for (i = 0; i < buf->nPersistentBlocks; i++) {
        if (buf->pin[i] == 'F') {
            if (buf->isVolatile[i] == 'T') {
                if (buf->persistentTimestamp[i] < oldestVolPage) {
                    oldestVolPage = buf->persistentTimestamp[i];
                    oldestVolIndex = i;
                }
            } else if (buf->dirty[i] == 'T') {
                if (buf->persistentTimestamp[i] < oldestDirtyPage) {
                    oldestDirtyPage = buf->persistentTimestamp[i];
                    oldestDirtyIndex = i;
                }
            } else {
                if(buf->persistentTimestamp[i] < oldestCleanPage) {
                    oldestCleanPage = buf->persistentTimestamp[i];
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
int lru_volatileEvict(Buffer *buf) {
    int i, oldestIndex = -1;
    //oldest clean/dirty pages- start from current operation count
    unsigned long oldestPage = ops + 1;
    
    if (buf == NULL) {
        fprintf(stderr, "Null buffer ptr passed to eviction fn\n");
    }
    
    //find the oldest pages by iterating through timestamps
    for (i = 0; i < buf->nVolatileBlocks; i++) {
        if (buf->volatileTimestamp[i] < oldestPage) {
            oldestPage = buf->volatileTimestamp[i];
            oldestIndex = i;
        }
    }
    
    if (buf->numVolatileOccupied < buf->nVolatileBlocks) {
        fprintf(stderr, "WARNING: lru_volatileEvict called on non-full volatile storage\n");
    }
    
    printf("oldest slot: %d, with timestamp: %lu\n", oldestIndex, oldestPage);
	return oldestIndex;
}



