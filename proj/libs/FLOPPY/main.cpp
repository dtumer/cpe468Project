#include <stdlib.h>
#include <string>
#include <iostream>

#include "FLOPPYParser.h"

int main(int argc, char *argv[]) {
   std::string query = std::string("CREATE TABLE floppy VOLATILE (dekhtyar INT, PRIMARY KEY(dekhtyar));");

   FLOPPYOutput *result = FLOPPYParser::parseFLOPPYString(query);

   if (result->isValid) {
      printf("Parsed successfully!\n");
   } else {
      printf("Invalid FLOPPY!\n");
   }
   return 0;
}
