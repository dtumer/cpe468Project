#include <stdio.h>
#include <limits.h>

#include "hashmap.h"
#include "smartalloc.h"

typedef struct DiskAddress {
   int FD;
   int pageId; 
} DiskAddress;

//determine number of spaces for integer
int numPlaces(int n) {
    if (n < 0) n = (n == INT_MIN) ? INT_MAX : -n;
    if (n < 10) return 1;
    if (n < 100) return 2;
    if (n < 1000) return 3;
    if (n < 10000) return 4;
    if (n < 100000) return 5;
    if (n < 1000000) return 6;
    if (n < 10000000) return 7;
    if (n < 100000000) return 8;
    if (n < 1000000000) return 9;
    /*      2147483647 is 2^31-1 - add more ifs as needed
 *           and adjust this final return as well. */
    return 10;
}

char* diskAddressToString(DiskAddress diskAdd) {
   int lenFD = numPlaces(diskAdd.FD);
   int lenPageId = numPlaces(diskAdd.pageId);
   char *str = calloc(lenFD + 1 + lenPageId + 1, sizeof(char));

   sprintf(str, "%d,%d", diskAdd.FD, diskAdd.pageId);
   
   return str;
}

int main(int argv, char *argc[]) {
   
   map_t diskMap;
   DiskAddress *value;
   char *diskAdd;
   int error, *index, *retValue;

   value = calloc(1, sizeof(DiskAddress));
   value->FD = 1;
   value->pageId = 3;
   index = malloc(sizeof(int));
   *index = 23;

   //create hashmap
   diskMap = hashmap_new();

   //create string key from the diskaddress
   diskAdd = diskAddressToString(*value);
   //store index value into hashmap
   error = hashmap_put(diskMap, diskAdd, index);
   
   //get the value from the hashmap
   error = hashmap_get(diskMap, diskAdd, (void**)&retValue);
   //free the key value
   free(diskAdd);

   printf("INDEX: %d\n", *retValue);
   
   
   diskAdd = diskAddressToString(*value);
   printf("hashmap size before remove: %d\n", hashmap_length(diskMap));
  
   
   
   error = hashmap_get(diskMap, diskAdd, (void **)&retValue);
   printf("error should be 0: %d\n", error);
   error = hashmap_remove(diskMap, diskAdd);
   free(diskAdd);
   printf("hashmap size after remove: %d\n", hashmap_length(diskMap));
   free(index);
   hashmap_free(diskMap);
   free(value);
   
   return 0;
}
