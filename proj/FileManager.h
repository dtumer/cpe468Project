#ifndef FileManager_h
#define FileManager_h

#include <stdint.h>

#include "BufferManager.h"
#include "ReadWrite.h"

#define HEAP_FILE_TYPE 1
#define SEQ_FILE_TYPE 2
#define HASH_FILE_TYPE 3

typedef struct FileHeader {
	uint32_t fileType; //1: Heap, 2: Sequential, 3: Hash
	char fileName[16];
}__attribute__((packed)) FileHeader;

typedef struct HashFileHeader {

}__attribute__((packed)) HashFileHeader;

typedef struct HashPageHeader {
    
}__attribute__((packed)) HashPageHeader;

/* FileHeader Functions */
FileHeader * file_getHeader(Buffer *buf, fileDescriptor fd);
void file_writeHeader(Buffer *buf, fileDescriptor fd, FileHeader *header);

/* General Helper Functions */
int calcBitmapSize(int recordSize, int pageSize, int headerSize, int curBitmapSize);
int getFirstFreeRecord(uint8_t *bitmap, int numRecords);
void setBitmapRecordEmpty(uint8_t *bitmap, int recordNum);
void setBitmapRecordFull(uint8_t *bitmap, int recordNum);

/* Test Functions */
void printFileHeader(Buffer *buf, fileDescriptor fd);
void printBitmap(uint8_t *bitmap, int numRecords);

#endif
