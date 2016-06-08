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
    FLOPPYOutput *parsedCommand = FLOPPYParser::parseFLOPPYString(sql);
    FLOPPYResult *result;
    
    if (parsedCommand->isValid) { // Use isValid flag to detect is parser parsed correctly.
        
        switch (parsedCommand->statement->type()) {
            case StatementType::ErrorStatement:
                break;
            case StatementType::CreateTableStatement:
                result = createTable((FLOPPYCreateTableStatement*) parsedCommand->statement);
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
                result = insertRecord((FLOPPYInsertStatement*) parsedCommand->statement);
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
                result = selectRecords((FLOPPYSelectStatement*) parsedCommand->statement);
                break;
        }
    } else {
        result =  new FLOPPYResult(ErrorType);
        result->msg = "Unable to parse FLOPPY SQL";
    }
    
    delete parsedCommand;
    
    return result;
}

FLOPPYResult * FLOPPY_DBMS::createTable(FLOPPYCreateTableStatement *statement) {
    FLOPPYHeapFile *heap = FLOPPYHeapFile::createFile(buf, statement);
    
    char *msg = (char*)calloc(sizeof(char), 100);
    sprintf(msg, "Created the table `%s` successfully.", statement->tableName);
    
    FLOPPYResult *result = new FLOPPYResult(MessageType);
    result->msg = msg;
    
    delete heap;
    
    return result;
}


FLOPPYResult * FLOPPY_DBMS::insertRecord(FLOPPYInsertStatement *statement) {
    FLOPPYHeapFile *heap = new FLOPPYHeapFile(buf, statement->name);
    heap->insertStatement(statement);
    
    char *msg = (char*)calloc(sizeof(char), 100);
    sprintf(msg, "Inserted record successfully.");
    
    FLOPPYResult *result = new FLOPPYResult(InsertType);
    result->msg = msg;
    
    delete heap;
    
    return result;
}

FLOPPYResult * FLOPPY_DBMS::selectRecords(FLOPPYSelectStatement *statement) {
    FLOPPYHeapFile *heap = new FLOPPYHeapFile(buf, "teachers");
    
    FLOPPYResult *result = new FLOPPYResult(SelectType);
    result->recordSet = heap->getAllRecords();
    
    delete heap;
    
    return result;
}



FLOPPY_DBMS::~FLOPPY_DBMS() {
    delete buf;
}