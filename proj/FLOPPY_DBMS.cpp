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

FLOPPYResult* FLOPPY_DBMS::execute(std::string sql) {
    FLOPPYOutput *parsedCommand = FLOPPYParser::parseFLOPPYString(sql.c_str());
    FLOPPYResult *result;
    
    if (parsedCommand->isValid) { // Use isValid flag to detect is parser parsed correctly.
        
        switch (parsedCommand->statement->type()) {
            case StatementType::ErrorStatement:
                break;
            case StatementType::CreateTableStatement:
                printf("CREATE TABLE\n");
                printCreateTableStatement((FLOPPYCreateTableStatement*) parsedCommand->statement);
                break;
            case StatementType::DropTableStatement:
                printf("DROP TABLE\n");
                printDropTableStatement((FLOPPYDropTableStatement*) parsedCommand->statement);
                break;
            case StatementType::CreateIndexStatement:
                printf("CREATE INDEX\n");
                //printCreateIndexStatement((FLOPPYCreateIndexStatement*) parsedCommand->statement);
                break;
            case StatementType::DropIndexStatement:
                printf("DROP INDEX\n");
                printDropIndexStatement((FLOPPYDropIndexStatement*) parsedCommand->statement);
                break;
            case StatementType::InsertStatement:
                printf("INSERT\n");
                printInsertStatement((FLOPPYInsertStatement*) parsedCommand->statement);
                break;
            case StatementType::DeleteStatement:
                printf("DELETE\n");
                printDeleteStatement((FLOPPYDeleteStatement*) parsedCommand->statement);
                break;
            case StatementType::UpdateStatement:
                printf("UPDATE\n");
                printUpdateStatement((FLOPPYUpdateStatement*) parsedCommand->statement);
                break;
            case StatementType::SelectStatement:
                printf("SELECT\n");
                printSelectStatement((FLOPPYSelectStatement*) parsedCommand->statement);
                break;
        }
        
        
    } else {
        result =  new FLOPPYResult(ErrorType);
        result->errorMsg = "Unable to parse FLOPPY SQL";
    }
    
    return result;
}

/*FLOPPYResult* FLOPPY_DBMS:: createTable(FLOPPYCreateTableStatement *statement) {
    
    fileDescriptor fd = heap_createFile(this->buf, statement->tableName.c_str(), tableDescription *tableDesc, statement->flags->volatileFlag);
    
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
}*/




FLOPPY_DBMS::~FLOPPY_DBMS() {
    int ret = squash(buf);
}