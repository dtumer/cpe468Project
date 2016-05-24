#include "SeqFile.h"
#include "BufferManager.h"

void seq_getFileName(char *tableName, char *fileName) {
    strcpy(fileName, "T_");
    strncpy(&fileName[2], tableName, FILE_NAME_SIZE-3);
    fileName[FILE_NAME_SIZE-1] = '\0';
    return;
}

SeqFileHeader * seq_getFileHeader(Buffer *buf, fileDescriptor fd) {
    DiskAddress page;
    unsigned char *data;
    
    page.FD = fd;
    page.pageId = 0;
    
    data = buf_read(buf, page, sizeof(FileHeader), sizeof(SeqFileHeader));
    
    return (SeqFileHeader *) data;
}

int seq_writeFileHeader(Buffer *buf, fileDescriptor fd, SeqFileHeader *seqFileHeader) {
    DiskAddress page;
    
    page.FD = fd;
    page.pageId = 0;
    
    return buf_write(buf, page, sizeof(FileHeader), sizeof(SeqFileHeader), (char *) seqFileHeader);
}

SeqPageHeader * seq_getPageHeader(Buffer *buf, DiskAddress page) {
    unsigned char *data;
    
    data = buf_read(buf, page, 0, sizeof(SeqPageHeader));
    
    return (SeqPageHeader *) data;
}

int seq_writePageHeader(Buffer *buf, DiskAddress page, SeqPageHeader *seqPageHeader) {
    return buf_write(buf, page, 0, sizeof(SeqPageHeader), (char *) seqPageHeader);
}

void seq_createNewPage(Buffer *buf, fileDescriptor fd, SeqFileHeader *seqFileHeader) {
    FileHeader *fileHeader = file_getHeader(buf, fd);
    SeqPageHeader *seqPageHeader = malloc(sizeof(SeqPageHeader));
    DiskAddress dAdd;
    
    //open file
    dAdd.FD = fd;
    dAdd.pageId = ++seqFileHeader->numPages;
    
    //create file
    if(seqFileHeader->isVolatile) {
    	allocateCachePage(buf, dAdd);
    } else {
        newPage(buf, dAdd);
    }
    
    //add basic data
    seqPageHeader->pageId = dAdd.pageId;
    //seqPageHeader->nextPage = 0;
    seqPageHeader->numRecords = 0;
    strncpy(seqPageHeader->fileName, fileHeader->fileName, FILE_NAME_SIZE);
    
    //update free slot data
    seqPageHeader->nextFreeSlotPage = seqFileHeader->firstFreeSlotPage;
    seqFileHeader->firstFreeSlotPage = dAdd.pageId;
    
    //update next page
    seqPageHeader->nextPage = seqFileHeader->firstPage;
    seqFileHeader->firstPage = dAdd.pageId;
    
    char *bitmap = calloc(seqFileHeader->bitmapSize, sizeof(uint8_t));
    
    //write new page
    
    buf_write(buf, dAdd, 0, sizeof(SeqPageHeader), (char *) seqPageHeader);
    buf_write(buf, dAdd, sizeof(SeqPageHeader), seqFileHeader->bitmapSize, bitmap);
    
    //free all data
    free(fileHeader);
    free(seqPageHeader);
    free(bitmap);
}

/* file-level functions */

fileDescriptor seq_createFile(Buffer *buf, char *tableName, tableDescription *tableDesc, int volatileFlag) {
    FileHeader *fileHeader = malloc(sizeof(FileHeader));
    SeqFileHeader *seqFileHeader = malloc(sizeof(SeqFileHeader));
    DiskAddress headerPage;
    
    //FileHeader
    fileHeader->fileType = HEAP_FILE_TYPE;
    
    seq_getFileName(tableName, fileHeader->fileName);
    
    //open file
    headerPage.FD = getFileDescriptor(buf, fileHeader->fileName);
    headerPage.pageId = 0;
    
    //create files
    if(volatileFlag) {
        allocateCachePage(buf, headerPage);
    } else {
        newPage(buf, headerPage);
    }
    
    
    //SeqFileHeader
    strncpy(seqFileHeader->tableName, tableName, 10);
    seqFileHeader->firstPage = 0;
    seqFileHeader->firstFreeSlotPage = 0;
    seqFileHeader->numPages = 0;
    seqFileHeader->isVolatile = volatileFlag;
    
    
    //seqFileHeader->recordSize;
    seqFileHeader->recordSize = 48;
    //seqFileHeader->recordDescription[600];
    seqFileHeader->bitmapSize = calcBitmapSize(seqFileHeader->recordSize, BLOCKSIZE, sizeof(SeqPageHeader), 0);
    
    seqFileHeader->numRecordsPerPage = (BLOCKSIZE - sizeof(SeqPageHeader) - seqFileHeader->bitmapSize) / seqFileHeader->recordSize;
    
    //need to write fileHeader
    file_writeHeader(buf, headerPage.FD, fileHeader);
    
    //create page 1
    seq_createNewPage(buf, headerPage.FD, seqFileHeader);
    
    //need to write seqFileHeader
    seq_writeFileHeader(buf, headerPage.FD, seqFileHeader);
   	
    //free data
    free(fileHeader);
    free(seqFileHeader);
    
    return headerPage.FD;
}

