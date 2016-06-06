#include "FLOPPYHeapFile.h"

#include <stdio.h>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
    int ndx;
    DiskAddress dAdd;
    char diskName[] = "Foo.disk";
    char tableName[] = "people";
    
	tableDescription *table = (tableDescription *)calloc(1, sizeof(tableDescription)); //description of table (after parsing)
    
    FLOPPYBufferManager *buf = new FLOPPYBufferManager(diskName, 5, 5);
    
    FLOPPYHeapFile *heap = new FLOPPYHeapFile(buf);

    fileDescriptor fd = heap->createFile(tableName, table, 1);
    dAdd.FD = fd;
    
    //print before adding data
    printf("\nEmpty heap file\n");
    heap->printFileHeader();
    heap->printFileInfo(fd);
    heap->printPageInfo(fd, 1);
    
    char *record = (char*) calloc(48, sizeof(char));
    strcpy(record, "1234567890;1234567890;1234567890;1234567890;aaa");
    
    //add data till almost full
    printf("\nadd data till almost full\n");
    for(ndx=0; ndx<41; ndx++) {
    	heap->insertRecord(tableName, record);
    }
    
    heap->printFileInfo(fd);
    heap->printPageInfo(fd, 1);
    
    //fill page
    printf("\nfill page\n");
    heap->insertRecord(tableName, record);
    
    heap->printFileInfo(fd);
    heap->printPageInfo(fd, 1);
    heap->printPageInfo(fd, 2);
    
    //delete record from page 1
    printf("\ndelete record from page 1\n");
    dAdd.pageId = 1;
    heap->deleteRecord(dAdd, 9);
    
    heap->printFileInfo(fd);
    heap->printPageInfo(fd, 1);
    heap->printPageInfo(fd, 2);
    
    //re-fill page
    printf("\nre-fill page\n");
    heap->insertRecord(tableName, record);
    
    heap->printFileInfo(fd);
    heap->printPageInfo(fd, 1);
    heap->printPageInfo(fd, 2);
    
	
	//cleanup
    free(record);
    delete buf;

	return 0;
}