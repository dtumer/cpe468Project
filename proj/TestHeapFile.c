#include "HeapFile.h"

#include <stdlib.h>
#include <string.h>

//prints out attributes
void printAttributes(Attribute *head, char *prefix) {
	Attribute *temp = head;
	
	while (temp) {
		printf("%s-Name: %s\n", prefix, temp->attName);
		printf("%s-Type: %d\n", prefix, temp->attType);
		
		temp = temp->next;
	}
}

void printForeignKeys(foreignKeys *head) {
	foreignKeys *temp = head;
	char keyPrefix[5] = "FKEY";
	
	while (temp) {
		printf("REF TABLE: %s\n", temp->tableName);
		printAttributes(head->key, keyPrefix);	
		
		temp = temp->next;
	}
}

//prints the entire table description
void printTableDesc(tableDescription *desc) {
	char attPrefix[5] = "ATTR";
	char keyPrefix[5] = "PKEY";
	
	printf("TABLE: %s\n", desc->tableName);
	printAttributes(desc->attributeList, attPrefix);
	
	printf("\nPrimary Keys:\n");
	printAttributes(desc->pKey, keyPrefix);
	
	printf("\nForeign Keys:\n");
	printForeignKeys(desc->fKeys);
}

//attribute types
//1: integer (4 bytes)
//2: float (8 bytes)
//3: char (n bytes)
//4: varchar (max n bytes)
int getAttType() {
	return (rand() % 4) + 1;
}

//initializes an attributes list and returns the pointer to it
Attribute* initAttributes() {
	int i, attType;
	int numAttrs = rand() % 10;
	Attribute *att, *head, *temp;
	char *attName;
	
	//initialize attributes
	for (i = 0; i < numAttrs; i++) {
		attType = getAttType();
		attName = (char *)calloc(10, sizeof(char));
		snprintf(attName, 10, "attrTest%d", i + 1);
		
		att = (Attribute *)calloc(1, sizeof(Attribute));
		att->attName = attName;
		att->attType = attType;
		
		if (i == 0) {
			head = att;
		}
		else {
			temp = head;
			while (temp->next) {
				temp = temp->next;
			}
			
			temp->next = att;
		}
	}
	
	return head;
}

//sets up the keys
Attribute* getKeys(tableDescription *desc) {
	int numKeys = 1, i;
	Attribute *attHead = desc->attributeList;
	Attribute *key = (Attribute *)calloc(1, sizeof(Attribute));
	
	for (i = 0; i < numKeys; i++) {
		key->attName = (char *)calloc(strlen(attHead->attName) + 1, sizeof(char));
		strcpy(key->attName, attHead->attName);
		key->attType = attHead->attType;
		key->next = NULL;
	}
	
	return key;
}

//initializes the table description
void initTableDesc(tableDescription *desc) {
	char tableName[10] = "hello";
	
	desc->tableName = tableName;
	desc->attributeList = initAttributes();
	desc->pKey = getKeys(desc);
	desc->fKeys = NULL;
}

//frees an attribute list
void freeAttList(Attribute *head) {
	Attribute *temp;
	
	while (head) {
		temp = head;
		head = head->next;
		
		free(temp->attName);
		free(temp);
	}
}

//cleans table
void cleanupTable(tableDescription *table) {
	foreignKeys *fKey, *tmpfHead;
	
	//free(table->tableName);
	
	//free attributes
	freeAttList(table->attributeList);
	
	//free pkeys
	freeAttList(table->pKey);
		
	//free fkeys
	tmpfHead = table->fKeys;
	while (tmpfHead) {
		fKey = tmpfHead;
		tmpfHead = tmpfHead->next;
		
		free(fKey->tableName);
		freeAttList(fKey->key);
		free(fKey);
	}
}

//cleanup
void cleanup(tableDescription *table) {
	cleanupTable(table);
	
	free(table);
}

int main(int argc, char *argv[]) {
    int ndx;
    DiskAddress dAdd;
    
	tableDescription *table = (tableDescription *)calloc(1, sizeof(tableDescription)); //description of table (after parsing)
	Buffer *buf = (Buffer *)calloc(1, sizeof(Buffer)); //buffer object for page storage
    commence("Foo.disk", buf, 5, 5);

    
	initTableDesc(table);
	
	printTableDesc(table);
    fileDescriptor fd = heap_createFile(buf, "people", table, 1);
    dAdd.FD = fd;
    
    //print before adding data
    printf("\nEmpty heap file\n");
    printFileHeader(buf, fd);
    printHeapFileInfo(buf, fd);
    printHeapPageInfo(buf, fd, 1);
    
    char *record = calloc(48, sizeof(char));
    strcpy(record, "1234567890;1234567890;1234567890;1234567890;aaa");
    
    //add data till almost full
    printf("\nadd data till almost full\n");
    for(ndx=0; ndx<41; ndx++) {
    	heap_insertRecord(buf, "people", record);
    }
    
    printHeapFileInfo(buf, fd);
    printHeapPageInfo(buf, fd, 1);
    
    //fill page
    printf("\nfill page\n");
    heap_insertRecord(buf, "people", record);
    
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
    heap_insertRecord(buf, "people", record);
    
    printHeapFileInfo(buf, fd);
    printHeapPageInfo(buf, fd, 1);
    printHeapPageInfo(buf, fd, 2);
    
	
	//cleanup
    free(record);
	cleanup(table);
    squash(buf);

	return 0;
}