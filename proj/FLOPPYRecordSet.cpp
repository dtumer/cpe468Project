//
//  FLOPPYRecordSet.cpp
//

#include "FLOPPYRecordSet.h"
#include <functional>
#include <stdlib.h>
#include <algorithm>


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
                //printf("\t Attr: \"%s.%s\n", (*selectItr)->tableAttribute->tableName, (*selectItr)->tableAttribute->attribute);
                
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
                    
                    if((*selectItr)->aggregate.op != FLOPPYAggregateOperator::CountStarAggregate) {
                        //skip different attribute
                        if (strcmp((*selectItr)->aggregate.value->sVal, tempAttr->name) != 0) {
                            colItr++;
                            continue;
                        }
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

// returns boolean of whether or not a record should be added to the grouping
bool FLOPPYRecordSet::shouldBeAddedToGrouping(FLOPPYRecord *record, std::list<FLOPPYRecord *> *retRecords, std::vector<FLOPPYTableAttribute *> *groupByAttributes) {	
	unsigned int numGroupingEqual;
	
	if (retRecords->size() == 0) {
		return true;
	}
	
	//look through all records to be added and see if it should be added
	for (auto itr = retRecords->begin(); itr != retRecords->end(); itr++) {
		numGroupingEqual = 0;
		
		//look through each group by attribute and see if the record is equal to both
		for (auto groupingItr = groupByAttributes->begin(); groupingItr != groupByAttributes->end(); groupingItr++) {
			if (FLOPPYRecord::compare(record, *itr, *groupingItr) == 0) {
				numGroupingEqual++;
			}
		}
		
		if (numGroupingEqual == groupByAttributes->size()) {
			return false;
		}
	}
	
	return true;
}

void FLOPPYRecordSet::addColumns(FLOPPYRecord *record, FLOPPYRecord **newRecord) {
	//go through each attribute, add it to the new record
	for (auto colItr = record->columns->begin(); colItr != record->columns->end(); colItr++) {
		(*newRecord)->columns->push_back((*colItr)->clone());
		// auto attrItr = record->columns->begin();
// 		
// 		while (attrItr != record->columns->end()) {
// 			if ((*groupingItr)->tableName) {
//             	if (strcmp((*groupingItr)->tableName, (*attrItr)->tableName) != 0) {
//                     attrItr++;
//                     continue;
//                 }
//             }
//                      
//             if (strcmp((*groupingItr)->attribute, (*attrItr)->name) != 0) {
//                 attrItr++;
//                 continue;
//         	}
//         	
//         	//add attribute to new record
//         	(*newRecord)->columns->push_back((*attrItr)->clone());
//         	break;
// 		}
	}
}

//checks if an attribute is in the groupin attributes
bool FLOPPYRecordSet::isInGroupingAttrs(FLOPPYRecordAttribute *attr, std::vector<FLOPPYTableAttribute *> *groupByAttributes) {
	for (auto itr = groupByAttributes->begin(); itr != groupByAttributes->end(); itr++) {
		if ((*itr)->tableName) {
            if (strcmp((*itr)->tableName, attr->tableName) == 0) {
                return true;
            }
        }
          
        //column of type COUNT(*) will be null           
        if (attr->name) {
        	if (strcmp((*itr)->attribute, attr->name) == 0) {
				return true;
       		}
        }
	}
	
	return false;
}

//checks if an attribute is in the aggregates
bool FLOPPYRecordSet::isInAggregates(FLOPPYRecordAttribute *attr, std::vector<FLOPPYSelectItem *> *aggregates) {
	for (auto itr = aggregates->begin(); itr != aggregates->end(); itr++) {
		if (attr->isAggregate && (*itr)->aggregate.op == attr->op) {
			return true;
		}
	}
	
	return false;
}

void FLOPPYRecordSet::pruneColumns(std::list<FLOPPYRecord *> *newRecords, std::vector<FLOPPYTableAttribute *> *groupByAttributes, std::vector<FLOPPYSelectItem *> *aggregates) {
	//go through each column attribute, see if its in the grouping attribute and remove it if its not
	for (auto newRecord = newRecords->begin(); newRecord != newRecords->end(); newRecord++) {
		auto attrItr = (*newRecord)->columns->begin();
		
		while (attrItr != (*newRecord)->columns->end()) {
			if (!isInGroupingAttrs(*attrItr, groupByAttributes) && !isInAggregates(*attrItr, aggregates)) {
				attrItr = (*newRecord)->columns->erase(attrItr);	
				continue;
			}
			
			attrItr++;
		}
	}
}
//initializes columns for aggregations
void FLOPPYRecordSet::initializeAggregations(FLOPPYRecord *record, FLOPPYRecord **newRecord, std::vector<FLOPPYSelectItem *> *aggregates) {
	FLOPPYRecordAttribute *aggregate;
	
	for (auto aggItr = aggregates->begin(); aggItr != aggregates->end(); aggItr++) {
		aggregate = new FLOPPYRecordAttribute();
		
		aggregate->isAggregate = true;
		aggregate->op = (*aggItr)->aggregate.op;
		
		//if we're looking at a count(*)
		if ((*aggItr)->aggregate.op == FLOPPYAggregateOperator::CountStarAggregate) {
			aggregate->val = new FLOPPYValue(IntValue);
			aggregate->val->iVal = 0;
		}
		//don't do COUNT(<col>)
		else if ((*aggItr)->aggregate.op == FLOPPYAggregateOperator::CountAggregate) {
			printf("ERROR - Unsupported aggregation operator COUNT(<col>)\n");
		}
		//all others
		else {
			//add column name
			aggregate->name = (char*)calloc(strlen((*aggItr)->aggregate.value->sVal) + 1, sizeof(char));
    		strcpy(aggregate->name, (*aggItr)->aggregate.value->sVal);
    		
			//go through all attributes in tempCurRecord to find type of attribute and create FLOPPY value of that attribute type
    		for (auto recItr = record->columns->begin(); recItr != record->columns->end(); recItr++) {
    			if (strcmp((*recItr)->name, (*aggItr)->aggregate.value->sVal) != 0)
    				continue;
                        
    			if ((*recItr)->val->type() == ValueType::IntValue) {
                    aggregate->val = new FLOPPYValue(IntValue);
    				aggregate->val->iVal = 0;
    			}
    			else if ((*recItr)->val->type() == ValueType::FloatValue) {
                    aggregate->val = new FLOPPYValue(FloatValue);
    				aggregate->val->fVal = 0.0;
    			}
    			else {
    				printf("ERROR - Cannot aggregate on that column type\n");
    			}
    					
    			break;
    		}
		}
		
		(*newRecord)->columns->push_back(aggregate);
	}
}

void FLOPPYRecordSet::incrementCountStar(FLOPPYRecord *record) {
	for (auto itr = record->columns->begin(); itr != record->columns->end(); itr++) {
		if ((*itr)->isAggregate && (*itr)->op == FLOPPYAggregateOperator::CountStarAggregate) {
			(*itr)->val->iVal++;
		}
	}
}

//update max value
void FLOPPYRecordSet::updateMax(FLOPPYRecord *record, FLOPPYSelectItem *aggregate, FLOPPYRecordAttribute *attribute) {
	for (auto colItr = record->columns->begin(); colItr != record->columns->end(); colItr++) {
		if ((*colItr)->isAggregate && (*colItr)->op == FLOPPYAggregateOperator::MaxAggregate) {
			if (attribute->val->type() == ValueType::IntValue) {
				//printf("ATTR VAL: %d\n", attribute->val->iVal);
				if (attribute->val->iVal > (*colItr)->val->iVal) {
					(*colItr)->val->iVal = attribute->val->iVal;
				}
			}
			else if (attribute->val->type() == ValueType::FloatValue) {
				if (attribute->val->fVal > (*colItr)->val->fVal) {
					(*colItr)->val->fVal = attribute->val->fVal;
				}
			}
		}
	}
}

//update min value
void FLOPPYRecordSet::updateMin(FLOPPYRecord *record, FLOPPYSelectItem *aggregate, FLOPPYRecordAttribute *attribute) {
	for (auto colItr = record->columns->begin(); colItr != record->columns->end(); colItr++) {
		if ((*colItr)->isAggregate && (*colItr)->op == FLOPPYAggregateOperator::MaxAggregate) {
			if (attribute->val->type() == ValueType::IntValue) {
				if (attribute->val->iVal < (*colItr)->val->iVal) {
					(*colItr)->val->iVal = attribute->val->iVal;
				}
			}
			else if (attribute->val->type() == ValueType::FloatValue) {
				if (attribute->val->fVal < (*colItr)->val->fVal) {
					(*colItr)->val->fVal = attribute->val->fVal;
				}
			}
		}
	}
}

//update sum value
void FLOPPYRecordSet::updateSum(FLOPPYRecord *record, FLOPPYSelectItem *aggregate, FLOPPYRecordAttribute *attribute) {
	for (auto colItr = record->columns->begin(); colItr != record->columns->end(); colItr++) {
		if ((*colItr)->isAggregate && (*colItr)->op == FLOPPYAggregateOperator::SumAggregate) {
			if (attribute->val->type() == ValueType::IntValue) {
				(*colItr)->val->iVal += attribute->val->iVal;
			}
			else if (attribute->val->type() == ValueType::FloatValue) {
				(*colItr)->val->fVal += attribute->val->fVal;
			}
		}
	}
}

void FLOPPYRecordSet::countAggregateColumns(FLOPPYRecord *record, std::list<FLOPPYRecord *> *newRecords, std::vector<FLOPPYTableAttribute *> *groupByAttributes, std::vector<FLOPPYSelectItem *> *aggregates) {
	for (auto aggItr = aggregates->begin(); aggItr != aggregates->end(); aggItr++) {
		for (auto recItr = newRecords->begin(); recItr != newRecords->end(); recItr++) {
			if ((*aggItr)->aggregate.op == FLOPPYAggregateOperator::CountStarAggregate) {
				unsigned int numGroupEqual = 0;
				
				for (auto groupingItr = groupByAttributes->begin(); groupingItr != groupByAttributes->end(); groupingItr++) {
					if (FLOPPYRecord::compare(record, *recItr, *groupingItr) == 0) {
						numGroupEqual++;
					}	
				}
				
				if (numGroupEqual == groupByAttributes->size()) {
					incrementCountStar(*recItr);
				}
			}
			//handle all other aggregations
			else {
				//go through all the columns in this record and find the column represented by the aggregation
				for (auto colItr = (*recItr)->columns->begin(); colItr != (*recItr)->columns->end(); colItr++) {
					//get the common column for this aggregation
					if (strcmp((*aggItr)->aggregate.value->sVal, (*colItr)->name) == 0) {
						if ((*aggItr)->aggregate.op == FLOPPYAggregateOperator::AverageAggregate) {
							printf("AVG()\n");
						}
						else if ((*aggItr)->aggregate.op == FLOPPYAggregateOperator::MaxAggregate) {
							//check if current column value is greater than the MAX value
							updateMax(*recItr, *aggItr, *colItr);
						}
						else if ((*aggItr)->aggregate.op == FLOPPYAggregateOperator::MinAggregate) {
							updateMin(*recItr, *aggItr, *colItr);
						}
						else if ((*aggItr)->aggregate.op == FLOPPYAggregateOperator::SumAggregate) {
							updateSum(*recItr, *aggItr, *colItr);
						}
						else {
							printf("ERROR - Unsupported aggregation type!\n");
						}
						
// 						printf("COL VAL: %d\n", (*colItr)->val->iVal);
					}
				}
			}
		}
	}
}

void FLOPPYRecordSet::groupBy(std::vector<FLOPPYTableAttribute *> *groupByAttributes, std::vector<FLOPPYSelectItem *> *aggregates) {
	std::list<FLOPPYRecord *> *newRecords = new std::list<FLOPPYRecord *>();
	
	auto recordItr = records->begin();
	while (recordItr != records->end()) {
		if (shouldBeAddedToGrouping(*recordItr, newRecords, groupByAttributes)) {
			//add initialized aggregation columns
			FLOPPYRecord *newRecord = new FLOPPYRecord();
			
			addColumns(*recordItr, &newRecord);
			initializeAggregations(*recordItr, &newRecord, aggregates);
			
			//push to new records
			newRecords->push_back(newRecord);
		}
		
		countAggregateColumns(*recordItr, newRecords, groupByAttributes, aggregates);
		
		recordItr = records->erase(recordItr);
	}
	
	pruneColumns(newRecords, groupByAttributes, aggregates);
	
	records->swap(*newRecords);
	
	delete newRecords;
	
	
	
    // go through each record on the table
    // auto recordItr = records->begin();
// 	while (recordItr != records->end()) {
// 		FLOPPYRecord *tempAggRecord = NULL;
// 		FLOPPYRecord *tempCurRecord = *recordItr;
// 		
//     	for (auto newRecordItr = newRecords->begin(); newRecordItr != newRecords->end(); newRecordItr++) {
//     		int cmp = 0;
//         	
//             for (auto groupByItr = groupByAttributes->begin(); groupByItr != groupByAttributes->end(); groupByItr++) {
//         		cmp = FLOPPYRecord::compare(tempCurRecord, *newRecordItr, *groupByItr);
//         		if (cmp != 0)
//         			break;
//         	}
//         	
//         	if (cmp == 0) {
//         		tempAggRecord = *newRecordItr;
//         	}
//         	
//     	}
//     	
//         grouped record does not exist so we need to create it
//     	if (!tempAggRecord) {
//     		tempAggRecord = new FLOPPYRecord();
//     		
//             go through each group by attribute and see if it's in the columns and add it to the new record
//             for (auto groupByItr2 = groupByAttributes->begin(); groupByItr2 != groupByAttributes->end(); groupByItr2++) {
//                 auto attributeItr = tempCurRecord->columns->begin();
//                 
//                 while (attributeItr != tempCurRecord->columns->end()) {
//                     FLOPPYRecordAttribute *tempAttr = *attributeItr;
//                     
//                     if ((*groupByItr2)->tableName) {
//                         if (strcmp((*attributeItr)->tableName, (*groupByItr2)->tableName) != 0) {
//                             attributeItr++;
//                             continue;
//                         }
//                     }
//                     
//                     if (strcmp((*attributeItr)->name, (*groupByItr2)->attribute) != 0) {
//                         attributeItr++;
//                         continue;
//                     }
//                     
//                     create FLOPPYTableAttribute column
//                     FLOPPYRecordAttribute *aggAttr = new FLOPPYRecordAttribute();
//                     
//                     if(tempAttr->tableName) {
//                         aggAttr->tableName = (char*)calloc(strlen(tempAttr->tableName) + 1, sizeof(char));
//                         strcpy(aggAttr->tableName, tempAttr->tableName);
//                     }
//                     
//                     aggAttr->name = (char*)calloc(strlen(tempAttr->name) + 1, sizeof(char));
//                     strcpy(aggAttr->name, tempAttr->name);
//                     
//                     
//                     if(tempAttr->val->type() == ValueType::StringValue) {
//                         aggAttr->val = new FLOPPYValue(StringValue);
//                         aggAttr->val->sVal = (char*)calloc(sizeof(char), strlen(tempAttr->val->sVal) + 1);
//                         strcpy(aggAttr->val->sVal, tempAttr->val->sVal);
//                     }
//                     else if(tempAttr->val->type() == ValueType::IntValue) {
//                         aggAttr->val = new FLOPPYValue(IntValue);
//                         aggAttr->val->iVal = tempAttr->val->iVal;
//                     }
//                     else if(tempAttr->val->type() == ValueType::FloatValue) {
//                         aggAttr->val = new FLOPPYValue(FloatValue);
//                         aggAttr->val->fVal = tempAttr->val->fVal;
//                     }
//                     else if(tempAttr->val->type() == ValueType::BooleanValue) {
//                         aggAttr->val = new FLOPPYValue(BooleanValue);
//                         aggAttr->val->bVal = tempAttr->val->bVal;
//                     }
//                     else if(tempAttr->val->type() == ValueType::NullValue) {
//                         aggAttr->val = new FLOPPYValue(NullValue);
//                     }
//                     
//                     tempAggRecord->columns->push_back(aggAttr);
//                     break;
//                 }
//             }
//     		
//     		add aggregation columns...if exists
//     		for (auto aggItr = aggregates->begin(); aggItr != aggregates->end(); aggItr++) {
//     			create FLOPPYTableAttribute column
//     			FLOPPYRecordAttribute *aggAttr = new FLOPPYRecordAttribute();
//     			
//     			aggAttr->isAggregate = true;
//     			aggAttr->op = (*aggItr)->aggregate.op;
//     			
//     			check for star type
//     			if ((*aggItr)->aggregate.op == FLOPPYAggregateOperator::CountStarAggregate) {
//     				aggAttr->val = new FLOPPYValue(IntValue);
//     				aggAttr->val->iVal = 0;
//     			}
//     			else {
//     				aggAttr->name = (char*)calloc(strlen((*aggItr)->aggregate.value->sVal) + 1, sizeof(char));
//     				strcpy(aggAttr->name, (*aggItr)->aggregate.value->sVal);
//     			
//     				go through all attributes in tempCurRecord to find type of attribute and create FLOPPY value of that attribute type
//     				for (auto tempRecItr = tempCurRecord->columns->begin(); tempRecItr != tempCurRecord->columns->end(); tempRecItr++) {
//                         
//                         check column
//     					if (strcmp((*tempRecItr)->name, (*aggItr)->aggregate.value->sVal) != 0)
//     						continue;
//                         
//                         aggregated column is of type INT
//     					if ((*tempRecItr)->val->type() == ValueType::IntValue) {
//                             aggAttr->val = new FLOPPYValue(IntValue);
//     						aggAttr->val->iVal = 0;
//     					}
//     					aggregated column is of type FLOAT
//     					else if ((*tempRecItr)->val->type() == ValueType::FloatValue) {
//                             aggAttr->val = new FLOPPYValue(FloatValue);
//     						aggAttr->val->fVal = 0.0;
//     					}
//     					else {
//     						printf("ERROR - Cannot aggregate on that column type\n");
//     					}
//     					
//     					break;
//     				}
//     			}
//     			
//     			tempAggRecord->columns->push_back(aggAttr);
//     		}
//             
//             add aggregation "set to zero"
//             newRecords->push_back(tempAggRecord);
//         }
//         
//         tempAggRecord
//         
//         do the math for each aggregation
//         for (auto tempRecItr = tempAggRecord->columns->begin(); tempRecItr != tempAggRecord->columns->end(); tempRecItr++) {
//             FLOPPYRecordAttribute *col = *tempRecItr;
//             
//             skip non-aggregates
//             if(!col->isAggregate)
//                 continue;
//             
//             if (col->op == FLOPPYAggregateOperator::CountStarAggregate) {
//                 col->val->iVal++;
//             }
//             else {
//                 
//                 loop through cols
//                 for (auto dataColItr = tempCurRecord->columns->begin(); dataColItr != tempCurRecord->columns->end(); dataColItr++) {
//                     FLOPPYRecordAttribute *dataCol = *dataColItr;
//                     
//                     check if name matches
//                     if (strcmp(col->name, col->name) != 0)
//                         continue;
//                     
//                     
//                     
//                     if(col->op == FLOPPYAggregateOperator::CountAggregate)
//                         printf("COUNT(%s) (", col->name);
//                     else if(col->op == FLOPPYAggregateOperator::AverageAggregate)
//                         printf("AVG(%s) (", col->name);
//                     else if(col->op == FLOPPYAggregateOperator::MinAggregate)
//                         printf("MIN(%s) (", col->name);
//                     else if(col->op == FLOPPYAggregateOperator::MaxAggregate)
//                         printf("MAX(%s) (", col->name);
//                     else if(col->op == FLOPPYAggregateOperator::SumAggregate) {
//                         if(col->val->type() == ValueType::IntValue)
//                             col->val->iVal += dataCol->val->iVal;
//                         else if(col->val->type() == ValueType::FloatValue)
//                             col->val->fVal += dataCol->val->fVal;
//                         else
//                             printf("error type SUM(%s)\n", col->name);;
//                     }
//                     
//                             
//                             
//                             /*AttributeValue,
//                             TableAttributeValue,
//                             StringValue,
//                             IntValue,
//                             FloatValue,
//                             BooleanValue,
//                             NullValue
//                              */
//                     }
//             }
//         
//             
//             printf("\n");
//             break;
//         }
//         
//         delete each record from old records
//         recordItr = records->erase(recordItr);
//         delete tempCurRecord;
//     }
// 	
// 	records->swap(*newRecords);
// 	
// 	delete newRecords;
}

void FLOPPYRecordSet::distinct() {
    std::list<FLOPPYRecord *> *newRecords = new std::list<FLOPPYRecord *>();
    
    //go through each record on the table
    auto recordItr = records->begin();
    while (recordItr != records->end()) {
        FLOPPYRecord *tempNewRecord = NULL;
        FLOPPYRecord *tempCurRecord = *recordItr;
        
        for (auto newRecordItr = newRecords->begin(); newRecordItr != newRecords->end(); newRecordItr++) {
            int cmp = 0;
            
            for (unsigned int i=0; i<tempCurRecord->columns->size(); i++) {
                FLOPPYRecordAttribute *colA = tempCurRecord->columns->at(i);
                FLOPPYRecordAttribute *colB = (*newRecordItr)->columns->at(i);
                
                cmp = FLOPPYRecordAttribute::compareValues(colA->val, colB->val);
                
                if (cmp != 0)
                    break;
            }
            
            if (cmp == 0)
                tempNewRecord = *newRecordItr;
        }
        
        //new record, lets add it
        if (!tempNewRecord) {
            recordItr = records->erase(recordItr);
            newRecords->push_back(tempCurRecord);
        }
        else {
            //delete each record from old records
            recordItr = records->erase(recordItr);
            delete tempCurRecord;
        }
    }
    
    records->swap(*newRecords);
    
    delete newRecords;
}

void FLOPPYRecordSet::print() {
    std::list<FLOPPYRecord *>::iterator itr = records->begin();
    
    while (itr != records->end()) {
        printf("  %02d-%02d: ", (*itr)->pageId, (*itr)->recordId);
        
        for (unsigned i=0; i<(*itr)->columns->size(); i++) {
            if(i>0)
                printf(", ");
            
            FLOPPYRecordAttribute *col = (*itr)->columns->at(i);
            
            if (col->isAggregate && col->op == FLOPPYAggregateOperator::CountStarAggregate) {
            	printf("COUNT(*) %ld", col->val->iVal);
            } 
            else {
            	if (col->isAggregate) {
            		if(col->op == FLOPPYAggregateOperator::CountAggregate)
                        printf("COUNT(%s) (", col->name);
            		else if(col->op == FLOPPYAggregateOperator::AverageAggregate)
            			printf("AVG(%s) (", col->name);
            		else if(col->op == FLOPPYAggregateOperator::MinAggregate)
            			printf("MIN(%s) (", col->name);
            		else if(col->op == FLOPPYAggregateOperator::MaxAggregate)
            			printf("MAX(%s) (", col->name);
					else if(col->op == FLOPPYAggregateOperator::SumAggregate)
            			printf("SUM(%s) (", col->name);
            	}
            	else {
            		if(col->tableName)
                		printf("%s.",col->tableName);
           			printf("%s (",col->name);
                }
                
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
        }
        
        itr++;
        
        printf("\n");
    }
}
