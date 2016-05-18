//
//  BplusTree.c
//  proj
//
//  Created by David Ellison on 5/17/16.
//
//

#include "BplusTree.h"

#define HEAP_FILE_TYPE 1
#define SEQ_FILE_TYPE 2
#define HASH_FILE_TYPE 3

int createBTree(char *table, Attribute *key, int volatileFlag) {
    FileHeader *fileHeader = malloc(sizeof(FileHeader));
    HeapFileHeader *bPlusHeader = malloc(sizeof(HeapFileHeader));
    const char *tbl = "I_";
    char *ptr;
    DiskAddress headerPage, firstPage;
    
    //FileHeader
    fileHeader->fileType = HEAP_FILE_TYPE;
    
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
    
    
    //bPlusHeader
    strncpy(bPlusHeader->tableName, tableName, 10);
    bPlusHeader->firstPage = 1;
    bPlusHeader->firstFreeSlotPage = 1;
    bPlusHeader->numPages = 1;
    
    
    //bPlusHeader->recordSize;
    //bPlusHeader->numRecordsPerPage;
    //bPlusHeader->recordDescription[600];
    
    
    //need to write pages
    file_writeHeader(buf, headerPage.FD, fileHeader);
    buf_write(buf, headerPage, sizeof(FileHeader), sizeof(HeapFileHeader), (char *) bPlusHeader);
        
    printf("filename: %s;\n", fileHeader->fileName);
    
    printFileHeader(buf, firstPage.FD);
    
    free(fileHeader);
    free(bPlusHeader);
    return 0;
}

int dropBTree(char *btreeName) {
}
