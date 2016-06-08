//
//  FLOPPYBitmap.cpp
//

#include "FLOPPYBitmap.h"

//constructor
FLOPPYBitmap::FLOPPYBitmap(uint8_t *bitmap, int numRecords) : bitmap(bitmap), numRecords(numRecords) {
    curBitmapByte = NULL;
    curBit = 0;
    curRecordOffset = 0;
}

int FLOPPYBitmap::nextRecord() {
	uint8_t curRecord = 0;
		
	if (curBitmapByte == NULL) { //make sure curBitmap is set to original byte
		curBitmapByte = bitmap;
	}
	
	//check to see if there are any more occupied records in the bitmap
	if (numRecords - curRecordOffset > 0) {
		curRecord = 1 << (7 - curBit) & *curBitmapByte;
		
		//if we've looked through the whole byte
		if (curBit == 7) {
			curBit = 0;
			curBitmapByte++;
		}
		else {
			curBit++;
		}
		
		//if we're looking at a bit that is empty call nextRecord() again
		if (curRecord == 0) {	
			curRecordOffset++;
			return nextRecord();
		}
		else {
			return curRecordOffset++;
		}
	}
	else { //reset current bitmap byte to the beginning of the bitmap
		curBitmapByte = bitmap;
		curRecordOffset = 0;
		return -1;
	}
}

int FLOPPYBitmap::getFirstFreeRecord() {
    return FLOPPYBitmap::getFirstFreeRecordHelper(bitmap, numRecords);
}

int FLOPPYBitmap::getFirstFreeRecordHelper(uint8_t *bitmap, int numRecords) {
    int result, counter = 0;
    uint16_t mask = 128;
    uint8_t *temp;
    
    printf(" ");
    printf(" ");
    
    if (numRecords <= 0)
        return -1;
    
    if (*bitmap == 0xff) {
        result = FLOPPYBitmap::getFirstFreeRecordHelper(bitmap + 1, numRecords - 8);
        
        if (result == -1)
            return -1;
        else
            return result + 8;
    }
    
    printf(" ");
    printf(" ");
    
    temp = bitmap;
    while (mask & *temp) {
        mask >>= 1;
        counter++;
    }
    
    if(counter >= numRecords)
        return -1;
    
    printf(" ");
    return counter;
}

void FLOPPYBitmap::setRecordEmpty(int recordNum) {
    int byteNum = recordNum / 8;
    int bitNum = recordNum % 8;
    uint8_t mask = 1 << (7 - bitNum);
    uint8_t *temp = bitmap;
    
    temp += byteNum; //navigates to correct byte number
    mask = ~mask; //creates the mask on the appropriate bit within the byte
    
    *temp = *temp & mask;
}

void FLOPPYBitmap::setRecordFull(int recordNum) {
    int byteNum = recordNum / 8;
    int bitNum = recordNum % 8;
    uint8_t mask = 1 << (7 - bitNum);
    uint8_t *temp = bitmap;
    
    temp += byteNum; //navigates to correct byte number
    
    *temp = *temp | mask;
}

int FLOPPYBitmap::calcBitmapSize(int recordSize, int pageSize, int headerSize, int curBitmapSize) {
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
    
    if (curBitmapSize == newBitmapSize) {
        return curBitmapSize;
    }
    else {
        return FLOPPYBitmap::calcBitmapSize(recordSize, pageSize, headerSize, newBitmapSize);
    }
}

/* Test Functions */
void FLOPPYBitmap::print() {
    FLOPPYBitmap::printHelper(bitmap, numRecords);
}

void FLOPPYBitmap::printHelper(uint8_t *bitmap, int numRecords) {
    if(numRecords >=1)
        printf("%d",(*bitmap & 0x80 ? 1 : 0));
    if(numRecords >=2)
        printf("%d",(*bitmap & 0x40 ? 1 : 0));
    if(numRecords >=3)
        printf("%d",(*bitmap & 0x20 ? 1 : 0));
    if(numRecords >=4)
        printf("%d",(*bitmap & 0x10 ? 1 : 0));
    if(numRecords >=5)
        printf("%d",(*bitmap & 0x08 ? 1 : 0));
    if(numRecords >=6)
        printf("%d",(*bitmap & 0x04 ? 1 : 0));
    if(numRecords >=7)
        printf("%d",(*bitmap & 0x02 ? 1 : 0));
    if(numRecords >=8)
        printf("%d",(*bitmap & 0x01 ? 1 : 0));
    
    if (numRecords > 8) {
        printf(" ");
        FLOPPYBitmap::printHelper(bitmap + 1, numRecords - 8);
    }
}
