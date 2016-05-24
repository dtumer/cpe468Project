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
    printf("\t TableName: %s\n", statement->tableName.c_str());
    printf("\t volatileFlag: %d\n", statement->flags->volatileFlag);
    printf("\t indexOnlyFlag: %d\n", statement->flags->indexOnlyFlag);
    printf("\t splitFlag: %d\n", statement->flags->splitFlag);
    
    
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
    
}

int main(int argc, char *argv[]) {
    //const char* sql = "CREATE TABLE NodeStats (IPAddress VARCHAR(20), BlocksUploaded  INT,BlocksDownloaded INT,RequestsReceived INT,RequestsSent INT,RequestsServed INT,RequestsFailed INT,DistrosUploaded  INT,DistrosDownloaded INT,PRIMARY KEY (IPAddress));";
    const char* sql = "CREATE TABLE Availability VOLATILE (NodeId  VARCHAR(20), DistroId  INT, FileId  INT, BlockID  INT, TimeStamp DATETIME, PRIMARY KEY(FileId, BlockId, NodeId), FOREIGN KEY(NodeId) REFERENCES Nodes, FOREIGN KEY (DistroId) REFERENCES Distros, FOREIGN KEY (FileId) REFERENCES Files);";
    //,FOREIGN KEY(NodeId) REFERENCES NodeStats
    
    FLOPPYOutput *result = FLOPPYParser::parseFLOPPYString(sql);
    
    if (result->isValid) { // Use isValid flag to detect is parser parsed correctly.
        
        printf("Parsed successfully!\n");
        
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
    
    return 0;
}