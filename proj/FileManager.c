#include "FileManager.h"

FileHeader * file_getHeader(Buffer *buf, fileDescriptor fd) {
    //FileHeader *header;
    DiskAddress page;
    unsigned char *data;
    
    page.FD = fd;
    page.pageId = 0;
    
    data = buf_read(buf, page, 0, sizeof(FileHeader));
    
    return (FileHeader *) data;
}

void file_writeHeader(Buffer *buf, fileDescriptor fd, FileHeader *header) {
	DiskAddress page;
    page.FD = fd;
    page.pageId = 0;
    
    buf_write(buf, page, 0, sizeof(FileHeader), (char *) header);
}
