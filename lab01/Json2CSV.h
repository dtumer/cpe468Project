//
//  Json2CSV.h
//  lab01
//
//  Created by Scott Tucker on 3/31/16.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jansson.h"

#ifndef Json2CSV_h
#define Json2CSV_h

typedef struct HeaderNode {
    char *headerName;
    struct HeaderNode *nextHeader;
} HeaderNode;

typedef struct DataFile {
    char *fileName;
    FILE *fp;
    HeaderNode *firstHeader;
    HeaderNode *lastHeader;
    int arrayDepth;
    int numCols;
    char **headers;
    int headerNdx;
    struct DataFile *nextFile;
    char **child_data;
} DataFile;

typedef struct DataFiles {
    DataFile *main;
    char *outputPath;
    char *name;
    size_t nameLen;
} DataFiles;

void parseHeaders(json_t *record, DataFiles *dataFiles, char *fileName, char *pkName, int outer);

#endif /* Json2CSV_h */
