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
        FLOPPYValue *val = statement->values->at(i);
        
        if(val->type() == ValueType::AttributeValue)
            printf("\t\tATTRIBUTE \"%s\"\n", val->sVal);
        else if(val->type() == ValueType::StringValue)
            printf("\t\tSTRING \"%s\"\n", val->sVal);
        else if(val->type() == ValueType::IntValue)
            printf("\t\tINT %d\n", val->iVal);
        else if(val->type() == ValueType::FloatValue)
            printf("\t\tFLOAT %f\n", val->fVal);
        else if(val->type() == ValueType::BooleanValue)
            printf("\t\tBOOLEAN %d\n", val->bVal);
        else if(val->type() == ValueType::NullValue)
            printf("\t\tNULL\n");
    }
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
                printf("Insert Statement\n");
                printInsertStatement((FLOPPYInsertStatement*) result->statement);
                break;
                
            case StatementType::DeleteStatement:
            case StatementType::UpdateStatement:
            case StatementType::SelectStatement:
                break;
        }
        
        if (result->statement->type() == StatementType::DropTableStatement) {
            std::cout << "Found Drop Table Statement" << std::endl;
        }
        
    } else {
        printf("Invalid FLOPPY!\n");
    }
}

int main(int argc, char *argv[]) {
    //TestSQLStatement("CREATE TABLE NodeStats (IPAddress VARCHAR(20), BlocksUploaded  INT,BlocksDownloaded INT,RequestsReceived INT,RequestsSent INT,RequestsServed INT,RequestsFailed INT,DistrosUploaded  INT,DistrosDownloaded INT,PRIMARY KEY (IPAddress));");
    
    TestSQLStatement("CREATE TABLE Availability VOLATILE (NodeId  VARCHAR(20), DistroId  INT, FileId  INT, BlockID  INT, TimeStamp DATETIME, PRIMARY KEY(FileId, BlockId, NodeId), FOREIGN KEY(NodeId) REFERENCES Nodes, FOREIGN KEY (DistroId) REFERENCES Distros, FOREIGN KEY (FileId) REFERENCES Files);");
    //,FOREIGN KEY(NodeId) REFERENCES NodeStats
    
    TestSQLStatement("INSERT INTO Availability VALUES ('test', 2, 10, 20);");
    
    
    
    return 0;
}