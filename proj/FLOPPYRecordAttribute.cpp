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