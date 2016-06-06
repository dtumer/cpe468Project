//
//  FLOPPYColumn.h
//

#ifndef FLOPPYColumn_h
#define FLOPPYColumn_h

#include "libs/FLOPPYParser/FLOPPY_statements/FLOPPYCreateColumn.h"

/*typedef enum {
    INT,
    FLOAT,
    DATETIME,
    VARCHAR,
    BOOLEAN
} ColumnType;*/

class FLOPPYColumn {
public:
	/* Constructors */
    FLOPPYColumn();
    FLOPPYColumn(char *name, ColumnType type, int size, int offset);
    
    /* Descructor */
    virtual ~FLOPPYColumn();
    
    /* Instance Variables */
    char *name;
    ColumnType type;
    int size;
    int offset;
};

#endif /* FLOPPYColumn_h */
