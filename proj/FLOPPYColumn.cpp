//
//  FLOPPYColumn.cpp
//

#include "FLOPPYColumn.h"
#include <stdlib.h>

FLOPPYColumn::FLOPPYColumn() {}

FLOPPYColumn::FLOPPYColumn(char *name, ColumnType type, int size, int offset) : name(name), type(type), size(size), offset(offset) {
    
}

FLOPPYColumn::~FLOPPYColumn() {
    free(name);
}
