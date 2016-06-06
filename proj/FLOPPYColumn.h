//
//  FLOPPYColumn.h
//

#ifndef FLOPPYColumn_h
#define FLOPPYColumn_h

typedef enum {
    INT,
    FLOAT,
    DATETIME,
    VARCHAR,
    BOOLEAN
} ColumnType;

class FLOPPYColumn {
public:
    FLOPPYColumn();
    virtual ~FLOPPYColumn();
    char *name;
    ColumnType type;
    int size;
    int offset;
};

#endif /* FLOPPYColumn_h */
