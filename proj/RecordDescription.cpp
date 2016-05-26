#include "RecordDescription.h"

#include <stdint.h>
#include <string.h>

#define ATTRIBUTE_OFFSET 4
#define MAX_LENGTH 600

//prints the record description
void printRecordDescription(char *data, int size) {
	int i;
	
	for (i = 0; i < size; i++) {
		printf("%02X ", data[i] & 0xff);
	}
	
	printf("\n");
}

//calculates the size of the foreign keys that will be needed in the byte array
uint16_t calcSizeOfForeignKeys(std::vector<FLOPPYForeignKey *> *fKeys) {
	uint16_t runningSize = 0;
	uint8_t fKeyTableNameLen = 0, fKeyAttrNameLen = 0;
	
	//go through each foreign key reference
	for (unsigned i=0; i<fKeys->size(); i++) {
        FLOPPYForeignKey *fk = fKeys->at(i);
		fKeyTableNameLen = strlen(fk->refTableName);
			
		//for each attribute associated with the table name:
		//add size of foreign table name, then add attribute
        for (unsigned j=0; j<fk->attributes->size(); j++) {
        	fKeyAttrNameLen = strlen(fk->attributes->at(j));
        	
        	runningSize += fKeyTableNameLen + 1 + fKeyAttrNameLen + 1;
        }
    }
	
	return runningSize;
}

//calculates the size of the primary keys that will be needed in the byte array
uint16_t calcSizeOfPrimaryKeys(FLOPPYPrimaryKey *pKeys) {
	uint16_t runningSize = 0;
	uint8_t pKeyNameLen = 0;
	
	for (unsigned i=0; i<pKeys->attributes->size(); i++) {
		pKeyNameLen = strlen(pKeys->attributes->at(i));
		
		runningSize += pKeyNameLen + 1;
    }
	
	return runningSize;
}

//calculates the size of the attributes that will be needed in the byte array
uint16_t calcSizeOfAttributes(std::vector<FLOPPYCreateColumn *> *columns) {
	uint16_t runningSize = 0;
	uint8_t attrNameLen = 0, attrType;
	
	for (unsigned i=0; i<columns->size(); i++) {
        FLOPPYCreateColumn *col = columns->at(i);
        
        attrNameLen = strlen(col->name);
        attrType = col->type;
        
        if (attrType == VARCHAR) {
			runningSize++;
		}
		
		runningSize += 1 + attrNameLen + 1;
    }
    
	return runningSize;
}

//packs meta data (pKeyOffset (2 bytes), fKeyOffset (2 bytes))
void packMetaData(uint16_t pKeyOffset, uint16_t fKeyOffset, char *data) {
	memcpy(data, &pKeyOffset, sizeof(uint16_t));
	memcpy(data + 2, &fKeyOffset, sizeof(uint16_t));
}

//packs the attributes into the data array
void packAttributes(uint16_t attrOffset, std::vector<FLOPPYCreateColumn *> *columns, char *data) {
	uint16_t currentOffset = attrOffset;
	uint8_t attrType = 0, colLen = 0, attrNameLen = 0;
	
	for (unsigned i=0; i<columns->size(); i++) {
        FLOPPYCreateColumn *col = columns->at(i);
        
        //pack attr type
        attrType = col->type;
        memcpy(data + currentOffset, &attrType, sizeof(uint8_t));
		currentOffset++;
        
        //pack attr length in 1 byte if type is a char or varchar
		if (attrType == VARCHAR) {
			colLen = col->size;
			memcpy(data + currentOffset, &colLen, sizeof(uint8_t));
			currentOffset++;
		}
		
		//pack name into attrNameLen+1 space
		attrNameLen = strlen(col->name);
		memcpy(data + currentOffset, col->name, attrNameLen + 1);
		currentOffset += attrNameLen + 1;
    }
}

//packs the foreign keys into the data array
void packForeignKeys(uint16_t fKeyOffset, std::vector<FLOPPYForeignKey *> *fKeys, char *data) {
	uint16_t currentOffset = fKeyOffset;
	uint8_t fKeyTableNameLen = 0, fKeyAttrNameLen = 0;
	char *tableName, *attrName;
	
	//go through each foreign key reference
	for (unsigned i=0; i<fKeys->size(); i++) {
        FLOPPYForeignKey *fk = fKeys->at(i);
        tableName = fk->refTableName;
		fKeyTableNameLen = strlen(tableName);
			
		//for each attribute associated with the table name:
		//pack the table name and then the attribute name        
        for (unsigned j=0; j<fk->attributes->size(); j++) {
        	attrName = fk->attributes->at(j);
        	
			//pack table name
			memcpy(data + currentOffset, tableName, fKeyTableNameLen + 1);
			currentOffset += fKeyTableNameLen + 1;
     		
     		//pack attr name
     		fKeyAttrNameLen = strlen(attrName);
			memcpy(data + currentOffset, attrName, fKeyAttrNameLen + 1);
			currentOffset += fKeyAttrNameLen + 1;
        }
    }
}

//packs the primary keys into the data array
void packPrimaryKeys(uint16_t pKeyOffset, FLOPPYPrimaryKey *pKeys, char *data) {
	char *pKeyName;
	uint16_t currentOffset = pKeyOffset;
	uint8_t attrNameLen = 0;
	
	//primary key is pack as the following:
	//attrName (null terminated)
	for (unsigned i=0; i<pKeys->attributes->size(); i++) {
		pKeyName = pKeys->attributes->at(i);
		attrNameLen = strlen(pKeyName);
		memcpy(data + currentOffset, pKeyName, attrNameLen + 1);
		currentOffset += attrNameLen + 1;
    }
}

