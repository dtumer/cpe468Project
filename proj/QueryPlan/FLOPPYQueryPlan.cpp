//FLOPPYQueryPlan.cpp
//

#include "FLOPPYQueryPlan.h"
#include "ProjectionNode.h"
#include "SelectionNode.h"
#include "GroupingNode.h"
#include "TableNode.h"
#include "CrossProductNode.h"
#include "DeduplicateNode.h"
#include "LimitNode.h"
#include "SortNode.h"

/**
* Create a FLOPPY query plan based on the statement 
*/
FLOPPYQueryPlan::FLOPPYQueryPlan(FLOPPYStatement *statement) {
    //FLOPPYResult *result; 
    ProjectionNode projectionTest;
    SelectionNode selectionTest;
    GroupingNode groupingTest;
    TableNode tableTest;
    LimitNode limitTest;
    SortNode sortTest;
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
            projectionTest.setItems(theStatement->selectItems);

            printf("Testing Selection: \n");
            if(theStatement->whereCondition) {
                selectionTest.setCondition(theStatement->whereCondition);
                selectionTest.printCondition();
            }

            printf("Testing Grouping: \n");
            if(theStatement->groupBy) {
                groupingTest.setStatement(theStatement);
                groupingTest.loadAggregates();
                groupingTest.getAggregates();
                groupingTest.printAggregates();
                groupingTest.printGroupByAttributes();
            }
            printf("Testing table node: \n");
            tableTest.setTableName((char*)"testTable");
            tableTest.printTableName();

            printf("Testing Limit Node: \n");
            if(theStatement->limit > 0) {
                limitTest.limit = theStatement->limit;
                printf("the limit is: %d\n", limitTest.limit);
            }

            printf("Testing Sort Node: \n");
            if(theStatement->orderBys) {
                sortTest.orderBys = theStatement->orderBys;
                printf("%s\n", sortTest.orderBys->at(0)->attribute);
            }

            FLOPPYQueryPlanNode *join;
            std::vector<FLOPPYTableSpec *> theTableSpecs = *(theStatement->tableSpecs);
            if(theTableSpecs.size() > 1) {
                join = createJoinTree(theStatement->tableSpecs);
                printCrossNode(join);
            }
            break;
    }
}

/*Create a right-deep tree of table nodes to represent the joins. 
Right-deep tree means that the left child of a cross product node is always going to be a
Table node; i.e. createJoinTree( [r1, r2, r3, r4] ) would produce this tree:
       X
      / \
    r1   X
        / \
        r2 X
          / \
        r3   r4
        
        The return value is a pointer to the root of the join-tree, which can be inserted
        into the select-tree. */ 
FLOPPYQueryPlanNode * FLOPPYQueryPlan::createJoinTree(std::vector<FLOPPYTableSpec *> *tableSpecs) {
    std::vector<FLOPPYTableSpec *> theTableSpecs = *tableSpecs;

    //lets reverse the vector
    std::reverse(theTableSpecs.begin(), theTableSpecs.end());

    //set up bottom most node with two children
    //since we know size >= 2
    CrossProductNode *node = new CrossProductNode();
    TableNode *leftTable = new TableNode();
    TableNode *rightTable = new TableNode();
    
    //Get first table spec for right most node
    FLOPPYTableSpec *tableSpec = theTableSpecs.at(0);

    //setup right most node
    //printf("table:name: ");
    //printf(tableSpec->tableName);
    //printf("\n");
    rightTable->setTableName(tableSpec->tableName);
    rightTable->setAlias(tableSpec->alias);
    rightTable->setOp(TABLE);

    //Get left sibling spec
    tableSpec = theTableSpecs.at(1);

    //setup left sibling
    leftTable->setTableName(tableSpec->tableName);
    leftTable->setAlias(tableSpec->alias);
    leftTable->setOp(TABLE);

    //actually setup the cross product node
    node->setOp(CROSS);
    node->setLeftChild(leftTable);
    node->setRightChild(rightTable);

    CrossProductNode *newCross;
    TableNode *newTable;
    for(unsigned int i = 2; i < theTableSpecs.size(); i++) { 
        tableSpec = theTableSpecs.at(i);
        printf(tableSpec->tableName);
        printf("\n");

        //Create new table node
        newTable = new TableNode();
        newTable->setOp(TABLE);
        newTable->setTableName(tableSpec->tableName);
        newTable->setAlias(tableSpec->alias);

        newCross = new CrossProductNode();
        newCross->setOp(CROSS);
        newCross->setLeftChild(newTable);
        
        //set right child to our previous node
        newCross->setRightChild(node);

        //update old node poiner
        node = newCross;
    }
    return node;
}

/**
 * Prints a given cross product node out allowing you to see the
 * different levels of the tree.
 */
void FLOPPYQueryPlan::printCrossNode(FLOPPYQueryPlanNode *node) {
    int i = 0;
   
    TableNode *tNode;
    while(node->rightChild->op != TABLE) {
        printf("%d leftTable: ", i);
        tNode = (TableNode*)node->leftChild;
        printf(tNode->getTableName());
        printf("\n");
        node = node->rightChild;
        i++;
    }
    printf("lastLeftTable: ");
    tNode = (TableNode*)node->leftChild;
    printf(tNode->getTableName());
    printf("\n");

    printf("lastRightTable: ");
    tNode = (TableNode*)node->rightChild;
    printf(tNode->getTableName());
    printf("\n");
}

/* Create the select statement tree.
   If DISTINCT, add a deduplicate at the root. Otherwise, the tree is: 
   Projection [select-set] ( Selection [where condition] (Join tree))
   Remember to free the current tree if it exists.
   */
void createSelectStatementTree(FLOPPYSelectStatement *statement) {
    if(statement->distinct) {
        DeduplicateNode *node = new DeduplicateNode();
    } else {
        ProjectionNode *node = new ProjectionNode();
    }

    for (unsigned i=0; i<statement->selectItems->size(); i++) {
        FLOPPYSelectItem *spec = statement->selectItems->at(i);
        if(spec->_type == FLOPPYSelectItemType::StarType) {
            //printf("\n\t\t%s", spec->attribute);
        }
        else if(spec->_type == FLOPPYSelectItemType::TableAttributeType) {
            //printf("\n\t\t%s", spec->tableAttribute->attribute);
        }
    }
}
//free all nodes in the tree, and any allocated instance variables
FLOPPYQueryPlan::~FLOPPYQueryPlan() {

}
