//FLOPPYQueryPlan.h

#ifndef FLOPPYQueryPlan_h
#define FLOPPYQueryPlan_h

#include "FLOPPYQueryPlanNode.h"
#include "../libs/FLOPPYParser/FLOPPYParser.h"
#include "../FLOPPYResult.h"
#include "../ParserDebug.h"
#include <algorithm>

class FLOPPYQueryPlan {
public:
	/* Constructors */
    FLOPPYQueryPlan();
    FLOPPYQueryPlan(FLOPPYStatement *statement);
	
    /* Destructor */
	virtual ~FLOPPYQueryPlan();

	FLOPPYQueryPlanNode *root;

private:
	FLOPPYQueryPlanNode * createJoinTree(std::vector<FLOPPYTableSpec *> *tableSpecs);
	FLOPPYQueryPlanNode * createSelectStatementTree(FLOPPYSelectStatement *statement);
	void printCrossNode(FLOPPYQueryPlanNode *node);
	
};

#endif /* FLOPPYQueryPlan_h */
