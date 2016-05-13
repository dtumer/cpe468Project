#include "FileManager.h"

//returns the index of the first free record in the bitmap
int getFirstFreeRecord(uint8_t *bitmap, int numRecords) {
	int result, counter = 0;
	uint16_t mask = 128;
	uint8_t *temp;
	
	if (numRecords <= 0) {
		return -1;
	}
	
	if (*bitmap == 0xff) {
		result = getFirstFreeRecord(bitmap + 1, numRecords - 8);
		
		if (result == -1) {
			return -1;
		}
		else {
			return result + 8;
		}
	}
	
	temp = bitmap;
	while (mask & *temp) {
		mask >>= 1;
		counter++;
	}
	
	return counter;
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


void printFileHeader(Buffer *buf, fileDescriptor fd) {
    FileHeader *header = file_getHeader(Buffer *buf, fileDescriptor fd);
    
    printf("File Header:\n");
    printf("\t FD: %d\n", fd);
    
    if(header->fileType == 1)
        printf("\t File Type: Heap\n");
    else if(header->fileType == 2)
        printf("\t File Type: Sequential\n");
    else if(header->fileType == 3)
        printf("\t File Type: Hash\n");
    else
        printf("\t File Type: Unkown\n");
    
    printf("\t File Name: %s\n", header->fileName);
}



