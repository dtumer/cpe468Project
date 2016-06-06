//
//  FLOPPYHeapFile.cpp
//

#include "FLOPPYHeapFile.h"

FLOPPYHeapFile::FLOPPYHeapFile(FLOPPYBufferManager *buf) : buf(buf) {
    
}

/* Helper functions */

void FLOPPYHeapFile::getFileName(char *tableName, char *fileName) {
    strcpy(fileName, "T_");
    strncpy(&fileName[2], tableName, FILE_NAME_SIZE-3);
    fileName[FILE_NAME_SIZE-1] = '\0';
    return;
}

HeapFileHeader * FLOPPYHeapFile::getFileHeader(fileDescriptor fd) {
    DiskAddress page;
    unsigned char *data;
    
    page.FD = fd;
    page.pageId = 0;
    
    data = buf->read(page, sizeof(FileHeader), sizeof(HeapFileHeader));
    
    return (HeapFileHeader *) data;
}

int FLOPPYHeapFile::writeFileHeader(fileDescriptor fd, HeapFileHeader *heapFileHeader) {
    DiskAddress page;
    
    page.FD = fd;
    page.pageId = 0;
    
    return buf->write(page, sizeof(FileHeader), sizeof(HeapFileHeader), (char *) heapFileHeader);
}

HeapPageHeader * FLOPPYHeapFile::getPageHeader(DiskAddress page) {
    unsigned char *data;
    
    data = buf->read(page, 0, sizeof(HeapPageHeader));
    
    return (HeapPageHeader *) data;
}

int FLOPPYHeapFile::writePageHeader(DiskAddress page, HeapPageHeader *heapPageHeader) {
    return buf->write(page, 0, sizeof(HeapPageHeader), (char *) heapPageHeader);
}

void FLOPPYHeapFile::createNewPage(fileDescriptor fd, HeapFileHeader *heapFileHeader) {
    FileHeader *fileHeader = file_getHeader(buf, fd);
    HeapPageHeader *heapPageHeader = (HeapPageHeader*) malloc(sizeof(HeapPageHeader));
    DiskAddress dAdd;
    
    //open file
    dAdd.FD = fd;
    dAdd.pageId = ++heapFileHeader->numPages;
    
    //create file
    if(heapFileHeader->isVolatile) {
        buf->allocateCachePage(dAdd);
    } else {
        buf->newPage(dAdd);
    }
    
    //add basic data
    heapPageHeader->pageId = dAdd.pageId;
    //heapPageHeader->nextPage = 0;
    heapPageHeader->numRecords = 0;
    strncpy(heapPageHeader->fileName, fileHeader->fileName, FILE_NAME_SIZE);
    
    //update free slot data
    heapPageHeader->nextFreeSlotPage = heapFileHeader->firstFreeSlotPage;
    heapFileHeader->firstFreeSlotPage = dAdd.pageId;
    
    //update next page
    heapPageHeader->nextPage = heapFileHeader->firstPage;
    heapFileHeader->firstPage = dAdd.pageId;
    
    char *bitmap = (char*) calloc(heapFileHeader->bitmapSize, sizeof(uint8_t));
    
    //write new page
    
    buf->write(dAdd, 0, sizeof(HeapPageHeader), (char *) heapPageHeader);
    buf->write(dAdd, sizeof(HeapPageHeader), heapFileHeader->bitmapSize, bitmap);
    
    //free all data
    free(fileHeader);
    free(heapPageHeader);
    free(bitmap);
}

/* file-level functions */

fileDescriptor FLOPPYHeapFile::createFile(char *tableName, tableDescription *tableDesc, int volatileFlag) {
    FileHeader *fileHeader = (FileHeader*) malloc(sizeof(FileHeader));
    HeapFileHeader *heapFileHeader = (HeapFileHeader*) malloc(sizeof(HeapFileHeader));
    DiskAddress headerPage;
    
    //FileHeader
    fileHeader->fileType = HEAP_FILE_TYPE;
    
    getFileName(tableName, fileHeader->fileName);
    
    //open file
    headerPage.FD = buf->getFileDescriptor(fileHeader->fileName);
    headerPage.pageId = 0;
    
    //create files
    if(volatileFlag) {
        buf->allocateCachePage(headerPage);
    } else {
        buf->newPage(headerPage);
    }
    
    
    //HeapFileHeader
    strncpy(heapFileHeader->tableName, tableName, 10);
    heapFileHeader->firstPage = 0;
    heapFileHeader->firstFreeSlotPage = 0;
    heapFileHeader->numPages = 0;
    heapFileHeader->isVolatile = volatileFlag;
    
    
    //heapFileHeader->recordSize;
    heapFileHeader->recordSize = 48;
    //heapFileHeader->recordDescription[600];
    heapFileHeader->bitmapSize = FLOPPYBitmap::calcBitmapSize(heapFileHeader->recordSize, BLOCKSIZE, sizeof(HeapPageHeader), 0);
    
    heapFileHeader->numRecordsPerPage = (BLOCKSIZE - sizeof(HeapPageHeader) - heapFileHeader->bitmapSize) / heapFileHeader->recordSize;
    
    //need to write fileHeader
    file_writeHeader(buf, headerPage.FD, fileHeader);
    
    //create page 1
    createNewPage(headerPage.FD, heapFileHeader);
    
    //need to write heapFileHeader
    writeFileHeader(headerPage.FD, heapFileHeader);
   	
    //free data
    free(fileHeader);
    free(heapFileHeader);
    
    return headerPage.FD;
}

