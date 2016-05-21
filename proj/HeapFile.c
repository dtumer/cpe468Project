#include "HeapFile.h"
#include "BufferManager.h"

/* Helper functions */

void heap_getFileName(char *tableName, char *fileName) {
    strcpy(fileName, "T_");
    strncpy(&fileName[2], tableName, FILE_NAME_SIZE-3);
    fileName[FILE_NAME_SIZE-1] = '\0';
    return;
}

HeapFileHeader * heap_getFileHeader(Buffer *buf, fileDescriptor fd) {
    DiskAddress page;
    unsigned char *data;
    
    page.FD = fd;
    page.pageId = 0;
    
    data = buf_read(buf, page, sizeof(FileHeader), sizeof(HeapFileHeader));
    
    return (HeapFileHeader *) data;
}

int heap_writeFileHeader(Buffer *buf, fileDescriptor fd, HeapFileHeader *heapFileHeader) {
    DiskAddress page;
    
    page.FD = fd;
    page.pageId = 0;
    
    return buf_write(buf, page, sizeof(FileHeader), sizeof(HeapFileHeader), (char *) heapFileHeader);
}

HeapPageHeader * heap_getPageHeader(Buffer *buf, DiskAddress page) {
    unsigned char *data;
    
    data = buf_read(buf, page, 0, sizeof(HeapPageHeader));
    
    return (HeapPageHeader *) data;
}

int heap_writePageHeader(Buffer *buf, DiskAddress page, HeapPageHeader *heapPageHeader) {
    return buf_write(buf, page, 0, sizeof(HeapPageHeader), (char *) heapPageHeader);
}

