#ifndef FLOPPYQueryPlanNode_h
#define FLOPPYQueryPlanNode_h

#include "FLOPPYQueryPlanNode.h"

typedef struct {
   char *tableAlias;
   char *colName;
} Column

class SelectionNode extends FLOPPYQueryPlanNode {
public:
	SelectionNode();
	
private:
	
};
#endif
