//
//  FLOPPYTableColumn.cpp
//

#include "FLOPPYTableColumn.h"
#include <stdlib.h>

FLOPPYTableColumn::FLOPPYTableColumn() {
	
}

FLOPPYTableColumn::~FLOPPYTableColumn() {
    free(name);
}
