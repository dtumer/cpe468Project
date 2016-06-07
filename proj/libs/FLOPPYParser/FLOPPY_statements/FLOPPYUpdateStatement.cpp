#include "FLOPPYUpdateStatement.h"
#include "FLOPPYNode.h"
#include <stdlib.h>

FLOPPYUpdateStatement::FLOPPYUpdateStatement() : FLOPPYStatement(UpdateStatement) {

}
FLOPPYUpdateStatement::~FLOPPYUpdateStatement() {
   free(tableName);
   free(attributeName);
   delete this->attributeExpression;
   delete this->whereExpression;
}
