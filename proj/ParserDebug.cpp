#include <stdlib.h>
#include <string>
#include <iostream>

#include "ParserDebug.h"

void printFLOPPYValue(FLOPPYValue *val) {
    if(val->type() == ValueType::AttributeValue)
        printf("ATTRIBUTE \"%s\"", val->sVal);
    else if(val->type() == ValueType::TableAttributeValue) {
        if(val->tableAttribute->tableName)
        	printf("TABLE ATTRIBUTE \"%s.%s\"", val->tableAttribute->tableName, val->tableAttribute->attribute);
        else
            printf("TABLE ATTRIBUTE \"%s\"", val->tableAttribute->attribute);
    }
    else if(val->type() == ValueType::StringValue)
        printf("STRING \"%s\"", val->sVal);
    else if(val->type() == ValueType::IntValue)
        printf("INT %ld", val->iVal);
    else if(val->type() == ValueType::FloatValue)
        printf("FLOAT %f", val->fVal);
    else if(val->type() == ValueType::BooleanValue)
        printf("BOOLEAN %d", val->bVal);
    else if(val->type() == ValueType::NullValue)
        printf("NULL");
}

void printFLOPPYAggregate(FLOPPYAggregateOperator op, FLOPPYValue *value) {
    if(op == FLOPPYAggregateOperator::CountAggregate) {
        printf("COUNT (");
        printFLOPPYValue(value);
    }
    else if(op == FLOPPYAggregateOperator::CountStarAggregate) {
        printf("COUNT (*");
    }
    else if(op == FLOPPYAggregateOperator::CountAggregate) {
        printf("COUNT (");
        printFLOPPYValue(value);
    }
    else if(op == FLOPPYAggregateOperator::AverageAggregate) {
        printf("AVG (");
        printFLOPPYValue(value);
    }
    else if(op == FLOPPYAggregateOperator::MinAggregate) {
        printf("MIN (");
        printFLOPPYValue(value);
    }
    else if(op == FLOPPYAggregateOperator::MaxAggregate) {
        printf("MAX (");
        printFLOPPYValue(value);
    }
    else if(op == FLOPPYAggregateOperator::SumAggregate) {
        printf("SUM (");
        printFLOPPYValue(value);
    }
    printf(")");
}

void printFLOPPYNode(int tabLvl, FLOPPYNode *node) {
    if(node->_type == FLOPPYNodeType::ConditionNode || node->_type == FLOPPYNodeType::ExpressionNode) {
        if(node->_type == FLOPPYNodeType::ConditionNode)
            printf("\n%sCondition (", std::string(tabLvl, '\t').c_str());
        else
            printf("\n%sExpression (", std::string(tabLvl, '\t').c_str());
        
        if(node->node.op == FLOPPYNodeOperator::AndOperator) {
            printFLOPPYNode(tabLvl+1, node->node.left);
            printf("\n%s&& ", std::string(tabLvl+1, '\t').c_str());
            printFLOPPYNode(tabLvl+1, node->node.right);
            printf("\n%s", std::string(tabLvl, '\t').c_str());
        }
        else if(node->node.op == FLOPPYNodeOperator::NotOperator) {
            printf("NOT ");
            printFLOPPYNode(tabLvl+1, node->node.left);
            printf("\n%s", std::string(tabLvl, '\t').c_str());
        }
        else if(node->node.op == FLOPPYNodeOperator::GreaterThanOperator) {
            printFLOPPYNode(tabLvl+1, node->node.left);
            printf(" > ");
            printFLOPPYNode(tabLvl+1, node->node.right);
        }
        else if(node->node.op == FLOPPYNodeOperator::GreaterThanEqualOperator) {
            printFLOPPYNode(tabLvl+1, node->node.left);
            printf(" >= ");
            printFLOPPYNode(tabLvl+1, node->node.right);
        }
        else if(node->node.op == FLOPPYNodeOperator::LessThanOperator) {
            printFLOPPYNode(tabLvl+1, node->node.left);
            printf(" < ");
            printFLOPPYNode(tabLvl+1, node->node.right);
        }
        else if(node->node.op == FLOPPYNodeOperator::LessThanEqualOperator) {
            printFLOPPYNode(tabLvl+1, node->node.left);
            printf(" <= ");
            printFLOPPYNode(tabLvl+1, node->node.right);
        }
        else if(node->node.op == FLOPPYNodeOperator::EqualOperator) {
            printFLOPPYNode(tabLvl+1, node->node.left);
            printf(" = ");
            printFLOPPYNode(tabLvl+1, node->node.right);
        }
        else if(node->node.op == FLOPPYNodeOperator::NotEqualOperator) {
            printFLOPPYNode(tabLvl+1, node->node.left);
            printf(" != ");
            printFLOPPYNode(tabLvl+1, node->node.right);
        }
        else if(node->node.op == FLOPPYNodeOperator::ParenthesisOperator) {
            printf("Parenthesis ");
            printFLOPPYNode(tabLvl+1, node->node.left);
            printf("\n%s", std::string(tabLvl, '\t').c_str());
        }
        
        printf(")");
    }
    else if(node->_type == FLOPPYNodeType::AggregateNode) {
        printf("\n%sAggregate ", std::string(tabLvl, '\t').c_str());
        printFLOPPYAggregate(node->aggregate.op, node->aggregate.value);
    }
    else if(node->_type == FLOPPYNodeType::ValueNode) {
        printFLOPPYValue(node->value);
    }
}

