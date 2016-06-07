#include "FLOPPYBufferManager.h"

#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <string.h>
#include <iostream>

//#include "libs/smartalloc.h"


//function for running the buffer
void runBuffer(FILE *fp) {
	FLOPPYBufferManager *buf;
	char command[10], diskName[1024], fileName[1024];
	int num1, num2, i;
	fileDescriptor FD;
	DiskAddress dAdd;
	
	while (fscanf(fp, "%s", command) == 1) {
        //start command
		if (!strcmp(command, "start")) {
			if (fscanf(fp, "%s", diskName) == 1 && fscanf(fp, "%d", &num1) == 1) {
				printf("START: %s, %d\n", diskName, num1);
                buf = new FLOPPYBufferManager(diskName, num1, num1);
			}
		}
		//end command
		else if (!strcmp(command, "end")) {
			printf("END\n");
			delete buf;
		}
		//read command
		else if (!strcmp(command, "read")) {
			if (fscanf(fp, "%s %d", fileName, &num1) == 2) {
				printf("READ: %s %d\n", fileName, num1);
				
				FD = buf->getFileDescriptor(fileName);
				dAdd.FD = FD;
				dAdd.pageId = num1;
 				buf->loadPersistentPage(dAdd);
			}
		}
		//write command
		else if (!strcmp(command, "write")) {
			if (fscanf(fp, "%s %d", fileName, &num1) == 2) {
				printf("WRITE: %s %d\n", fileName, num1);
				
				FD = buf->getFileDescriptor(fileName);
				dAdd.FD = FD;
				dAdd.pageId = num1;
				
 				buf->writePage(dAdd);
			}
		}
		//flush command
		else if (!strcmp(command, "flush")) {
			if (fscanf(fp, "%s %d", fileName, &num1) == 2) {
				printf("FLUSH: %s %d\n", fileName, num1);
				
				FD = buf->getFileDescriptor(fileName);
				dAdd.FD = FD;
				dAdd.pageId = num1;
				
 				buf->flushPage(dAdd);
			}
		}
		//pin command
		else if (!strcmp(command, "pin")) {
			if (fscanf(fp, "%s %d", fileName, &num1) == 2) {
				printf("PIN: %s %d\n", fileName, num1);
				
				FD = buf->getFileDescriptor(fileName);
				dAdd.FD = FD;
				dAdd.pageId = num1;
				
 				buf->pinPage(dAdd);
			}
		}
		//unpin command
		else if (!strcmp(command, "unpin")) {
			if (fscanf(fp, "%s %d", fileName, &num1) == 2) {
				printf("UNPIN: %s %d\n", fileName, num1);
				
				FD = buf->getFileDescriptor(fileName);
				dAdd.FD = FD;
				dAdd.pageId = num1;
				
 				buf->unPinPage(dAdd);
			}
		}
		//new command
		else if (!strcmp(command, "new")) {
			if (fscanf(fp, "%s %d %d", fileName, &num1, &num2) == 3) {
				printf("NEW: %s %d %d\n", fileName, num1, num2);
				
				//get file
				FD = buf->getFileDescriptor(fileName);
                printf("FD: %d\n", FD);
				dAdd.FD = FD;
				
				//for each disk page create it in the buffer
				for (i = num1; i <= num2; i++) {
					dAdd.pageId = i;
					
					buf->newPage(dAdd);
				}
			}
		}
        //new cache page
		else if (!strcmp(command, "newcache")) {
			if (fscanf(fp, "%s %d", fileName, &num1) == 2) {
				printf("NEW CACHE: %s %d\n", fileName, num1);
				
				//get file
				FD = buf->getFileDescriptor(fileName);
				dAdd.FD = FD;
                dAdd.pageId = num1;
				
                buf->allocateCachePage(dAdd);
			}
		}
        //remove cache page
		else if (!strcmp(command, "removecache")) {
			if (fscanf(fp, "%s %d", fileName, &num1) == 2) {
				printf("REMOVE CACHE: %s %d\n", fileName, num1);
				
				//get file
				FD = buf->getFileDescriptor(fileName);
				dAdd.FD = FD;
                dAdd.pageId = num1;
				
                buf->removeCachePage(dAdd);
			}
		}
		//check command
		else if (!strcmp(command, "check")) {
			printf("CHECK\n");
            buf->checkpoint();
		}
		else {
			printf("No command by the name: %s\n", command);
		}
	}
}

//main function for grabbing command line arguments and passing to runBuffer function
int main(int argv, char *argc[]) {
	FILE *fp;
	
	//check for arguments
	if (argv < 2) {
		printf("Error: not enough arguments\n");
		printf("Please provide a filename containing buffer commands\n");
		
		return -1;
	}
	
	//try to open file
	fp = fopen(argc[1], "r");
	
	//if file cannot be opened return error
	if (!fp) {
		printf("Error: file cannot be opened\n");
		
		return -1;
	}
	
	runBuffer(fp);
	
    fclose(fp);
    
	return 0;
}
