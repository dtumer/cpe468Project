#include <limits.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>

#include "BufferManager.h"
#include "hashmap.h"
#include "smartalloc.h"
#include "BasicList.h"

int lru_evict(Buffer *buf);
/* type for a replacement policy function 
   It returns the buffer index of the page that should be evicted */
static evictFn evictionPolicy = lru_evict;
/* using an operation counter rather than a timestamp because it's simpler*/
/* TODO? change to timevals, and alter the buffer struct accordingly?*/
static unsigned long ops = 0;

//global map variable for holding references to disk addresses and their location in the buffer
map_t diskMap;
/* used for looking up the (fd, page) pair for a page in the index */
map_t reverseMap;

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

/* return a string representation of the given int, so that it can be 
 used as a key in reverseMap */
char *indexToString(int index) {
   char *str = calloc(numDigits(index) + 1, sizeof(char));
   sprintf(str, "%d", index);
   return str;
}

/* This function wraps the hashmap get function for ease of use in this file
   Used for looking up the index in buffer of a given page */
int getIndex(DiskAddress diskAdd) {
     char *diskStr;
     int *retValue, error;
    
     diskStr = diskAddressToString(diskAdd);
     error = hashmap_put(diskMap, diskStr, (void**)&retValue);
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

/* remove a key from the hashmap */
int removeIndex(DiskAddress diskAdd) {
   char *diskStr;
   int error;
   
   diskStr = diskAddressToString(diskAdd);
   error = hashmap_remove(diskMap, diskStr);
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
    if (index >= 0 && index < buf->nBlocks) {
        return buf->pages[index];
    }
    else {
        return NULL;
    }
}

/*initializes the buffer
 
 */
void initBuffer(Buffer *buf, char *database, int nBlocks) {
    
    //copy database name over
    buf->database = calloc(strlen(database) + 1, sizeof(char));
    strcpy(buf->database, database);
    
    buf->nBlocks = nBlocks;
   /* allocate the page arrays for nBlocks pages */
    buf->pages = calloc(nBlocks, sizeof(Block*));
    buf->timestamp = calloc(nBlocks, sizeof(unsigned long));
    buf->pin = calloc(nBlocks, sizeof(char));
    buf->dirty = calloc(nBlocks, sizeof(char));
   /* a new buffer has no pages occupied */
    buf->numOccupied = 0;
   
   /* initialize the hashmap */
   diskMap = hashmap_new();
   reverseMap = hashmap_new();
}


/* unpins all pages and flushes all pages
   called by squash()
 */
int cleanupBuffer(Buffer *buf) {
    int i;
    Block *pageBlock;
        
    for (i = 0; i < buf->nBlocks; i++) {
    	pageBlock = buf->pages[i];
    	
    	if (page != NULL) {
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
    
    //clear buffer 
    free(buf->database);
    free(buf->pages);
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
 * \param nBlocks   how many buffer slots to create in the buffer
 *
 * Open or create a TinyFS filesystem using tfs_mount/tfs_mkfs,
 * then initialize the buffer with nBlocks buffer slots.
 * Load any pages that might be needed into the buffer. 
 * In our case, pages needed are:
 *
 * Commence requires that the buffer be a valid pointer (i.e. retrieved from malloc or 
 *  the address of a local variable).
 *
 * Commence returns BFMG_OK upon success, BFMG_ERR upon other failure
 * If more error codes are needed, feel free to #define them
 */
int commence(char *Database, Buffer *buf, int nBlocks) {
    int tfsErr, retVal;

    tfsErr = tfs_mount(Database);

    if (tfsErr != 0) {
        tfs_mkfs(Database, DEFAULT_DISK_SIZE);
        //do we need to mount it here?
    }
    
    initBuffer(buf, Database, nBlocks);
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
   and places the input page at the correct spot. */
int evictPage(Buffer *buf, Block *newBlock) {
   
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
 * returns the index of the requested page, or BFMG_ERR if something is wrong.
 * on error, errno is also set
 */
int readPage(Buffer *buf, DiskAddress diskPage) {
   int result;
   int evictSlot, test;
   DiskAddress *occupant;
   /* index is now the index in buffer of that page if it exists,
   retrieved from the hashmap,
   and -1 otherwise */
   int index = getIndex(diskPage), retValue, slotToFill;
   
   /* not in the hashmap, need to retrieve it from disk */
   if (index == -1) {
      /* if there are empty slots in the buffer, put the new page into the
      first empty one */
      if (buf->numOccupied < buf->nBlocks) {
         /* numOccupied is the first open index */
         slotToFill = numOccupied;
      } else {
         /* something needs to be evicted */
         evictSlot = evictionPolicy(buf);
         /* flush the page, and update the map */
         flushPage(buf, buf->pages[evictSlot].diskAddress);
         removeIndex(buf->pages[evictSlot].diskAddress);
         /* and update the slot */
         slotToFill = evictSlot;
      }
      
      result = tfs_readPage(diskPage.FD, diskPage.pageId,
                             buffer->pages[slotToFill]);
      if (result != 0) {
         putIndex(diskPage, slotToFill);
         printf("putting ( {%d, %d} -> %d) in map\n", diskPage.FD, diskPage.pageId,
                slotToFill);
      }
      retValue = slotToFill;
      buf->timestamp[slotToFill] = ops++;
   } else {
      /* page already in buffer */
      retValue = index;
      buf->timestamp[index] = ops++;
   }

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
    int index = getIndex(diskPage), retValue;
    Block *pageBlock = findPageInBuffer(buf, index);
    
    if (pageBlock) {
        buf->dirty[index] = 'T';
        buf->timestamp[index] = ops++;
        retValue = BFMG_OK;
    }
    else {
        retValue = BFMG_ERR;
    }
    
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
    int index = getIndex(diskPage);
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
    int index = getIndex(diskPage), retValue;
    Block *pageBlock = findPageInBuffer(buf, index);
    
    if (pageBlock) {
        buf->pin[index] = 'T';
        retValue = BFMG_OK;
    }
    else {
        retValue = BFMG_ERR;
    }
    
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
    int index = getIndex(diskPage), retValue;
    Block *pageBlock = findPageInBuffer(buf, index);
   
    if (pageBlock) {
        buf->pin[index] = 'F';
        retValue = BFMG_OK;
    }
    else {
        retValue = BFMG_ERR;
    }
    
    return retValue;
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
int newPage(Buffer *buf, fileDescriptor FD, DiskAddress *diskPage) {
    Block *pageBlock = calloc(1, sizeof(Block));
    
    diskPage->FD = FD;
    diskPage->pageId = tfs_numPages(FD) + 1;
    
    tfs_writePage(FD, diskPage->pageId, pageBlock->block);
    
    //do eviction
    
    
    return 0;
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
    printf("Slots Occupied: %d\n", buf->numOccupied);
    
    
    for(i=0; i < buf->nBlocks; i++) {
        if(i > buf->numOccupied) {
            printf("Slot %d is empty\n", i);
        }
        else {
            printf("Slot %d:\n", i);
            printf("\ttinyFS blockID: %d\n", buf->pages[i]->diskAddress.pageId);
            printf("\ttimestamps for the block: %ld\n", buf->timestamp[i]);
            printf("\tpin flag: %d\n", buf->pin[i]);
            printf("\tdirty flag: %d\n", buf->dirty[i]);
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
    if (index >= 0 && index < buf->nBlocks) {
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
    int index = getIndex(diskPage);
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
 **/
int lru_evict(Buffer *buf) {
   int i, oldestIndex;
   unsigned long oldest = 0;
   
   if (buf == NULL) {
      fprintf(stderr, "Null buffer ptr passed to eviction fn\n")
   }
   /* find the oldest page by iterating through 
      timestamps*/
   for (i = 0; i < buf->nBlocks; i++) {
      if (buf->timestamp[i] <= oldest && buf->pin[i] != 'T') {
         oldestIndex = i;
         oldest = buf->timestamp[i];
      }
      
   }
   
   printf("oldest slot: %d, with timestamp: %ul\n", oldestIndex, oldest);
   
   return oldestIndex;
}


