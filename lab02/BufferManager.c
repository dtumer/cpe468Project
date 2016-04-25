#include <limits.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>

#include "BufferManager.h"
#include "hashmap.h"
#include "smartalloc.h"


int lru_evict(Buffer *buf);
/* type for a replacement policy function 
   It returns the buffer index of the page that should be evicted */
static evictFn evictionPolicy = lru_evict;
/* using an operation counter rather than a timestamp because it's simpler*/
/* TODO? change to timevals, and alter the buffer struct accordingly?*/
static unsigned long ops = 0;

//global map variable for holding references to disk addresses and their location in the buffer
map_t bufMap;
map_t cacheMap;

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
        //printf("ERROR: There is no such element in the Map!\n");
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

/* This function wraps the hashmap get function for ease of use in this file
   Used for looking up the index in buffer of a given page */
int getIndex(map_t map, DiskAddress diskAdd) {
     char *diskStr;
     int *retValue, error;
    
     diskStr = diskAddressToString(diskAdd);
     error = hashmap_get(map, diskStr, (void**)&retValue);
     free(diskStr);
    
     //print error if the map returned an error
     if (error != MAP_OK) {
        /* if not in map, error should be MAP_MISSING */
         printHashmapError(error);
         return -1;
     }
     else {
         return *retValue;
    }
}

//This function wraps the hashmap put function for ease of use in this file
int putIndex(map_t map, DiskAddress diskAdd, int index) {
    char *diskStr;
    int error;
    
    diskStr = diskAddressToString(diskAdd);
    error = hashmap_put(map, diskStr, &index);
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
    if (index >= 0 && index < buf->nBufferBlocks) {
        return buf->pages[index];
    }
    else {
        return NULL;
    }
}

/*initializes the buffer
 
 */
void initBuffer(Buffer *buf, char *database, int nBufferBlocks, int nCacheBlocks) {
    
    //copy database name over
    buf->database = calloc(strlen(database) + 1, sizeof(char));
    strcpy(buf->database, database);
    
    //buffer
    buf->nBufferBlocks = nBufferBlocks;
    /* allocate the page arrays for nBufferBlocks pages */
    buf->pages = calloc(nBufferBlocks, sizeof(Block*));
    buf->timestamp = calloc(nBufferBlocks, sizeof(unsigned long));
    buf->pin = calloc(nBufferBlocks, sizeof(char));
    buf->dirty = calloc(nBufferBlocks, sizeof(char));
    /* a new buffer has no pages occupied */
    buf->numBufferOccupied = 0;
    
    //cache
    buf->cache = calloc(nCacheBlocks, sizeof(Block*));
    buf->nCacheBlocks = nCacheBlocks;
    buf->numCacheOccupied = 0;
    
    /* initialize the hashmaps */
    bufMap = hashmap_new();
    cacheMap = hashmap_new();
}


/* unpins all pages and flushes all pages
   called by squash()
 */
int cleanupBuffer(Buffer *buf) {
    int i;
    Block *pageBlock;
        
    for (i = 0; i < buf->nBufferBlocks; i++) {
    	pageBlock = buf->pages[i];
    	
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
            buf->pages[i] = NULL;
    	}
    }
    
    for (i = 0; i < buf->nCacheBlocks; i++) {
        pageBlock = buf->cache[i];
        
        if (pageBlock != NULL) {
            //free the page
            free(pageBlock);
            buf->cache[i] = NULL;
        }
    }
    
    
    //clear buffer 
    free(buf->database);
    free(buf->pages);
    free(buf->cache);
    free(buf->timestamp);
    free(buf->pin);
    free(buf->dirty);
    free(buf);
    
    return BFMG_OK;
}

/**
 * commence is called once at the beginning of a program that uses the buffer.
 * \param Database  name of the tinyFS disk file
 * \param buf       The buffer to create/initialize
 * \param nBufferBlocks   how many buffer slots to create in the buffer
 *
 * Open or create a TinyFS filesystem using tfs_mount/tfs_mkfs,
 * then initialize the buffer with nBufferBlocks buffer slots.
 * Load any pages that might be needed into the buffer. 
 * In our case, pages needed are:
 *
 * Commence requires that the buffer be a valid pointer (i.e. retrieved from malloc or 
 *  the address of a local variable).
 *
 * Commence returns BFMG_OK upon success, BFMG_ERR upon other failure
 * If more error codes are needed, feel free to #define them
 */
