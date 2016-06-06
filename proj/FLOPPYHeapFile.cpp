//
//  FLOPPYHeapFile.cpp
//

#include "FLOPPYHeapFile.h"

FLOPPYHeapFile::FLOPPYHeapFile(FLOPPYBufferManager *buf, fileDescriptor fd) : FLOPPYFileManager(buf, fd) {
    
}

FLOPPYHeapFile::FLOPPYHeapFile(FLOPPYBufferManager *buf, std::string tableName) : FLOPPYFileManager(buf) {
    char *fileName = (char*) calloc(FILE_NAME_SIZE, sizeof(char));
    getFileName(tableName, fileName);
    fd = buf->getFileDescriptor(fileName);
    free(fileName);
}

/* Helper functions */

void FLOPPYHeapFile::getFileName(std::string tableName, char *fileName) {
    strcpy(fileName, "T_");
    strncpy(&fileName[2], tableName.c_str(), FILE_NAME_SIZE-3);
    fileName[FILE_NAME_SIZE-1] = '\0';
    return;
}

HeapFileHeader * FLOPPYHeapFile::getHeapFileHeader() {
    DiskAddress page = getDiskAddress(0);
    
    return (HeapFileHeader *) buf->read(page, sizeof(FileHeader), sizeof(HeapFileHeader));
}

int FLOPPYHeapFile::writeHeapFileHeader(HeapFileHeader *heapFileHeader) {
    DiskAddress page = getDiskAddress(0);
    
    return buf->write(page, sizeof(FileHeader), sizeof(HeapFileHeader), (char *) heapFileHeader);
}

HeapPageHeader * FLOPPYHeapFile::getPageHeader(int pageId) {
    unsigned char *data;
    DiskAddress page = getDiskAddress(pageId);
    data = buf->read(page, 0, sizeof(HeapPageHeader));
    
    return (HeapPageHeader *) data;
}

int FLOPPYHeapFile::writePageHeader(int pageId, HeapPageHeader *heapPageHeader) {
    DiskAddress page = getDiskAddress(pageId);
    return buf->write(page, 0, sizeof(HeapPageHeader), (char *) heapPageHeader);
}

void FLOPPYHeapFile::createNewPage(HeapFileHeader *heapFileHeader) {
    FileHeader *fileHeader = getFileHeader();
    HeapPageHeader *heapPageHeader = (HeapPageHeader*) malloc(sizeof(HeapPageHeader));
    int pageId = ++heapFileHeader->numPages;
    DiskAddress page = getDiskAddress(pageId);
    
    //create file
    if(heapFileHeader->isVolatile) {
        buf->allocateCachePage(page);
    } else {
        buf->newPage(page);
    }
    
    //add basic data
    heapPageHeader->pageId = pageId;
    heapPageHeader->numRecords = 0;
    strncpy(heapPageHeader->fileName, fileHeader->fileName, FILE_NAME_SIZE);
    
    //update free slot data
    heapPageHeader->nextFreeSlotPage = heapFileHeader->firstFreeSlotPage;
    heapFileHeader->firstFreeSlotPage = pageId;
    
    //update next page
    heapPageHeader->nextPage = heapFileHeader->firstPage;
    heapFileHeader->firstPage = pageId;
    
    char *bitmap = (char*) calloc(heapFileHeader->bitmapSize, sizeof(uint8_t));
    
    //write new page
    
    buf->write(page, 0, sizeof(HeapPageHeader), (char *) heapPageHeader);
    buf->write(page, sizeof(HeapPageHeader), heapFileHeader->bitmapSize, bitmap);
    
    //free all data
    free(fileHeader);
    free(heapPageHeader);
    free(bitmap);
}

DiskAddress FLOPPYHeapFile::getDiskAddress(int pageId) {
    DiskAddress page;
    page.FD = fd;
    page.pageId = pageId;
    return page;
}

/* file-level functions */

