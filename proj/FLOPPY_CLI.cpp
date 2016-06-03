#include <stdio.h>
#include <iostream>


#include "DBMS.h"
using namespace std;

int main(int argc, char *argv[]) {
    string query;
    string input;
    bool exit = false;
    
    cout << "Welcome to FLOPPY!\n";
    cout << "Disk Path: ";
    
    cin >> input;

	DBMS *db = new DBMS(input, 5, 5);
    
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
            db->execute(query);
        }
    }
    
    delete db;
}