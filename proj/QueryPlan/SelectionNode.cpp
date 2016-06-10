#include "SelectionNode.h"

SelectionNode::SelectionNode() {
	this->clause = NULL;
}

SelectionNode::SelectionNode(FLOPPYNode *clause) {
	this->clause = clause;
}

SelectionNode::~SelectionNode() {
	delete clause;
}

// void SelectionNode::setCondition(FLOPPYNode *node) {
//     this->condition = node;
// }
// 
// void SelectionNode::printCondition() {
//     printFLOPPYNode(0, condition);
//     printf("\n");
// }
