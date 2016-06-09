//
//  FLOPPYRecordSet.h
//

#ifndef FLOPPYRecordSet_h
#define FLOPPYRecordSet_h

#include "FLOPPYRecordAttribute.h"
#include "FLOPPYRecord.h"

#include <list>
#include <stdio.h>

class FLOPPYRecordSet {
public:
    FLOPPYRecordSet();
    ~FLOPPYRecordSet();
    
    void filter(FLOPPYNode *filter);
    void limit(int limit);
    void print();
    
	std::list<FLOPPYRecord *> *records;
};


#endif /* FLOPPYRecordSet_h */
