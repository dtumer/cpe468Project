//FLOPPYQueryPlan.h

#ifndef FLOPPYQueryPlan_h
#define FLOPPYQueryPlan_h

class FLOPPYQueryPlan {
public:
	/* Constructors */
    FLOPPYQueryPlan();
    FLOPPYQueryPlan(FLOPPYStatement *statement);
	
    
    /* Descructor */
    virtual ~FLOPPYQueryPlan();
    
    /* methods to create a query plan*/
    createSelectQueryPlan(FLOPPYSelectStatement *stmt);
    create
    
    /* Instance Variables */
    char *name;
    ColumnType type;
    int size;
    int offset;
    

	
};

#endif /* FLOPPYQueryPlan_h */