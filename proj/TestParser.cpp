#include <stdlib.h>
#include <string>
#include <iostream>

#include "libs/FLOPPYParser/FLOPPYParser.h"
#include "ParserDebug.h"
#include "FLOPPY_DBMS.h"

int main(int argc, char *argv[]) {
    //TestSQLStatement("CREATE TABLE NodeStats (IPAddress VARCHAR(20), BlocksUploaded  INT,BlocksDownloaded INT,RequestsReceived INT,RequestsSent INT,RequestsServed INT,RequestsFailed INT,DistrosUploaded  INT,DistrosDownloaded INT,PRIMARY KEY (IPAddress));");
    std::string diskN = std::string("test");
    FLOPPY_DBMS dbms(diskN, 5, 5);
   
    TestSQLStatement("CREATE TABLE Availability VOLATILE (NodeId  VARCHAR(20), DistroId  INT, FileId  INT, BlockID  INT, TimeStamp DATETIME, PRIMARY KEY(FileId, BlockId, NodeId), FOREIGN KEY(NodeId) REFERENCES Nodes, FOREIGN KEY (DistroId) REFERENCES Distros, FOREIGN KEY (FileId) REFERENCES Files);");
    TestSQLStatement("INSERT INTO Availability VALUES ('test', 2, 10, 20);");
    //TestSQLStatement("CREATE TABLE av (DistroId  INT, PRIMARY KEY(DistroId));"); //tablename leaks
    TestSQLStatement("INSERT INTO Availability VALUES (\"test\", 2, 10, 20);");
    TestSQLStatement("DELETE FROM Availability WHERE NodeId>1 AND DistroId<5;");
    
    TestSQLStatement("DELETE FROM Availability WHERE NodeId>1 AND NOT(DistroId<5);");
    TestSQLStatement("DELETE FROM Availability WHERE NodeId=1 AND NOT((DistroId / 10) > 5);");
    TestSQLStatement("UPDATE Availability SET DistroId=10 WHERE NodeId=1;");
    TestSQLStatement("SELECT a.NodeId, a.FileId FROM Availability a WHERE NodeId=25;");    
    
    TestSQLStatement("CREATE INDEX ndx1 ON Availability (NodeId, DistroId);");
    TestSQLStatement("SELECT * FROM Availability;");
    TestSQLStatement("SELECT NodeId FROM Availability;");
    TestSQLStatement("SELECT DISTINCT NodeId FROM Availability;");
    TestSQLStatement("SELECT DISTINCT NodeId FROM Availability WHERE FileId=10;");
    TestSQLStatement("SELECT NodeId FROM Availability GROUP BY BlockID;");
    TestSQLStatement("SELECT NodeId, COUNT(*) FROM Availability GROUP BY BlockID;");
    TestSQLStatement("SELECT NodeId, COUNT(BlockID) FROM Availability GROUP BY BlockID;");
    TestSQLStatement("SELECT DISTINCT NodeId, COUNT(BlockID) FROM Availability WHERE FileId=10 GROUP BY NodeId HAVING BlockID>2 ORDER BY BlockID LIMIT 10;");
    TestSQLStatement("SELECT DISTINCT NodeId, COUNT(BlockID) FROM Availability, files WHERE FileId=10 GROUP BY NodeId HAVING BlockID>2 ORDER BY BlockID LIMIT 10;");
    TestSQLStatement("DROP INDEX ndx2 on Availability;");
    TestSQLStatement("DROP TABLE Availability;");
    
    //TestSQLStatement("CREATE TABLE list( LastName VARCHAR(16), FirstName VARCHAR(16), grade INT, classroom INT,  PRIMARY KEY(FirstName,LastName));");
    //TestSQLStatement("CREATE TABLE teachers(Last VARCHAR(16), First VARCHAR(16), classroom INT, PRIMARY KEY(classroom));");
    TestSQLStatement("INSERT INTO list VALUES('AMY', 'PATRINA', 1, 102);");
    TestSQLStatement("INSERT INTO list VALUES('AREHART', 'VERTIE', 3, 107);");
    TestSQLStatement("INSERT INTO list VALUES('ATWOOD', 'BETHEL', 5, 109);");
    TestSQLStatement("INSERT INTO list VALUES('BALBOA', 'MEL', 1, 103);");
    TestSQLStatement("INSERT INTO list VALUES('BARTKUS', 'REYNALDO', 1, 102);");
    TestSQLStatement("INSERT INTO list VALUES('BIBB', 'SHANAE', 1, 103);");
    TestSQLStatement("INSERT INTO list VALUES('BRIGHTBILL', 'ANTONY', 1, 102);");
    TestSQLStatement("INSERT INTO list VALUES('BRINE', 'FRANKLYN', 0, 106);");
    TestSQLStatement("INSERT INTO list VALUES('BROMLEY', 'EVELINA', 1, 103);");
    TestSQLStatement("INSERT INTO list VALUES('BUSTILLOS', 'HILMA', 0, 106);");
    TestSQLStatement("INSERT INTO list VALUES('BYRUM', 'BENNIE', 0, 105);");
    TestSQLStatement("INSERT INTO list VALUES('CAR', 'MAUDE', 2, 101);");
    TestSQLStatement("INSERT INTO list VALUES('CHIARAMONTE', 'NOVELLA', 2, 101);");
    TestSQLStatement("INSERT INTO list VALUES('CRANMER', 'CAREY', 5, 109);");
    TestSQLStatement("INSERT INTO list VALUES('DANESE', 'JANEE', 4, 111);");
    TestSQLStatement("INSERT INTO list VALUES('DROP', 'SHERMAN', 0, 104);");
    TestSQLStatement("INSERT INTO list VALUES('FLACHS', 'JEFFRY', 5, 109);");
    TestSQLStatement("INSERT INTO list VALUES('FULVIO', 'ELTON', 4, 111);");
    TestSQLStatement("INSERT INTO list VALUES('GELL', 'TAMI', 0, 104);");
    TestSQLStatement("INSERT INTO list VALUES('GERSTEIN', 'AL', 5, 109);");
    TestSQLStatement("INSERT INTO list VALUES('GOODNOE', 'GAYLE', 4, 111);");
    TestSQLStatement("INSERT INTO list VALUES('GRABILL', 'JULIENNE', 0, 106);");
    TestSQLStatement("INSERT INTO list VALUES('GROENEWEG', 'CRYSTA', 3, 107);");
    TestSQLStatement("INSERT INTO list VALUES('GRUNIN', 'EMILE', 5, 109);");
    TestSQLStatement("INSERT INTO list VALUES('HANNEMANN', 'CHANTAL', 1, 102);");
    TestSQLStatement("INSERT INTO list VALUES('HOESCHEN', 'LYNNETTE', 4, 108);");
    TestSQLStatement("INSERT INTO list VALUES('HONES', 'GUILLERMINA', 0, 104);");
    TestSQLStatement("INSERT INTO list VALUES('HOOSOCK', 'LANCE', 1, 103);");
    TestSQLStatement("INSERT INTO list VALUES('HOUTCHENS', 'THEO', 0, 106);");
    TestSQLStatement("INSERT INTO list VALUES('HUANG', 'TAWANNA', 5, 109);");
    TestSQLStatement("INSERT INTO list VALUES('JAGNEAUX', 'ELVIRA', 6, 112);");
    TestSQLStatement("INSERT INTO list VALUES('KRISTENSEN', 'STORMY', 6, 112);");
    TestSQLStatement("INSERT INTO list VALUES('LAPLANT', 'SUMMER', 2, 101);");
    TestSQLStatement("INSERT INTO list VALUES('LARKINS', 'GAYLE', 4, 110);");
    TestSQLStatement("INSERT INTO list VALUES('LEAPER', 'ADRIAN', 4, 111);");
    TestSQLStatement("INSERT INTO list VALUES('LUSKEY', 'BRITT', 4, 108);");
    TestSQLStatement("INSERT INTO list VALUES('MACIAG', 'CHET', 5, 109);");
    TestSQLStatement("INSERT INTO list VALUES('MADLOCK', 'RAY', 4, 110);");
    TestSQLStatement("INSERT INTO list VALUES('MOWATT', 'KITTIE', 0, 105);");
    TestSQLStatement("INSERT INTO list VALUES('NABOZNY', 'CHRISSY', 3, 107);");
    TestSQLStatement("INSERT INTO list VALUES('NAKAHARA', 'SHERON', 0, 105);");
    TestSQLStatement("INSERT INTO list VALUES('NETZEL', 'JODY', 0, 105);");
    TestSQLStatement("INSERT INTO list VALUES('NOGODA', 'ISMAEL', 0, 105);");
    TestSQLStatement("INSERT INTO list VALUES('PASSEY', 'RAYLENE', 4, 110);");
    TestSQLStatement("INSERT INTO list VALUES('PINNELL', 'ROBBY', 3, 107);");
    TestSQLStatement("INSERT INTO list VALUES('PREHM', 'SHANEL', 0, 104);");
    TestSQLStatement("INSERT INTO list VALUES('RANSLER', 'RODGER', 1, 102);");
    TestSQLStatement("INSERT INTO list VALUES('RODDEY', 'CYRUS', 4, 110);");
    TestSQLStatement("INSERT INTO list VALUES('RUNKLE', 'MARCUS', 1, 102);");
    TestSQLStatement("INSERT INTO list VALUES('SAADE', 'TOBIE', 4, 110);");
    TestSQLStatement("INSERT INTO list VALUES('SANTORY', 'JORDON', 3, 107);");
    TestSQLStatement("INSERT INTO list VALUES('SCHUTZE', 'LANELLE', 5, 109);");
    TestSQLStatement("INSERT INTO list VALUES('SOLOMAN', 'BRODERICK', 0, 106);");
    TestSQLStatement("INSERT INTO list VALUES('STIRE', 'SHIRLEY', 6, 112);");
    TestSQLStatement("INSERT INTO list VALUES('SUDA', 'DARLEEN', 4, 110);");
    TestSQLStatement("INSERT INTO list VALUES('TRAYWICK', 'KERI', 1, 102);");
    TestSQLStatement("INSERT INTO list VALUES('VANDERWOUDE', 'SHERWOOD', 3, 107);");
    TestSQLStatement("INSERT INTO list VALUES('VANVLIET', 'COLLIN', 0, 106);");
    TestSQLStatement("INSERT INTO list VALUES('WIRTZFELD', 'DELORAS', 0, 106);");
    TestSQLStatement("INSERT INTO list VALUES('YUEN', 'ANIKA', 1, 103);");
    
    TestSQLStatement("INSERT INTO teachers VALUES('MACROSTIE', 'MIN', 101);");
    TestSQLStatement("INSERT INTO teachers VALUES('COVIN', 'JEROME', 102);");
    TestSQLStatement("INSERT INTO teachers VALUES('MOYER', 'OTHA', 103);");
    TestSQLStatement("INSERT INTO teachers VALUES('NIBLER', 'JERLENE', 104);");
    TestSQLStatement("INSERT INTO teachers VALUES('MARROTTE', 'KIRK', 105);");
    TestSQLStatement("INSERT INTO teachers VALUES('TARRING', 'LEIA', 106);");
    TestSQLStatement("INSERT INTO teachers VALUES('URSERY', 'CHARMAINE', 107);");
    TestSQLStatement("INSERT INTO teachers VALUES('ONDERSMA', 'LORIA', 108);");
    TestSQLStatement("INSERT INTO teachers VALUES('KAWA', 'GORDON', 109);");
    TestSQLStatement("INSERT INTO teachers VALUES('SUMPTION', 'GEORGETTA', 110);");
    TestSQLStatement("INSERT INTO teachers VALUES('KRIENER', 'BILLIE', 111);");
    TestSQLStatement("INSERT INTO teachers VALUES('SUGAI', 'ALFREDA', 112);");
    
    
    TestSQLStatement("SELECT * FROM teachers;");
    TestSQLStatement("SELECT * FROM list WHERE classroom = 103;");
    TestSQLStatement("SELECT Last, First FROM teachers;");
    TestSQLStatement("SELECT LastName, FirstName FROM list WHERE grade = 2;");
    TestSQLStatement("SELECT * FROM list WHERE grade < 2;");
    TestSQLStatement("SELECT * FROM list WHERE grade > 4;");
    TestSQLStatement("SELECT * FROM list WHERE grade <= 2;");
    TestSQLStatement("SELECT * FROM list WHERE grade >=5;");
    TestSQLStatement("SELECT * FROM list WHERE grade != 4;");
    TestSQLStatement("SELECT * FROM list WHERE grade < 5 AND grade > 3;");
    TestSQLStatement("SELECT DISTINCT grade FROM list;");
    TestSQLStatement("SELECT DISTINCT classroom FROM list WHERE grade = 1;");
    TestSQLStatement("SELECT classroom FROM list WHERE LastName = 'BIBB';");
    TestSQLStatement("SELECT * FROM teachers WHERE Last = 'MOYER';");
    TestSQLStatement("SELECT * FROM teachers WHERE Last <> 'MOYER';");
    TestSQLStatement("SELECT COUNT(*) FROM teachers;");
    TestSQLStatement("SELECT COUNT(*) FROM list;");
    TestSQLStatement("SELECT MAX(grade), MIN(grade), MAX(classroom), MIN(classroom) FROM list;");
    TestSQLStatement("SELECT grade, COUNT(*) FROM list GROUP BY grade;");
    TestSQLStatement("SELECT classroom, COUNT(*) FROM list WHERE grade = 1 GROUP BY classroom;");
    TestSQLStatement("SELECT grade, COUNT(*) FROM list GROUP BY grade HAVING COUNT(*) > 8;");
    TestSQLStatement("SELECT DISTINCT grade FROM list ORDER BY grade;");
    TestSQLStatement("SELECT DISTINCT classroom FROM list ORDER BY grade;");
    TestSQLStatement("SELECT FirstName, LastName, First, Last FROM list, teachers WHERE list.classroom = teachers.classroom;");
    TestSQLStatement("SELECT l.FirstName, l.LastName, t.First, t.Last FROM list l,  teachers t WHERE l.classroom = t.classroom;");
    TestSQLStatement("SELECT l.FirstName, l.LastName FROM list l, list pa WHERE pa.FirstName = 'AMY' AND pa.LastName = 'PATRINA' AND l.classroom = pa.classroom;");
    TestSQLStatement("SELECT l.FirstName, l.LastName FROM list l, list pa WHERE pa.FirstName = 'AMY' AND pa.LastName = 'PATRINA' AND l.classroom = pa.classroom ORDER BY l.LastName;");
    TestSQLStatement("SELECT l.FirstName, l.LastName FROM list l, teachers t WHERE t.classroom = 103 AND t.classroom = l.classroom AND t.LastName <> 'BIBB';");
    TestSQLStatement("SELECT l.classroom, COUNT(*) FROM list l,  teachers t WHERE l.classroom = t.classroom AND l.grade > 3 GROUP BY l.classroom;");

    dbms.execute("SELECT MAX(NodeId), FileId FROM Availability WHERE x = 5 GROUP By l.classroom;");
    
    return 0;
}
