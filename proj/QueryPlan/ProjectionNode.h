#ifndef FLOPPYQueryPlanNode_h
#define FLOPPYQueryPlanNode_h

#include "FLOPPYQueryPlanNode.h"

typedef struct {
   std::string tableName;
   std::string colName;
} Column

class ProjectionNode extends FLOPPYQueryPlanNode {
    std::vector<Column> columns;
    ProjectionNode(); 
    ~ProjectionNode(); 
};

#endif