int FLOPPYHeapFile::deleteFile(char * tableName) {
    return 0;
}

/* File Header Functions */

// Get the table name from a given header page
int FLOPPYHeapFile::headerGetTableName(fileDescriptor fd, char *name) {
    HeapFileHeader *header = getFileHeader(fd);
    name = (char*) calloc(strlen(header->tableName), sizeof(char));
    
    strcpy(name, header->tableName);
    
    free(header);
    return 0;
}

// Get the file descriptor structure
uint16_t FLOPPYHeapFile::headerGetRecordDesc(fileDescriptor fd, tableDescription *desc) {
    HeapFileHeader *header = getFileHeader(fd);
    uint16_t recordSize = header->recordSize;
    
    free(header);
    return recordSize;
}

// Return the address of the next page in the pagelist list
DiskAddress FLOPPYHeapFile::headerGetNextPage(fileDescriptor fd) {
    HeapFileHeader *header = getFileHeader(fd);
    DiskAddress dAdd;
    
    dAdd.FD = fd;
    dAdd.pageId = header->firstFreeSlotPage;
    
    free(header);
    return dAdd;
}

/* Data Page functions */

//given a page id and a location on the page (represented by id) retrieve the contents of the record into output array
unsigned char * FLOPPYHeapFile::getRecord(DiskAddress page, uint16_t dataOffset, uint16_t recordSize, int recordId) {
    int startOffset = dataOffset + (recordSize * recordId);
    
    return buf->read(page, startOffset, recordSize);
}

//given a page id and a location on the page, place the given record onto the appropriate slot of the page
int FLOPPYHeapFile::putRecord(DiskAddress page, uint16_t dataOffset, uint16_t recordSize, int recordId, char *data) {
    int startOffset = dataOffset + (recordSize * recordId);
    
    return buf->write(page, startOffset, recordSize, data);
}

//
int FLOPPYHeapFile::pHGetRecSize(DiskAddress page) {
    return 0;
}
int FLOPPYHeapFile::pHGetMaxRecords(DiskAddress page) {
    return 0;
}

/* Heap File CRUD operations */

int FLOPPYHeapFile::insertRecord(char * tableName, char * record) {
    HeapFileHeader *heapFileHeader;
    HeapPageHeader *heapPageHeader;
    uint8_t *bitmapData;
    FLOPPYBitmap *bitmap;
    DiskAddress dAdd;
    
    char *fileName = (char*) calloc(FILE_NAME_SIZE, sizeof(char));
    getFileName(tableName, fileName);
    fileDescriptor fd = buf->getFileDescriptor(fileName);
    free(fileName);
    
    heapFileHeader = getFileHeader(fd);
    
    //find open spot
    dAdd.FD = fd;
    dAdd.pageId = heapFileHeader->firstFreeSlotPage;
    
    heapPageHeader = getPageHeader(dAdd);
    bitmapData = (uint8_t *) buf->read(dAdd, sizeof(HeapPageHeader), heapFileHeader->bitmapSize);
    bitmap = new FLOPPYBitmap(bitmapData, heapFileHeader->numRecordsPerPage);
    
    int recordId = bitmap->getFirstFreeRecord();
    //printf("recordId: %d\n",recordId);
    
    //write data
    buf->write(dAdd, sizeof(HeapPageHeader) + heapFileHeader->bitmapSize + (heapFileHeader->recordSize * recordId), heapFileHeader->recordSize, record);
    
    
    //update record count
    heapPageHeader->numRecords++;
    
    //update bitmap
    bitmap->setRecordFull(recordId);
    buf->write(dAdd, sizeof(HeapPageHeader), heapFileHeader->bitmapSize, (char *) bitmap);
    
    if(bitmap->getFirstFreeRecord() == -1) {
        //page is full
        heapFileHeader->firstFreeSlotPage = heapPageHeader->nextFreeSlotPage;
        heapPageHeader->nextFreeSlotPage = 0;
        
        if(heapFileHeader->firstFreeSlotPage == 0) {
            //no empty pages, need to create a new one
            createNewPage(fd, heapFileHeader);
        }
        
        //write heapFileHeader
        writeFileHeader(fd, heapFileHeader);
    }
    
    //write and free heapPageHeader
    writePageHeader(dAdd, heapPageHeader);
    free(bitmapData);
    free(heapPageHeader);
    free(heapFileHeader);
    return 0;
}

