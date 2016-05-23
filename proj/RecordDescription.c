#include "RecordDescription.h"

#include <stdint.h>
#include <string.h>

#define ATTRIBUTE_OFFSET 6

//calculates the size of the foreign keys that will be needed in the byte array
uint16_t calcSizeOfForeignKeys(foreignKeys *fKeys) {
	uint16_t runningSize = 0;
	foreignKeys *temp = fKeys;
	uint8_t fKeyTableNameLen = 0, fKeyAttrNameLen = 0, fKeyAttrType;
	
	//tableNameLen, tableName + 1, attrType, colLen(if applicable), attrNameLen, attrName + 1
	while (temp) {
		fKeyTableNameLen = strlen(temp->tableName);
		fKeyAttrNameLen = strlen(temp->key->attName);
		fKeyAttrType = temp->key->attType;
		
		if (fKeyAttrType == VARCHAR || fKeyAttrType == CHAR) {
			runningSize++;
		}
		
		//tableNameLen, tableName + 1, attrType, attrNameLen, attrName + 1
		runningSize += 1 + fKeyTableNameLen + 3 + fKeyAttrNameLen + 1;
		temp = temp->next;
	}
	
	return runningSize;
}

//calculates the size of the primary keys that will be needed in the byte array
uint16_t calcSizeOfPrimaryKeys(Attribute *pKeys) {
	uint16_t runningSize = 0;
	Attribute *temp = pKeys;
	uint8_t pKeyNameLen = 0, pKeyAttrType;
	
	while (temp) {
		pKeyNameLen = strlen(temp->attName);
		pKeyAttrType = temp->attType;
		
		if (pKeyAttrType == VARCHAR || pKeyAttrType == CHAR) {
			runningSize++; //add extra byte for pkey col size
		}
			
		runningSize += 2 + pKeyNameLen + 1;
		temp = temp->next;
	}
	
	return runningSize;
}

//calculates the size of the attributes that will be needed in the byte array
uint16_t calcSizeOfAttributes(Attribute *attrs) {
	uint16_t runningSize = 0;
	Attribute *temp = attrs;
	uint8_t attrNameLen = 0, attrType;
	
	//for each attribute in the attributes list
	//1: Pack attribute type
	//2: Pack col length n if varchar (if not of type int or float)
	//3: Pack attribute name length
	//4: Pack attribute name
	while (temp) {
		attrNameLen = strlen(temp->attName);
		attrType = temp->attType;
		
		if (attrType == VARCHAR || attrType == CHAR) {
			runningSize++;
		}
		
		//add two bytes, one for each of the uint8's and then add the length of the
		//name of the attribute + 1
		runningSize += 2 + attrNameLen + 1;
		temp = temp->next;
	}
	
	return runningSize;
}

void packMetaData(uint16_t pKeyOffset, uint16_t fKeyOffset, uint16_t dataLen, char *data) {
	memcpy(data, &dataLen, sizeof(uint16_t));
	memcpy(data + 2, &pKeyOffset, sizeof(uint16_t));
	memcpy(data + 4, &fKeyOffset, sizeof(uint16_t));
}

//packs the attributes into the data array
void packAttributes(uint16_t attrOffset, Attribute *attrs, char *data) {
	uint16_t currentOffset = attrOffset;
	uint8_t attrType = 0, colLen = 0, attrNameLen = 0;
	Attribute *temp = attrs;
	
	//for each attribute in the attributes list
	//1: Pack attribute type
	//2: Pack col length n if varchar (if not of type int or float)
	//3: Pack attribute name length
	//4: Pack attribute name
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
		
		//pack name length into one byte
		attrNameLen = strlen(temp->attName);
		memcpy(data + currentOffset, &attrNameLen, sizeof(uint8_t));
		currentOffset++;
		
		//pack name into attrNameLen+1 space
		memcpy(data + currentOffset, temp->attName, attrNameLen + 1);
		currentOffset += attrNameLen + 1;
		
		printf("ATTR: colType: %d, colLen: %d, colNameLen: %d\n", attrType, colLen, attrNameLen);		
		temp = temp->next;
	}
}

