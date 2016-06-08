//FLOPPYQueryPlanNode.h

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
};

class SelectionNode extends FLOPPYQueryPlanNode {

};

class ProjectionNode extends FLOPPYQueryPlanNode {
};

class DeduplicateNode extends FLOPPYQueryPlanNode {
};

class GroupingNode extends FLOPPYQueryPlanNode {
};

