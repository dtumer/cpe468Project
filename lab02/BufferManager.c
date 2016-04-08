#include "BufferManager.h"
#include "hashmap.h"

/* type for a replacement policy function 
   It returns the buffer index of the page that should be evicted */
typedef int replacement()

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
   return 0;
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
   return BFMG_OK
}

