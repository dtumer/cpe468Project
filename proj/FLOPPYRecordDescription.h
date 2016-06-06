//
//  FLOPPYRecordDescription.h
//

#ifndef FLOPPYRecordDescription_h
#define FLOPPYRecordDescription_h

#include "FLOPPYColumn.h"

class FLOPPYRecordDescription {
public:
    FLOPPYRecordDescription();
    ~FLOPPYRecordDescription();
    
    void addColumn(char *name, ColumnType type, int size, int offset);
    
    std::vector<FLOPPYColumn *> *columns;
};


#endif /* FLOPPYRecordDescription_h */
