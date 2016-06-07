#include "FLOPPYDropTableStatement.h"
#include <stdlib.h>

FLOPPYDropTableStatement::FLOPPYDropTableStatement() : 
   FLOPPYStatement(DropTableStatement) {
}

FLOPPYDropTableStatement::~FLOPPYDropTableStatement() {
   free(table);
}
