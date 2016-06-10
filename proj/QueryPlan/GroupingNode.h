#ifndef GroupingNode_h
#define GroupingNode_h

#include "FLOPPYQueryPlanNode.h"
#include "../libs/FLOPPYParser/FLOPPY_statements/FLOPPYSelectItem.h"
#include "../libs/FLOPPYParser/FLOPPY_statements/FLOPPYSelectStatement.h"
#include "../libs/FLOPPYParser/FLOPPY_statements/FLOPPYValue.h"
#include "../libs/FLOPPYParser/FLOPPY_statements/FLOPPYNode.h"
#include <vector>
#include <iostream>

class GroupingNode : public FLOPPYQueryPlanNode {
private:
    FLOPPYSelectStatement *statement;
    void loadHavingAggregates(FLOPPYNode*);
public:
    GroupingNode();
    ~GroupingNode();    
    std::vector<FLOPPYSelectItem *> aggregates;
    void setStatement(FLOPPYSelectStatement *statement);
    void loadAggregates();
    void printAggregates();
    void printGroupByAttributes();
    std::vector<FLOPPYTableAttribute *> * getGroupByAttributes();
    std::vector<FLOPPYSelectItem *> getAggregates();
};

#endif
