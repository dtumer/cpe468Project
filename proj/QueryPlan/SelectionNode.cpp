#include "SelectionNode.h"

SelectionNode::SelectionNode() {
}

void SelectionNode::setCondition(FLOPPYNode *node) {
    this->condition = node;
}

void SelectionNode::printCondition() {
    printFLOPPYNode(0, condition);
    printf("\n");
}

SelectionNode::~SelectionNode() {
}
