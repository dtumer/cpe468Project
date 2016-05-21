#ifndef HeapFile_h
#define HeapFile_h

#include "BufferManager.h"
#include "ReadWrite.h"
#include "StatementParser.h"
#include "FileManager.h"

#include <stdlib.h>
#include <string.h>

//structure storing heap file information
typedef struct HeapFileHeader {
	uint16_t recordSize;
	uint16_t numRecordsPerPage;
	uint16_t numPages;
	uint16_t firstPage;
	uint16_t firstFreeSlotPage;
    uint16_t bitmapSize;
    uint16_t isVolatile;
	char tableName[10];
	char recordDescription[600]; //uint8_t colType, uint8_t colSize(optional), uint8_t nameLen, char colName[nameLen + 1]
}__attribute__((packed)) HeapFileHeader;

typedef struct HeapPageHeader {
    uint16_t pageId;
	uint16_t nextFreeSlotPage;
    uint16_t nextPage;
    uint16_t numRecords;
    char fileName[FILE_NAME_SIZE];
}__attribute__((packed)) HeapPageHeader;


/* file-level functions */
fileDescriptor heap_createFile(Buffer *buf, char *tableName, tableDescription *tableDesc, int volatileFlag);
int heap_deleteFile(Buffer *buf, char *tableName);

/* File Header functions */
// Get the table name from a given header page
int heap_headerGetTableName(Buffer *buf, fileDescriptor fd, char *name);
// Get the file descriptor structure
uint16_t heap_headerGetRecordDesc(Buffer *buf, fileDescriptor fd, tableDescription *desc);
// Return the address of the next page in the pagelist list
DiskAddress heap_headerGetNextPage(Buffer *buf, fileDescriptor fd);
// Return the address of the next page in the freespace list
int heap_headerGetFreeSpace(Buffer *buf, fileDescriptor fd, DiskAddress *page);

/* Data Page functions */
//given a page id and a location on the page (represented by id) retrieve the contents of the record into output array
unsigned char * heap_getRecord(Buffer *buf, DiskAddress page, uint16_t dataOffset, uint16_t recordSize, int recordId);
//given a page id and a location on the page, place the given record onto the appropriate slot of the page
int heap_putRecord(Buffer *buf, DiskAddress page, uint16_t dataOffset, uint16_t recordSize, int recordId, char *data);


int pHGetRecSize(Buffer *buf, DiskAddress page);
int pHGetMaxRecords(Buffer *buf, DiskAddress page);

/* Heap File CRUD operations */

int heap_insertRecord(Buffer *buf, char * tableName, char * record);

int heap_deleteRecord(Buffer *buf, DiskAddress page, int recordId);

int heap_updateRecord(Buffer *buf, DiskAddress page, int recordId, char * record);


/* Test Functions */
void printHeapFileInfo(Buffer *buf, fileDescriptor fd);
void printHeapPageInfo(Buffer *buf, DiskAddress page);

#endif
