#ifndef HeapFile_h
#define HeapFile_h

#include "ReadWrite.h"
#include "StatementParser.h"
#include "FileManager.h"

//structure storing heap file information
typedef struct HeapFileHeader {
	uint16_t recordSize;
	uint16_t numRecordsPerPage;
	uint16_t numPages;
	uint16_t firstPage;
	uint16_t firstFreeSlotPage;
	char tableName[32];
	char recordDescription[600]; //uint8_t colType, uint8_t colSize(optional), uint8_t nameLen, char colName[nameLen + 1]
}__attribute__((packed)) HeapFileHeader;

typedef struct HeapPageHeader {
	uint16_t nextFreeSlotPage;
}__attribute__((packed)) HeapPageHeader;

/* file-level functions */
int heap_createFile(Buffer *buf, char *tableName, tableDescription *tableDesc, int volatileFlag);
int heap_deleteFile(Buffer *buf, char *tableName);

/* File Header functions */
// Get the table name from a given header page
int heap_headerGetTableName(Buffer *buf, fileDescriptor fd, char *name);
// Get the file descriptor structure
int heap_headerGetRecordDesc(Buffer *buf, fileDescriptor fd, tableDescription *desc);
// Return the address of the next page in the pagelist list
int head_headerGetNextPage(Buffer *buf, fileDescriptor fd, DiskAddress *page);
// Return the address of the next page in the freespace list
int heap_headerGetFreeSpace(Buffer *buf, fileDescriptor fd, DiskAddress *page);

/* Data Page functions */
//given a page id and a location on the page (represented by id) retrieve the contents of the record into output array
int getRecord(Buffer *buf, DiskAddress page, int recordId, char *bytes);
//given a page id and a location on the page, place the given record onto the appropriate slot of the page
int putRecord(Buffer *buf, DiskAddress page, int recordId, char *bytes);
//
int pHGetRecSize(Buffer *buf, DiskAddress page);
int pHGetMaxRecords(Buffer *buf, DiskAddress page);



#endif