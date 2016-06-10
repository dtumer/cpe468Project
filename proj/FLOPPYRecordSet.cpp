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

void FLOPPYRecordSet::projection(std::vector<FLOPPYSelectItem *> *items) {
    std::vector<FLOPPYRecordAttribute *> *tempAttributes;
    
    //if no items are in the list or if it is
    if(items->size() < 1 || (items->size() == 1 && items->front()->_type == FLOPPYSelectItemType::StarType)) {
        return;
    }
    
    //loop through each record
    std::list<FLOPPYRecord *>::iterator recItr = records->begin();
    while (recItr != records->end()) {
        tempAttributes = new std::vector<FLOPPYRecordAttribute *>();
        
        //loop though each select item
        std::vector<FLOPPYSelectItem *>::iterator selectItr = items->begin();
        while (selectItr != items->end()) {
            
            if((*selectItr)->_type == FLOPPYSelectItemType::TableAttributeType) {
                printf("\t Attr: \"%s.%s\n", (*selectItr)->tableAttribute->tableName, (*selectItr)->tableAttribute->attribute);
                
                //loop through all columns still in record
                std::vector<FLOPPYRecordAttribute *>::iterator colItr = (*recItr)->columns->begin();
                while (colItr != (*recItr)->columns->end()) {
                    FLOPPYRecordAttribute *tempAttr = *colItr;
                    
                    //skip aggregates
                    if(tempAttr->isAggregate) {
                        colItr++;
                        continue;
                    }
                    
                    //skip different table names
                    if ((*selectItr)->tableAttribute->tableName) {
                        if (strcmp((*selectItr)->tableAttribute->tableName, tempAttr->tableName) != 0) {
                            colItr++;
                            continue;
                        }
                    }
                    
                    //skip different attribute
                    if (strcmp((*selectItr)->tableAttribute->attribute, tempAttr->name) != 0) {
                        colItr++;
                        continue;
                    }
                    
                    //add to tempAttr
                    colItr = (*recItr)->columns->erase(colItr);
                    tempAttributes->push_back(tempAttr);
                }
                selectItr++;
            }
            else if((*selectItr)->_type == FLOPPYSelectItemType::AggregateType) {
                printf("\t Aggregate: \"%s.%s\n", (*selectItr)->aggregate.value->tableAttribute->tableName, (*selectItr)->aggregate.value->tableAttribute->attribute);
                
                //loop through all columns still in record
                std::vector<FLOPPYRecordAttribute *>::iterator colItr = (*recItr)->columns->begin();
                while (colItr != (*recItr)->columns->end()) {
                    FLOPPYRecordAttribute *tempAttr = *colItr;
                    
                    //skip attributes
                    if(!tempAttr->isAggregate) {
                        colItr++;
                        continue;
                    }
                    
                    //skip different operations
                    if((*selectItr)->aggregate.op != tempAttr->op) {
                        colItr++;
                        continue;
                    }
                    
                    //skip different table names
                    if ((*selectItr)->aggregate.value->tableAttribute->tableName) {
                        if (strcmp((*selectItr)->aggregate.value->tableAttribute->tableName, tempAttr->tableName) != 0) {
                            colItr++;
                            continue;
                        }
                    }
                    
                    //skip different attribute
                    if (strcmp((*selectItr)->aggregate.value->tableAttribute->attribute, tempAttr->name) != 0) {
                        colItr++;
                        continue;
                    }
                    
                    //add to tempAttr
                    colItr = (*recItr)->columns->erase(colItr);
                    tempAttributes->push_back(tempAttr);
                }
                selectItr++;
            }
        }
        
        (*recItr)->columns->swap(*tempAttributes);
        
        for (auto itr = tempAttributes->begin() ; itr != tempAttributes->end(); itr++) {
            delete (*itr);
        }
        delete tempAttributes;
        recItr++;
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

FLOPPYRecordSet* FLOPPYRecordSet::crossProduct(FLOPPYRecordSet *set1, FLOPPYRecordSet *set2) {
	unsigned int k;
	FLOPPYRecordSet *retRecordSet = new FLOPPYRecordSet();
	FLOPPYRecord *record, *set1Record, *set2Record;
	
	std::list<FLOPPYRecord *>::iterator set1Itr = set1->records->begin();
	std::list<FLOPPYRecord *>::iterator set2Itr;
	
	//for each record in set 1
	while (set1Itr != set1->records->end()) {
		set1Record = *set1Itr;
		
		set2Itr = set2->records->begin();
		//for each record in set 2
		while(set2Itr != set2->records->end()) {
			set2Record = *set2Itr;
			
			//create a new record that will have all columns of set1 and this set2 record
			record = new FLOPPYRecord();
			
			//add all columns from set1
			for (k = 0; k < set1Record->columns->size(); k++) {
				record->columns->push_back(set1Record->columns->at(k)->clone());
			}
			//add all columns from set2
			for (k = 0; k < set2Record->columns->size(); k++) {
				record->columns->push_back(set2Record->columns->at(k)->clone());
			}
			
			retRecordSet->records->push_back(record);

			set2Itr++;
		}
		
		set1Itr++;
	}
	
	return retRecordSet;
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