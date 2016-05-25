#include <stdlib.h>
#include <string>
#include <iostream>

#include "FLOPPYParser.h"

int main(int argc, char *argv[]) {
   if (argc > 1) {
      std::string query = std::string(argv[1]);

      FLOPPYOutput *result = FLOPPYParser::parseFLOPPYString(query);

      if (result->isValid) {
         printf("Parsed successfully!\n");
      } else {
         printf("Invalid FLOPPY!\n");
      }
   }
   return 0;
}
