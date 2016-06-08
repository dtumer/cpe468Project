//
//  FLOPPYHeapFile.cpp
//

#include "FLOPPYHeapFile.h"

FLOPPYHeapFile::FLOPPYHeapFile(FLOPPYBufferManager *buf, fileDescriptor fd) : FLOPPYFileManager(buf, fd) {
    _tblDes = NULL;
}

FLOPPYHeapFile::FLOPPYHeapFile(FLOPPYBufferManager *buf, std::string tableName) : FLOPPYFileManager(buf) {
    _tblDes = NULL;
    
    char *fileName = (char*) calloc(FILE_NAME_SIZE, sizeof(char));
    getFileName(tableName, fileName);
    fd = buf->getFileDescriptor(fileName);
    free(fileName);
}

FLOPPYHeapFile::~FLOPPYHeapFile() {
    if(_tblDes)
        delete _tblDes;
}

/* Helper functions */

void FLOPPYHeapFile::getFileName(std::string tableName, char *fileName) {
    strcpy(fileName, "T_");
    strncpy(&fileName[2], tableName.c_str(), FILE_NAME_SIZE-3);
    fileName[FILE_NAME_SIZE-1] = '\0';
    return;
}

HeapFileHeader * FLOPPYHeapFile::getHeapFileHeader() {
    DiskAddress page = getDiskAddress(0);
    
    return (HeapFileHeader *) buf->read(page, sizeof(FileHeader), sizeof(HeapFileHeader));
}

int FLOPPYHeapFile::writeHeapFileHeader(HeapFileHeader *heapFileHeader) {
    DiskAddress page = getDiskAddress(0);
    
    return buf->write(page, sizeof(FileHeader), sizeof(HeapFileHeader), (char *) heapFileHeader);
}

HeapPageHeader * FLOPPYHeapFile::getPageHeader(int pageId) {
    unsigned char *data;
    DiskAddress page = getDiskAddress(pageId);
    data = buf->read(page, 0, sizeof(HeapPageHeader));
    
    return (HeapPageHeader *) data;
}

int FLOPPYHeapFile::writePageHeader(int pageId, HeapPageHeader *heapPageHeader) {
    DiskAddress page = getDiskAddress(pageId);
    return buf->write(page, 0, sizeof(HeapPageHeader), (char *) heapPageHeader);
}

void FLOPPYHeapFile::createNewPage(HeapFileHeader *heapFileHeader) {
    FileHeader *fileHeader = getFileHeader();
    HeapPageHeader *heapPageHeader = (HeapPageHeader*) malloc(sizeof(HeapPageHeader));
    int pageId = ++heapFileHeader->numPages;
    DiskAddress page = getDiskAddress(pageId);
    
    //create file
    if(heapFileHeader->isVolatile) {
        buf->allocateCachePage(page);
    } else {
        buf->newPage(page);
    }
    
    //add basic data
    heapPageHeader->pageId = pageId;
    heapPageHeader->numRecords = 0;
    strncpy(heapPageHeader->fileName, fileHeader->fileName, FILE_NAME_SIZE);
    
    //update free slot data
    heapPageHeader->nextFreeSlotPage = heapFileHeader->firstFreeSlotPage;
    heapFileHeader->firstFreeSlotPage = pageId;
    
    //update next page
    heapPageHeader->nextPage = heapFileHeader->firstPage;
    heapFileHeader->firstPage = pageId;
    
    char *bitmap = (char*) calloc(heapFileHeader->bitmapSize, sizeof(uint8_t));
    
    //write new page
    
    buf->write(page, 0, sizeof(HeapPageHeader), (char *) heapPageHeader);
    buf->write(page, sizeof(HeapPageHeader), heapFileHeader->bitmapSize, bitmap);
    
    //free all data
    free(fileHeader);
    free(heapPageHeader);
    free(bitmap);
}

DiskAddress FLOPPYHeapFile::getDiskAddress(int pageId) {
    DiskAddress page;
    page.FD = fd;
    page.pageId = pageId;
    return page;
}

/* file-level functions */

