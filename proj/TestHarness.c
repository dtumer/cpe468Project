#include <stdio.h>

#include "BufferManager.h"
#include "libs/smartalloc.h"

typedef struct FileNode {
	fileDescriptor FD;
	char *fileName;
	struct FileNode *next;
} FileNode;

//prints all open files
void printOpenFilesNow(FileNode *first) {
	FileNode *temp = first;
	
	while (temp != NULL) {
		printf("File: %s, %d\n", temp->fileName, temp->FD);
		temp = temp->next;
	}
}


//Closes open files
void closeFiles(FileNode *node) {
    if(node != NULL) {
        closeFiles(node->next);
        
        tfs_closeFile(node->FD);
        free(node->fileName);
        free(node);
    }
}

//adds file node to end of file list
void addFileNode(FileNode **list, FileNode *add) {
	FileNode *first = *list;
	FileNode *temp = first;
	
	//if the first node is null set it
	if (first == NULL) {
		*list = add;
	}
	else {
		while (temp != NULL) {
			if (temp->next == NULL) {
				temp->next = add;
				break;
			}
		
			temp = temp->next;
		}
	}
} 

//gets FD of open file
fileDescriptor getFileDescriptor(FileNode **node, char *fileName) {
    FileNode *newNode, *tempNode = *node;
    
    while (tempNode != NULL) {
        if (strcmp(fileName, tempNode->fileName) == 0) {
            return tempNode->FD;
        }
        tempNode = tempNode->next;
    }
    
    newNode = calloc(1, sizeof(FileNode));
    newNode->fileName = calloc(strlen(fileName) + 1, sizeof(char));
    newNode->FD = tfs_openFile(fileName);
    strcpy(newNode->fileName, fileName);
    newNode->next = *node;
    
    *node = newNode;
    
	return newNode->FD;
}

//function for running the buffer
void runBuffer(FILE *fp) {
	Buffer *buf = calloc(1, sizeof(Buffer));
	FileNode *first = NULL;
	char command[10], diskName[1024], fileName[1024];
	int num1, num2, i;
	fileDescriptor FD;
	DiskAddress dAdd;
	
	while (fscanf(fp, "%s", command) == 1) {
		//start command
		if (!strcmp(command, "start")) {
			if (fscanf(fp, "%s", diskName) == 1 && fscanf(fp, "%d", &num1) == 1) {
				printf("START: %s, %d\n", diskName, num1);
				commence(diskName, buf, num1, num1);
			}
		}
		//end command
		else if (!strcmp(command, "end")) {
			printf("END\n");
			squash(buf);
			
			printOpenFilesNow(first);
			closeFiles(first);
		}
		//read command
		else if (!strcmp(command, "read")) {
			if (fscanf(fp, "%s %d", fileName, &num1) == 2) {
				printf("READ: %s %d\n", fileName, num1);
				
				FD = getFileDescriptor(&first, fileName);
				dAdd.FD = FD;
				dAdd.pageId = num1;
 				loadPersistentPage(buf, dAdd);
			}
		}
		//write command
		else if (!strcmp(command, "write")) {
			if (fscanf(fp, "%s %d", fileName, &num1) == 2) {
				printf("WRITE: %s %d\n", fileName, num1);
				
				FD = getFileDescriptor(&first, fileName);
				dAdd.FD = FD;
				dAdd.pageId = num1;
				
 				writePage(buf, dAdd);
			}
		}
		//flush command
		else if (!strcmp(command, "flush")) {
			if (fscanf(fp, "%s %d", fileName, &num1) == 2) {
				printf("FLUSH: %s %d\n", fileName, num1);
				
				FD = getFileDescriptor(&first, fileName);
				dAdd.FD = FD;
				dAdd.pageId = num1;
				
 				flushPage(buf, dAdd);
			}
		}
		//pin command
		else if (!strcmp(command, "pin")) {
			if (fscanf(fp, "%s %d", fileName, &num1) == 2) {
				printf("PIN: %s %d\n", fileName, num1);
				
				FD = getFileDescriptor(&first, fileName);
				dAdd.FD = FD;
				dAdd.pageId = num1;
				
 				pinPage(buf, dAdd);
			}
		}
		//unpin command
		else if (!strcmp(command, "unpin")) {
			if (fscanf(fp, "%s %d", fileName, &num1) == 2) {
				printf("UNPIN: %s %d\n", fileName, num1);
				
				FD = getFileDescriptor(&first, fileName);
				dAdd.FD = FD;
				dAdd.pageId = num1;
				
 				unPinPage(buf, dAdd);
			}
		}
		//new command
		else if (!strcmp(command, "new")) {
			if (fscanf(fp, "%s %d %d", fileName, &num1, &num2) == 3) {
				printf("NEW: %s %d %d\n", fileName, num1, num2);
				
				//get file
				FD = getFileDescriptor(&first, fileName);
				dAdd.FD = FD;
				
				//for each disk page create it in the buffer
				for (i = num1; i <= num2; i++) {
					dAdd.pageId = i;
					
					newPage(buf, dAdd);	
				}
			}
		}
        //new cache page
		else if (!strcmp(command, "newcache")) {
			if (fscanf(fp, "%s %d", fileName, &num1) == 2) {
				printf("NEW CACHE: %s %d\n", fileName, num1);
				
				//get file
				FD = getFileDescriptor(&first, fileName);
				dAdd.FD = FD;
                dAdd.pageId = num1;
				
                allocateCachePage(buf, dAdd);
			}
		}
        //remove cache page
		else if (!strcmp(command, "removecache")) {
			if (fscanf(fp, "%s %d", fileName, &num1) == 2) {
				printf("REMOVE CACHE: %s %d\n", fileName, num1);
				
				//get file
				FD = getFileDescriptor(&first, fileName);
				dAdd.FD = FD;
                dAdd.pageId = num1;
				
                removeCachePage(buf, dAdd);
			}
		}
		//check command
		else if (!strcmp(command, "check")) {
			printf("CHECK\n");
            checkpoint(buf);
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
	
	return 0;
}
