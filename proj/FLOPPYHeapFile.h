//
//  FLOPPYHeapFile.h
//

#ifndef FLOPPYHeapFile_h
#define FLOPPYHeapFile_h

#include "FLOPPYBufferManager.h"
#include "FLOPPYFileManager.h"
#include "FLOPPYBitmap.h"
#include "RecordDescription.h"
#include "libs/FLOPPYParser/FLOPPYParser.h"


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
    char tableName[TABLE_NAME_SIZE];
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
    FLOPPYHeapFile(FLOPPYBufferManager *buf, std::string tableName); // constructor
    
    /* file-level functions */
    static FLOPPYHeapFile * createFile(FLOPPYBufferManager *buf, FLOPPYCreateTableStatement *statement);
    static int deleteFile(std::string tableName);
    
    /* File Header functions */
    // Get the table name from a given header page
    int headerGetTableName(char *name);
    // Get the file descriptor structure
    //uint16_t headerGetRecordDesc(tableDescription *desc);
    // Return the address of the next page in the pagelist list
    DiskAddress headerGetNextPage();
    // Return the address of the next page in the freespace list
    //int headerGetFreeSpace(DiskAddress *page);
    
    /* Data Page functions */
    //given a page id and a location on the page (represented by id) retrieve the contents of the record into output array
    unsigned char * getRecord(int pageId, uint16_t dataOffset, uint16_t recordSize, int recordId);
    //given a page id and a location on the page, place the given record onto the appropriate slot of the page
    int putRecord(int pageId, uint16_t dataOffset, uint16_t recordSize, int recordId, char *data);
    
    /* Heap File CRUD operations */
    int insertRecord(char * record);
    int deleteRecord(int pageId, int recordId);
    int updateRecord(int pageId, int recordId, char * record);
    
    /* Test Functions */
    void printFileInfo();
    void printPageInfo(int pageId);
    void getAllRecords(int pageId);
    
private:
    static void getFileName(std::string tableName, char *fileName);
    HeapFileHeader * getHeapFileHeader();
    int writeHeapFileHeader(HeapFileHeader *heapFileHeader);
    HeapPageHeader * getPageHeader(int pageId);
    int writePageHeader(int pageId, HeapPageHeader *heapPageHeader);
    void createNewPage(HeapFileHeader *heapFileHeader);
    
    DiskAddress getDiskAddress(int pageId);
};
#endif /* FLOPPYHeapFile_h */
