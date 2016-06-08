//
//  FLOPPYRecordSet.cpp
//

#include "FLOPPYRecordSet.h"


FLOPPYRecordSet::FLOPPYRecordSet() {
    records = new std::vector<FLOPPYRecord *>();
}

FLOPPYRecordSet::~FLOPPYRecordSet() {
    
}

void FLOPPYRecordSet::print() {
    printf("Records:\n");
    for (unsigned i=0; i<records->size(); i++) {
        FLOPPYRecord *record = records->at(i);
        printf("  %02d-%02d: ", record->pageId, record->recordId);
        
        for (unsigned i=0; i<record->columns->size(); i++) {
            if(i>0)
                printf(", ");
            
            FLOPPYRecordAttribute *col = record->columns->at(i);
            if(col->tableName)
                printf("%s.",col->tableName);
            printf("%s (",col->name);
            
            if(col->val->type() == ValueType::StringValue)
                printf("STRING \"%s\"", &(col->val->sVal));
            else if(col->val->type() == ValueType::IntValue)
                printf("INT %ld", col->val->iVal);
            else if(col->val->type() == ValueType::FloatValue)
                printf("FLOAT %f", col->val->fVal);
            else if(col->val->type() == ValueType::BooleanValue)
                printf("BOOLEAN %d", col->val->bVal);
            else if(col->val->type() == ValueType::NullValue)
                printf("NULL");
            
            printf(")");
        }
        
        printf("\n");
    }
}