void printCreateTableStatement(FLOPPYCreateTableStatement *statement) {
    printf("\tTableName: %s\n", statement->tableName);
    printf("\tvolatileFlag: %d\n", statement->flags->volatileFlag);
    printf("\tindexOnlyFlag: %d\n", statement->flags->indexOnlyFlag);
    printf("\tsplitFlag: %d\n", statement->flags->splitFlag);
    
    
    printf("\t Columns:\n");
    for (unsigned i=0; i<statement->columns->size(); i++) {
        FLOPPYCreateColumn *col = statement->columns->at(i);
        if(col->type == ColumnType::INT)
            printf("\t\t%s INT\n", col->name);
        else if(col->type == ColumnType::FLOAT)
            printf("\t\t%s FLOAT\n", col->name);
        else if(col->type == ColumnType::DATETIME)
            printf("\t\t%s DATETIME\n", col->name);
        else if(col->type == ColumnType::VARCHAR)
            printf("\t\t%s VARCHAR(%d)\n", col->name, col->size);
        else if(col->type == ColumnType::BOOLEAN)
            printf("\t\t%s BOOLEAN\n", col->name);
    }
    
    printf("\t Primary Key(s):\n");
    for (unsigned i=0; i<statement->pk->attributes->size(); i++) {
        printf("\t\t%s\n", statement->pk->attributes->at(i));
    }
    
    printf("\t Foreign Key(s):\n");
    if(statement->fk) {
    	for (unsigned i=0; i<statement->fk->size(); i++) {
        	FLOPPYForeignKey *fk = statement->fk->at(i);
        	printf("\t\tReference Table: %s\n", fk->refTableName);
        	for (unsigned j=0; j<fk->attributes->size(); j++) {
        	    printf("\t\t\t%s\n", fk->attributes->at(j));
        	}
        }
        printf("\t\tNone\n");
    }
}

void printInsertStatement(FLOPPYInsertStatement *statement) {
    printf("\tTableName: %s\n", statement->name);
    printf("\tValues:\n");
    for (unsigned i=0; i<statement->values->size(); i++) {
        printf("\t\t");
        printFLOPPYValue(statement->values->at(i));
        printf("\n");
    }
}

void printDeleteStatement(FLOPPYDeleteStatement *statement) {
    printf("\tTableName: %s\n", statement->name);
    printf("\tWhere: ");
    printFLOPPYNode(2, statement->where);
    printf("\n");
}

void printUpdateStatement(FLOPPYUpdateStatement *statement) {
    printf("\tTableName: %s\n", statement->tableName);
    printf("\tAttribute: %s\n", statement->attributeName);
    printf("\tattributeExpression: ");
    printFLOPPYNode(2, statement->attributeExpression);
    printf("\n");
    printf("\tWhere: ");
    printFLOPPYNode(2, statement->whereExpression);
    printf("\n");
}

