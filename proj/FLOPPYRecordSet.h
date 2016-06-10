//
//  FLOPPYRecordSet.h
//

#ifndef FLOPPYRecordSet_h
#define FLOPPYRecordSet_h

#include "FLOPPYRecordAttribute.h"
#include "FLOPPYRecord.h"
#include "libs/FLOPPYParser/FLOPPY_statements/FLOPPYSelectItem.h"

#include "libs/FLOPPYParser/FLOPPY_statements/FLOPPYSelectItem.h"

#include <list>
#include <stdio.h>

class FLOPPYRecordSet {
public:
    FLOPPYRecordSet();
    ~FLOPPYRecordSet();
    
    void filter(FLOPPYNode *filter);
    void sort(std::vector<FLOPPYTableAttribute *> *orderBys);
    void limit(int limit);
    void groupBy(std::vector<FLOPPYTableAttribute *> *groupByAttributes, std::vector<FLOPPYSelectItem *> *aggregates);
    void distinct();
    void projection(std::vector<FLOPPYSelectItem *> *items);
    static FLOPPYRecordSet* crossProduct(FLOPPYRecordSet *set1, FLOPPYRecordSet *set2);
    
    virtual int count() {
        return records->size();
    }
    
    void print();
    
	std::list<FLOPPYRecord *> *records;

private:
    static bool sortHelper(FLOPPYRecord *recA, FLOPPYRecord *recB, std::vector<FLOPPYTableAttribute *> *orderBys);
    
    void mergeSort(std::vector<FLOPPYTableAttribute *> *orderBys);
    void mergeSortMerge(FLOPPYRecordSet *setA, FLOPPYRecordSet *setB, std::vector<FLOPPYTableAttribute *> *orderBys);
    
    FLOPPYValue* createInitialFLOPPYValue();
    
    //helpers for grouping/aggregation
    bool shouldBeAddedToGrouping(FLOPPYRecord *record, std::list<FLOPPYRecord *> *retRecords, std::vector<FLOPPYTableAttribute *> *groupByAttributes);
	void addGroupByColumns(FLOPPYRecord *record, FLOPPYRecord **newRecord, std::vector<FLOPPYTableAttribute *> *groupByAttributes);
	void initializeAggregations(FLOPPYRecord *record, FLOPPYRecord **newRecord, std::vector<FLOPPYSelectItem *> *aggregates);
};


#endif /* FLOPPYRecordSet_h */
