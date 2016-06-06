#include "FLOPPYHeapFile.h"

#include <stdio.h>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
    int ndx;
    DiskAddress dAdd;
    char diskName[] = "Foo.disk";
    char tableName[] = "people";
    FLOPPYHeapFile *heap;
    
	tableDescription *table = (tableDescription *)calloc(1, sizeof(tableDescription)); //description of table (after parsing)
    
    FLOPPYBufferManager *buf = new FLOPPYBufferManager(diskName, 5, 5);
    
    heap = FLOPPYHeapFile::createFile(buf, tableName, table, 1);
    
    dAdd.FD = heap->fd;
    
    fileDescriptor fd = heap->fd;
    
    //print before adding data
    printf("\nEmpty heap file\n");
    heap->printFileHeader();
    heap->printFileInfo();
    heap->printPageInfo(1);
    
    char *record = (char*) calloc(48, sizeof(char));
    strcpy(record, "1234567890;1234567890;1234567890;1234567890;aaa");
    
    //add data till almost full
    printf("\nadd data till almost full\n");
    for(ndx=0; ndx<41; ndx++) {
    	heap->insertRecord(record);
    }
    
    heap->printFileInfo();
    heap->printPageInfo(1);
    
    //fill page
    printf("\nfill page\n");
    heap->insertRecord(record);
    
    heap->printFileInfo();
    heap->printPageInfo(1);
    heap->printPageInfo(2);
    
    //delete record from page 1
    printf("\ndelete record from page 1\n");
    heap->deleteRecord(1, 9);
    
    heap->printFileInfo();
    heap->printPageInfo(1);
    heap->printPageInfo(2);
    
    //re-fill page
    printf("\nre-fill page\n");
    heap->insertRecord(record);
    
    heap->printFileInfo();
    heap->printPageInfo(1);
    heap->printPageInfo(2);
    
	
	//cleanup
    free(record);
    delete buf;

	return 0;
}