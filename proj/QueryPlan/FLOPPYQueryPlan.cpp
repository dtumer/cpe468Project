//FLOPPYQueryPlan.cpp
//

#include "FLOPPYQueryPlan.h"
#include "ProjectionNode.h"
#include "SelectionNode.h"
#include "GroupingNode.h"
#include "TableNode.h"

/**
* Create a FLOPPY query plan based on the statement 
*/
FLOPPYQueryPlan::FLOPPYQueryPlan(FLOPPYStatement *statement) {
    //FLOPPYResult *result; 
    ProjectionNode projectionTest;
    SelectionNode selectionTest;
    GroupingNode groupingTest;
    TableNode tableTest;
    switch (statement->type()) {
        case StatementType::ErrorStatement:
            break;
        case StatementType::CreateTableStatement:
            //result = createTable((FLOPPYCreateTableStatement*) statement);
            break;
        case StatementType::DropTableStatement:
            printf("DROP TABLE\n");
            printDropTableStatement((FLOPPYDropTableStatement*) statement);
            break;
        case StatementType::CreateIndexStatement:
            printf("CREATE INDEX\n");
            //printCreateIndexStatement((FLOPPYCreateIndexStatement*) statement);
            break;
        case StatementType::DropIndexStatement:
            printf("DROP INDEX\n");
            printDropIndexStatement((FLOPPYDropIndexStatement*) statement);
            break;
        case StatementType::InsertStatement:
            printf("INSERT\n");
            printInsertStatement((FLOPPYInsertStatement*) statement);
            break;
        case StatementType::DeleteStatement:
            printf("DELETE\n");
            printDeleteStatement((FLOPPYDeleteStatement*) statement);
            break;
        case StatementType::UpdateStatement:
            printf("UPDATE\n");
            printUpdateStatement((FLOPPYUpdateStatement*) statement);
            break;
        case StatementType::SelectStatement:
            printf("SELECTQueryPlan\n");
            //root = new SelectionNode();
            FLOPPYSelectStatement *theStatement = (FLOPPYSelectStatement *)statement;
            printSelectStatement(theStatement);
            printf("Testing Projection: \n");
            projectionTest.addToProjection("hello", "world");
            projectionTest.addToProjection("test", "ing");
            projectionTest.printColumns();

            printf("Testing Selection: ");
            if(theStatement->whereCondition) {
                selectionTest.setCondition(theStatement->whereCondition);
                selectionTest.printCondition();
            }

            printf("Testing Grouping: ");
            if(theStatement->groupBy) {
                groupingTest.setStatement(theStatement);
                groupingTest.getAggregates();
                groupingTest.printAggregates();
            }
            printf("Testing table node: ");
            tableTest.setTableName("testTable");
            tableTest.printTableName();
            break;
    }
}

//free all nodes in the tree, and any allocated instance variables
FLOPPYQueryPlan::~FLOPPYQueryPlan() {

}