FLOPPYHeapFile * FLOPPYHeapFile::createFile(FLOPPYBufferManager *buf, FLOPPYCreateTableStatement *statement) {
    FLOPPYHeapFile *heapFile;
    FileHeader *fileHeader = (FileHeader*) malloc(sizeof(FileHeader));
    HeapFileHeader *heapFileHeader = (HeapFileHeader*) malloc(sizeof(HeapFileHeader));
    
    //FileHeader
    fileHeader->fileType = HEAP_FILE_TYPE;
    
    FLOPPYHeapFile::getFileName(statement->tableName, fileHeader->fileName);
    
    //open file
    heapFile = new FLOPPYHeapFile(buf, buf->getFileDescriptor(fileHeader->fileName));
    DiskAddress headerPage = heapFile->getDiskAddress(0);
    
    //create files
    if(statement->flags->volatileFlag) {
        buf->allocateCachePage(headerPage);
    } else {
        buf->newPage(headerPage);
    }
    
    //HeapFileHeader
    strncpy(heapFileHeader->tableName, statement->tableName.c_str(), 10);
    heapFileHeader->firstPage = 0;
    heapFileHeader->firstFreeSlotPage = 0;
    heapFileHeader->numPages = 0;
    heapFileHeader->isVolatile = statement->flags->volatileFlag;
    
    packRecordDescription(statement, heapFileHeader->recordDescription);
    heapFileHeader->recordSize = getRecordLength(heapFileHeader->recordDescription);
    
    //heapFileHeader->recordSize = 48;
    //heapFileHeader->recordDescription[600];
    heapFileHeader->bitmapSize = FLOPPYBitmap::calcBitmapSize(heapFileHeader->recordSize, BLOCKSIZE, sizeof(HeapPageHeader), 0);
    
    heapFileHeader->numRecordsPerPage = (BLOCKSIZE - sizeof(HeapPageHeader) - heapFileHeader->bitmapSize) / heapFileHeader->recordSize;
    
    //need to write fileHeader
    heapFile->writeFileHeader(fileHeader);
    
    //create page 1
    heapFile->createNewPage(heapFileHeader);
    
    //need to write heapFileHeader
    heapFile->writeHeapFileHeader(heapFileHeader);
   	
    //free data
    free(fileHeader);
    free(heapFileHeader);
    
    return heapFile;
}

int FLOPPYHeapFile::deleteFile(std::string tableName) {
    return 0;
}

/* File Header Functions */

// Get the table name from a given header page
int FLOPPYHeapFile::headerGetTableName(char *name) {
    HeapFileHeader *header = getHeapFileHeader();
    name = (char*) calloc(strlen(header->tableName), sizeof(char));
    
    strcpy(name, header->tableName);
    
    free(header);
    return 0;
}

// Get the file descriptor structure
/*uint16_t FLOPPYHeapFile::headerGetRecordDesc(tableDescription *desc) {
    
    return 0;
}*/

// Return the address of the next page in the pagelist list
DiskAddress FLOPPYHeapFile::headerGetNextPage() {
    HeapFileHeader *header = getHeapFileHeader();
    DiskAddress page = getDiskAddress(header->firstFreeSlotPage);
    free(header);
    return page;
}

/* Data Page functions */

//given a page id and a location on the page (represented by id) retrieve the contents of the record into output array
unsigned char * FLOPPYHeapFile::getRecord(int pageId, uint16_t dataOffset, uint16_t recordSize, int recordId) {
    DiskAddress page = getDiskAddress(pageId);
    int startOffset = dataOffset + (recordSize * recordId);
    return buf->read(page, startOffset, recordSize);
}

//given a page id and a location on the page, place the given record onto the appropriate slot of the page
int FLOPPYHeapFile::putRecord(int pageId, uint16_t dataOffset, uint16_t recordSize, int recordId, char *data) {
    DiskAddress page = getDiskAddress(pageId);
    int startOffset = dataOffset + (recordSize * recordId);
    return buf->write(page, startOffset, recordSize, data);
}


/* Heap File CRUD operations */