int FLOPPYHeapFile::deleteRecord(DiskAddress page, int recordId) {
    HeapFileHeader *heapFileHeader = getFileHeader(page.FD);
    HeapPageHeader *heapPageHeader = getPageHeader(page);
    uint8_t *bitmapData;
    FLOPPYBitmap *bitmap;
    
    //get bitmap
    bitmapData = (uint8_t *) buf->read(page, sizeof(HeapPageHeader), heapFileHeader->bitmapSize);
    bitmap = new FLOPPYBitmap(bitmapData, heapFileHeader->numRecordsPerPage);
    int prevFreeRecordId = bitmap->getFirstFreeRecord();
    
    //update record count
    heapPageHeader->numRecords--;
    
    //update bitmap
    bitmap->setRecordEmpty(recordId);
    buf->write(page, sizeof(HeapPageHeader), heapFileHeader->bitmapSize, (char *) bitmapData);
    
    if(prevFreeRecordId == -1) {
        //page was full
        heapPageHeader->nextFreeSlotPage = heapFileHeader->firstFreeSlotPage;
        heapFileHeader->firstFreeSlotPage = page.pageId;
        
        //write heapFileHeader
        writeFileHeader(page.FD, heapFileHeader);
    }
    
    //write and free heapPageHeader
    writePageHeader(page, heapPageHeader);
    free(bitmapData);
    free(heapPageHeader);
    free(heapFileHeader);
    return 0;
}

int FLOPPYHeapFile::updateRecord(DiskAddress page, int recordId, char * record) {
    HeapFileHeader *heapFileHeader = getFileHeader(page.FD);
    
    //write data
    buf->write(page, sizeof(HeapPageHeader) + heapFileHeader->bitmapSize + (heapFileHeader->recordSize * recordId), heapFileHeader->recordSize, record);
    
    //free heapFileHeader
    free(heapFileHeader);
    return 0;
}


/* Test Functions */

void FLOPPYHeapFile::printFileInfo(fileDescriptor fd) {
    HeapFileHeader *heapFileHeader = getFileHeader(fd);
    FileHeader *fileHeader = file_getHeader(buf, fd);
    
    printf("Heap File Info:\n");
    printf("\t fileName: %s\n", fileHeader->fileName);
    printf("\t tableName: %s\n", heapFileHeader->tableName);
    printf("\t recordSize: %d\n", heapFileHeader->recordSize);
    printf("\t numRecordsPerPage: %d\n", heapFileHeader->numRecordsPerPage);
    printf("\t numPages: %d\n", heapFileHeader->numPages);
    printf("\t firstPage: %d\n", heapFileHeader->firstPage);
    printf("\t firstFreeSlotPage: %d\n", heapFileHeader->firstFreeSlotPage);
    printf("\t bitmapSize: %d\n", heapFileHeader->bitmapSize);
    printf("\t isVolatile: %d\n", heapFileHeader->isVolatile);
    
    //header->recordDescription;
    
    free(heapFileHeader);
    free(fileHeader);
}

void FLOPPYHeapFile::printPageInfo(fileDescriptor fd, int pageId) {
    DiskAddress page;
    page.FD = fd;
    page.pageId = pageId;
    
    HeapPageHeader *heapPageHeader = getPageHeader(page);
    HeapFileHeader *heapFileHeader = getFileHeader(page.FD);
    
    printf("Heap Page %d Info:\n", heapPageHeader->pageId);
    printf("\t fileName: %s;\n", heapPageHeader->fileName);
    printf("\t pageId: %d\n", heapPageHeader->pageId);
    printf("\t nextFreeSlotPage: %d\n", heapPageHeader->nextFreeSlotPage);
    printf("\t nextPage: %d\n", heapPageHeader->nextPage);
    printf("\t numRecords: %d\n", heapPageHeader->numRecords);
    
    uint8_t *bitmapData = (uint8_t *) buf->read(page, sizeof(HeapPageHeader), heapFileHeader->bitmapSize);
    FLOPPYBitmap *bitmap = new FLOPPYBitmap(bitmapData, heapFileHeader->numRecordsPerPage);
    
    //bitmap
    printf("\t bitmap: ");
    bitmap->print();
    printf("\n");
    
    free(heapPageHeader);
    free(heapFileHeader);
    free(bitmapData);
}