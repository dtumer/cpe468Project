#include "HeapFile.h"

#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int ndx;
    DiskAddress dAdd;
    char diskName[] = "Foo.disk";
    char tableName[] = "people";
    
	tableDescription *table = (tableDescription *)calloc(1, sizeof(tableDescription)); //description of table (after parsing)
	Buffer *buf = (Buffer *)calloc(1, sizeof(Buffer)); //buffer object for page storage
    commence(diskName, buf, 5, 5);

    fileDescriptor fd = heap_createFile(buf, tableName, table, 1);
    dAdd.FD = fd;
    
    //print before adding data
    printf("\nEmpty heap file\n");
    printFileHeader(buf, fd);
    printHeapFileInfo(buf, fd);
    printHeapPageInfo(buf, fd, 1);
    
    char *record = (char*) calloc(48, sizeof(char));
    strcpy(record, "1234567890;1234567890;1234567890;1234567890;aaa");
    
    //add data till almost full
    printf("\nadd data till almost full\n");
    for(ndx=0; ndx<41; ndx++) {
    	heap_insertRecord(buf, tableName, record);
    }
    
    printHeapFileInfo(buf, fd);
    printHeapPageInfo(buf, fd, 1);
    
    //fill page
    printf("\nfill page\n");
    heap_insertRecord(buf, tableName, record);
    
    printHeapFileInfo(buf, fd);
    printHeapPageInfo(buf, fd, 1);
    printHeapPageInfo(buf, fd, 2);
    
    //delete record from page 1
    printf("\ndelete record from page 1\n");
    dAdd.pageId = 1;
    heap_deleteRecord(buf, dAdd, 9);
    
    printHeapFileInfo(buf, fd);
    printHeapPageInfo(buf, fd, 1);
    printHeapPageInfo(buf, fd, 2);
    
    //re-fill page
    printf("\nre-fill page\n");
    heap_insertRecord(buf, tableName, record);
    
    printHeapFileInfo(buf, fd);
    printHeapPageInfo(buf, fd, 1);
    printHeapPageInfo(buf, fd, 2);
    
	
	//cleanup
    free(record);
    squash(buf);

	return 0;
}