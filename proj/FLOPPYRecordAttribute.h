//
//  FLOPPYRecordAttribute.h
//

#ifndef FLOPPYRecordAttribute_h
#define FLOPPYRecordAttribute_h

#include <stdio.h>
#include "libs/FLOPPYParser/FLOPPY_statements/FLOPPYCreateColumn.h"
#include "libs/FLOPPYParser/FLOPPY_statements/FLOPPYValue.h"


class FLOPPYRecordAttribute {
public:
    FLOPPYRecordAttribute();
    ~FLOPPYRecordAttribute();
    
    char *tableName;
    char *name;
    FLOPPYValue *val;
    
};

#endif /* FLOPPYRecordAttribute_h */
