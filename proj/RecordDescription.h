#ifndef RecordDescription_h
#define RecordDescription_h

#include "StatementParser.h"

#include <stdlib.h>

typedef struct AttrDescription {
	ATT_TYPE attrType;
	int attrSize;
} AttrDescription;

int packRecordDescription(tableDescription *desc, char *data);
AttrDescription getAttributeDescription(char *data, int dataSize, char *colName);
void printRecordDescription(char *data, int size);
int getAttrTypeByteSize(ATT_TYPE attrType);

#endif