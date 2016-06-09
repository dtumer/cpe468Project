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

FLOPPYRecordAttribute* FLOPPYRecordAttribute::clone() {
	FLOPPYRecordAttribute *ret = new FLOPPYRecordAttribute();
	FLOPPYValue *value;
	
	ret->tableName = (char*)calloc(strlen(tableName) + 1, sizeof(char));
	memcpy(ret->tableName, tableName, strlen(tableName));
	
	ret->name = (char*)calloc(strlen(name) + 1, sizeof(char));
	memcpy(ret->name, name, strlen(name));
	
	if (val->type() == ValueType::StringValue) {
		value = new FLOPPYValue(StringValue);
		value->sVal = (char*)calloc(strlen(val->sVal) + 1, sizeof(char));
		memcpy(value->sVal, val->sVal, strlen(val->sVal));
	}
	else if (val->type() == ValueType::IntValue) {
		value = new FLOPPYValue(IntValue);
		value->iVal = val->iVal;
	}
	else if (val->type() == ValueType::FloatValue) {
		value = new FLOPPYValue(FloatValue);
		value->fVal = val->fVal;
	}
	else if (val->type() == ValueType::BooleanValue) {
		value = new FLOPPYValue(BooleanValue);
		value->bVal = val->bVal;		
	}
	else {
		value = new FLOPPYValue(NullValue);
	}
	
	ret->val = value;
	
	return ret;
}
