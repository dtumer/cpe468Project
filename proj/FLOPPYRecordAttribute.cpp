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

int FLOPPYRecordAttribute::compareValues(FLOPPYValue *recA, FLOPPYValue *recB) {
    if(recA->type() == ValueType::StringValue)
        return strcmp(recA->sVal, recB->sVal);
    else if(recA->type() == ValueType::IntValue)
        return recA->iVal - recB->iVal;
    else if(recA->type() == ValueType::FloatValue) {
        float temp = recA->fVal - recB->fVal;
        
        if(temp == 0)
            return 0;
        else if(temp < 0)
            return -1;
        else
            return 0;
    }
    else if(recA->type() == ValueType::BooleanValue)
        return recA->bVal - recB->bVal;
    else
        return 0;
}
