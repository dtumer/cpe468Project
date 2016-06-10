#include "FLOPPY_DBMS.h"
#include "FLOPPYResult.h"

FLOPPY_DBMS::FLOPPY_DBMS (std::string diskName, int nPersistentBlocks, int nVolatileBlocks) {
    //create temp char*
    char *diskNameC = new char[diskName.length() + 1];
    strcpy(diskNameC, diskName.c_str());
    
    buf = new FLOPPYBufferManager(diskNameC, nPersistentBlocks, nVolatileBlocks);
    
    //free memory
    delete [] diskNameC;
}

FLOPPY_DBMS::~FLOPPY_DBMS() {
    delete buf;
}

FLOPPYResult* FLOPPY_DBMS::execute(std::string sql) {
    FLOPPYOutput *parsedCommand = FLOPPYParser::parseFLOPPYString(sql);
    FLOPPYResult *result;
    
    if (parsedCommand->isValid) { // Use isValid flag to detect is parser parsed correctly.
        
        switch (parsedCommand->statement->type()) {
            case StatementType::CreateTableStatement:
                result = createTable((FLOPPYCreateTableStatement*) parsedCommand->statement);
                break;
            case StatementType::DropTableStatement:
                result = dropTable((FLOPPYDropTableStatement*) parsedCommand->statement);
                break;
            case StatementType::InsertStatement:
                result = insertRecord((FLOPPYInsertStatement*) parsedCommand->statement);
                break;
            case StatementType::SelectStatement:
                result = selectRecords((FLOPPYSelectStatement*) parsedCommand->statement);
                break;
            case StatementType::DeleteStatement:
                printf("DELETE\n");
                result = deleteRecords((FLOPPYDeleteStatement*) parsedCommand->statement);
                printDeleteStatement((FLOPPYDeleteStatement*) parsedCommand->statement);
                break;
                
            /*
            case StatementType::UpdateStatement:
                printf("UPDATE\n");
                printUpdateStatement((FLOPPYUpdateStatement*) parsedCommand->statement);
                break;
            case StatementType::CreateIndexStatement:
                printf("CREATE INDEX\n");
                //printCreateIndexStatement((FLOPPYCreateIndexStatement*) parsedCommand->statement);
                break;
            case StatementType::DropIndexStatement:
                printf("DROP INDEX\n");
                printDropIndexStatement((FLOPPYDropIndexStatement*) parsedCommand->statement);
                break;
             case StatementType::ErrorStatement:
                break;
             */
            default:
                result = new FLOPPYResult(ErrorType);
                result->msg = (char*)calloc(sizeof(char), 50);
                sprintf(result->msg, "Unhandled FLOPPY SQL command");
                
        }
    } else {
        result = new FLOPPYResult(ErrorType);
        result->msg = (char*)calloc(sizeof(char), 50);
        sprintf(result->msg, "Unable to parse FLOPPY SQL");
    }
    
    delete parsedCommand;
    
    return result;
}

FLOPPYResult * FLOPPY_DBMS::createTable(FLOPPYCreateTableStatement *statement) {
    FLOPPYHeapFile *heap = FLOPPYHeapFile::createFile(buf, statement);
    
    FLOPPYResult *result = new FLOPPYResult(MessageType);
    result->msg = (char*)calloc(sizeof(char), 100);
    sprintf(result->msg, "Created the table `%s` successfully.", statement->tableName);
    
    delete heap;
    
    return result;
}


FLOPPYResult * FLOPPY_DBMS::insertRecord(FLOPPYInsertStatement *statement) {
    FLOPPYHeapFile *heap = new FLOPPYHeapFile(buf, statement->name);
    heap->insertStatement(statement);
    
    FLOPPYResult *result = new FLOPPYResult(MessageType);
    result->msg = (char*)calloc(sizeof(char), 100);
    sprintf(result->msg, "Inserted record successfully.");
    
    delete heap;
    
    return result;
}

/*
 * THIS NEEDS TO NOT JUST USE A HARD CODED TABLE
 */
FLOPPYResult * FLOPPY_DBMS::selectRecords(FLOPPYSelectStatement *statement) {
    std::vector<FLOPPYSelectItem *> *aggregations = getAggregations(statement);
    
    
    //Get tables and do cross products as needed
    FLOPPYRecordSet *recordSet = NULL, *tempRS, *newRS;
    for (auto itr = statement->tableSpecs->begin() ; itr != statement->tableSpecs->end(); itr++) {
        FLOPPYHeapFile *heap = new FLOPPYHeapFile(buf, (*itr)->tableName);
        
        //move previous recordSet to temp storage
        tempRS = recordSet;
        
        if((*itr)->alias)
            newRS = heap->getAllRecords((*itr)->alias);
        else
            newRS = heap->getAllRecords();
        
        //merge record sets if more than one
        if(tempRS) {
            recordSet = FLOPPYRecordSet::crossProduct(tempRS, newRS);
            
            delete tempRS;
            delete newRS;
        }
        else
            recordSet = newRS;
        
        delete heap;
    }
    
    //WHERE
    if(statement->whereCondition)
        recordSet->filter(statement->whereCondition);
    
    
    if (statement->groupBy) {
        //GROUP BY
    	if (statement->groupBy->groupByAttributes) {
            recordSet->groupBy(statement->groupBy->groupByAttributes, aggregations);
    	}
    	
        //HAVING
    	if (statement->groupBy->havingCondition) {
            recordSet->filter(statement->groupBy->havingCondition);
    	}
    }
    else {
        if(aggregations->size() == statement->selectItems->size()) {
            std::vector<FLOPPYTableAttribute *> *emptyGroupBy = new std::vector<FLOPPYTableAttribute *>();
        	recordSet->groupBy(emptyGroupBy, getAggregations(statement));
            delete emptyGroupBy;
        }
    }
    
    //ORDER BY
    if(statement->orderBys)
    	recordSet->sort(statement->orderBys);
    
    //Projection
    recordSet->projection(statement->selectItems);
    
    //DISTINCT
    if(statement->distinct)
        recordSet->distinct();
    
    //LIMIT
    if(statement->limit > 0)
        recordSet->limit(statement->limit);
    
    //get results
    FLOPPYResult *result = new FLOPPYResult(SelectType);
    result->recordSet = recordSet;
    
    delete aggregations;
    return result;
}

