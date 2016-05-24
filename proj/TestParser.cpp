//
//  TestParser.c
//  proj
//
//  Created by David Ellison on 5/3/16.
//
//

#include <stdlib.h>
#include <string>
#include <iostream>


#include "libs/FLOPPY/FLOPPYParser.h"


void printFLOPPYValue(FLOPPYValue *val) {
    if(val->type() == ValueType::AttributeValue)
        printf("ATTRIBUTE \"%s\"", val->sVal);
    else if(val->type() == ValueType::StringValue)
        printf("STRING \"%s\"", val->sVal);
    else if(val->type() == ValueType::IntValue)
        printf("INT %d", val->iVal);
    else if(val->type() == ValueType::FloatValue)
        printf("FLOAT %f", val->fVal);
    else if(val->type() == ValueType::BooleanValue)
        printf("BOOLEAN %d", val->bVal);
    else if(val->type() == ValueType::NullValue)
        printf("NULL");
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
            printf("NOT ", std::string(tabLvl, '\t').c_str());
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
        
        if(node->aggregate.op == FLOPPYAggregateOperator::CountAggregate) {
            printf("COUNT (", std::string(tabLvl, '\t').c_str());
            printFLOPPYValue(node->aggregate.value);
        }
        else if(node->aggregate.op == FLOPPYAggregateOperator::CountStarAggregate) {
            printf("COUNT (*", std::string(tabLvl, '\t').c_str());
        }
        else if(node->aggregate.op == FLOPPYAggregateOperator::CountAggregate) {
            printf("COUNT (", std::string(tabLvl, '\t').c_str());
            printFLOPPYValue(node->aggregate.value);
        }
        else if(node->aggregate.op == FLOPPYAggregateOperator::AverageAggregate) {
            printf("AVG (", std::string(tabLvl, '\t').c_str());
            printFLOPPYValue(node->aggregate.value);
        }
        else if(node->aggregate.op == FLOPPYAggregateOperator::MinAggregate) {
            printf("MIN (", std::string(tabLvl, '\t').c_str());
            printFLOPPYValue(node->aggregate.value);
        }
        else if(node->aggregate.op == FLOPPYAggregateOperator::MaxAggregate) {
            printf("MAX (", std::string(tabLvl, '\t').c_str());
            printFLOPPYValue(node->aggregate.value);
        }
        else if(node->aggregate.op == FLOPPYAggregateOperator::SumAggregate) {
            printf("SUM (", std::string(tabLvl, '\t').c_str());
            printFLOPPYValue(node->aggregate.value);
        }
        
        printf("\n%s)", std::string(tabLvl, '\t').c_str());
    }
    else if(node->_type == FLOPPYNodeType::ValueNode) {
        printFLOPPYValue(node->value);
    }
}

void printCreateTableStatement(FLOPPYCreateTableStatement* statement) {
    printf("\tTableName: %s\n", statement->tableName.c_str());
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
    for (unsigned i=0; i<statement->fk->size(); i++) {
        FLOPPYForeignKey *fk = statement->fk->at(i);
        printf("\t\tReference Table: %s\n", fk->refTableName);
        for (unsigned j=0; j<fk->attributes->size(); j++) {
            printf("\t\t\t%s\n", fk->attributes->at(j));
        }
    }
}

void printInsertStatement(FLOPPYInsertStatement* statement) {
    printf("\tTableName: %s\n", statement->name);
    printf("\tValues:\n");
    for (unsigned i=0; i<statement->values->size(); i++) {
        printf("\t\t");
        printFLOPPYValue(statement->values->at(i));
        printf("\n");
    }
}

void printDeleteStatement(FLOPPYDeleteStatement* statement) {
    printf("\tTableName: %s\n", statement->name);
    printf("\tWhere ");
    printFLOPPYNode(2, statement->where);
    printf("\n");
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
            case StatementType::CreateIndexStatement:
            case StatementType::DropIndexStatement:
            case StatementType::InsertStatement:
                printf("INSERT\n");
                printInsertStatement((FLOPPYInsertStatement*) result->statement);
                break;
            case StatementType::DeleteStatement:
                printf("DELETE\n");
                printDeleteStatement((FLOPPYDeleteStatement*) result->statement);
                break;
            case StatementType::UpdateStatement:
            case StatementType::SelectStatement:
                break;
        }
        
        
    } else {
        printf("Invalid FLOPPY!\n");
    }
}

int main(int argc, char *argv[]) {
    //TestSQLStatement("CREATE TABLE NodeStats (IPAddress VARCHAR(20), BlocksUploaded  INT,BlocksDownloaded INT,RequestsReceived INT,RequestsSent INT,RequestsServed INT,RequestsFailed INT,DistrosUploaded  INT,DistrosDownloaded INT,PRIMARY KEY (IPAddress));");
    
    //TestSQLStatement("CREATE TABLE Availability VOLATILE (NodeId  VARCHAR(20), DistroId  INT, FileId  INT, BlockID  INT, TimeStamp DATETIME, PRIMARY KEY(FileId, BlockId, NodeId), FOREIGN KEY(NodeId) REFERENCES Nodes, FOREIGN KEY (DistroId) REFERENCES Distros, FOREIGN KEY (FileId) REFERENCES Files);");
    //,FOREIGN KEY(NodeId) REFERENCES NodeStats
    
    TestSQLStatement("INSERT INTO Availability VALUES ('test', 2, 10, 20);");
    TestSQLStatement("DELETE FROM Availability WHERE NodeId>1 AND DistroId<5;");
    TestSQLStatement("DELETE FROM Availability WHERE NodeId>1 AND NOT(DistroId<5);");
    TestSQLStatement("DELETE FROM Availability WHERE NodeId=1 AND NOT((DistroId / 10) > 5);");
    
    
    
    return 0;
}