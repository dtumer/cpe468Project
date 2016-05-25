#include "RecordDescription.h"

#include <stdint.h>
#include <string.h>

#define ATTRIBUTE_OFFSET 4
#define MAX_LENGTH 600

//calculates the size of the foreign keys that will be needed in the byte array
uint16_t calcSizeOfForeignKeys(foreignKeys *fKeys) {
	uint16_t runningSize = 0;
	foreignKeys *temp = fKeys;
	uint8_t fKeyTableNameLen = 0, fKeyAttrNameLen = 0;
	
	//foreign keys are packed in the following way:
	//tableName (null terminated), attrName (null terminated)
	while (temp) {
		fKeyTableNameLen = strlen(temp->tableName);
		fKeyAttrNameLen = strlen(temp->key->attName);
		
		runningSize +=  fKeyTableNameLen + 1 + fKeyAttrNameLen + 1;
		temp = temp->next;
	}
	
	return runningSize;
}

//calculates the size of the primary keys that will be needed in the byte array
uint16_t calcSizeOfPrimaryKeys(Attribute *pKeys) {
	uint16_t runningSize = 0;
	Attribute *temp = pKeys;
	uint8_t pKeyNameLen = 0;
	
	//primary key is pack as the following:
	//attrName (null terminated)
	while (temp) {
		pKeyNameLen = strlen(temp->attName);
			
		runningSize += pKeyNameLen + 1;
		temp = temp->next;
	}
	
	return runningSize;
}

//calculates the size of the attributes that will be needed in the byte array
uint16_t calcSizeOfAttributes(Attribute *attrs) {
	uint16_t runningSize = 0;
	Attribute *temp = attrs;
	uint8_t attrNameLen = 0, attrType;
	
	//attributes are packed as the following:
	//attrType (1 byte), colLen (1 byte if applicable), attrName (null terminated)
	while (temp) {
		attrNameLen = strlen(temp->attName);
		attrType = temp->attType;
		
		if (attrType == VARCHAR || attrType == CHAR) {
			runningSize++;
		}
		
		runningSize += 1 + attrNameLen + 1;
		temp = temp->next;
	}
	
	return runningSize;
}

void packMetaData(uint16_t pKeyOffset, uint16_t fKeyOffset, char *data) {
	memcpy(data, &pKeyOffset, sizeof(uint16_t));
	memcpy(data + 2, &fKeyOffset, sizeof(uint16_t));
}

//packs the attributes into the data array
void packAttributes(uint16_t attrOffset, Attribute *attrs, char *data) {
	uint16_t currentOffset = attrOffset;
	uint8_t attrType = 0, colLen = 0, attrNameLen = 0;
	Attribute *temp = attrs;
	
	//attributes are packed as the following:
	//attrType (1 byte), colLen (1 byte if applicable), attrName (null terminated)
	while (temp) {
		//pack attr type in one byte
		attrType = temp->attType;
		memcpy(data + currentOffset, &attrType, sizeof(uint8_t));
		currentOffset++;
		
		//pack attr length in 1 byte if type is a char or varchar
		if (attrType == VARCHAR || attrType == CHAR) {
			colLen = temp->attSize;
			memcpy(data + currentOffset, &colLen, sizeof(uint8_t));
			currentOffset++;
		}
		
		//pack name into attrNameLen+1 space
		attrNameLen = strlen(temp->attName);
		memcpy(data + currentOffset, temp->attName, attrNameLen + 1);
		currentOffset += attrNameLen + 1;
		
		temp = temp->next;
	}
}

//packs the primary keys into the data array
void packPrimaryKeys(uint16_t pKeyOffset, Attribute *pKeys, char *data) {
	uint16_t currentOffset = pKeyOffset;
	uint8_t attrNameLen = 0;
	Attribute *temp = pKeys;
	
	//primary key is pack as the following:
	//attrName (null terminated)
	while (temp) {
		//pack attrName
		attrNameLen = strlen(temp->attName);
		memcpy(data + currentOffset, temp->attName, attrNameLen + 1);
		currentOffset += attrNameLen + 1;
		
		temp = temp->next;
	}
}