int deleteSeqFile(char * tableName) {
    return 0;
}

/* File Header Functions */

// Get the table name from a given header page
int seq_headerGetTableName(Buffer *buf, fileDescriptor fd, char *name) {
    SeqFileHeader *header = seq_getFileHeader(buf, fd);
    name = calloc(strlen(header->tableName), sizeof(char));
    
    strcpy(name, header->tableName);
    
    free(header);
    return 0;
}

// Get the file descriptor structure
uint16_t seq_headerGetRecordDesc(Buffer *buf, fileDescriptor fd, tableDescription *desc) {
 	SeqFileHeader *header = seq_getFileHeader(buf, fd);
    uint16_t recordSize = header->recordSize;
    
 	free(header);
 	return recordSize;
}

// Return the address of the next page in the pagelist list
DiskAddress seq_headerGetNextPage(Buffer *buf, fileDescriptor fd) {
	SeqFileHeader *header = seq_getFileHeader(buf, fd);
    DiskAddress dAdd;
    
    dAdd.FD = fd;
    dAdd.pageId = header->firstFreeSlotPage;
    
	free(header);
	return dAdd;
}

/* Data Page functions */

//given a page id and a location on the page (represented by id) retrieve the contents of the record into output array
unsigned char * seq_getRecord(Buffer *buf, DiskAddress page, uint16_t dataOffset, uint16_t recordSize, int recordId) {
    int startOffset = dataOffset + (recordSize * recordId);
    
    return buf_read(buf, page, startOffset, recordSize);
}

