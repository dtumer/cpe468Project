#ifndef TableNode_h
#define TableNode_h

#include "FLOPPYQueryPlanNode.h"
#include <string>
#include <iostream>

class TableNode : public FLOPPYQueryPlanNode {
private:
   std::string tableName;
public:
    TableNode(); 
    ~TableNode(); 
    void setTableName(std::string tableName);
    void printTableName();
};

#endif