FLOPPYHeapFile * FLOPPYHeapFile::createFile(FLOPPYBufferManager *buf, FLOPPYCreateTableStatement *statement) {
    FLOPPYHeapFile *heapFile;
    FileHeader *fileHeader = (FileHeader*) malloc(sizeof(FileHeader));
    HeapFileHeader *heapFileHeader = (HeapFileHeader*) calloc(1, sizeof(HeapFileHeader));
    
    //FileHeader
    fileHeader->fileType = HEAP_FILE_TYPE;
    
    FLOPPYHeapFile::getFileName(statement->tableName, fileHeader->fileName);
    
    //open file
    heapFile = new FLOPPYHeapFile(buf, buf->getFileDescriptor(fileHeader->fileName));
    DiskAddress headerPage = heapFile->getDiskAddress(0);
    
    //create files
    if(statement->flags->volatileFlag) {
        buf->allocateCachePage(headerPage);
    } else {
        buf->newPage(headerPage);
    }
    
    //HeapFileHeader
    strncpy(heapFileHeader->tableName, statement->tableName, TABLE_NAME_SIZE);
    heapFileHeader->firstPage = 0;
    heapFileHeader->firstFreeSlotPage = 0;
    heapFileHeader->numPages = 0;
    heapFileHeader->isVolatile = statement->flags->volatileFlag;
    
    //record description
    heapFileHeader->recordSize = FLOPPYTableDescription::packRecordDescription(statement, heapFileHeader->recordDescription);
    
    //bitmap
    heapFileHeader->bitmapSize = FLOPPYBitmap::calcBitmapSize(heapFileHeader->recordSize, BLOCKSIZE, sizeof(HeapPageHeader), 0);
    
    heapFileHeader->numRecordsPerPage = (BLOCKSIZE - sizeof(HeapPageHeader) - heapFileHeader->bitmapSize) / heapFileHeader->recordSize;
    
    //write fileHeader then free
    heapFile->writeFileHeader(fileHeader);
    free(fileHeader);
    
    //create page 1
    heapFile->createNewPage(heapFileHeader);
    
    //write heapFileHeader then free
    heapFile->writeHeapFileHeader(heapFileHeader);
    free(heapFileHeader);
    
    return heapFile;
}

int FLOPPYHeapFile::deleteFile(std::string tableName) {
    return 0;
}

/* Data Page functions */

//given a page id and a location on the page (represented by id) retrieve the contents of the record into output array
unsigned char * FLOPPYHeapFile::getRecord(int pageId, uint16_t dataOffset, uint16_t recordSize, int recordId) {
    DiskAddress page = getDiskAddress(pageId);
    int startOffset = dataOffset + (recordSize * recordId);
    return buf->read(page, startOffset, recordSize);
}

//given a page id and a location on the page, place the given record onto the appropriate slot of the page
int FLOPPYHeapFile::putRecord(int pageId, uint16_t dataOffset, uint16_t recordSize, int recordId, char *data) {
    DiskAddress page = getDiskAddress(pageId);
    int startOffset = dataOffset + (recordSize * recordId);
    return buf->write(page, startOffset, recordSize, data);
}


/* Heap File CRUD operations */

int FLOPPYHeapFile::insertRecord(char * record) {
    HeapFileHeader *heapFileHeader = getHeapFileHeader();
    HeapPageHeader *heapPageHeader;
    uint8_t *bitmapData;
    FLOPPYBitmap *bitmap;
    int pageId;
    
    //find open spot
    pageId = heapFileHeader->firstFreeSlotPage;
    DiskAddress page = getDiskAddress(pageId);
    
    heapPageHeader = getPageHeader(heapFileHeader->firstFreeSlotPage);
    bitmapData = (uint8_t *) buf->read(page, sizeof(HeapPageHeader), heapFileHeader->bitmapSize);
    bitmap = new FLOPPYBitmap(bitmapData, heapFileHeader->numRecordsPerPage);
    
    int recordId = bitmap->getFirstFreeRecord();
    //printf("recordId: %d\n",recordId);
    
    //write data
    buf->write(page, sizeof(HeapPageHeader) + heapFileHeader->bitmapSize + (heapFileHeader->recordSize * recordId), heapFileHeader->recordSize, record);
    
    
    //update record count
    heapPageHeader->numRecords++;
    
    //update bitmap
    bitmap->setRecordFull(recordId);
    buf->write(page, sizeof(HeapPageHeader), heapFileHeader->bitmapSize, (char *) bitmapData);
    
    if(bitmap->getFirstFreeRecord() == -1) {
        //page is full
        heapFileHeader->firstFreeSlotPage = heapPageHeader->nextFreeSlotPage;
        heapPageHeader->nextFreeSlotPage = 0;
        
        if(heapFileHeader->firstFreeSlotPage == 0) {
            //no empty pages, need to create a new one
            createNewPage(heapFileHeader);
        }
        
        //write heapFileHeader
        writeHeapFileHeader(heapFileHeader);
    }
    
    //write and free heapPageHeader
    writePageHeader(pageId, heapPageHeader);
    delete bitmap;
    free(bitmapData);
    free(heapPageHeader);
    free(heapFileHeader);
    return 0;
}

