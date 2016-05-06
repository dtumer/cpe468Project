#ifndef FileManager_h
#define FileManager_h

#include <stdint.h>

typedef struct FileHeader {
	uint32_t fileType; //1: Heap, 2: Sequential, 3: Hash
	char fileName[64];
}__attribute__((packed));

typedef struct HeapFile {
	uint16_t recordSize;
	uint16_t numRecordsPerPage;
	uint16_t numPages;
	uint16_t firstPage;
	uint16_t firstFreeSlotPage;
	char tableName[32];
	char recordDescription[600]; //uint8_t colType, uint8_t colSize(optional), uint8_t nameLen, char colName[nameLen + 1]
}__attribute__((packed));

typedef struct SeqFile {

}__attribute__((packed));

typedef struct HashFile {

}__attribute__((packed));

#endif