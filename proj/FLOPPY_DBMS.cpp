#include "FLOPPY_DBMS.h"
#include "FLOPPYResult.h"
#include "QueryPlan/FLOPPYQueryPlan.h"

FLOPPY_DBMS::FLOPPY_DBMS (std::string diskName, int nPersistentBlocks, int nVolatileBlocks) {
    //create temp char*
    char *diskNameC = new char[diskName.length() + 1];
    strcpy(diskNameC, diskName.c_str());
    
    buf = new FLOPPYBufferManager(diskNameC, nPersistentBlocks, nVolatileBlocks);
    
    //free memory
    delete [] diskNameC;
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
            /*
            case StatementType::UpdateStatement:
                printf("UPDATE\n");
                printUpdateStatement((FLOPPYUpdateStatement*) parsedCommand->statement);
                break;
            case StatementType::DeleteStatement:
                printf("DELETE\n");
                printDeleteStatement((FLOPPYDeleteStatement*) parsedCommand->statement);
                break;
            case StatementType::CreateIndexStatement:
                printf("CREATE INDEX\n");
                //printCreateIndexStatement((FLOPPYCreateIndexStatement*) parsedCommand->statement);
                break;
            case StatementType::SelectStatement:
                printf("SELECT\n");
                printSelectStatement((FLOPPYSelectStatement*) parsedCommand->statement);
                FLOPPYQueryPlan *queryPlan = new FLOPPYQueryPlan(parsedCommand->statement);
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
    FLOPPYHeapFile *heap = new FLOPPYHeapFile(buf, statement->tableSpecs->at(0)->tableName);
    
    FLOPPYResult *result = new FLOPPYResult(SelectType);
    result->recordSet = heap->getAllRecords();
    
    delete heap;
    
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



FLOPPY_DBMS::~FLOPPY_DBMS() {
    delete buf;
}
