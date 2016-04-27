#include <stdio.h>

#include "BufferManager.h"
#include "smartalloc.h"

#define NUM_BLOCKS 100

void printBuffer(Buffer *buf) {
    printf("BUFFER:\n");
    printf("database: %s\n", buf->database); 
}

int main(int argc, char *argv[]) {
    Buffer *buf = calloc(1, sizeof(Buffer)); 
    
    commence("tinyTestTest", buf, NUM_BLOCKS);
    printBuffer(buf);
    squash(buf);

    return 0;
}
