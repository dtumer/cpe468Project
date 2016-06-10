#ifndef ProjectionNode_h
#define ProjectionNode_h

#include <string>
#include <vector>
#include <iostream>
#include "FLOPPYQueryPlanNode.h"
#include "../libs/FLOPPYParser/FLOPPY_statements/FLOPPYSelectItem.h"

typedef struct {
   std::string tableName;
   std::string colName;
} Column;

class ProjectionNode : public FLOPPYQueryPlanNode {
public:
    ProjectionNode(); 
    ~ProjectionNode(); 
    void addToProjection(std::string tableName, std::string colName);
    void printColumns();
    void setItems(std::vector<FLOPPYSelectItem *> *aggregates);
    std::vector<FLOPPYSelectItem *> *getItems();
    std::vector<Column> columns;
    std::vector<FLOPPYSelectItem *> *items;
};

#endif
