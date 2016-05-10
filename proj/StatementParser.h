//
//  StatementParser.h
//  proj
//
//  Created by David Ellison on 5/3/16.
//
//

#ifndef StatementParser_h
#define StatementParser_h

#include <stdio.h>

typedef struct Attributes {
   char *attName;
   int attType; //change to enum
   struct Attributes *next;
} Attribute;

typedef struct FK {
   char *tableName; /* table that this FK references*/
   Attribute *key;
   struct FK *next;
} foreignKeys;

typedef struct Tables {
   char *tableName;
   Attribute *attributeList; /* pointer to head of attributes list*/
   Attribute *pKey;
   foreignKeys * fKeys;
} tableDescription;

//creates the buffer pages necessary for the creation of the table
//also writes to the header page all necessary information
int createPersistentTable(Buffer buf, tableDescription table);

#endif /* StatementParser_h */