int FLOPPYHeapFile::deleteRecord(int pageId, int recordId) {
    HeapFileHeader *heapFileHeader = getHeapFileHeader();
    HeapPageHeader *heapPageHeader = getPageHeader(pageId);
    DiskAddress page = getDiskAddress(pageId);
    uint8_t *bitmapData;
    FLOPPYBitmap *bitmap;
    
    //get bitmap
    bitmapData = (uint8_t *) buf->read(page, sizeof(HeapPageHeader), heapFileHeader->bitmapSize);
    bitmap = new FLOPPYBitmap(bitmapData, heapFileHeader->numRecordsPerPage);
    int prevFreeRecordId = bitmap->getFirstFreeRecord();
    
    //update record count
    heapPageHeader->numRecords--;
    
    //update bitmap
    bitmap->setRecordEmpty(recordId);
    buf->write(page, sizeof(HeapPageHeader), heapFileHeader->bitmapSize, (char *) bitmapData);
    
    if(prevFreeRecordId == -1) {
        //page was full
        heapPageHeader->nextFreeSlotPage = heapFileHeader->firstFreeSlotPage;
        heapFileHeader->firstFreeSlotPage = page.pageId;
        
        //write heapFileHeader
        writeHeapFileHeader(heapFileHeader);
    }
    
    //write and free heapPageHeader
    writePageHeader(pageId, heapPageHeader);
    delete bitmap;
    free(bitmapData);
    free(heapPageHeader);
    free(heapFileHeader);
    return 0;
}

int FLOPPYHeapFile::updateRecord(int pageId, int recordId, char * record) {
    HeapFileHeader *heapFileHeader = getHeapFileHeader();
    DiskAddress page = getDiskAddress(pageId);
    
    //write data
    buf->write(page, sizeof(HeapPageHeader) + heapFileHeader->bitmapSize + (heapFileHeader->recordSize * recordId), heapFileHeader->recordSize, record);
    
    //free heapFileHeader
    free(heapFileHeader);
    return 0;
}

FLOPPYRecordSet * FLOPPYHeapFile::getAllRecords() {
    FLOPPYRecordSet *rs = new FLOPPYRecordSet();
    HeapFileHeader *heapFileHeader = getHeapFileHeader();
    FLOPPYTableDescription *tblDesc = getTableDescription();
    int pageId = heapFileHeader->firstPage;
    
    DiskAddress page;
    HeapPageHeader *heapPageHeader;
    uint8_t *bitmapData;
    FLOPPYBitmap *bitmap;
    int recordId;
    
    while (pageId) {
        heapPageHeader = getPageHeader(pageId);
        page = getDiskAddress(pageId);
        
        bitmapData = (uint8_t *) buf->read(page, sizeof(HeapPageHeader), heapFileHeader->bitmapSize);
        bitmap = new FLOPPYBitmap(bitmapData, heapFileHeader->numRecordsPerPage);
        
        recordId = bitmap->nextRecord();
        while (recordId >= 0) {
            FLOPPYRecord *record = new FLOPPYRecord();
            unsigned char *data = buf->read(page, sizeof(HeapPageHeader) + heapFileHeader->bitmapSize + (heapFileHeader->recordSize * recordId), heapFileHeader->recordSize);
            unsigned char *ptr;
            
            record->pageId = pageId;
            record->recordId = recordId;
            
            for (unsigned i=0; i<tblDesc->columns->size(); i++) {
                FLOPPYTableColumn *tblCol = tblDesc->columns->at(i);
                FLOPPYRecordAttribute *recCol = new FLOPPYRecordAttribute();
                
                //table name
                recCol->tableName = (char*)calloc(sizeof(char*), strlen(heapFileHeader->tableName)+1);
                strcpy(recCol->tableName, heapFileHeader->tableName);
                
                //attribute name
                recCol->name = (char*)calloc(sizeof(char*), strlen(tblCol->name)+1);
                strcpy(recCol->name, tblCol->name);
                
                
                //value
                ptr = data + tblCol->offset;
                
                if(tblCol->type == ColumnType::INT) {
                    recCol->val = new FLOPPYValue(IntValue);
                    recCol->val->iVal = (uint64_t)*ptr;
                }
                else if(tblCol->type == ColumnType::FLOAT) {
                    recCol->val = new FLOPPYValue(FloatValue);
                    recCol->val->fVal = (float)*ptr;
                }
                else if(tblCol->type == ColumnType::DATETIME) {
                    recCol->val = new FLOPPYValue(FloatValue);
                    recCol->val->fVal = (float)*ptr;
                }
                else if(tblCol->type == ColumnType::VARCHAR) {
                    recCol->val = new FLOPPYValue(StringValue);
                    recCol->val->sVal = (char*)calloc(sizeof(char*), tblCol->size);
                    memcpy(recCol->val->sVal, ptr, tblCol->size);
                }
                else if(tblCol->type == ColumnType::BOOLEAN) {
                    recCol->val = new FLOPPYValue(BooleanValue);
                    recCol->val->bVal = (bool)*ptr;
                }
                record->columns->push_back(recCol);
                
            }
            
            rs->records->push_back(record);
            
            //printf("\tRECORD: %d %d\n", pageId, recordId);
            recordId = bitmap->nextRecord();
        }
        
        //get next page
        pageId = heapPageHeader->nextPage;
        
        //free this page data
        free(heapPageHeader);
        free(bitmapData);
        delete bitmap;
    }
    
    //free file data
    free(heapFileHeader);
    
    return rs;
}

