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
    void sort(std::vector<FLOPPYTableAttribute *> *orderBys);
    void limit(int limit);
    void print();
    
	std::list<FLOPPYRecord *> *records;
    
private:
    static bool sortHelper(FLOPPYRecord *recA, FLOPPYRecord *recB, std::vector<FLOPPYTableAttribute *> *orderBys);
    
    void mergeSort(std::vector<FLOPPYTableAttribute *> *orderBys);
    void mergeSortMerge(FLOPPYRecordSet *setA, FLOPPYRecordSet *setB, std::vector<FLOPPYTableAttribute *> *orderBys);
};


#endif /* FLOPPYRecordSet_h */
