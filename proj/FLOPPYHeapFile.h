//
//  FLOPPYHeapFile.h
//

#ifndef FLOPPYHeapFile_h
#define FLOPPYHeapFile_h

#include "FLOPPYBufferManager.h"
#include "StatementParser.h"
#include "FLOPPYFileManager.h"
#include "FLOPPYBitmap.h"

#include <stdio.h>

#include <string>
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


class FLOPPYHeapFile : public FLOPPYFileManager {
public:
    FLOPPYHeapFile(FLOPPYBufferManager *buf, fileDescriptor fd); // constructor
    
    /* file-level functions */
    fileDescriptor createFile(char *tableName, tableDescription *tableDesc, int volatileFlag);
    int deleteFile(char *tableName);
    
    /* File Header functions */
    // Get the table name from a given header page
    int headerGetTableName(fileDescriptor fd, char *name);
    // Get the file descriptor structure
    uint16_t headerGetRecordDesc(fileDescriptor fd, tableDescription *desc);
    // Return the address of the next page in the pagelist list
    DiskAddress headerGetNextPage(fileDescriptor fd);
    // Return the address of the next page in the freespace list
    int headerGetFreeSpace(fileDescriptor fd, DiskAddress *page);
    
    /* Data Page functions */
    //given a page id and a location on the page (represented by id) retrieve the contents of the record into output array
    unsigned char * getRecord(DiskAddress page, uint16_t dataOffset, uint16_t recordSize, int recordId);
    //given a page id and a location on the page, place the given record onto the appropriate slot of the page
    int putRecord(DiskAddress page, uint16_t dataOffset, uint16_t recordSize, int recordId, char *data);
    
    int pHGetRecSize(DiskAddress page);
    int pHGetMaxRecords(DiskAddress page);
    
    /* Heap File CRUD operations */
    int insertRecord(char * tableName, char * record);
    int deleteRecord(DiskAddress page, int recordId);
    int updateRecord(DiskAddress page, int recordId, char * record);
    
    /* Test Functions */
    void printFileInfo(fileDescriptor fd);
    void printPageInfo(fileDescriptor fd, int pageId);
    
private:
    void getFileName(char *tableName, char *fileName);
    HeapFileHeader * getHeapFileHeader();
    int writeHeapFileHeader(HeapFileHeader *heapFileHeader);
    HeapPageHeader * getPageHeader(DiskAddress page);
    int writePageHeader(DiskAddress page, HeapPageHeader *heapPageHeader);
    void createNewPage(fileDescriptor fd, HeapFileHeader *heapFileHeader);
};
#endif /* FLOPPYHeapFile_h */
