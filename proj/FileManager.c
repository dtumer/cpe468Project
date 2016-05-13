#include "FileManager.h"

//returns the index of the first free record in the bitmap
int getFirstFreeRecord(char *bitmap, int numRecords) {
	return 0;
}

//calculates the size of the bitmap given a page
int calcBitmapSize(int recordSize, int pageSize, int headerSize, int curBitmapSize) {
	int numRecordsOnPage = (pageSize - headerSize - curBitmapSize) / recordSize;
	int newBitmapSize = numRecordsOnPage / 8;
	int newHeaderSize;
	
	if (numRecordsOnPage % 8 > 0) {
		newBitmapSize++;
	}
	
	newHeaderSize = headerSize + newBitmapSize;
	
	if (newHeaderSize % 8 > 0) {
		newBitmapSize += 8 - newHeaderSize % 8;
	}
	
	printf("New Bitmap Size: %d\n", newBitmapSize);
	
	if (curBitmapSize == newBitmapSize) {
		return curBitmapSize;
	}
	else {
		return calcBitmapSize(recordSize, pageSize, headerSize, newBitmapSize);
	}
}

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
