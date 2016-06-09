#ifndef GroupingNode_h
#define GroupingNode_h

#include "FLOPPYQueryPlanNode.h"
#include "../libs/FLOPPYParser/FLOPPY_statements/FLOPPYSelectItem.h"
#include "../libs/FLOPPYParser/FLOPPY_statements/FLOPPYSelectStatement.h"
#include "../libs/FLOPPYParser/FLOPPY_statements/FLOPPYValue.h"
#include <vector>
#include <iostream>

class GroupingNode : public FLOPPYQueryPlanNode {
private:
    FLOPPYSelectStatement *statement;
public:
    GroupingNode();
    ~GroupingNode();    
    std::vector<FLOPPYSelectItem *> aggregates;
    void setStatement(FLOPPYSelectStatement *statement);
    void getAggregates();
    void printAggregates();
};

#endif
