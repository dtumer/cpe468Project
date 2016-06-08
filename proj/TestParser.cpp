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
    TestSQLStatement("DELETE FROM Availability WHERE NodeId>1 AND DistroId<5;");
    TestSQLStatement("DELETE FROM Availability WHERE NodeId>1 AND NOT(DistroId<5);");
    TestSQLStatement("DELETE FROM Availability WHERE NodeId=1 AND NOT((DistroId / 10) > 5);");
    TestSQLStatement("UPDATE Availability SET DistroId=10 WHERE NodeId=1;");
    TestSQLStatement("SELECT a.NodeId, a.FileId FROM Availability a WHERE NodeId=25;");    
    
    TestSQLStatement("CREATE INDEX ndx1 ON Availability (NodeId, DistroId);");
    TestSQLStatement("SELECT DISTINCT NodeId, COUNT(BlockID) FROM Availability WHERE FileId=10 GROUP BY NodeId HAVING BlockID>2 ORDER BY BlockID LIMIT 10;");
    TestSQLStatement("DROP INDEX ndx2 on Availability;");
    TestSQLStatement("DROP TABLE Availability;");
    dbms.execute("SELECT NodeId, FileId, FROM Availability WHERE x = 5;");
    
    return 0;
}
