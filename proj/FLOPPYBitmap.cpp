//
//  FLOPPYBitmap.cpp
//

#include "FLOPPYBitmap.h"

//constructor
FLOPPYBitmap::FLOPPYBitmap(uint8_t *bitmap, int numRecords) : bitmap(bitmap), numRecords(numRecords) {
    
}

int FLOPPYBitmap::getFirstFreeRecord() {
    FLOPPYBitmap::getFirstFreeRecordHelper(bitmap, numRecords);
}

int FLOPPYBitmap::getFirstFreeRecordHelper(uint8_t *bitmap, int numRecords) {
    int result, counter = 0;
    uint16_t mask = 128;
    uint8_t *temp;
    
    if (numRecords <= 0)
        return -1;
    
    if (*bitmap == 0xff) {
        result = FLOPPYBitmap::getFirstFreeRecordHelper(bitmap + 1, numRecords - 8);
        
        if (result == -1)
            return -1;
        else
            return result + 8;
    }
    
    temp = bitmap;
    while (mask & *temp) {
        mask >>= 1;
        counter++;
    }
    
    if(counter >= numRecords)
        return -1;
    
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
