#include "FLOPPYSelectItem.h"
#include "FLOPPYValue.h"

FLOPPYSelectItem::FLOPPYSelectItem(FLOPPYSelectItemType type) : _type(type) {}
FLOPPYSelectItem::~FLOPPYSelectItem() {
   if (_type == AggregateType) {
      delete aggregate.value;
   } else if (_type == TableAttributeType) {
      delete tableAttribute.tableName;
      delete tableAttribute.attribute;
   } else {
      delete attribute;
   }
}
