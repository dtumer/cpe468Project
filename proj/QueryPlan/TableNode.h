#ifndef TableNode_h
#define TableNode_h

#include "FLOPPYQueryPlanNode.h"
#include <string>
#include <iostream>

class TableNode : public FLOPPYQueryPlanNode {
private:
   std::string tableName;
   std::string alias;
public:
    TableNode(); 
    ~TableNode(); 
    void setTableName(char* tableName);
    const char* getTableName();
    void setAlias(char* tableAlias);
    const char* getAlias();
    void printTableName();
};

#endif
