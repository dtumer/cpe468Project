#include "FLOPPYCreateColumn.h"
#include <stdlib.h>

FLOPPYCreateColumn::FLOPPYCreateColumn() {}
FLOPPYCreateColumn::~FLOPPYCreateColumn() {
   free(name);
}

