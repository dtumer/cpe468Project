#include "FLOPPYTableSpec.h"
#include <stdlib.h>

FLOPPYTableSpec::FLOPPYTableSpec() {}
FLOPPYTableSpec::~FLOPPYTableSpec() {
   free(tableName);
   free(alias);
}
