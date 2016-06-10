#include "FLOPPYQueryPlanNode.h"

FLOPPYQueryPlanNode::FLOPPYQueryPlanNode() {
}

void FLOPPYQueryPlanNode::setOp(RelationalOperation r) {
    this->op = r;
}

RelationalOperation FLOPPYQueryPlanNode::getOp() {
    return this->op;
}

void FLOPPYQueryPlanNode::setLeftChild(FLOPPYQueryPlanNode *leftChild) {
    this->leftChild = leftChild;
}
void FLOPPYQueryPlanNode::setRightChild(FLOPPYQueryPlanNode *rightChild) {
    this->rightChild = rightChild;
}

FLOPPYQueryPlanNode::~FLOPPYQueryPlanNode() {
}
