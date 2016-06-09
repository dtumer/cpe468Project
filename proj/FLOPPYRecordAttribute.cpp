//
//  FLOPPYRecordAttribute.cpp
//

#include "FLOPPYRecordAttribute.h"
#include <stdlib.h>

FLOPPYRecordAttribute::FLOPPYRecordAttribute() {
    tableName = NULL;
    name = NULL;
}

FLOPPYRecordAttribute::~FLOPPYRecordAttribute() {
    if(tableName)
        free(tableName);
    free(name);
    delete val;
}

int FLOPPYRecordAttribute::compareValues(FLOPPYValue *valA, FLOPPYValue *valB) {
    if(valA->type() == ValueType::StringValue)
        return strcmp(valA->sVal, valB->sVal);
    else if(valA->type() == ValueType::IntValue)
        return valA->iVal - valB->iVal;
    else if(valA->type() == ValueType::FloatValue) {
        float temp = valA->fVal - valB->fVal;
        
        if(temp == 0)
            return 0;
        else if(temp < 0)
            return -1;
        else
            return 0;
    }
    else if(valA->type() == ValueType::BooleanValue)
        return valA->bVal - valB->bVal;
    else
        return 0;
}
