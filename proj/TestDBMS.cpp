//
//  TestDBMS.cpp
//

#include <stdio.h>
#include "FLOPPY_DBMS.h"
#include "FLOPPYResult.h"

int main(int argc, char *argv[]) {
    FLOPPY_DBMS *db = new FLOPPY_DBMS("Foo.disk", 5, 5);
    FLOPPYResult *result;
    
    
    result = db->execute("CREATE TABLE teachers VOLATILE (Last VARCHAR(16), First VARCHAR(16), classroom INT, PRIMARY KEY(classroom));");
    result->print();
    delete result;
    result = db->execute("INSERT INTO teachers VALUES('MACROSTIE', 'MIN', 101);");
    result->print();
    delete result;
    result = db->execute("INSERT INTO teachers VALUES('COVIN', 'JEROME', 102);");
    result->print();
    delete result;
    result = db->execute("INSERT INTO teachers VALUES('MOYER', 'OTHA', 103);");
    result->print();
    delete result;
    result = db->execute("INSERT INTO teachers VALUES('NIBLER', 'JERLENE', 104);");
    result->print();
    delete result;
    result = db->execute("INSERT INTO teachers VALUES('MARROTTE', 'KIRK', 105);");
    result->print();
    delete result;
    result = db->execute("INSERT INTO teachers VALUES('TARRING', 'LEIA', 106);");
    result->print();
    delete result;
    result = db->execute("INSERT INTO teachers VALUES('URSERY', 'CHARMAINE', 107);");
    result->print();
    delete result;
    result = db->execute("INSERT INTO teachers VALUES('ONDERSMA', 'LORIA', 108);");
    result->print();
    delete result;
    result = db->execute("INSERT INTO teachers VALUES('KAWA', 'GORDON', 109);");
    result->print();
    delete result;
    result = db->execute("INSERT INTO teachers VALUES('SUMPTION', 'GEORGETTA', 110);");
    result->print();
    delete result;
    result = db->execute("INSERT INTO teachers VALUES('KRIENER', 'BILLIE', 111);");
    result->print();
    delete result;
    result = db->execute("INSERT INTO teachers VALUES('SUGAI', 'ALFREDA', 112);");
    result->print();
    delete result;
    
    result = db->execute("SELECT * FROM teachers;");
    result->print();
    
    
    delete result;
    delete db;
    
}