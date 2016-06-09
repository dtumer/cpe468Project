//
//  FLOPPYRecordSet.cpp
//

#include "FLOPPYRecordSet.h"
#include <functional>


FLOPPYRecordSet::FLOPPYRecordSet() {
    records = new std::list<FLOPPYRecord *>();
}

FLOPPYRecordSet::~FLOPPYRecordSet() {
    for (auto itr = this->records->begin() ; itr != this->records->end(); itr++) {
        delete (*itr);
    }
    delete records;
}

void FLOPPYRecordSet::filter(FLOPPYNode *filter) {
	FLOPPYRecord *tempRec;
    
    std::list<FLOPPYRecord *>::iterator itr = records->begin();
	while (itr != records->end()) {
        tempRec = *itr;
        FLOPPYValue *val = tempRec->filter(filter);
		
		if (val->type() == ValueType::BooleanValue) {
			if (!val->bVal) {
				itr = records->erase(itr);
				delete tempRec;
			}
            else
                itr++;
		}
		else {
			printf("ERROR - FLOPPYRecordSet::filter\n");
            itr++;
		}
	}
}

bool FLOPPYRecordSet::sortHelper(FLOPPYRecord *recA, FLOPPYRecord *recB, std::vector<FLOPPYTableAttribute *> *orderBys) {
    int cmp = 0;
    for (unsigned i=0; i<orderBys->size(); i++) {
        cmp = FLOPPYRecord::compare(recA, recB, orderBys->at(i));
        if(cmp != 0)
            break;
    }
    return (cmp <= 0);
}

void FLOPPYRecordSet::sort(std::vector<FLOPPYTableAttribute *> *orderBys) {
    using namespace std::placeholders;
    
    if(records->size() < 5) {
        records->sort(std::bind(&FLOPPYRecordSet::sortHelper, _1, _2, orderBys));
    }
    else
        mergeSort(orderBys);
}

void FLOPPYRecordSet::mergeSort(std::vector<FLOPPYTableAttribute *> *orderBys) {
    if(records->size() <= 1)
        return;
        
    FLOPPYRecordSet *setA = new FLOPPYRecordSet();
    FLOPPYRecordSet *setB = new FLOPPYRecordSet();
    FLOPPYRecord *tempRec;
    int count = 0;
    
    std::list<FLOPPYRecord *>::iterator itr = records->begin();
    while (records->size() > 0 && itr != records->end()) {
        tempRec = *itr;
        if(count%2) {
            setA->records->push_back(tempRec);
            itr = records->erase(itr);
        }
        else {
            setB->records->push_back(tempRec);
            itr = records->erase(itr);
        }
        count++;
    }
    
    setA->sort(orderBys);
    setB->sort(orderBys);
    
    mergeSortMerge(setA, setB, orderBys);
    
    delete setA;
    delete setB;
}

void FLOPPYRecordSet::mergeSortMerge(FLOPPYRecordSet *setA, FLOPPYRecordSet *setB, std::vector<FLOPPYTableAttribute *> *orderBys) {
    std::list<FLOPPYRecord *>::iterator itrA = setA->records->begin();
    std::list<FLOPPYRecord *>::iterator itrB = setB->records->begin();
    FLOPPYRecord *tempRecA, *tempRecB;
    
    while ((itrA != setA->records->end()) && (itrB != setB->records->end())) {
        tempRecA = *itrA;
        tempRecB = *itrB;
        int cmp = 0;
        
		//do compare
		for (unsigned i=0; i<orderBys->size(); i++) {
            cmp = FLOPPYRecord::compare(tempRecA, tempRecB, orderBys->at(i));
            if(cmp != 0)
                break;
        }
        
        if(cmp > 0){
            records->push_back(tempRecB);
            itrB = records->erase(itrB);
        }
        else {
            records->push_back(tempRecA);
            itrA = records->erase(itrA);
        }
    }
    
    while (itrA != setA->records->end()) {
        tempRecA = *itrA;
        records->push_back(tempRecA);
        itrA = records->erase(itrA);
    }
    
    while (itrB != setB->records->end()) {
        tempRecB = *itrB;
        records->push_back(tempRecB);
        itrB = records->erase(itrB);
    }
}

void FLOPPYRecordSet::limit(int limit) {
    int count = 0;
    FLOPPYRecord *tempRec;
    
    std::list<FLOPPYRecord *>::iterator itr = records->begin();
    while (itr != records->end()) {
        tempRec = *itr;
        
        if(count >= limit) {
            itr = records->erase(itr);
            delete tempRec;
        }
        else {
            itr++;
            count++;
        }
    }
}

void FLOPPYRecordSet::print() {
    printf("Records:\n");
    
    std::list<FLOPPYRecord *>::iterator itr = records->begin();
    
    while (itr != records->end()) {
        printf("  %02d-%02d: ", (*itr)->pageId, (*itr)->recordId);
        
        for (unsigned i=0; i<(*itr)->columns->size(); i++) {
            if(i>0)
                printf(", ");
            
            FLOPPYRecordAttribute *col = (*itr)->columns->at(i);
            if(col->tableName)
                printf("%s.",col->tableName);
            printf("%s (",col->name);
            
            if(col->val->type() == ValueType::StringValue)
                printf("STRING \"%s\"", col->val->sVal);
            else if(col->val->type() == ValueType::IntValue)
                printf("INT %ld", col->val->iVal);
            else if(col->val->type() == ValueType::FloatValue)
                printf("FLOAT %f", col->val->fVal);
            else if(col->val->type() == ValueType::BooleanValue)
                printf("BOOLEAN %d", col->val->bVal);
            else if(col->val->type() == ValueType::NullValue)
                printf("NULL");
            
            printf(")");
        }
        
        itr++;
        
        printf("\n");
    }
}