#ifndef SelectionNode_h
#define SelectionNode_h

#include "FLOPPYQueryPlanNode.h"
#include "../libs/FLOPPYParser/FLOPPYParser.h"
#include "../ParserDebug.h"


class SelectionNode : public FLOPPYQueryPlanNode {
public:
	SelectionNode();
	SelectionNode(FLOPPYNode *clause);
	
	~SelectionNode();
    
    FLOPPYNode *clause;
    
    //void setCondition(FLOPPYNode *node);
    //void printCondition();
};
#endif
