//
//  TestDBMS.cpp
//

#include <string>
#include <stdio.h>
#include "FLOPPY_DBMS.h"
#include "FLOPPYResult.h"


class FLOPPY_TEST : private FLOPPY_DBMS {
public:
    FLOPPY_TEST(); //constructor
    void executeTest(std::string query);
};

FLOPPY_TEST::FLOPPY_TEST()
: FLOPPY_DBMS("Foo.disk", 5, 5) {
    
}

void FLOPPY_TEST::executeTest(std::string query) {
    FLOPPYResult *result = this->execute(query);
    result->print();
    delete result;
}

int main(int argc, char *argv[]) {
    FLOPPY_TEST *db = new FLOPPY_TEST();
    
    
    db->executeTest("CREATE TABLE teachers VOLATILE (Last VARCHAR(16), First VARCHAR(16), classroom INT, PRIMARY KEY(classroom));");
    db->executeTest("INSERT INTO teachers VALUES('MACROSTIE', 'MIN', 101);");
    db->executeTest("INSERT INTO teachers VALUES('COVIN', 'JEROME', 102);");
    db->executeTest("INSERT INTO teachers VALUES('MOYER', 'OTHA', 103);");
    db->executeTest("INSERT INTO teachers VALUES('NIBLER', 'JERLENE', 104);");
    db->executeTest("INSERT INTO teachers VALUES('MARROTTE', 'KIRK', 105);");
    db->executeTest("INSERT INTO teachers VALUES('TARRING', 'LEIA', 106);");
    db->executeTest("INSERT INTO teachers VALUES('URSERY', 'CHARMAINE', 107);");
    db->executeTest("INSERT INTO teachers VALUES('ONDERSMA', 'LORIA', 108);");
    db->executeTest("INSERT INTO teachers VALUES('KAWA', 'GORDON', 109);");
    db->executeTest("INSERT INTO teachers VALUES('SUMPTION', 'GEORGETTA', 110);");
    db->executeTest("INSERT INTO teachers VALUES('KRIENER', 'BILLIE', 111);");
    db->executeTest("INSERT INTO teachers VALUES('SUGAI', 'ALFREDA', 112);");
    
    
    db->executeTest("CREATE TABLE list VOLATILE( LastName VARCHAR(16), FirstName VARCHAR(16), grade INT, classroom INT,  PRIMARY KEY(FirstName,LastName));");
	db->executeTest("INSERT INTO list VALUES('AMY', 'PATRINA', 1, 102);");
	db->executeTest("INSERT INTO list VALUES('AREHART', 'VERTIE', 3, 107);");
	db->executeTest("INSERT INTO list VALUES('ATWOOD', 'BETHEL', 5, 109);");
	db->executeTest("INSERT INTO list VALUES('BALBOA', 'MEL', 1, 103);");
	db->executeTest("INSERT INTO list VALUES('BARTKUS', 'REYNALDO', 1, 102);");
    db->executeTest("INSERT INTO list VALUES('BIBB', 'SHANAE', 1, 103);");
	db->executeTest("INSERT INTO list VALUES('BRIGHTBILL', 'ANTONY', 1, 102);");
    db->executeTest("INSERT INTO list VALUES('BRINE', 'FRANKLYN', 0, 106);");
    db->executeTest("INSERT INTO list VALUES('BROMLEY', 'EVELINA', 1, 103);");
    db->executeTest("INSERT INTO list VALUES('BUSTILLOS', 'HILMA', 0, 106);");
    db->executeTest("INSERT INTO list VALUES('BYRUM', 'BENNIE', 0, 105);");
    db->executeTest("INSERT INTO list VALUES('CAR', 'MAUDE', 2, 101);");
    db->executeTest("INSERT INTO list VALUES('CHIARAMONTE', 'NOVELLA', 2, 101);");
    db->executeTest("INSERT INTO list VALUES('CRANMER', 'CAREY', 5, 109);");
    db->executeTest("INSERT INTO list VALUES('DANESE', 'JANEE', 4, 111);");
    db->executeTest("INSERT INTO list VALUES('DROP', 'SHERMAN', 0, 104);");
    db->executeTest("INSERT INTO list VALUES('FLACHS', 'JEFFRY', 5, 109);");
    db->executeTest("INSERT INTO list VALUES('FULVIO', 'ELTON', 4, 111);");
    db->executeTest("INSERT INTO list VALUES('GELL', 'TAMI', 0, 104);");
	db->executeTest("INSERT INTO list VALUES('GERSTEIN', 'AL', 5, 109);");
    db->executeTest("INSERT INTO list VALUES('GOODNOE', 'GAYLE', 4, 111);");
    db->executeTest("INSERT INTO list VALUES('GRABILL', 'JULIENNE', 0, 106);");
    db->executeTest("INSERT INTO list VALUES('GROENEWEG', 'CRYSTA', 3, 107);");
    db->executeTest("INSERT INTO list VALUES('GRUNIN', 'EMILE', 5, 109);");
    db->executeTest("INSERT INTO list VALUES('HANNEMANN', 'CHANTAL', 1, 102);");
    db->executeTest("INSERT INTO list VALUES('HOESCHEN', 'LYNNETTE', 4, 108);");
    db->executeTest("INSERT INTO list VALUES('HONES', 'GUILLERMINA', 0, 104);");
    db->executeTest("INSERT INTO list VALUES('HOOSOCK', 'LANCE', 1, 103);");
    db->executeTest("INSERT INTO list VALUES('HOUTCHENS', 'THEO', 0, 106);");
    db->executeTest("INSERT INTO list VALUES('HUANG', 'TAWANNA', 5, 109);");
    db->executeTest("INSERT INTO list VALUES('JAGNEAUX', 'ELVIRA', 6, 112);");
    db->executeTest("INSERT INTO list VALUES('KRISTENSEN', 'STORMY', 6, 112);");
    db->executeTest("INSERT INTO list VALUES('LAPLANT', 'SUMMER', 2, 101);");
    db->executeTest("INSERT INTO list VALUES('LARKINS', 'GAYLE', 4, 110);");
    db->executeTest("INSERT INTO list VALUES('LEAPER', 'ADRIAN', 4, 111);");
    db->executeTest("INSERT INTO list VALUES('LUSKEY', 'BRITT', 4, 108);");
    db->executeTest("INSERT INTO list VALUES('MACIAG', 'CHET', 5, 109);");
    db->executeTest("INSERT INTO list VALUES('MADLOCK', 'RAY', 4, 110);");
    db->executeTest("INSERT INTO list VALUES('MOWATT', 'KITTIE', 0, 105);");
    db->executeTest("INSERT INTO list VALUES('NABOZNY', 'CHRISSY', 3, 107);");
    db->executeTest("INSERT INTO list VALUES('NAKAHARA', 'SHERON', 0, 105);");
    db->executeTest("INSERT INTO list VALUES('NETZEL', 'JODY', 0, 105);");
    db->executeTest("INSERT INTO list VALUES('NOGODA', 'ISMAEL', 0, 105);");
    db->executeTest("INSERT INTO list VALUES('PASSEY', 'RAYLENE', 4, 110);");
	db->executeTest("INSERT INTO list VALUES('PINNELL', 'ROBBY', 3, 107);");
    db->executeTest("INSERT INTO list VALUES('PREHM', 'SHANEL', 0, 104);");
    db->executeTest("INSERT INTO list VALUES('RANSLER', 'RODGER', 1, 102);");
    db->executeTest("INSERT INTO list VALUES('RODDEY', 'CYRUS', 4, 110);");
    db->executeTest("INSERT INTO list VALUES('RUNKLE', 'MARCUS', 1, 102);");
    db->executeTest("INSERT INTO list VALUES('SAADE', 'TOBIE', 4, 110);");
    db->executeTest("INSERT INTO list VALUES('SANTORY', 'JORDON', 3, 107);");
    db->executeTest("INSERT INTO list VALUES('SCHUTZE', 'LANELLE', 5, 109);");
    db->executeTest("INSERT INTO list VALUES('SOLOMAN', 'BRODERICK', 0, 106);");
    db->executeTest("INSERT INTO list VALUES('STIRE', 'SHIRLEY', 6, 112);");
    db->executeTest("INSERT INTO list VALUES('SUDA', 'DARLEEN', 4, 110);");
    db->executeTest("INSERT INTO list VALUES('TRAYWICK', 'KERI', 1, 102);");
    db->executeTest("INSERT INTO list VALUES('VANDERWOUDE', 'SHERWOOD', 3, 107);");
    db->executeTest("INSERT INTO list VALUES('VANVLIET', 'COLLIN', 0, 106);");
    db->executeTest("INSERT INTO list VALUES('WIRTZFELD', 'DELORAS', 0, 106);");
    db->executeTest("INSERT INTO list VALUES('YUEN', 'ANIKA', 1, 103);");
    
    //ok
    //db->executeTest("SELECT * FROM teachers;");
    
    //ok
    //db->executeTest("SELECT * FROM list WHERE classroom = 103;");
    
    //ok
    //db->executeTest("SELECT Last, First FROM teachers;");
    
    //ok
    //db->executeTest("SELECT LastName, FirstName FROM list WHERE grade = 2;");
    
    //ok
    //db->executeTest("SELECT * FROM list WHERE grade < 2;");
    
    //ok
    //db->executeTest("SELECT * FROM list WHERE grade > 4;");
    
    //ok
    //db->executeTest("SELECT * FROM list WHERE grade <= 2;");
    
    //ok
    //db->executeTest("SELECT * FROM list WHERE grade >=5;");
    
    //ok
    //db->executeTest("SELECT * FROM list WHERE grade != 4;");
    
    //ok
    //db->executeTest("SELECT * FROM list WHERE grade < 5 AND grade > 3;");
    
    //ok
    //db->executeTest("SELECT DISTINCT grade FROM list;");
    
    //ok
    //db->executeTest("SELECT DISTINCT classroom FROM list WHERE grade = 1;");
    
    //ok
    //db->executeTest("SELECT classroom FROM list WHERE LastName = 'BIBB';");
    
    //ok
    //db->executeTest("SELECT * FROM teachers WHERE Last = 'MOYER';");
    
    //ok
    //db->executeTest("SELECT * FROM teachers WHERE Last <> 'MOYER';");
    
    //ok
    //db->executeTest("SELECT COUNT(*) FROM teachers;");
    
    //ok
    //db->executeTest("SELECT COUNT(*) FROM list;");
    
    //broken
    //db->executeTest("SELECT MAX(grade), MIN(grade), MAX(classroom), MIN(classroom) FROM list;");
    
    //ok
    //db->executeTest("SELECT grade, COUNT(*) FROM list GROUP BY grade;");
    
    //ok
    //db->executeTest("SELECT classroom, COUNT(*) FROM list WHERE grade = 1 GROUP BY classroom;");
    
    //ok
    //db->executeTest("SELECT grade, COUNT(*) FROM list GROUP BY grade HAVING COUNT(*) > 8;");
    
    //ok
    //db->executeTest("SELECT DISTINCT grade FROM list ORDER BY grade;");
    
    //ok
    //db->executeTest("SELECT DISTINCT classroom FROM list ORDER BY grade;");
    
    //ok
    //db->executeTest("SELECT FirstName, LastName, First, Last FROM list, teachers WHERE list.classroom = teachers.classroom;");
    
    //ok
    //db->executeTest("SELECT l.FirstName, l.LastName, t.First, t.Last FROM list l,  teachers t WHERE l.classroom = t.classroom;");
    
    //ok
    //db->executeTest("SELECT l.FirstName, l.LastName FROM list l, list pa WHERE pa.FirstName = 'AMY' AND pa.LastName = 'PATRINA' AND l.classroom = pa.classroom;");
    
    //ok
    //db->executeTest("SELECT l.FirstName, l.LastName FROM list l, list pa WHERE pa.FirstName = 'AMY' AND pa.LastName = 'PATRINA' AND l.classroom = pa.classroom ORDER BY l.LastName;");
    
    //ok
    //db->executeTest("SELECT l.FirstName, l.LastName FROM list l, teachers t WHERE t.classroom = 103 AND t.classroom = l.classroom AND t.LastName <> 'BIBB';");
    
    //ok
    //db->executeTest("SELECT l.classroom, COUNT(*) FROM list l,  teachers t WHERE l.classroom = t.classroom AND l.grade > 3 GROUP BY l.classroom;");
    
    //ok
    //db->executeTest("SELECT grade, classroom, COUNT(*) FROM list AS l GROUP BY grade, classroom;");
    
    //ok
    //db->executeTest("SELECT * FROM list AS l ORDER BY classroom, LastName;");
    
    //ok
    //db->executeTest("SELECT * FROM teachers AS t WHERE classroom > 105 LIMIT 2;");
	
    //ok
    //db->executeTest("SELECT FirstName, LastName, First, Last FROM list, teachers AS t;");
    
    //ok
    //db->executeTest("SELECT FirstName, LastName, First, Last FROM list, teachers AS t WHERE list.classroom = t.classroom;");
    
    //broken
    //db->executeTest("SELECT MAX(grade) FROM list;");
    
    //ok
    //db->executeTest("DROP TABLE teachers;");
    
    
    delete db;
    
}