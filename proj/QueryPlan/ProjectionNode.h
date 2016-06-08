#ifndef ProjectionNode_h
#define ProjectionNode_h

#include <string>
#include <vector>
#include "FLOPPYQueryPlanNode.h"

typedef struct {
   std::string tableName;
   std::string colName;
} Column;

class ProjectionNode : public FLOPPYQueryPlanNode {
public:
    ProjectionNode(); 
    ~ProjectionNode(); 
    void addToProjection(std::string tableName, std::string colName);
    std::vector<Column> columns;
};

#endif
