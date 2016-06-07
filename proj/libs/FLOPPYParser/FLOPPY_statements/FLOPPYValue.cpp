#include "FLOPPYValue.h"
#include <stdlib.h>

FLOPPYValue::FLOPPYValue(ValueType type) : _type(type) {}
FLOPPYValue::~FLOPPYValue() {
   if (_type == StringValue) {
      free(sVal);
   }
   else if (_type == AttributeValue) {
      free(sVal);
   }
   else if (_type == TableAttributeValue) {
      delete this->tableAttribute;
   }
}