//given a page id and a location on the page, place the given record onto the appropriate slot of the page
int seq_putRecord(Buffer *buf, DiskAddress page, uint16_t dataOffset, uint16_t recordSize, int recordId, char *data) {
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

/* Seq File CRUD operations */

int seq_insertRecord(Buffer *buf, char * tableName, char * record) {
    SeqFileHeader *seqFileHeader;
    SeqPageHeader *seqPageHeader;
    uint8_t *bitmap;
    DiskAddress dAdd;
    
    char *fileName = calloc(FILE_NAME_SIZE, sizeof(char));
    seq_getFileName(tableName, fileName);
    fileDescriptor fd = getFileDescriptor(buf, fileName);
    free(fileName);
    
    seqFileHeader = seq_getFileHeader(buf, fd);
    
    //find open spot
    dAdd.FD = fd;
    dAdd.pageId = seqFileHeader->firstFreeSlotPage;
    
    seqPageHeader = seq_getPageHeader(buf, dAdd);
    bitmap = (uint8_t *) buf_read(buf, dAdd, sizeof(SeqPageHeader), seqFileHeader->bitmapSize);
    int recordId = getFirstFreeRecord(bitmap, seqFileHeader->numRecordsPerPage);
    //printf("recordId: %d\n",recordId);
    
    //write data
    buf_write(buf, dAdd, sizeof(SeqPageHeader) + seqFileHeader->bitmapSize + (seqFileHeader->recordSize * recordId), seqFileHeader->recordSize, record);
    
    
    //update record count
    seqPageHeader->numRecords++;
    
    //update bitmap
    setBitmapRecordFull(bitmap, recordId);
    buf_write(buf, dAdd, sizeof(SeqPageHeader), seqFileHeader->bitmapSize, (char *) bitmap);
    
    if(getFirstFreeRecord(bitmap, seqFileHeader->numRecordsPerPage) == -1) {
        //page is full
        seqFileHeader->firstFreeSlotPage = seqPageHeader->nextFreeSlotPage;
        seqPageHeader->nextFreeSlotPage = 0;
        
        if(seqFileHeader->firstFreeSlotPage == 0) {
            //no empty pages, need to create a new one
            seq_createNewPage(buf, fd, seqFileHeader);
        }
        
        //write seqFileHeader
        seq_writeFileHeader(buf, fd, seqFileHeader);
    }
    
    //write and free seqPageHeader
    seq_writePageHeader(buf, dAdd, seqPageHeader);
    free(bitmap);
    free(seqPageHeader);
    free(seqFileHeader);
    return 0;
}

int seq_deleteRecord(Buffer *buf, DiskAddress page, int recordId) {
    SeqFileHeader *seqFileHeader = seq_getFileHeader(buf, page.FD);
    SeqPageHeader *seqPageHeader = seq_getPageHeader(buf, page);
    uint8_t *bitmap;
    
    //get bitmap
    bitmap = (uint8_t *) buf_read(buf, page, sizeof(SeqPageHeader), seqFileHeader->bitmapSize);
    int prevFreeRecordId = getFirstFreeRecord(bitmap, seqFileHeader->numRecordsPerPage);
    
    //update record count
    seqPageHeader->numRecords--;
    
    //update bitmap
    setBitmapRecordEmpty(bitmap, recordId);
    buf_write(buf, page, sizeof(SeqPageHeader), seqFileHeader->bitmapSize, (char *) bitmap);
    
    if(prevFreeRecordId == -1) {
        //page was full
        seqPageHeader->nextFreeSlotPage = seqFileHeader->firstFreeSlotPage;
        seqFileHeader->firstFreeSlotPage = page.pageId;
        
        //write seqFileHeader
        seq_writeFileHeader(buf, page.FD, seqFileHeader);
    }
    
    //write and free seqPageHeader
    seq_writePageHeader(buf, page, seqPageHeader);
    free(bitmap);
    free(seqPageHeader);
    free(seqFileHeader);
    return 0;
}

int seq_updateRecord(Buffer *buf, DiskAddress page, int recordId, char * record) {
    SeqFileHeader *seqFileHeader = seq_getFileHeader(buf, page.FD);
    
    //write data
    buf_write(buf, page, sizeof(SeqPageHeader) + seqFileHeader->bitmapSize + (seqFileHeader->recordSize * recordId), seqFileHeader->recordSize, record);
    
    //free seqFileHeader
    free(seqFileHeader);
    return 0;
}


/* Test Functions */

void printSeqFileInfo(Buffer *buf, fileDescriptor fd) {
    SeqFileHeader *seqFileHeader = seq_getFileHeader(buf, fd);
    FileHeader *fileHeader = file_getHeader(buf, fd);
    
    printf("Seq File Info:\n");
    printf("\t fileName: %s\n", fileHeader->fileName);
    printf("\t tableName: %s\n", seqFileHeader->tableName);
    printf("\t recordSize: %d\n", seqFileHeader->recordSize);
    printf("\t numRecordsPerPage: %d\n", seqFileHeader->numRecordsPerPage);
    printf("\t numPages: %d\n", seqFileHeader->numPages);
    printf("\t firstPage: %d\n", seqFileHeader->firstPage);
    printf("\t firstFreeSlotPage: %d\n", seqFileHeader->firstFreeSlotPage);
    printf("\t bitmapSize: %d\n", seqFileHeader->bitmapSize);
    printf("\t isVolatile: %d\n", seqFileHeader->isVolatile);
    
    //header->recordDescription;
    
    free(seqFileHeader);
    free(fileHeader);
}

void printSeqPageInfo(Buffer *buf, fileDescriptor fd, int pageId) {
    DiskAddress page;
    page.FD = fd;
    page.pageId = pageId;
    
    SeqPageHeader *seqPageHeader = seq_getPageHeader(buf, page);
    SeqFileHeader *seqFileHeader = seq_getFileHeader(buf, page.FD);
    uint8_t *bitmap;
    
    printf("Seq Page %d Info:\n", seqPageHeader->pageId);
    printf("\t fileName: %s;\n", seqPageHeader->fileName);
    printf("\t pageId: %d\n", seqPageHeader->pageId);
    printf("\t nextFreeSlotPage: %d\n", seqPageHeader->nextFreeSlotPage);
    printf("\t nextPage: %d\n", seqPageHeader->nextPage);
    printf("\t numRecords: %d\n", seqPageHeader->numRecords);
    
    bitmap = (uint8_t *) buf_read(buf, page, sizeof(SeqPageHeader), seqFileHeader->bitmapSize);
    
    //bitmap
    printf("\t bitmap: ");
    printBitmap(bitmap, seqFileHeader->numRecordsPerPage);
    printf("\n");
    
    free(seqPageHeader);
    free(seqFileHeader);
    free(bitmap);
}

