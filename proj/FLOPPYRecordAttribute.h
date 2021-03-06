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
    bool isAggregate;
    FLOPPYAggregateOperator op;
    
    static int compareValues(FLOPPYValue *valA, FLOPPYValue *valB);

    FLOPPYRecordAttribute* clone();
};

#endif /* FLOPPYRecordAttribute_h */
