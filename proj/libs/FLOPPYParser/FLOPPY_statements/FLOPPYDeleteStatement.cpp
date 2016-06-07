#include "FLOPPYDeleteStatement.h"
#include "FLOPPYNode.h"
#include <stdlib.h>

FLOPPYDeleteStatement::FLOPPYDeleteStatement() : FLOPPYStatement(DeleteStatement) {}
FLOPPYDeleteStatement::~FLOPPYDeleteStatement() {
   free(name);
   delete where;
}
