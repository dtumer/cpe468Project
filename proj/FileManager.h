#ifndef FileManager_h
#define FileManager_h

#include <stdint.h>

#include "BufferManager.h"
#include "ReadWrite.h"

typedef struct FileHeader {
	uint32_t fileType; //1: Heap, 2: Sequential, 3: Hash
	char fileName[16];
}__attribute__((packed)) FileHeader;

typedef struct SeqFileHeader {
    uint16_t recordSize;
    uint16_t numRecordsPerPage;
    uint16_t numPages;
    uint16_t nextPage;
    uint16_t lastPage;
    char tableName[10];
    char recordDescription[600]; //uint8_t colType, uint8_t colSize(optional), uint8_t nameLen, char colName[nameLen + 1]
}__attribute__((packed)) SeqFileHeader;

typedef struct SeqPageHeader {
    uint16_t nextPage;
    uint8_t seqPageType; //1: node, 2: leaf
}__attribute__((packed)) SeqPageHeader;

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

/* Test Fucntions */
void printFileHeader(Buffer *buf, fileDescriptor fd);

#endif
