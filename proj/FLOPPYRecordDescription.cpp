//
//  FLOPPYRecordDescription.cpp
//

#include "FLOPPYRecordDescription.h"

FLOPPYRecordDescription::FLOPPYRecordDescription() {
    columns = new std::vector<FLOPPYColumn *>();
}

FLOPPYRecordDescription::FLOPPYRecordDescription(char *recordDescription) {
    FLOPPYColumn *column;
    uint16_t pkOffset = 0, fkOffset = 0;
    char *ptr = recordDescription;
    int recordOffset = 0;
    
    //allocate columns store
    columns = new std::vector<FLOPPYColumn *>();
    
    //grab primary key offset
    pkOffset = (uint16_t) *ptr;
    ptr += sizeof(uint16_t);
    fkOffset = (uint16_t) *ptr;
    ptr += sizeof(uint16_t);
    
    while (ptr < (recordDescription + pkOffset)) {
        column = new FLOPPYColumn();
        
        //get attribute type
        column->type = (ColumnType)*ptr;
        ptr += sizeof(uint8_t);
        
        //get col length
        if (column->type == ColumnType::VARCHAR) {
            column->size = (uint8_t)*ptr;
        }
        else if (column->type == ColumnType::INT) {
            column->size = 4;
            recordOffset += recordOffset % 4;
        }
        else if (column->type == ColumnType::FLOAT) {
            column->size = 8;
            recordOffset += recordOffset % 8;
        }
        else if (column->type == ColumnType::DATETIME) {
            column->size = 8;
            recordOffset += recordOffset % 8;
        }
        else if (column->type == ColumnType::BOOLEAN) {
            column->size = 1;
        }
        
        //add colOffset and update
        column->offset = recordOffset;
        recordOffset += column->size;
        
        //get attr name
        column->name = (char*)calloc(sizeof(char*), strlen(ptr));
        strcpy(column->name, ptr);
        ptr += (strlen(ptr)+1);
        
        //add to column list
        columns->push_back(column);
    }
}

FLOPPYRecordDescription::~FLOPPYRecordDescription() {
    for (auto itr = this->columns->begin() ; itr != this->columns->end(); itr++) {
        delete (*itr);
    }
    delete columns;
}

int FLOPPYRecordDescription::packRecordDescription(FLOPPYCreateTableStatement *statement, char *data) {
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
    
    //printf("RECORD DESCRIPTION LENGTH: %d\n", totalSize);
    
    //packMetaData
    packMetaData(pKeyOffset, fKeyOffset, data);
    packAttributes(attrOffset, statement->columns, data);
    packPrimaryKeys(pKeyOffset, statement->pk, data);
    packForeignKeys(fKeyOffset, statement->fk, data);
    
    return FLOPPYRecordDescription::calcRecordLength(data);
}

FLOPPYColumn* FLOPPYRecordDescription::getAttribute(char *colName) {
    int cmp = -1;
    
    for (unsigned i = 0; i < columns->size(); i++) {
        cmp = strcmp((*columns)[i]->name, colName);
        
        if (cmp == 0) {
            return (*columns)[i];
        }
    }
    
    return NULL;
}

void FLOPPYRecordDescription::addColumn(char *name, ColumnType type, int size, int offset) {
    FLOPPYColumn *column = new FLOPPYColumn(name, type, size, offset);
    columns->push_back(column);
}


uint16_t FLOPPYRecordDescription::calcSizeOfAttributes(std::vector<FLOPPYCreateColumn *> *columns) {
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

uint16_t FLOPPYRecordDescription::calcSizeOfPrimaryKeys(FLOPPYPrimaryKey *pKeys) {
    uint16_t runningSize = 0;
    uint8_t pKeyNameLen = 0;
    
    for (unsigned i=0; i<pKeys->attributes->size(); i++) {
        pKeyNameLen = strlen(pKeys->attributes->at(i));
        
        runningSize += pKeyNameLen + 1;
    }
    
    return runningSize;
}

uint16_t FLOPPYRecordDescription::calcSizeOfForeignKeys(std::vector<FLOPPYForeignKey *> *fKeys) {
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

void FLOPPYRecordDescription::packMetaData(uint16_t pKeyOffset, uint16_t fKeyOffset, char *data) {
    memcpy(data, &pKeyOffset, sizeof(uint16_t));
    memcpy(data + 2, &fKeyOffset, sizeof(uint16_t));
}

void FLOPPYRecordDescription::packAttributes(uint16_t attrOffset, std::vector<FLOPPYCreateColumn *> *columns, char *data) {
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

void FLOPPYRecordDescription::packPrimaryKeys(uint16_t pKeyOffset, FLOPPYPrimaryKey *pKeys, char *data) {
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

void FLOPPYRecordDescription::packForeignKeys(uint16_t fKeyOffset, std::vector<FLOPPYForeignKey *> *fKeys, char *data) {
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

int FLOPPYRecordDescription::calcRecordLength(char *data) {
    int recLen = 0, pkOffset = 0, curOffset = 4, attrType, colLen;
    
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

int FLOPPYRecordDescription::getAttrTypeByteSize(ColumnType attrType) {
    if (attrType == ColumnType::INT)
        return 4;
    else if (attrType == ColumnType::FLOAT)
        return 8;
    else if (attrType == ColumnType::VARCHAR)
        return 0;
    else if (attrType == ColumnType::DATETIME)
        return 8;
    else if (attrType == ColumnType::BOOLEAN)
        return 1;
    else
        return -1;
}

//returns the size of the name and also stores the name into a string
void FLOPPYRecordDescription::buildColumnName(char *data, int nameLen, char *storedName) {
    int i;
    
    for (i = 0; i < nameLen; i++) {
        storedName[i] = data[i];
    }
}

//gets the length of the column name in the binary data array
int FLOPPYRecordDescription::getColumnNameLen(char *data) {
    char *temp = data;
    int nameLen = 0;
    
    while (*temp) {
        nameLen++;
        temp++;
    }
    
    return nameLen;
}


/* Debug Output */
void FLOPPYRecordDescription::print() {
    printf("Columns\n");
    for (unsigned i=0; i<columns->size(); i++) {
        FLOPPYColumn *column = columns->at(i);
        
        printf("\t Column: %s\n", column->name);
        if(column->type == ColumnType::INT)
            printf("\t\t type: INT\n");
        else if(column->type == ColumnType::FLOAT)
            printf("\t\t type: FLOAT\n");
        else if(column->type == ColumnType::DATETIME)
            printf("\t\t type: DATETIME\n");
        else if(column->type == ColumnType::VARCHAR)
            printf("\t\t type: VARCHAR\n");
        else if(column->type == ColumnType::BOOLEAN)
            printf("\t\t type: BOOLEAN\n");
        printf("\t\t size: %d\n", column->size);
        printf("\t\t offset: %d\n", column->offset);
    }
}
