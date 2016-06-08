//FLOPPYQueryPlanNode.h

#include "SelectionNode.h"
#include "ProjectionNode.h"
#include "DeduplicateNode.h"
#include "GroupingNode.h"
#include "CrossProductNode.h"
#include "TableNode.h"

//incomplete list!
//TODO fill it out and figure out data representations for arguments;
//ie columns for grouping, attribute list for projection
typedef {
   SELECTION,
   PROJECTION,
   DEDUPLICATE,
   GROUP,
   UNION,
   INTERSECT,
   CROSS,
   DIFFERENCE,
   SORT,
   TABLE
   } RelationalOperation;

class FLOPPYQueryPlanNode {
public:
	RelationalOperation operator;

   FLOPPYQueryPlanNode leftChild;
   FLOPPYQueryPlanNode rightChild;
   
   void apply();  
   
};





