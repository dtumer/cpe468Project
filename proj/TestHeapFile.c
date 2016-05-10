#include "HeapFile.h"

#include <stdlib.h>
#include <string.h>

typedef struct FileNode {
	fileDescriptor FD;
	char *fileName;
	struct FileNode *next;
} FileNode;


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
void cleanup(tableDescription *table, Buffer *buf) {
	cleanupTable(table);
	
	free(table);
	free(buf);
}

int main(int argc, char *argv[]) {
	
	tableDescription *table = (tableDescription *)calloc(1, sizeof(tableDescription)); //description of table (after parsing)
	Buffer *buf = (Buffer *)calloc(1, sizeof(Buffer)); //buffer object for page storage
	
	initTableDesc(table);
	
	printTableDesc(table);
	
	//2. create table
		//create header page for table
	//3. store data?
	
	//cleanup
	cleanup(table, buf);

	return 0;
}