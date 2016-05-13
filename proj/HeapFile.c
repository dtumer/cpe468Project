#include "HeapFile.h"


HeapFileHeader * getHeapFileHeader(Buffer *buf, fileDescriptor fd) {
    DiskAddress page;
    unsigned char *data;
    
    page.FD = fd;
    page.pageId = 0;
    
    data = buf_read(buf, page, sizeof(FileHeader), sizeof(HeapFileHeader));
    
    return (HeapFileHeader *) data;
}


int heap_createFile(Buffer *buf, char *tableName, tableDescription *tableDesc, int volatileFlag) {
    FileHeader *fileHeader = malloc(sizeof(FileHeader));
    HeapFileHeader *heapHeader = malloc(sizeof(HeapFileHeader));
    const char *tbl = "T_";
    char *ptr;
    DiskAddress headerPage, firstPage;
    
    //FileHeader
    fileHeader->fileType = 1;
    
    ptr = fileHeader->fileName;
    strcpy(ptr, tbl);
    ptr += 2;
    strncpy(ptr, tableName, 14);
    
    //open file
    headerPage.FD = tfs_openFile(fileHeader->fileName);
    headerPage.pageId = 0;
    
    firstPage.FD = headerPage.FD;
    firstPage.pageId = 1;
    
    //create files
    if(volatileFlag) {
        allocateCachePage(buf, headerPage);
        allocateCachePage(buf, firstPage);
    } else {
        newPage(buf, headerPage);
        newPage(buf, firstPage);
    }
    
    
    //HeapFileHeader
    strncpy(heapHeader->tableName, tableName, 10);
    heapHeader->firstPage = 1;
    heapHeader->firstFreeSlotPage = 1;
    heapHeader->numPages = 1;
    
    
    //heapHeader->recordSize;
    //heapHeader->numRecordsPerPage;
    //heapHeader->recordDescription[600];
    
    
    //need to write pages
    file_writeHeader(buf, headerPage.FD, fileHeader);
    buf_write(buf, headerPage, sizeof(FileHeader), sizeof(HeapFileHeader), (char *) heapHeader);
        
    printf("filename: %s;\n", fileHeader->fileName);
    
    printFileHeader(buf, firstPage.FD);
    return 0;
}


int heap_headerGetTableName(Buffer *buf, fileDescriptor fd, char *name) {
    HeapFileHeader *header = getHeapFileHeader(buf, fd);
    
    name = header->tableName;
    
    return 0;
}
