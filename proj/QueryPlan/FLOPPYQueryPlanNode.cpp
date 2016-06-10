#include "FLOPPYQueryPlanNode.h"

FLOPPYQueryPlanNode::FLOPPYQueryPlanNode() {
	leftChild = NULL;
	rightChild = NULL;
}

FLOPPYQueryPlanNode::~FLOPPYQueryPlanNode() {
	if (leftChild) {
		delete leftChild;
	}
	
	if (rightChild) {
		delete rightChild;
	}
}
