//
//  FLOPPYRecord.h
//

#ifndef FLOPPYRecord_h
#define FLOPPYRecord_h

#include "FLOPPYRecordAttribute.h"

#include <vector>

class FLOPPYRecord {
public:
    FLOPPYRecord();
    ~FLOPPYRecord();
    
    int pageId;
    int recordId;
    
    FLOPPYRecordAttribute * getColByTblAttr(FLOPPYTableAttribute *attr);
    
    static int compare(FLOPPYRecord *recA, FLOPPYRecord *recB, FLOPPYTableAttribute *cmpAttr);
    FLOPPYValue *filter(FLOPPYNode *node);
    
    std::vector<FLOPPYRecordAttribute *> *columns;
    
private:
    std::vector<FLOPPYValue *> *tempNodes;
};

#endif /* FLOPPYRecord_h */
