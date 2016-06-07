#include "FLOPPYForeignKey.h"
#include <stdlib.h>

FLOPPYForeignKey::FLOPPYForeignKey() {}
FLOPPYForeignKey::~FLOPPYForeignKey() {
   for (auto itr = this->attributes->begin() ; itr != this->attributes->end(); itr++) {
      free(*itr);
   }

   delete attributes;

   free(refTableName);
}
