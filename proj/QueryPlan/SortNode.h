#ifndef SortNode_h
#define SortNode_h

#include "FLOPPYQueryPlanNode.h"
#include "../libs/FLOPPYParser/FLOPPY_statements/FLOPPYSelectItem.h"
#include "../libs/FLOPPYParser/FLOPPY_statements/FLOPPYSelectStatement.h"
#include "../libs/FLOPPYParser/FLOPPY_statements/FLOPPYValue.h"
#include "../libs/FLOPPYParser/FLOPPY_statements/FLOPPYNode.h"
#include <vector>

class SortNode : public FLOPPYQueryPlanNode {
    public:
        SortNode();
        ~SortNode();
        std::vector<FLOPPYTableAttribute *> *orderBys;
};
#endif
