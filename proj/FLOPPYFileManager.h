//
//  FLOPPYFileManager.h
//

#ifndef FLOPPYFileManager_h
#define FLOPPYFileManager_h

#include <stdio.h>
#include "FLOPPYBufferManager.h"

#define HEAP_FILE_TYPE 1
#define SEQ_FILE_TYPE 2
#define HASH_FILE_TYPE 3

#define FILE_NAME_SIZE 8
#define TABLE_NAME_SIZE 20

typedef struct FileHeader {
    uint32_t fileType; //1: Heap, 2: Sequential, 3: Hash
    char fileName[FILE_NAME_SIZE];
}__attribute__((packed)) FileHeader;


class FLOPPYFileManager {
protected:
    FLOPPYBufferManager *buf;
public:
    FLOPPYFileManager(FLOPPYBufferManager *buf); // constructor
    FLOPPYFileManager(FLOPPYBufferManager *buf, fileDescriptor fd); // constructor
    fileDescriptor fd;
    
    FileHeader * getFileHeader();
    void writeFileHeader(FileHeader *header);
    
    void printFileHeader();
private:
    
};


#endif /* FLOPPYFileManager_h */
