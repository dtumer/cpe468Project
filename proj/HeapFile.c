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

void heap_writeFileHeader(Buffer *buf, fileDescriptor fd, HeapFileHeader *heapFileHeader) {
    DiskAddress page;
    
    page.FD = fd;
    page.pageId = 0;
    
    buf_write(buf, page, sizeof(FileHeader), sizeof(HeapFileHeader), (char *) heapFileHeader);
}

HeapPageHeader * heap_getPageHeader(Buffer *buf, fileDescriptor fd, int pageId) {
    DiskAddress page;
    unsigned char *data;
    
    page.FD = fd;
    page.pageId = pageId;
    
    data = buf_read(buf, page, 0, sizeof(HeapPageHeader));
    
    return (HeapPageHeader *) data;
}

void heap_createNewPage(Buffer *buf, fileDescriptor fd) {
    HeapFileHeader *heapFileHeader = heap_getFileHeader(buf, fd);
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
    
    //update free slot data
    heapPageHeader->nextFreeSlotPage = heapFileHeader->firstFreeSlotPage;
    heapFileHeader->firstFreeSlotPage = dAdd.pageId;
    
    char *bitmap = calloc(heapFileHeader->bitmapSize, sizeof(uint8_t));
    
    //write new page
    buf_write(buf, dAdd, 0, sizeof(HeapPageHeader), (char *) heapPageHeader);
    buf_write(buf, dAdd, sizeof(HeapPageHeader), heapFileHeader->bitmapSize, bitmap);
    
    //write updated header page
    heap_writeFileHeader(buf, fd, heapFileHeader);
    
    //free all data
    free(heapFileHeader);
    free(heapPageHeader);
    free(bitmap);
}

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
    heapFileHeader->firstPage = 1;
    heapFileHeader->firstFreeSlotPage = 0;
    heapFileHeader->numPages = 0;
    heapFileHeader->isVolatile = volatileFlag;
    
    
    //heapFileHeader->recordSize;
    //heapFileHeader->numRecordsPerPage;
    //heapFileHeader->recordDescription[600];
    heapFileHeader->bitmapSize = calcBitmapSize(heapFileHeader->recordSize, BLOCKSIZE, sizeof(HeapPageHeader), 0);
    
    //need to write pages
    file_writeHeader(buf, headerPage.FD, fileHeader);
    heap_writeFileHeader(buf, headerPage.FD, heapFileHeader);
   	
    //free data
    free(fileHeader);
    free(heapFileHeader);
    
    //create page 1
    heap_createNewPage(buf, headerPage.FD);
    return headerPage.FD;
}


int heap_headerGetTableName(Buffer *buf, fileDescriptor fd, char *name) {
    HeapFileHeader *header = heap_getFileHeader(buf, fd);
    
    name = header->tableName;
    
    free(header);
    return 0;
}

int heap_headerGetRecordDesc(Buffer *buf, fileDescriptor fd, tableDescription *desc) {
 	HeapFileHeader *header = heap_getFileHeader(buf, fd);
 	
 	free(header);
 	return 0;
}

int heap_headerGetNextPage(Buffer *buf, fileDescriptor fd, DiskAddress *page) {
	HeapFileHeader *header = heap_getFileHeader(buf, fd);
	
	free(header);
	return 0;
}

void printHeapFileHeader(Buffer *buf, fileDescriptor fd) {
    HeapFileHeader *header = heap_getFileHeader(buf, fd);
    
    printf("Heap File Header:\n");
    
    printf("\t tableName: %s\n", header->tableName);
    printf("\t recordSize: %d\n", header->recordSize);
    printf("\t numRecordsPerPage: %d\n", header->numRecordsPerPage);
    printf("\t numPages: %d\n", header->numPages);
    printf("\t firstPage: %d\n", header->firstPage);
    printf("\t firstFreeSlotPage: %d\n", header->firstFreeSlotPage);
    printf("\t bitmapSize: %d\n", header->bitmapSize);
    printf("\t isVolatile: %d\n", header->isVolatile);
    
    //header->recordDescription;
    
    
    free(header);
}

