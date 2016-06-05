#include "libs/FLOPPYParser/FLOPPYParser.h"
#include <stdlib.h>

typedef struct AttrDescription {
	int attrType;
	int attrSize;
	int offset;
} AttrDescription;

AttrDescription getAttributeDescription(char *data, char *colName);
int packRecordDescription(FLOPPYCreateTableStatement *statement, char *data);
void printRecordDescription(char *data, int size);
int getRecordLength(char *data);