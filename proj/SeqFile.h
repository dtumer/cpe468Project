#ifndef SeqFile_h
#define SeqFile_h

#include "ReadWrite.h"
#include "StatementParser.h"
#include "FileManager.h"

typedef struct SeqFileHeader {
    uint16_t recordSize;
    uint16_t numRecordsPerPage;
    uint16_t numPages;
    uint16_t nextPage;
    uint16_t lastPage;
    char tableName[10];
    char recordDescription[600]; //uint8_t colType, uint8_t colSize(optional), uint8_t nameLen, char colName[nameLen + 1]
}__attribute__((packed)) SeqFileHeader;

typedef struct SeqPageHeader {
    uint16_t nextPage;
    uint8_t seqPageType; //1: node, 2: leaf
}__attribute__((packed)) SeqPageHeader;

/* Sequential File Creation functions */
int createSequentialFile(Buffer *buf, char *name, tableDescription *desc, int volatileFlag);
int deleteSequentialFile(Buffer *buf, char *name);

/* Header file functions */
int seq_headerGetRecordDesc(Buffer *buf, fileDescriptor fd, tableDescription *desc);
int seq_headerSetRecordDesc(Buffer *buf, fileDescriptor fd, tableDescription *desc);
int seq_headerGetNextPage(Buffer *buf, fileDescriptor fd, DiskAddress *page);
int seq_headerSetNextPage(Buffer *buf, fileDescriptor fd, DiskAddress *page);

/* Data Page functions */
int seq_getRecord(Buffer *buf, DiskAddress page, int recordId, char *bytes);
int seq_putRecord(Buffer *buf, DiskAddress page, int recordId, char *bytes);
int seq_deleteRecord(Buffer *buf, DiskAddress page, int recordId);

int pHGetRecSize(Buffer *buf, DiskAddress page);
int pHGetMaxRecords(Buffer *buf, DiskAddress page);
int pHGetNumRecords(Buffer *buf, DiskAddress page);
int pHGetNextPage(Buffer *buf, DiskAddress page);

/* CRUD operations */
int seqInsertRecord(Buffer *buf, char *name, char *record, DiskAddress *location);
int seqDeleteRecord(Buffer *buf, DiskAddress page, int recordId);
int seqUpdateRecord(Buffer *buf, DiskAddress page, int recordId, char *record);

#endif