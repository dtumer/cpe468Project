//
//  FLOPPYTableDescription.h
//

#ifndef FLOPPYTableDescription_h
#define FLOPPYTableDescription_h

#define ATTRIBUTE_OFFSET 4
#define MAX_LENGTH 600

#include <vector>
#include <string.h>

#include "FLOPPYTableColumn.h"
#include "libs/FLOPPYParser/FLOPPYParser.h"

class FLOPPYTableDescription {
public:
    FLOPPYTableDescription();
    FLOPPYTableDescription(char *recordDescription); //parses data and returns list of columns
    virtual ~FLOPPYTableDescription();
    
    /* Statement packing method */
    static int packRecordDescription(FLOPPYCreateTableStatement *statement, char *data); //look into seeing if function can return size of a single record
    
    /* Get data associated with the specified column name */
    FLOPPYTableColumn* getAttribute(char *colName);
    
    /* Adds a column to record description object. Public for projecting certain columns and not others */
    void addColumn(char *name, ColumnType type, int size, int offset);
    
    /* Debugging */
    void print();
	
    /* Instance Variables */
    char *recordDescription;
    std::vector<FLOPPYTableColumn *> *columns;

private:
	/* Helper functions */
	/* calculates size of each section of the record description */
	static uint16_t calcSizeOfAttributes(std::vector<FLOPPYCreateColumn *> *columns);
	static uint16_t calcSizeOfPrimaryKeys(FLOPPYPrimaryKey *pKeys);
	static uint16_t calcSizeOfForeignKeys(std::vector<FLOPPYForeignKey *> *fKeys);

	/* packs each section of the record description */
	static void packMetaData(uint16_t pKeyOffset, uint16_t fKeyOffset, char *data);
	static void packAttributes(uint16_t attrOffset, std::vector<FLOPPYCreateColumn *> *columns, char *data);
	static void packPrimaryKeys(uint16_t pKeyOffset, FLOPPYPrimaryKey *pKeys, char *data);
	static void packForeignKeys(uint16_t fKeyOffset, std::vector<FLOPPYForeignKey *> *fKeys, char *data);
	
	/* Calcs size of a record in this table given its record description */
	static int calcRecordLength(char *data);
	static int getAttrTypeByteSize(ColumnType attrType);
	
	void buildColumnName(char *data, int nameLen, char *storedName);
	int getColumnNameLen(char *data);
};

#endif /* FLOPPYTableDescription_h */
