//FLOPPYQueryPlan.h

#ifndef FLOPPYQueryPlan_h
#define FLOPPYQueryPlan_h

#include "FLOPPYQueryPlanNode.h"



class FLOPPYQueryPlan {
public:
	/* Constructors */
    FLOPPYQueryPlan();
    FLOPPYQueryPlan(FLOPPYStatement *statement);
	
    
    /* Descructor */
    virtual ~FLOPPYQueryPlan();

	/* getter for the tree structure */ 
    FLOPPYQueryPlanNode *getTree();
    /* Instance Variables */
private:
   FLOPPYQueryPlanNode *root;
   

	
};

#endif /* FLOPPYQueryPlan_h */