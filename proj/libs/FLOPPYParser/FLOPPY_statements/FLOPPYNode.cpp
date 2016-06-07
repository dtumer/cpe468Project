#include "FLOPPYNode.h"
#include "FLOPPYValue.h"
#include <stdlib.h>

FLOPPYNode::FLOPPYNode(FLOPPYNodeType type) : _type(type) {
    if (_type == AggregateNode) {
       aggregate.value = NULL;
    } else if (_type == ValueNode) {
        value = NULL;
    } else {
        node.left = NULL;
        node.right = NULL;
    }
}
FLOPPYNode::~FLOPPYNode() {
   if (_type == AggregateNode) {
      delete this->aggregate.value;
   } else if (_type == ValueNode) {
      delete this->value;
   } else {
      if (this->node.left) {
         delete this->node.left;
      }
      if (this->node.right) {
         delete this->node.right;
      }
   }
}
