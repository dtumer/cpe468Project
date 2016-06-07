//FLOPPYQueryPlan.h

#ifndef FLOPPYQueryPlan_h
#define FLOPPYQueryPlan_h

//incomplete list!
//TODO fill it out and figure out data representations for arguments;
//ie columns for grouping, attribute list for projection
typedef {
   SELECTION,
   PROJECTION,
   DEDUPLICATE,
   RENAME,
   CROSS,
   SORT,
   GROUP,
   TABLE
   } RelationalOperation;


class FLOPPYQueryPlan {
public:
	/* Constructors */
    FLOPPYQueryPlan();
    FLOPPYQueryPlan(FLOPPYStatement *statement);
	
    
    /* Descructor */
    virtual ~FLOPPYQueryPlan();
    
    
    //inner class for tree nodes
    struct QueryNode {
       //if TABLE, there are no operands, just a name; both the child pointers should
       //be NULL. if not TABLE, tablename should be NULL.
       RelationalOperation op;
       char *tablename = NULL;
       
       void *operationData = NULL;       
       QueryNode *firstChild = NULL;
       QueryNode *secondChild = NULL;
    }
    
    /* Instance Variables */
    

	
};

#endif /* FLOPPYQueryPlan_h */