FLOPPYTableDescription * FLOPPYHeapFile::getTableDescription() {
    if(!_tblDes) {
        HeapFileHeader *heapFileHeader = getHeapFileHeader();
        _tblDes = new FLOPPYTableDescription(heapFileHeader->recordDescription);
        
        free(heapFileHeader);
    }
    
    return _tblDes;
}

/* Test Functions */

void FLOPPYHeapFile::printFileInfo() {
    HeapFileHeader *heapFileHeader = getHeapFileHeader();
    FileHeader *fileHeader = getFileHeader();
    
    printf("Heap File Info:\n");
    printf("\t fileName: %.*s\n", FILE_NAME_SIZE, fileHeader->fileName);
    printf("\t tableName: %.*s\n", TABLE_NAME_SIZE, heapFileHeader->tableName);
    printf("\t recordSize: %d\n", heapFileHeader->recordSize);
    printf("\t numRecordsPerPage: %d\n", heapFileHeader->numRecordsPerPage);
    printf("\t numPages: %d\n", heapFileHeader->numPages);
    printf("\t firstPage: %d\n", heapFileHeader->firstPage);
    printf("\t firstFreeSlotPage: %d\n", heapFileHeader->firstFreeSlotPage);
    printf("\t bitmapSize: %d\n", heapFileHeader->bitmapSize);
    printf("\t isVolatile: %d\n", heapFileHeader->isVolatile);
    
    free(heapFileHeader);
    free(fileHeader);
}

void FLOPPYHeapFile::printPageInfo(int pageId) {
    DiskAddress page = getDiskAddress(pageId);
    
    HeapPageHeader *heapPageHeader = getPageHeader(pageId);
    HeapFileHeader *heapFileHeader = getHeapFileHeader();
    
    printf("Heap Page %d Info:\n", heapPageHeader->pageId);
    printf("\t fileName: %.*s;\n", FILE_NAME_SIZE, heapPageHeader->fileName);
    printf("\t pageId: %d\n", heapPageHeader->pageId);
    printf("\t nextFreeSlotPage: %d\n", heapPageHeader->nextFreeSlotPage);
    printf("\t nextPage: %d\n", heapPageHeader->nextPage);
    printf("\t numRecords: %d\n", heapPageHeader->numRecords);
    
    uint8_t *bitmapData = (uint8_t *) buf->read(page, sizeof(HeapPageHeader), heapFileHeader->bitmapSize);
    FLOPPYBitmap *bitmap = new FLOPPYBitmap(bitmapData, heapFileHeader->numRecordsPerPage);
    
    //bitmap
    printf("\t bitmap: ");
    bitmap->print();
    printf("\n");
    
    free(heapPageHeader);
    free(heapFileHeader);
    delete bitmap;
    free(bitmapData);
}

void FLOPPYHeapFile::printTableDescription() {
	getTableDescription()->print();
}