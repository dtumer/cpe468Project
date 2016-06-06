//
//  FLOPPYBitmap.h
//

#ifndef FLOPPYBitmap_h
#define FLOPPYBitmap_h

#include <stdio.h>
#include <cstdint>

class FLOPPYBitmap {
public:
    FLOPPYBitmap(uint8_t *bitmap, int numRecords); //constructor
    
    int getFirstFreeRecord();
    
    void setRecordEmpty(int recordNum);
    void setRecordFull(int recordNum);
    
    /* Test Functions */
    void print();
    
    static int calcBitmapSize(int recordSize, int pageSize, int headerSize, int curBitmapSize);
    
    
private:
    uint8_t *bitmap;
    int numRecords;
    
    static int getFirstFreeRecordHelper(uint8_t *bitmap, int numRecords);
    static void printHelper(uint8_t *bitmap, int numRecords);
};

#endif /* FLOPPYBitmap_h */