void heap_createNewPage(Buffer *buf, fileDescriptor fd, HeapFileHeader *heapFileHeader) {
    FileHeader *fileHeader = file_getHeader(buf, fd);
    HeapPageHeader *heapPageHeader = malloc(sizeof(HeapPageHeader));
    DiskAddress dAdd;
    
    //open file
    dAdd.FD = fd;
    dAdd.pageId = ++heapFileHeader->numPages;
    
    //create file
    if(heapFileHeader->isVolatile) {
    	allocateCachePage(buf, dAdd);
    } else {
        newPage(buf, dAdd);
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
    
    char *bitmap = calloc(heapFileHeader->bitmapSize, sizeof(uint8_t));
    
    //write new page
    
    buf_write(buf, dAdd, 0, sizeof(HeapPageHeader), (char *) heapPageHeader);
    buf_write(buf, dAdd, sizeof(HeapPageHeader), heapFileHeader->bitmapSize, bitmap);
    
    //free all data
    free(fileHeader);
    free(heapPageHeader);
    free(bitmap);
}

/* file-level functions */

fileDescriptor heap_createFile(Buffer *buf, char *tableName, tableDescription *tableDesc, int volatileFlag) {
    FileHeader *fileHeader = malloc(sizeof(FileHeader));
    HeapFileHeader *heapFileHeader = malloc(sizeof(HeapFileHeader));
    DiskAddress headerPage;
    
    //FileHeader
    fileHeader->fileType = HEAP_FILE_TYPE;
    
    heap_getFileName(tableName, fileHeader->fileName);
    
    //open file
    headerPage.FD = getFileDescriptor(buf, fileHeader->fileName);
    headerPage.pageId = 0;
    
    //create files
    if(volatileFlag) {
        allocateCachePage(buf, headerPage);
    } else {
        newPage(buf, headerPage);
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
    heapFileHeader->bitmapSize = calcBitmapSize(heapFileHeader->recordSize, BLOCKSIZE, sizeof(HeapPageHeader), 0);
    
    heapFileHeader->numRecordsPerPage = (BLOCKSIZE - sizeof(HeapPageHeader) - heapFileHeader->bitmapSize) / heapFileHeader->recordSize;
    
    //need to write fileHeader
    file_writeHeader(buf, headerPage.FD, fileHeader);
    
    //create page 1
    heap_createNewPage(buf, headerPage.FD, heapFileHeader);
    
    //need to write heapFileHeader
    heap_writeFileHeader(buf, headerPage.FD, heapFileHeader);
   	
    //free data
    free(fileHeader);
    free(heapFileHeader);
    
    return headerPage.FD;
}

int deleteHeapFile(char * tableName) {
    return 0;
}

/* File Header Functions */

// Get the table name from a given header page
int heap_headerGetTableName(Buffer *buf, fileDescriptor fd, char *name) {
    HeapFileHeader *header = heap_getFileHeader(buf, fd);
    name = calloc(strlen(header->tableName), sizeof(char));
    
    strcpy(name, header->tableName);
    
    free(header);
    return 0;
}

// Get the file descriptor structure
uint16_t heap_headerGetRecordDesc(Buffer *buf, fileDescriptor fd, tableDescription *desc) {
 	HeapFileHeader *header = heap_getFileHeader(buf, fd);
    uint16_t recordSize = header->recordSize;
    
 	free(header);
 	return recordSize;
}

// Return the address of the next page in the pagelist list
DiskAddress heap_headerGetNextPage(Buffer *buf, fileDescriptor fd) {
	HeapFileHeader *header = heap_getFileHeader(buf, fd);
    DiskAddress dAdd;
    
    dAdd.FD = fd;
    dAdd.pageId = header->firstFreeSlotPage;
    
	free(header);
	return dAdd;
}

/* Data Page functions */

//given a page id and a location on the page (represented by id) retrieve the contents of the record into output array
unsigned char * heap_getRecord(Buffer *buf, DiskAddress page, uint16_t dataOffset, uint16_t recordSize, int recordId) {
    int startOffset = dataOffset + (recordSize * recordId);
    
    return buf_read(buf, page, startOffset, recordSize);
}

//given a page id and a location on the page, place the given record onto the appropriate slot of the page
int heap_putRecord(Buffer *buf, DiskAddress page, uint16_t dataOffset, uint16_t recordSize, int recordId, char *data) {
    int startOffset = dataOffset + (recordSize * recordId);
    
    return buf_write(buf, page, startOffset, recordSize, data);
}

//
int pHGetRecSize(Buffer *buf, DiskAddress page) {
    return 0;
}
int pHGetMaxRecords(Buffer *buf, DiskAddress page) {
    return 0;
}

/* Heap File CRUD operations */

int heap_insertRecord(Buffer *buf, char * tableName, char * record) {
    HeapFileHeader *heapFileHeader;
    HeapPageHeader *heapPageHeader;
    uint8_t *bitmap;
    DiskAddress dAdd;
    
    char *fileName = calloc(FILE_NAME_SIZE, sizeof(char));
    heap_getFileName(tableName, fileName);
    fileDescriptor fd = getFileDescriptor(buf, fileName);
    free(fileName);
    
    heapFileHeader = heap_getFileHeader(buf, fd);
    
    //find open spot
    dAdd.FD = fd;
    dAdd.pageId = heapFileHeader->firstFreeSlotPage;
    
    heapPageHeader = heap_getPageHeader(buf, dAdd);
    bitmap = (uint8_t *) buf_read(buf, dAdd, sizeof(HeapPageHeader), heapFileHeader->bitmapSize);
    int recordId = getFirstFreeRecord(bitmap, heapFileHeader->numRecordsPerPage);
    //printf("recordId: %d\n",recordId);
    
    //write data
    buf_write(buf, dAdd, sizeof(HeapPageHeader) + heapFileHeader->bitmapSize + (heapFileHeader->recordSize * recordId), heapFileHeader->recordSize, record);
    
    
    //update record count
    heapPageHeader->numRecords++;
    
    //update bitmap
    setBitmapRecordFull(bitmap, recordId);
    buf_write(buf, dAdd, sizeof(HeapPageHeader), heapFileHeader->bitmapSize, (char *) bitmap);
    
    if(getFirstFreeRecord(bitmap, heapFileHeader->numRecordsPerPage) == -1) {
        //page is full
        heapFileHeader->firstFreeSlotPage = heapPageHeader->nextFreeSlotPage;
        heapPageHeader->nextFreeSlotPage = 0;
        
        if(heapPageHeader->nextFreeSlotPage == 0) {
            //no empty pages, need to create a new one
            heap_createNewPage(buf, fd, heapFileHeader);
        }
        
        //write heapFileHeader
        heap_writeFileHeader(buf, fd, heapFileHeader);
    }
    
    //write and free heapPageHeader
    heap_writePageHeader(buf, dAdd, heapPageHeader);
    free(bitmap);
    free(heapPageHeader);
    free(heapFileHeader);
    return 0;
}

//int heap_deleteRecord(Buffer *buf, DiskAddress page, int recordId);

//int heap_updateRecord(Buffer *buf, DiskAddress page, int recordId, char * record) {}


/* Test Functions */

void printHeapFileInfo(Buffer *buf, fileDescriptor fd) {
    HeapFileHeader *heapFileHeader = heap_getFileHeader(buf, fd);
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

void printHeapPageInfo(Buffer *buf, DiskAddress page) {
    HeapPageHeader *heapPageHeader = heap_getPageHeader(buf, page);
    HeapFileHeader *heapFileHeader = heap_getFileHeader(buf, page.FD);
    uint8_t *bitmap;
    
    printf("Heap Page %d Info:\n", heapPageHeader->pageId);
    printf("\t fileName: %s;\n", heapPageHeader->fileName);
    printf("\t pageId: %d\n", heapPageHeader->pageId);
    printf("\t nextFreeSlotPage: %d\n", heapPageHeader->nextFreeSlotPage);
    printf("\t nextPage: %d\n", heapPageHeader->nextPage);
    printf("\t numRecords: %d\n", heapPageHeader->numRecords);
    
    bitmap = (uint8_t *) buf_read(buf, page, sizeof(HeapPageHeader), heapFileHeader->bitmapSize);
    
    //bitmap
    printf("\t bitmap: ");
    printBitmap(bitmap, heapFileHeader->numRecordsPerPage);
    printf("\n");
    
    free(heapPageHeader);
    free(heapFileHeader);
    free(bitmap);
}

