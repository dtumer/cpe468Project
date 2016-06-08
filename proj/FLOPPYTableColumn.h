//
//  FLOPPYTableColumn.h
//

#ifndef FLOPPYTableColumn_h
#define FLOPPYTableColumn_h

#include "libs/FLOPPYParser/FLOPPY_statements/FLOPPYCreateColumn.h"

/*typedef enum {
    INT,
    FLOAT,
    DATETIME,
    VARCHAR,
    BOOLEAN
} ColumnType;*/

class FLOPPYTableColumn {
public:
	/* Constructors */
    FLOPPYTableColumn();
    
    /* Descructor */
    virtual ~FLOPPYTableColumn();
    
    /* Instance Variables */
    char *name;
    ColumnType type;
    int size;
    int offset;
};

#endif /* FLOPPYTableColumn_h */