//packs the primary keys into the data array
void packPrimaryKeys(uint16_t pKeyOffset, Attribute *pKeys, char *data) {
	uint16_t currentOffset = pKeyOffset;
	uint8_t attrType = 0, colLen = 0, attrNameLen = 0;
	Attribute *temp = pKeys;
	
	//Do same thing as attributes
	//attrType, col length (if applicable), attrNameLen, attrName
	while (temp) {
		attrType = temp->attType;
		memcpy(data + currentOffset, &attrType, sizeof(uint8_t));
		currentOffset++;
		
		//check for varchar and char types
		if (attrType == VARCHAR || attrType == CHAR) {
			colLen = temp->attSize;
			memcpy(data + currentOffset, &colLen, sizeof(uint8_t));
			currentOffset++;
		}
		
		//pack name length
		attrNameLen = strlen(temp->attName);
		memcpy(data + currentOffset, &attrNameLen, sizeof(uint8_t));
		currentOffset++;
		
		//pack attr name
		memcpy(data + currentOffset, temp->attName, attrNameLen + 1);
		currentOffset += attrNameLen + 1;
		
		temp = temp->next;
	}
}

//packs the foreign keys into the data array
void packForeignKeys(uint16_t fKeyOffset, foreignKeys *fKeys, char *data) {
	uint16_t currentOffset = fKeyOffset;
	uint8_t fKeyTableNameLen = 0, fKeyAttrNameLen = 0, fKeyAttrType, colLen = 0;
	foreignKeys *temp = fKeys;
	
	//tableNameLen, tableName + 1, attrType, colLen(if applicable), attrNameLen, attrName + 1
	while (temp) {
		//pack table name len
		fKeyTableNameLen = strlen(temp->tableName);
		memcpy(data + currentOffset, &fKeyTableNameLen, sizeof(uint8_t));
		currentOffset++;
		
		//pack table name
		memcpy(data + currentOffset, temp->tableName, fKeyTableNameLen + 1);
		currentOffset += fKeyTableNameLen + 1;
		
		//pack attrType
		fKeyAttrType = temp->key->attType;
		memcpy(data + currentOffset, &fKeyAttrType, sizeof(uint8_t));
		currentOffset++;
		
		if (fKeyAttrType == VARCHAR || fKeyAttrType == CHAR) {
			//pack colLen
			colLen = temp->key->attSize;
			memcpy(data + currentOffset, &colLen, sizeof(uint8_t));
			currentOffset++;
		}
		
		//pack att name length
		fKeyAttrNameLen = strlen(temp->key->attName);
		memcpy(data + currentOffset, &fKeyAttrNameLen, sizeof(uint8_t));
		currentOffset++;
		
		//pack att name
		memcpy(data + currentOffset, temp->key->attName, fKeyAttrNameLen + 1);
		currentOffset += fKeyAttrNameLen + 1;
		
		temp = temp->next;
	}
}

char* packRecordDescription(tableDescription *desc) {
	uint16_t attrSize = calcSizeOfAttributes(desc->attributeList);
	uint16_t pKeySize = calcSizeOfPrimaryKeys(desc->pKey);
	uint16_t fKeySize = calcSizeOfForeignKeys(desc->fKeys);
	uint16_t attrOffset = ATTRIBUTE_OFFSET;
	uint16_t pKeyOffset = attrOffset + attrSize;
	uint16_t fKeyOffset = pKeyOffset + pKeySize;
	char *data = calloc(attrSize + pKeySize + fKeySize + ATTRIBUTE_OFFSET, sizeof(char));
	
	packMetaData(attrOffset, fKeyOffset, attrSize + pKeySize + fKeySize + ATTRIBUTE_OFFSET, data);
	packAttributes(attrOffset, desc->attributeList, data);
	packPrimaryKeys(pKeyOffset, desc->pKey, data);
	packForeignKeys(fKeyOffset, desc->fKeys, data);
	
	printf("\nSIZES:\n Attributes: %d\n pKeys: %d\n  pKeyOffset: %d\n fKeys: %d\n  fKeyOffset: %d\n", attrSize, pKeySize, pKeyOffset, fKeySize, fKeyOffset);
	
	return data;
}