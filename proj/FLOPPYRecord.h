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
    
    std::vector<FLOPPYRecordAttribute *> *columns;
};

#endif /* FLOPPYRecord_h */
