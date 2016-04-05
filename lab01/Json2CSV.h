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

typedef struct DataFile {
    char *fileName;
    FILE *fp;
    char **headers;
    size_t numCols;
    struct DataFile *nextFile;
} DataFile;

typedef struct DataFiles {
    DataFile *main;
    char *outputPath;
    char *name;
    size_t nameLen;
} DataFiles;

int getColumnsArray(json_t *record, DataFiles *dataFiles, int j);
int getColumnsObject(json_t *record, DataFiles *dataFiles, int j);

#endif /* Json2CSV_h */
