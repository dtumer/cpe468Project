//FLOPPYQueryPlan.h

#ifndef FLOPPYQueryPlan_h
#define FLOPPYQueryPlan_h

#include "FLOPPYQueryPlanNode.h"
#include "../libs/FLOPPYParser/FLOPPYParser.h"
#include "../FLOPPYResult.h"
#include "../ParserDebug.h"

class FLOPPYQueryPlan {
public:
	/* Constructors */
    FLOPPYQueryPlan();
    FLOPPYQueryPlan(FLOPPYStatement *statement);
	
    
    /* Destructor */
    virtual ~FLOPPYQueryPlan();

	/* getter for the tree structure */ 
    FLOPPYQueryPlanNode *getTree();
private:
   FLOPPYQueryPlanNode *root;
   void createJoinTree(std::vector<FLOPPYTableSpec *> tableSpecs);
   void createSelectStatementTree(FLOPPYSelectStatement *statement);
	
};

#endif /* FLOPPYQueryPlan_h */
