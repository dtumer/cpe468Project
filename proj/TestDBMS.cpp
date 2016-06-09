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
    //result->print();
    delete result;
    result = db->execute("INSERT INTO teachers VALUES('MACROSTIE', 'MIN', 101);");
    delete result;
    result = db->execute("INSERT INTO teachers VALUES('COVIN', 'JEROME', 102);");
    delete result;
    result = db->execute("INSERT INTO teachers VALUES('MOYER', 'OTHA', 103);");
    delete result;
    result = db->execute("INSERT INTO teachers VALUES('NIBLER', 'JERLENE', 104);");
    delete result;
    result = db->execute("INSERT INTO teachers VALUES('MARROTTE', 'KIRK', 105);");
    delete result;
    result = db->execute("INSERT INTO teachers VALUES('TARRING', 'LEIA', 106);");
    delete result;
    result = db->execute("INSERT INTO teachers VALUES('URSERY', 'CHARMAINE', 107);");
    delete result;
    result = db->execute("INSERT INTO teachers VALUES('ONDERSMA', 'LORIA', 108);");
    delete result;
    result = db->execute("INSERT INTO teachers VALUES('KAWA', 'GORDON', 109);");
    delete result;
    result = db->execute("INSERT INTO teachers VALUES('SUMPTION', 'GEORGETTA', 110);");
    delete result;
    result = db->execute("INSERT INTO teachers VALUES('KRIENER', 'BILLIE', 111);");
    delete result;
    result = db->execute("INSERT INTO teachers VALUES('SUGAI', 'ALFREDA', 112);");
    delete result;
    
    
    result = db->execute("CREATE TABLE list VOLATILE( LastName VARCHAR(16), FirstName VARCHAR(16), grade INT, classroom INT,  PRIMARY KEY(FirstName,LastName));");
	delete result;
    result = db->execute("INSERT INTO list VALUES('AMY', 'PATRINA', 1, 102);");
	delete result;
    result = db->execute("INSERT INTO list VALUES('AREHART', 'VERTIE', 3, 107);");
	delete result;
    result = db->execute("INSERT INTO list VALUES('ATWOOD', 'BETHEL', 5, 109);");
	delete result;
    result = db->execute("INSERT INTO list VALUES('BALBOA', 'MEL', 1, 103);");
	delete result;
    result = db->execute("INSERT INTO list VALUES('BARTKUS', 'REYNALDO', 1, 102);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('BIBB', 'SHANAE', 1, 103);");
	delete result;
    result = db->execute("INSERT INTO list VALUES('BRIGHTBILL', 'ANTONY', 1, 102);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('BRINE', 'FRANKLYN', 0, 106);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('BROMLEY', 'EVELINA', 1, 103);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('BUSTILLOS', 'HILMA', 0, 106);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('BYRUM', 'BENNIE', 0, 105);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('CAR', 'MAUDE', 2, 101);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('CHIARAMONTE', 'NOVELLA', 2, 101);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('CRANMER', 'CAREY', 5, 109);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('DANESE', 'JANEE', 4, 111);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('DROP', 'SHERMAN', 0, 104);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('FLACHS', 'JEFFRY', 5, 109);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('FULVIO', 'ELTON', 4, 111);");
    delete result;
	result = db->execute("INSERT INTO list VALUES('GELL', 'TAMI', 0, 104);");
	delete result;
    result = db->execute("INSERT INTO list VALUES('GERSTEIN', 'AL', 5, 109);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('GOODNOE', 'GAYLE', 4, 111);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('GRABILL', 'JULIENNE', 0, 106);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('GROENEWEG', 'CRYSTA', 3, 107);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('GRUNIN', 'EMILE', 5, 109);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('HANNEMANN', 'CHANTAL', 1, 102);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('HOESCHEN', 'LYNNETTE', 4, 108);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('HONES', 'GUILLERMINA', 0, 104);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('HOOSOCK', 'LANCE', 1, 103);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('HOUTCHENS', 'THEO', 0, 106);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('HUANG', 'TAWANNA', 5, 109);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('JAGNEAUX', 'ELVIRA', 6, 112);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('KRISTENSEN', 'STORMY', 6, 112);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('LAPLANT', 'SUMMER', 2, 101);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('LARKINS', 'GAYLE', 4, 110);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('LEAPER', 'ADRIAN', 4, 111);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('LUSKEY', 'BRITT', 4, 108);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('MACIAG', 'CHET', 5, 109);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('MADLOCK', 'RAY', 4, 110);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('MOWATT', 'KITTIE', 0, 105);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('NABOZNY', 'CHRISSY', 3, 107);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('NAKAHARA', 'SHERON', 0, 105);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('NETZEL', 'JODY', 0, 105);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('NOGODA', 'ISMAEL', 0, 105);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('PASSEY', 'RAYLENE', 4, 110);");
	delete result;
    result = db->execute("INSERT INTO list VALUES('PINNELL', 'ROBBY', 3, 107);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('PREHM', 'SHANEL', 0, 104);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('RANSLER', 'RODGER', 1, 102);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('RODDEY', 'CYRUS', 4, 110);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('RUNKLE', 'MARCUS', 1, 102);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('SAADE', 'TOBIE', 4, 110);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('SANTORY', 'JORDON', 3, 107);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('SCHUTZE', 'LANELLE', 5, 109);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('SOLOMAN', 'BRODERICK', 0, 106);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('STIRE', 'SHIRLEY', 6, 112);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('SUDA', 'DARLEEN', 4, 110);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('TRAYWICK', 'KERI', 1, 102);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('VANDERWOUDE', 'SHERWOOD', 3, 107);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('VANVLIET', 'COLLIN', 0, 106);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('WIRTZFELD', 'DELORAS', 0, 106);");
    delete result;
    result = db->execute("INSERT INTO list VALUES('YUEN', 'ANIKA', 1, 103);");
    delete result;
    
    
    result = db->execute("SELECT * FROM teachers AS t WHERE classroom > 105;");
	//result = db->execute("SELECT FirstName, LastName, First, Last FROM list, teachers AS t;");
    //result = db->execute("SELECT FirstName, LastName, First, Last FROM list, teachers AS t WHERE list.classroom = teachers.classroom;");
    result->print();
    delete result;
    
    result = db->execute("DROP TABLE teachers;");
    delete result;
    
    
    delete db;
    
}