FLOPPYResult * FLOPPY_DBMS::dropTable(FLOPPYDropTableStatement *statement) {
    FLOPPYHeapFile *heap = new FLOPPYHeapFile(buf, statement->table);
    
    FLOPPYResult *result = new FLOPPYResult(MessageType);
    result->msg = (char*)calloc(sizeof(char), 100);
    sprintf(result->msg, "Dropped the table `%s` successfully.", statement->table);
    
    delete heap;
    
    return result;
}

FLOPPYResult * FLOPPY_DBMS::deleteRecords(FLOPPYDeleteStatement *statement) {
    FLOPPYHeapFile *heap = new FLOPPYHeapFile(buf, statement->name);
    FLOPPYRecordSet *recordSet = heap->getAllRecords();
    
    //WHERE
    recordSet->filter(statement->where);
    
    //do delete
    heap->deleteRecords(recordSet);
    
    FLOPPYResult *result = new FLOPPYResult(MessageType);
    result->msg = (char*)calloc(sizeof(char), 100);
    if(recordSet->count() == 1)
    	sprintf(result->msg, "Deleted 1 record successfully.");
    else
        sprintf(result->msg, "Deleted %d records successfully.", recordSet->count());
    
    delete heap;
    delete recordSet;
    return result;
}

std::vector<FLOPPYSelectItem *>* FLOPPY_DBMS::getAggregations(FLOPPYSelectStatement *statement) {
    std::vector<FLOPPYSelectItem *> *retAggs = new std::vector<FLOPPYSelectItem *>();
    
    //get aggregations from the projection part of the select statement
    for (auto itr = statement->selectItems->begin(); itr != statement->selectItems->end(); itr++) {
        if ((*itr)->_type == FLOPPYSelectItemType::AggregateType) {
            retAggs->push_back(*itr);
        }
    }
    
    //get the aggregations from the having clause of the selection statement if it's present
    if (statement->groupBy) {
    	if (statement->groupBy->havingCondition) {
    		std::vector<FLOPPYSelectItem *> *havingAggs = new std::vector<FLOPPYSelectItem *>();
    		
    		getAggregationsFromHaving(statement->groupBy->havingCondition, &havingAggs);
    		
    		//iterate through each having aggregation we want to add
    		auto havingAggsItr = havingAggs->begin();
    		
    		while (havingAggsItr != havingAggs->end()) {
    			bool isInList = false;
    			
    			//go through every aggregation we will return
    			for (auto retAggsItr = retAggs->begin(); retAggsItr != retAggs->end(); retAggsItr++) {
    				//if the operations are the same, check attribute name 
    				if ((*havingAggsItr)->aggregate.op == (*retAggsItr)->aggregate.op) {
    					//if not count star check attribute name
    					if ((*havingAggsItr)->aggregate.op != FLOPPYAggregateOperator::CountStarAggregate) {
    						//if same attribute names DONT ADD
    						if (strcmp((*havingAggsItr)->aggregate.value->sVal, (*retAggsItr)->aggregate.value->sVal) == 0) {
    							isInList = true;
    							break;
    						}
    					}
    					//if operator is count(*)
    					else {
    						isInList = true;
    						break;
    					}
    				}
    			}
    			
    			//if its not in the list add it
    			if (!isInList) {
    				retAggs->push_back(*havingAggsItr);
    			}
    			
    			havingAggsItr = havingAggs->erase(havingAggsItr);
    		}
    		
    		delete havingAggs;
    	}
    }
    
    return retAggs;
}

//gets the aggregations from the having clause
void FLOPPY_DBMS::getAggregationsFromHaving(FLOPPYNode *havingClause, std::vector<FLOPPYSelectItem *> **retAggregations) {
	if (havingClause->_type == FLOPPYNodeType::ConditionNode || havingClause->_type == FLOPPYNodeType::ExpressionNode) {
		if (havingClause->node.left) {
			getAggregationsFromHaving(havingClause->node.left, retAggregations);
		}
		
		if (havingClause->node.left) {
			getAggregationsFromHaving(havingClause->node.right, retAggregations);
		}
	}
	else if (havingClause->_type == FLOPPYNodeType::AggregateNode) {
		FLOPPYSelectItem *aggItem = new FLOPPYSelectItem(AttributeType);
		
		aggItem->aggregate.op = havingClause->aggregate.op;
		
		if (havingClause->aggregate.op != FLOPPYAggregateOperator::CountStarAggregate) {
			FLOPPYValue *val = new FLOPPYValue(AttributeValue);
			
			val->sVal = (char*)calloc(strlen(havingClause->aggregate.value->sVal) + 1,sizeof(char));
			strcpy(val->sVal, havingClause->aggregate.value->sVal);
			
			aggItem->aggregate.value = val;
		}
		
		(*retAggregations)->push_back(aggItem);
	}
}