int commence(char *Database, Buffer *buf, int nBufferBlocks, int nCacheBlocks) {
    int tfsErr, retVal;

    tfsErr = tfs_mount(Database);

    if (tfsErr != 0) {
        tfs_mkfs(Database, DEFAULT_DISK_SIZE);
        tfs_mount(Database);
    }
    
    initBuffer(buf, Database, nBufferBlocks, nCacheBlocks);
    retVal = BFMG_OK;
	
    return retVal;
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
    int tfsErr, retVal;
    
    //clears all buffer slots
    cleanupBuffer(buf);
    
    //closes tinyFS disk associated with buffer
    tfsErr = tfs_unmount();
    
    if (tfsErr) {
        retVal = BFMG_ERR;
    }
    else {
        retVal = BFMG_OK;
    }
    
    return retVal;
}

/* evictPage takes a page to load into the buffer, uses the 
   eviction algorithm to find and remove a page (flushing it if necessary),
   and places the input page at the correct spot. 
   It also free()s the page if one is evicted. */
int placePageInBuffer(Buffer *buf, Block *newBlock) {
   int index, toEvict, insertNdx, retval;
   /* if there are empty buffer slots, put the page in the first empty one.
      if not, ask the eviction policy.
    Priority for eviction:
    1. Unpinned dirty pages (flush and replace)
    2. If none of those, pick from the unpinned, un-dirty pages and ask the eviction policy (for now, LRU)  */
   index = getIndex(bufMap, newBlock->diskAddress);
   if (index == -1) {
      /* need to fetch it */
      if (buf->numBufferOccupied < buf->nBufferBlocks) {
         /* numBufferOccupied is the first open index */
         insertNdx = buf->numBufferOccupied;
         buf->numBufferOccupied++;
      } else {
         /* these calls will be replaced by evictPage */
         toEvict = evictionPolicy(buf);
         /* flush the page, and update the map */
         if (buf->dirty[toEvict] == 'T') {
            if (flushPage(buf, buf->pages[toEvict]->diskAddress) == BFMG_ERR) {
               fprintf(stderr, "placePageInBuffer: failed to flush dirty page\n");
               return BFMG_ERR;
            }
         }
         removeIndex(bufMap, buf->pages[toEvict]->diskAddress);
         fprintf(stderr, "info: freeing the page at %d\n", toEvict);
         free(buf->pages[toEvict]);
         
         insertNdx = toEvict;
      }
      buf->pages[insertNdx] = newBlock;
      buf->pin[insertNdx] = 'F';
      buf->dirty[insertNdx] = 'F';
      buf->timestamp[insertNdx] = ops++;
      putIndex(bufMap, buf->pages[insertNdx]->diskAddress, insertNdx);
      
      retval = insertNdx;
   } else {
      buf->timestamp[index] = ops++;
      if (newBlock != NULL) {
         fprintf(stderr, "warn: allocated block passed to placePageInBuffer for an existing page\n");
      }
      retval = index;
   }
   
   return retval;
}

/**
 * This function provides read access to the specified block of the disk.
 * \param buf       the currently used buffer struct
 * \param diskPage  a TinyFS identifier for the disk page needed
 *
 * readPage guarantees that after its return, the parameter page will be in the 
 * buffer. Further calls can use the other map-access function (not yet defined)
 * to access the page read by this function.
 * returns the index of the requested page, or BFMG_ERR if something is wrong.
 * on error, errno is also set
 */
