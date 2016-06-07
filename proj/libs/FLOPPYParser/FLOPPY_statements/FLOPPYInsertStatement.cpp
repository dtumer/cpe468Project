#include "FLOPPYInsertStatement.h"
#include "FLOPPYValue.h"
#include <stdlib.h>

FLOPPYInsertStatement::FLOPPYInsertStatement() : FLOPPYStatement(InsertStatement) {}
FLOPPYInsertStatement::~FLOPPYInsertStatement() {
   free(name);
   for (auto itr = this->values->begin() ; itr != this->values->end(); itr++) {
      delete (*itr);
   }

   delete values;

}
