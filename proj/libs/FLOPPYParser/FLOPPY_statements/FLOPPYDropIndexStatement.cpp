#include "FLOPPYDropIndexStatement.h"
#include <stdlib.h>

FLOPPYDropIndexStatement::FLOPPYDropIndexStatement() : FLOPPYStatement(DropIndexStatement) {

}
FLOPPYDropIndexStatement::~FLOPPYDropIndexStatement() {
   free(indexName);
   free(tableName);
}