int readPage(Buffer *buf, DiskAddress diskPage) {
   int result;
   int existingIndex;
   Block *newBlock;
   
   existingIndex = getIndex(bufMap, diskPage);
   if (existingIndex == -1) {
      newBlock = malloc(sizeof(Block));
      newBlock->diskAddress = diskPage;
      
      result = tfs_readPage(diskPage.FD, diskPage.pageId,
                            newBlock->block);
      if (result != 0) {
         fprintf(stderr, "tfs_readPage returned %d\n", result);
      }
   }
   
   result = placePageInBuffer(buf, newBlock);

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
    int index = getIndex(bufMap, diskPage);
    Block *pageBlock = findPageInBuffer(buf, index);
    
    if (pageBlock) {
        buf->dirty[index] = 'T';
        buf->timestamp[index] = ops++;
        return BFMG_OK;
    }
    else {
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
    int index = getIndex(bufMap, diskPage);
    Block *pageBlock = findPageInBuffer(buf, index);

    //Exits if page is not in the buffer
    if (!pageBlock) {
        return BFMG_ERR;
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
    int index = getIndex(bufMap, diskPage);
    Block *pageBlock = findPageInBuffer(buf, index);
    
    if (pageBlock) {
        buf->pin[index] = 'T';
        return BFMG_OK;
    }
    else {
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
    int index = getIndex(bufMap, diskPage);
    Block *pageBlock = findPageInBuffer(buf, index);
   
    if (pageBlock) {
        buf->pin[index] = 'F';
        return BFMG_OK;
    }
    else {
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
    Block *pageBlock = calloc(1, sizeof(Block));
    
    pageBlock->diskAddress = diskPage;
    
    tfs_writePage(diskPage.FD, diskPage.pageId, pageBlock->block);
    
    //do eviction
    result = placePageInBuffer(buf, pageBlock);
    
    return BFMG_OK;
}


int allocateCachePage(Buffer *buf, DiskAddress diskPage) {
    return BFMG_ERR;
}


int removeCachePage(Buffer *buf, DiskAddress diskPage) {
    return BFMG_ERR;
}

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
 *    timestamps for the block
 *    pin flag
 *    dirty flag
 *    any other attributes about the slot
 */
void checkpoint(Buffer * buf) {
    int i;
    
    printf("Disk: %s\n", buf->database);
    printf("Buffer Slots Occupied: %d\n", buf->numBufferOccupied);
    
    for(i=0; i < buf->nBufferBlocks; i++) {
        if(i > buf->numBufferOccupied) {
            printf("Buffer Slot %d is empty\n", i);
        }
        else {
            printf("Buffer Slot %d:\n", i);
            printf("\ttinyFS FD: %d\n", buf->pages[i]->diskAddress.FD);
            printf("\ttinyFS blockID: %d\n", buf->pages[i]->diskAddress.pageId);
            printf("\ttimestamps for the block: %ld\n", buf->timestamp[i]);
            printf("\tpin flag: %d\n", buf->pin[i]);
            printf("\tdirty flag: %d\n", buf->dirty[i]);
        }
    }
    
    printf("Cache Slots Occupied: %d\n", buf->numCacheOccupied);
    
    for(i=0; i < buf->nCacheBlocks; i++) {
        if(i > buf->numCacheOccupied) {
            printf("Cache Slot %d is empty\n", i);
        }
        else {
            printf("Cache Slot %d:\n", i);
            printf("\ttinyFS FD: %d\n", buf->pages[i]->diskAddress.FD);
            printf("\ttinyFS blockID: %d\n", buf->pages[i]->diskAddress.pageId);
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
    if (index >= 0 && index < buf->nBufferBlocks) {
        fwrite(&(buf->pages[index]), BLOCKSIZE, 1, stdout);
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
    int index = getIndex(bufMap, diskPage);
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
 * lru_evict analyzes the state of the input buffer and returns the slot which should be flushed/dropped,
 * and returns that. Called by readPage
 * readPage and writePage update the timestamp for
 * blocks used.
 * Least Recently Used algorithm finds the oldest
 * timestamp (i.e. the block which has been inactive
 * for longest) and returns that index.
 *
 * Priority:
 * 1. Unpinned dirty pages (flush and replace)
 * 2. If none of those, pick from the unpinned, un-dirty pages.  */
int lru_evict(Buffer *buf) {
   int i;
   /* oldest clean/dirty pages- start from current operation count */
   unsigned long oldestDirtyPage = ops + 1, oldestCleanPage = ops + 1;
   int oldestDirtyIndex = -1, oldestCleanIndex = -1;
   if (buf == NULL) {
       fprintf(stderr, "Null buffer ptr passed to eviction fn\n");
   }
   /* find the oldest pages by iterating through
      timestamps*/
   for (i = 0; i < buf->nBufferBlocks; i++) {
      if (buf->pin[i] == 'F') {
         if (buf->dirty[i] == 'T' && buf->timestamp[i] < oldestDirtyPage) {
            oldestDirtyPage = buf->timestamp[i];
            oldestDirtyIndex = i;
         }
         if (buf->dirty[i] == 'F' && buf->timestamp[i] < oldestCleanPage) {
            oldestCleanPage = buf->timestamp[i];
            oldestCleanIndex = i;
         }
      }
      
   }
   
   if (buf->numBufferOccupied < buf->nBufferBlocks) {
      fprintf(stderr, "WARNING: lru_evict called on a non-full buffer\n");
   }
   /* if there is an unpinned dirty page */
   if (oldestDirtyPage != ops + 1) {
      printf("oldest slot: %d (dirty), with timestamp: %lu\n",
             oldestDirtyIndex, oldestDirtyPage);
      return oldestDirtyIndex;
   } else {
      printf("oldest slot: %d (clean), with timestamp: %lu\n",
             oldestCleanIndex, oldestCleanPage);
      return oldestCleanIndex;
   }
}