int FLOPPYHeapFile::insertRecord(char * record) {
    HeapFileHeader *heapFileHeader = getHeapFileHeader();
    HeapPageHeader *heapPageHeader;
    uint8_t *bitmapData;
    FLOPPYBitmap *bitmap;
    int pageId;
    
    //find open spot
    pageId = heapFileHeader->firstFreeSlotPage;
    DiskAddress page = getDiskAddress(pageId);
    
    heapPageHeader = getPageHeader(heapFileHeader->firstFreeSlotPage);
    bitmapData = (uint8_t *) buf->read(page, sizeof(HeapPageHeader), heapFileHeader->bitmapSize);
    bitmap = new FLOPPYBitmap(bitmapData, heapFileHeader->numRecordsPerPage);
    
    int recordId = bitmap->getFirstFreeRecord();
    //printf("recordId: %d\n",recordId);
    
    //write data
    buf->write(page, sizeof(HeapPageHeader) + heapFileHeader->bitmapSize + (heapFileHeader->recordSize * recordId), heapFileHeader->recordSize, record);
    
    
    //update record count
    heapPageHeader->numRecords++;
    
    //update bitmap
    bitmap->setRecordFull(recordId);
    buf->write(page, sizeof(HeapPageHeader), heapFileHeader->bitmapSize, (char *) bitmapData);
    
    if(bitmap->getFirstFreeRecord() == -1) {
        //page is full
        heapFileHeader->firstFreeSlotPage = heapPageHeader->nextFreeSlotPage;
        heapPageHeader->nextFreeSlotPage = 0;
        
        if(heapFileHeader->firstFreeSlotPage == 0) {
            //no empty pages, need to create a new one
            createNewPage(heapFileHeader);
        }
        
        //write heapFileHeader
        writeHeapFileHeader(heapFileHeader);
    }
    
    //write and free heapPageHeader
    writePageHeader(pageId, heapPageHeader);
    free(bitmapData);
    free(heapPageHeader);
    free(heapFileHeader);
    return 0;
}

int FLOPPYHeapFile::deleteRecord(int pageId, int recordId) {
    HeapFileHeader *heapFileHeader = getHeapFileHeader();
    HeapPageHeader *heapPageHeader = getPageHeader(pageId);
    DiskAddress page = getDiskAddress(pageId);
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
        writeHeapFileHeader(heapFileHeader);
    }
    
    //write and free heapPageHeader
    writePageHeader(pageId, heapPageHeader);
    free(bitmapData);
    free(heapPageHeader);
    free(heapFileHeader);
    return 0;
}

int FLOPPYHeapFile::updateRecord(int pageId, int recordId, char * record) {
    HeapFileHeader *heapFileHeader = getHeapFileHeader();
    DiskAddress page = getDiskAddress(pageId);
    
    //write data
    buf->write(page, sizeof(HeapPageHeader) + heapFileHeader->bitmapSize + (heapFileHeader->recordSize * recordId), heapFileHeader->recordSize, record);
    
    //free heapFileHeader
    free(heapFileHeader);
    return 0;
}

/* Test Functions */

void FLOPPYHeapFile::printFileInfo() {
    HeapFileHeader *heapFileHeader = getHeapFileHeader();
    FileHeader *fileHeader = getFileHeader();
    
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

void FLOPPYHeapFile::printPageInfo(int pageId) {
    DiskAddress page = getDiskAddress(pageId);
    
    HeapPageHeader *heapPageHeader = getPageHeader(pageId);
    HeapFileHeader *heapFileHeader = getHeapFileHeader();
    
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

// testing function for printing all recordIds
void FLOPPYHeapFile::getAllRecords(int pageId) {
    DiskAddress page = getDiskAddress(pageId);
	HeapPageHeader *heapPageHeader = getPageHeader(pageId);
	HeapFileHeader *heapFileHeader = getHeapFileHeader();
	uint8_t *bitmapData = (uint8_t *) buf->read(page, sizeof(HeapPageHeader), heapFileHeader->bitmapSize);
    FLOPPYBitmap *bitmap = new FLOPPYBitmap(bitmapData, heapFileHeader->numRecordsPerPage);
    int recordId = 0;
    
    while (recordId >= 0) {
    	recordId = bitmap->nextRecord();
    	printf("RECORD: %d\n", recordId);
    }
}