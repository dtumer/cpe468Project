#include <stdio.h>

#include "BufferManager.h"
#include "smartalloc.h"

//function for running the buffer
void runBuffer(FILE *fp) {
	Buffer *buf = calloc(1, sizeof(Buffer));
	char command[10], diskName[1024], fileName[1024];
	int num1, num2;
	fileDescriptor FD;
	DiskAddress dAdd;
	
	while (fscanf(fp, "%s", command) == 1) {
		//start command
		if (!strcmp(command, "start")) {
			if (fscanf(fp, "%s", diskName) == 1 && fscanf(fp, "%d", &num1) == 1) {
				printf("START: %s, %d\n", diskName, num1);
				commence(diskName, buf, num1);
			}
		}
		//end command
		else if (!strcmp(command, "end")) {
			printf("END\n");
			squash(buf);
		}
		//read command
		else if (!strcmp(command, "read")) {
			if (fscanf(fp, "%s %d", fileName, &num1) == 2) {
				FD = tfs_openFile(fileName);
				
				dAdd.FD = FD;
				dAdd.pageId = num1;
				//readPage(buf, dAdd);
				
				printf("HERE\n");
				
				tfs_closeFile(FD);
				printf("READ: %s %d\n", fileName, num1);
			}
		}
		//write command
		else if (!strcmp(command, "write")) {
			if (fscanf(fp, "%s %d", fileName, &num1) == 2) {
				printf("WRITE: %s %d\n", fileName, num1);
			}
		}
		//flush command
		else if (!strcmp(command, "flush")) {
			if (fscanf(fp, "%s %d", fileName, &num1) == 2) {
				printf("FLUSH: %s %d\n", fileName, num1);
			}
		}
		//pin command
		else if (!strcmp(command, "pin")) {
			if (fscanf(fp, "%s %d", fileName, &num1) == 2) {
				printf("PIN: %s %d\n", fileName, num1);
			}
		}
		//unpin command
		else if (!strcmp(command, "unpin")) {
			if (fscanf(fp, "%s %d", fileName, &num1) == 2) {
				printf("UNPIN: %s %d\n", fileName, num1);
			}
		}
		//new command
		else if (!strcmp(command, "new")) {
			if (fscanf(fp, "%s %d %d", fileName, &num1, &num2) == 3) {
				printf("NEW: %s %d %d\n", fileName, num1, num2);

			}
		}
		//check command
		else if (!strcmp(command, "check")) {
			printf("CHECK\n");
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
