//
//  FLOPPYRecordAttribute.h
//

#ifndef FLOPPYRecordAttribute_h
#define FLOPPYRecordAttribute_h

#include <stdio.h>
#include <string.h>

#include "libs/FLOPPYParser/FLOPPY_statements/FLOPPYCreateColumn.h"
#include "libs/FLOPPYParser/FLOPPY_statements/FLOPPYValue.h"
#include "libs/FLOPPYParser/FLOPPY_statements/FLOPPYNode.h"


class FLOPPYRecordAttribute {
public:
    FLOPPYRecordAttribute();
    ~FLOPPYRecordAttribute();
    
    char *tableName;
    char *name;
    FLOPPYValue *val;
    
    static int compareValues(FLOPPYValue *recA, FLOPPYValue *recB);
    
};

#endif /* FLOPPYRecordAttribute_h */
