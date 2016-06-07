#include "FLOPPYPrimaryKey.h"
#include <stdlib.h>

FLOPPYPrimaryKey::FLOPPYPrimaryKey() {}
FLOPPYPrimaryKey::~FLOPPYPrimaryKey() {
   for (auto itr = this->attributes->begin() ; itr != this->attributes->end(); itr++) {
      free(*itr);
   }
   delete this->attributes;
}
