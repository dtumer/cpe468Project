#include <stdlib.h>
#include <string>
#include <iostream>

#include "libs/FLOPPY/FLOPPYParser.h"
#include "ParserDebug.h"

int main(int argc, char *argv[]) {
    //TestSQLStatement("CREATE TABLE NodeStats (IPAddress VARCHAR(20), BlocksUploaded  INT,BlocksDownloaded INT,RequestsReceived INT,RequestsSent INT,RequestsServed INT,RequestsFailed INT,DistrosUploaded  INT,DistrosDownloaded INT,PRIMARY KEY (IPAddress));");
    
    TestSQLStatement("CREATE TABLE Availability VOLATILE (NodeId  VARCHAR(20), DistroId  INT, FileId  INT, BlockID  INT, TimeStamp DATETIME, PRIMARY KEY(FileId, BlockId, NodeId), FOREIGN KEY(NodeId) REFERENCES Nodes, FOREIGN KEY (DistroId) REFERENCES Distros, FOREIGN KEY (FileId) REFERENCES Files);");
    TestSQLStatement("INSERT INTO Availability VALUES ('test', 2, 10, 20);");
    TestSQLStatement("DELETE FROM Availability WHERE NodeId>1 AND DistroId<5;");
    TestSQLStatement("DELETE FROM Availability WHERE NodeId>1 AND NOT(DistroId<5);");
    TestSQLStatement("DELETE FROM Availability WHERE NodeId=1 AND NOT((DistroId / 10) > 5);");
    TestSQLStatement("UPDATE Availability SET DistroId=10 WHERE NodeId=1;");
    
    TestSQLStatement("CREATE INDEX ndx1 ON Availability (NodeId, DistroId);");
    TestSQLStatement("SELECT DISTINCT NodeId, COUNT(BlockID) FROM Availability WHERE FileId=10 GROUP BY NodeId HAVING BlockID>2 ORDER BY BlockID LIMIT 10;");
    TestSQLStatement("DROP INDEX ndx2 on Availability;");
    TestSQLStatement("DROP TABLE Availability;");
    
    return 0;
}