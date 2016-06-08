//
//  FLOPPYRecordAttribute.cpp
//

#include "FLOPPYRecordAttribute.h"
#include <stdlib.h>

FLOPPYRecordAttribute::FLOPPYRecordAttribute() {
    tableName = NULL;
}

FLOPPYRecordAttribute::~FLOPPYRecordAttribute() {
    if(tableName)
        free(tableName);
    free(name);
}