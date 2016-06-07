#include "FLOPPYHeapFile.h"

#include <stdio.h>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
    int ndx;
    char diskName[] = "Foo.disk";
    char tableName[] = "people";
    FLOPPYBufferManager *buf = new FLOPPYBufferManager(diskName, 5, 5);
    
    FLOPPYOutput *result = FLOPPYParser::parseFLOPPYString("CREATE TABLE Availability VOLATILE (NodeId  VARCHAR(20), DistroId  INT, FileId  INT, BlockID  INT, TimeStamp DATETIME, PRIMARY KEY(FileId, BlockId, NodeId), FOREIGN KEY(NodeId) REFERENCES Nodes, FOREIGN KEY (DistroId) REFERENCES Distros, FOREIGN KEY (FileId) REFERENCES Files);");
    
    FLOPPYHeapFile *heap = FLOPPYHeapFile::createFile(buf, (FLOPPYCreateTableStatement*) result->statement);
    
    //print before adding data
    printf("\nEmpty heap file\n");
    heap->printFileHeader();
    heap->printFileInfo();
    heap->printPageInfo(1);
    
    char *record = (char*) calloc(48, sizeof(char));
    strcpy(record, "1234567890;1234567890;1234567890;1234567890;aaa");
    
    //add data till almost full
    printf("\nadd data till almost full\n");
    for(ndx=0; ndx<49; ndx++) {
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
    heap->deleteRecord(1, 49);
    
    heap->printFileInfo();
    heap->printPageInfo(1);
    heap->printPageInfo(2);
    
    heap->getAllRecords(1);
    heap->getAllRecords(2);
    
    //re-fill page
    printf("\nre-fill page\n");
    heap->insertRecord(record);
    
    heap->printFileInfo();
    heap->printPageInfo(1);
    heap->printPageInfo(2);
    
    
    heap->printRecordDescription();
    
	//cleanup
    free(record);
    delete heap;
    delete result;
    delete buf;

	return 0;
}