//packs the record description based on the statement given
int packRecordDescription(FLOPPYCreateTableStatement *statement, char *data) {
	uint16_t attrSize = calcSizeOfAttributes(statement->columns);
	uint16_t pKeySize = calcSizeOfPrimaryKeys(statement->pk);
	uint16_t fKeySize = calcSizeOfForeignKeys(statement->fk);
	uint16_t attrOffset = ATTRIBUTE_OFFSET;
	uint16_t pKeyOffset = attrOffset + attrSize;
	uint16_t fKeyOffset = pKeyOffset + pKeySize;
	int totalSize = attrSize + pKeySize + fKeySize + ATTRIBUTE_OFFSET;
	
	if (totalSize >= MAX_LENGTH) {
		printf("ERROR: Record description is too long!!\n");
		return -1;
	}
	
	packMetaData(pKeyOffset, fKeyOffset, data);
	packAttributes(attrOffset, statement->columns, data);
	packPrimaryKeys(pKeyOffset, statement->pk, data);
	packForeignKeys(fKeyOffset, statement->fk, data);
	
	//printf("\nSIZES:\n Attributes: %d\n pKeys: %d\n  pKeyOffset: %d\n fKeys: %d\n  fKeyOffset: %d\n", attrSize, pKeySize, pKeyOffset, fKeySize, fKeyOffset);
	
	return totalSize;
}

//initializes attrDescription to some invalid type
AttrDescription initAttrDescription() {
	AttrDescription attrDesc;
	
	attrDesc.attrType = -1;
	attrDesc.attrSize = -1;
	attrDesc.offset = -1;
	
	return attrDesc;
}

//returns the size of the name and also stores the name into a string
void buildColumnName(char *data, int nameLen, char *storedName) {
	int i;
	
	for (i = 0; i < nameLen; i++) {
		storedName[i] = data[i];
	}
}

//gets the length of the column name in the binary data array
int getColumnNameLen(char *data) {
	char *temp = data;
	int nameLen = 0;
	
	while (*temp) {
		nameLen++;
		temp++;
	}
	
	return nameLen;
}

//get byte size of attribute types
int getAttrTypeByteSize(ColumnType attrType) {
	if (attrType == INT) {
		return 4;
	}
	else if (attrType == FLOAT) {
		return 8;
	}
	else if (attrType == VARCHAR) {
		return 0;
	}
	else if (attrType == DATETIME) {
		return 8;
	}
	else if (attrType == BOOLEAN) {
		return 1;
	}
	else {
		return -1;
	}
}

//given a column name grab all the attributes associated with that column
AttrDescription getAttributeDescription(char *data, char *colName) {
	AttrDescription attrDesc = initAttrDescription();
	int cmp = -1, curOffset = 4, nameLen = 0, recordOffset = 0, prevRecSize = 0;
	char *attrName;
	uint8_t attrType = 0, colLen = 0;
	uint16_t pKeyOffset;
	
	//grab the primary key offset value
	memcpy(&pKeyOffset, data, sizeof(uint16_t));
	
	//while we haven't found the column name yet
	while (cmp && curOffset < pKeyOffset) {
		//add previous record size
		if (recordOffset != 0) {
			//add padding if prev attr was int
			if (attrType == INT) {
				recordOffset += recordOffset % 4;
			}
			//add padding if prev attr was float or datetime
			else if (attrType == FLOAT || attrType == DATETIME) {
				recordOffset += recordOffset % 8;
			}
		}
		recordOffset += prevRecSize;
		
		//grab the attribute type
		attrType = data[curOffset];
		curOffset++;
		
		//grab colLen if there is one
		if (attrType == VARCHAR) {
			colLen = data[curOffset];
			curOffset++;
		}
		else {
			colLen = getAttrTypeByteSize((ColumnType)attrType);
		}
		
		//set up previous col length
		prevRecSize = colLen;

		//get name length and build attrName data array		
		nameLen = getColumnNameLen(data + curOffset);
		attrName = (char *)calloc(nameLen + 1, sizeof(char));
		buildColumnName(data + curOffset, nameLen, attrName);
		
		//compare the two names
 		cmp = strcmp(attrName, colName);
		curOffset += nameLen + 1;
		
		free(attrName);
	}
	
	//if we didnt find the column and the offset has exceeded the attributes
	//return empty AttrDescription
	if (cmp && curOffset >= pKeyOffset) {
		return attrDesc;
	}
	
	attrDesc.attrType = attrType;
	attrDesc.attrSize = colLen;
	attrDesc.offset = recordOffset;
	
	return attrDesc;
}

//grabs the record length of a record on a data page of this type of data
int getRecordLength(char *data) {
	int recLen = 0, pkOffset, curOffset = 4, attrType, colLen;
	
	//grab primary key offset
	memcpy(&pkOffset, data, sizeof(uint16_t));
	
	while (curOffset < pkOffset) {
		//get attribute type
		attrType = data[curOffset++];	
		
		//grab col length of the attribut type
		if (attrType == VARCHAR) {
			colLen = data[curOffset++];
		}
		else {
			colLen = getAttrTypeByteSize((ColumnType)attrType);
		}
		
		//make sure the rec length isn't zero. If it isn't
		//make sure to add padding if necessary
		if (recLen != 0) {
			//add padding if prev attr was int
			if (attrType == INT) {
				recLen += recLen % 4;
			}
			//add padding if prev attr was float or datetime
			else if (attrType == FLOAT || attrType == DATETIME) {
				recLen += recLen % 8;
			}
		}
		
		recLen += colLen;
		
		//skip through the name of the attribute cause we're not using it
		while (data[curOffset] != 0) {
			curOffset++;
		}
		
		//skip null terminator
		curOffset++;
	}
	
	recLen += recLen % 8;
	
	return recLen;
}