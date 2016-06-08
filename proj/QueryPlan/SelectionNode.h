#ifndef SelectionNode_h
#define SelectionNode_h

#include "FLOPPYQueryPlanNode.h"
#include "../libs/FLOPPYParser/FLOPPYParser.h"
#include "../ParserDebug.h"


class SelectionNode : public FLOPPYQueryPlanNode {
private:
    FLOPPYNode *condition;
public:
	SelectionNode();
	~SelectionNode();
    void setCondition(FLOPPYNode *node);
    void printCondition();
};
#endif
