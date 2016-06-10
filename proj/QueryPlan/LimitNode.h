#ifndef LimitNode_h
#define LimitNode_h

#include "FLOPPYQueryPlanNode.h"

class LimitNode : public FLOPPYQueryPlanNode {
public:
    LimitNode();
    ~LimitNode();
    int limit;
};

#endif