//packs the foreign keys into the data array
void packForeignKeys(uint16_t fKeyOffset, foreignKeys *fKeys, char *data) {
	uint16_t currentOffset = fKeyOffset;
	uint8_t fKeyTableNameLen = 0, fKeyAttrNameLen = 0;
	foreignKeys *temp = fKeys;
	
	//foreign keys are packed in the following way:
	//tableName (null terminated), attrName (null terminated)
	while (temp) {
		//pack table name
		fKeyTableNameLen = strlen(temp->tableName);
		memcpy(data + currentOffset, temp->tableName, fKeyTableNameLen + 1);
		currentOffset += fKeyTableNameLen + 1;
		
		//pack attr name
		fKeyAttrNameLen = strlen(temp->key->attName);
		memcpy(data + currentOffset, temp->key->attName, fKeyAttrNameLen + 1);
		currentOffset += fKeyAttrNameLen + 1;
		
		temp = temp->next;
	}
}

//function for packing the record description into a byte array
int packRecordDescription(tableDescription *desc, char *data) {
	uint16_t attrSize = calcSizeOfAttributes(desc->attributeList);
	uint16_t pKeySize = calcSizeOfPrimaryKeys(desc->pKey);
	uint16_t fKeySize = calcSizeOfForeignKeys(desc->fKeys);
	uint16_t attrOffset = ATTRIBUTE_OFFSET;
	uint16_t pKeyOffset = attrOffset + attrSize;
	uint16_t fKeyOffset = pKeyOffset + pKeySize;
	int totalSize = attrSize + pKeySize + fKeySize + ATTRIBUTE_OFFSET;
	
	if (totalSize >= MAX_LENGTH) {
		printf("ERROR: Record description is too long!!\n");
		return -1;
	}
	
	packMetaData(pKeyOffset, fKeyOffset, data);
	packAttributes(attrOffset, desc->attributeList, data);
	packPrimaryKeys(pKeyOffset, desc->pKey, data);
	packForeignKeys(fKeyOffset, desc->fKeys, data);
	
	//printf("\nSIZES:\n Attributes: %d\n pKeys: %d\n  pKeyOffset: %d\n fKeys: %d\n  fKeyOffset: %d\n", attrSize, pKeySize, pKeyOffset, fKeySize, fKeyOffset);
	
	return totalSize;
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
int getAttrTypeByteSize(ATT_TYPE attrType) {
	if (attrType == INT) {
		return 4;
	}
	else if (attrType == FLOAT) {
		return 8;
	}
	else if (attrType == CHAR || attrType == VARCHAR) {
		return 0;
	}
	else if (attrType == DATETIME) {
		return 8;
	}
	else {
		return -1;
	}
}

//initializes attrDescription to some invalid type
AttrDescription initAttrDescription() {
	AttrDescription attrDesc;
	
	attrDesc.attrType = -1;
	attrDesc.attrSize = -1;
	
	return attrDesc;
}

//given a column name grab all the attributes associated with that column
AttrDescription getAttributeDescription(char *data, int dataSize, char *colName) {
	AttrDescription attrDesc = initAttrDescription();
	int cmp = -1, curOffset = 4, nameLen = 0;
	char *attrName;
	uint8_t attrType = 0, colLen = 0;
	uint16_t pKeyOffset;
	
	//grab the primary key offset value
	memcpy(&pKeyOffset, data, sizeof(uint16_t));
	
	//while we haven't found the column name yet
	while (cmp && curOffset < pKeyOffset) {
		//grab the attribute type
		attrType = data[curOffset];
		curOffset++;
		
		//grab colLen if there is one
		if (attrType == VARCHAR || attrType == CHAR) {
			colLen = data[curOffset];
			curOffset++;
		}
		else {
			colLen = getAttrTypeByteSize(attrType);
		}

		//get name length and build attrName data array		
		nameLen = getColumnNameLen(data + curOffset);
		attrName = calloc(nameLen + 1, sizeof(char));
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
	
	return attrDesc;
}

//prints the record description
void printRecordDescription(char *data, int size) {
	int i;
	
	for (i = 0; i < size; i++) {
		printf("%02X ", data[i]);
	}
	
	printf("\n");
}