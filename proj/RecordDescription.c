#include "RecordDescription.h"

#include <stdint.h>
#include <string.h>

#define ATTRIBUTE_OFFSET 4

//calculates the size of the foreign keys that will be needed in the byte array
uint16_t calcSizeOfForeignKeys(foreignKeys *fKeys) {
	uint16_t runningSize = 0;
	foreignKeys *temp = fKeys;
	
	//STILL NEED TO IMPLEMENT THIS
	
	return runningSize;
}

//calculates the size of the primary keys that will be needed in the byte array
uint16_t calcSizeOfPrimaryKeys(Attribute *pKeys) {
	uint16_t runningSize = 0;
	Attribute *temp = pKeys;
	uint8_t pKeyNameLen = 0;
	
	while (temp) {
		pKeyNameLen = strlen(temp->attName);
		
		runningSize += 3 + pKeyNameLen;
		temp = temp->next;
	}
	
	return runningSize;
}

//calculates the size of the attributes that will be needed in the byte array
uint16_t calcSizeOfAttributes(Attribute *attrs) {
	uint16_t runningSize = 0;
	Attribute *temp = attrs;
	uint8_t attrNameLen = 0, attrType, attrLen;
	
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

void packMetaData(uint16_t pKeyOffset, uint16_t fKeyOffset, char *data) {
	memcpy(data, &pKeyOffset, sizeof(uint16_t));
	memcpy(data + 2, &fKeyOffset, sizeof(uint16_t));
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
		attrType = temp->attType;
		
		if (attrType == VARCHAR || attrType == CHAR) {
			colLen = temp->attSize;
		}
		
		printf("ATTR: colType: %d, colLen: %d, colNameLen: %d\n", attrType, colLen, attrNameLen);		
		temp = temp->next;
	}
}

//packs the primary keys into the data array
void packPrimaryKeys(uint16_t pKeyOffset, char *data) {

}

//packs the foreign keys into the data array
void packForeignKeys(uint16_t fKeyOffset, char *data) {

}

char* packRecordDescription(tableDescription *desc) {
	uint16_t attrSize = calcSizeOfAttributes(desc->attributeList);
	uint16_t pKeySize = calcSizeOfPrimaryKeys(desc->pKey);
	uint16_t fKeySize = calcSizeOfForeignKeys(desc->fKeys);
	uint16_t attrOffset = ATTRIBUTE_OFFSET;
	uint16_t pKeyOffset = attrOffset + attrSize;
	uint16_t fKeyOffset = pKeyOffset + pKeySize;
	char *data = calloc(attrSize + pKeySize + fKeySize + ATTRIBUTE_OFFSET, sizeof(char));
	
	packMetaData(attrOffset, fKeyOffset, data);
	packAttributes(attrOffset, desc->attributeList, data);
	packPrimaryKeys(pKeyOffset, data);
	packForeignKeys(fKeyOffset, data);
	
	printf("\nSIZES:\n Attributes: %d\n pKeys: %d\n  pKeyOffset: %d\n fKeys: %d\n  fKeyOffset: %d\n data: %d\n", attrSize, pKeySize, pKeyOffset, fKeySize, fKeyOffset, strlen(data));
	
	return data;
}