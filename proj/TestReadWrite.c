//
//  TestReadWrite.c
//
//

#include <stdio.h>
#include "ReadWrite.h"
//#include "libs/smartalloc.h"

#define NUM_FILES 3

int FDs[NUM_FILES];

void setFileNames(char **names, int length);
void setDiskAddrs(DiskAddress **addrs, int length);
void setTestData(char **testDatas, int length);

int main(int argv, char *argc[]) {
    Buffer *buf = (Buffer *)calloc(1, sizeof(Buffer)); //buffer object for page storage
    DiskAddress *diskAddrs[10];
    char *fileNames[NUM_FILES];
    char *data[10];

    commence("Foo.disk", buf, 5, 5);
    setDiskAddrs(diskAddrs, 10);
    setTestData(data, 10);
    setFileNames(fileNames, NUM_FILES);
    int i;
    for(i = 0; i < NUM_FILES; i++) {
        printf("fn: %s\n", fileNames[i]);
    }

    //putPage(buf, data,

    //cleanup
    squash(buf);

    return 0;
}

void setFileNames(char **names, int length) {
    char fName[4] = "tfX";
    int i;
    for(i = 0; i <= length; i++) {
        names[i] = calloc(1, sizeof(fName));
        fName[2] = i+1 + '0';
        strncpy(names[i], fName, sizeof(fName));
        FDs[i] = tfs_openFile(fName);
    }
}
void setDiskAddrs(DiskAddress **addrs, int length) {
    int i;
    for(i = 0; i <= length; i++) {
        addrs[i] = calloc(1, sizeof(DiskAddress));
        addrs[i]->FD = rand() % NUM_FILES;
        addrs[i]->pageId = i+1;
    }
}
void setTestData(char **testDatas, int length) {
    char dataName[10] = "testDataX";
    int i;
    for(i = 0; i <= length; i++) {
        testDatas[i] = calloc(1, sizeof(dataName));
        dataName[8] = i+1;
        strncpy(testDatas[i], dataName, sizeof(dataName));
    }
}
