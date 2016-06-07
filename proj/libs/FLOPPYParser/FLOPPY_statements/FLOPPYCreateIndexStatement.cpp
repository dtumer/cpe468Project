#include "FLOPPYCreateIndexStatement.h"
#include <stdlib.h>

FLOPPYCreateIndexStatement::FLOPPYCreateIndexStatement() : FLOPPYStatement(CreateIndexStatement) {

}

FLOPPYCreateIndexStatement::~FLOPPYCreateIndexStatement() {
   free(name);
   free(tableReference);
   for (auto itr = this->attributes->begin() ; itr != this->attributes->end(); itr++) {
      free(*itr);
   }
   delete this->attributes;
}
