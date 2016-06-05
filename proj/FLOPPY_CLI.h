//
//  FLOPPY_CLI.h
//

#ifndef FLOPPY_CLI_h
#define FLOPPY_CLI_h

#include "FLOPPY_DBMS.h"
#include "FLOPPYResult.h"

class FLOPPY_CLI : private FLOPPY_DBMS {
public:
    FLOPPY_CLI(std::string diskName, int nPersistentBlocks, int nVolatileBlocks); //constructor
    void executeCLI(std::string query);
    
};

#endif /* FLOPPY_CLI_h */
