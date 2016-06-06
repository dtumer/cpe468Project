//
//  FLOPPYFileManager.cpp
//

#include "FLOPPYFileManager.h"

/*FLOPPYFileManager::FLOPPYFileManager(FLOPPYBufferManager *buf) : buf(buf) {
    
}*/

FLOPPYFileManager::FLOPPYFileManager(FLOPPYBufferManager *buf, fileDescriptor fd) : buf(buf), fd(fd) {
    
}

FLOPPYFileManager::FLOPPYFileManager(FLOPPYBufferManager *buf) : buf(buf) {
    
}

FileHeader * FLOPPYFileManager::getFileHeader() {
    DiskAddress page;
    unsigned char *data;
    
    page.FD = fd;
    page.pageId = 0;
    
    data = buf->read(page, 0, sizeof(FileHeader));
    
    return (FileHeader *) data;
}

void FLOPPYFileManager::writeFileHeader(FileHeader *header) {
    DiskAddress page;
    page.FD = fd;
    page.pageId = 0;
    
    buf->write(page, 0, sizeof(FileHeader), (char *) header);
}

/* Test Functions */

void FLOPPYFileManager::printFileHeader() {
    FileHeader *header = getFileHeader();
    
    printf("File Header:\n");
    printf("\t FD: %d\n", fd);
    
    if(header->fileType == HEAP_FILE_TYPE)
        printf("\t File Type: Heap\n");
    else if(header->fileType == SEQ_FILE_TYPE)
        printf("\t File Type: Sequential\n");
    else if(header->fileType == HASH_FILE_TYPE)
        printf("\t File Type: Hash\n");
    else
        printf("\t File Type: Unkown\n");
    
    printf("\t File Name: %s;\n", header->fileName);
    
    free(header);
}
