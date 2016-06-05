#include <stdio.h>
#include <iostream>


#include "FLOPPY_DBMS.h"
#include "FLOPPY_CLI.h"

using namespace std;

FLOPPY_CLI::FLOPPY_CLI(std::string diskName, int nPersistentBlocks, int nVolatileBlocks)
: FLOPPY_DBMS(diskName, nPersistentBlocks, nVolatileBlocks) {
    
}

void FLOPPY_CLI::executeCLI(string query) {
    FLOPPYResult *result = this->execute(query);
    
    switch (result->type()) {
        case FLOPPYResultType::ErrorType:
            cout << "ERROR - " << result->errorMsg << "\n";
            break;
        case FLOPPYResultType::SelectType:
            break;
        case FLOPPYResultType::UpdateType:
            break;
        case FLOPPYResultType::InsertType:
            break;
    }
}

int main(int argc, char *argv[]) {
    string query;
    string input;
    bool exit = false;
    
    cout << "Welcome to FLOPPY!\n";
    cout << "Disk Path: ";
    
    cin >> input;

	FLOPPY_CLI *db = new FLOPPY_CLI(input, 5, 5);
    
    cout << "Opened database disk \"" << input << "\".\n";
    cout << "To close FLOPPY enter the command \"exit\"\n\n";
    
    while(!exit) {
    	cout << "FLOPPY> ";
        query = "";
        
        getline(cin, input);
        
        if(input == "exit") {
            exit = true;
        }
        else {
            query += input;
            while(input.rfind(';') == string::npos) {
                getline(cin, input);
                query += input;
            }
            
            //run command
            db->executeCLI(query);
        }
    }
    
    delete db;
}

