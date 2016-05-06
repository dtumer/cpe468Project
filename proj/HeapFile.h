#ifndef HeapFile_h
#define HeapFile_h

#include "ReadWrite.h"
#include "StatementParser.h"

/* file-level functions */
int heap_createFile(char *tableName, tableDescription *tableDesc, int volatileFlag);
int heap_deleteFile(char *tableName);

/* File Header functions */
// Get the table name from a given header page
int heap_headerGetTableName(fileDescriptor fd, char *name);
// Get the file descriptor structure
int heap_headerGetRecordDesc(fileDescriptor fd);
// Return the address of the next page in the pagelist list
int head_headerGetNextPage(fileDescriptor fd, DiskAddress *page);
// Return the address of the next page in the freespace list
int heap_headerGetFreeSpace(fileDescriptor fd, DiskAddress *page);

/* Data Page functions */
//given a page id and a location on the page (represented by id) retrieve the contents of the record into output array
int getRecord(DiskAddress page, int recordId, char *bytes);
//given a page id and a location on the page, place the given record onto the appropriate slot of the page
int putRecord(DiskAddress page, int recordId, char *bytes);


#endif