void printSelectStatement(FLOPPYSelectStatement *statement) {
    printf("\tDistinct: %d\n", statement->distinct);
    printf("\tSelect Items: ");
    for (unsigned i=0; i<statement->selectItems->size(); i++) {
        FLOPPYSelectItem *spec = statement->selectItems->at(i);
        if(spec->_type == FLOPPYSelectItemType::AttributeType) {
            printf("\n\t\t%s", spec->attribute);
        }
        else if(spec->_type == FLOPPYSelectItemType::TableAttributeType) {
            if(spec->tableAttribute->tableName)
            	printf("\n\t\t%s.%s", spec->tableAttribute->tableName, spec->tableAttribute->attribute);
            else
                printf("\n\t\t%s", spec->tableAttribute->attribute);
        }
        else if(spec->_type == FLOPPYSelectItemType::AggregateType) {
            printf("\n\t\tAggregate ");
            printFLOPPYAggregate(spec->aggregate.op, spec->aggregate.value);
        }
        else if(spec->_type == FLOPPYSelectItemType::StarType) {
            printf("*");
        }
    }
    printf("\n");
    
    printf("\tTable Specs:\n");
    for (unsigned i=0; i<statement->tableSpecs->size(); i++) {
        FLOPPYTableSpec *spec = statement->tableSpecs->at(i);
        if(spec->alias == NULL)
            printf("\t\t%s\n", spec->tableName);
        else
            printf("\t\t%s AS %s\n", spec->tableName, spec->alias);
    }
    
    if(statement->whereCondition) {
        printf("\tWHERE: ");
        printFLOPPYNode(2, statement->whereCondition);
        printf("\n");
    }
    
    if(statement->groupBy) {
        printf("\tGROUP BY:\n");
        for (unsigned i=0; i<statement->groupBy->groupByAttributes->size(); i++) {
            FLOPPYTableAttribute *tableAttribute = statement->groupBy->groupByAttributes->at(i);
            if(tableAttribute->tableName)
            	printf("\t\t%s.%s\n", tableAttribute->tableName, tableAttribute->attribute);
            else
                printf("\t\t%s\n", tableAttribute->attribute);
        }
        
        if(statement->groupBy->havingCondition) {
            printf("\tHAVING: ");
            printFLOPPYNode(2, statement->groupBy->havingCondition);
            printf("\n");
        }
    }
    
    if(statement->orderBys) {
    	printf("\tORDER BY:\n");
    	for (unsigned i=0; i<statement->orderBys->size(); i++) {
    	    FLOPPYTableAttribute *tableAttribute = statement->orderBys->at(i);
    	    if(tableAttribute->tableName)
    	        printf("\t\t%s.%s\n", tableAttribute->tableName, tableAttribute->attribute);
    	    else
    	        printf("\t\t%s\n", tableAttribute->attribute);
    	}
    }
    
    if(statement->limit > 0) {
        printf("\tLIMIT: %d\n", statement->limit);
    }
}

void printDropIndexStatement(FLOPPYDropIndexStatement *statement) {
    printf("\tIndex Name: %s\n", statement->indexName);
    printf("\tTable Name: %s\n", statement->tableName);
}

void printDropTableStatement(FLOPPYDropTableStatement *statement) {
    printf("\tTable Name: %s\n", statement->table);
}

void TestSQLStatement(const char* sql) {
    FLOPPYOutput *result = FLOPPYParser::parseFLOPPYString(sql);
    
    if (result->isValid) { // Use isValid flag to detect is parser parsed correctly.
        
        switch (result->statement->type()) {
            case StatementType::ErrorStatement:
                break;
            case StatementType::CreateTableStatement:
                printf("CREATE TABLE\n");
                printCreateTableStatement((FLOPPYCreateTableStatement*) result->statement);
                break;
            case StatementType::DropTableStatement:
                printf("DROP TABLE\n");
                printDropTableStatement((FLOPPYDropTableStatement*) result->statement);
                break;
            case StatementType::CreateIndexStatement:
                printf("CREATE INDEX\n");
                //printCreateIndexStatement((FLOPPYCreateIndexStatement*) result->statement);
                break;
            case StatementType::DropIndexStatement:
                printf("DROP INDEX\n");
                printDropIndexStatement((FLOPPYDropIndexStatement*) result->statement);
                break;
            case StatementType::InsertStatement:
                printf("INSERT\n");
                printInsertStatement((FLOPPYInsertStatement*) result->statement);
                break;
            case StatementType::DeleteStatement:
                printf("DELETE\n");
                printDeleteStatement((FLOPPYDeleteStatement*) result->statement);
                break;
            case StatementType::UpdateStatement:
                printf("UPDATE\n");
                printUpdateStatement((FLOPPYUpdateStatement*) result->statement);
                break;
            case StatementType::SelectStatement:
                printf("SELECT\n");
                printSelectStatement((FLOPPYSelectStatement*) result->statement);
                break;
        }
        
        
    } else {
        printf("Invalid FLOPPY!\n");
    }
    delete result;
}
