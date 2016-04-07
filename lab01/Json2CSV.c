//
//  Json2CSV.c
//  lab01
//
//  Created by Scott Tucker on 3/30/16.
//

#include "Json2CSV.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#include "smartalloc.h"

#include "jansson.h"

DataFile* buildNewDataFile(DataFiles *dataFiles, const char *name, const char *fileName)
{
    char *filePath;
    DataFile *new_file = dataFiles->main;
    size_t len;
    
    while(new_file->nextFile != NULL)
        new_file = new_file->nextFile;
    
    new_file->nextFile = calloc(1, sizeof(DataFile));
    new_file = new_file->nextFile;
    
    filePath = calloc(strlen(dataFiles->outputPath) + strlen(dataFiles->name) + strlen(fileName) + 4, sizeof(char*));
    strcpy(filePath, dataFiles->outputPath);
    strcat(filePath, dataFiles->name);
    len = strlen(filePath);
    strcat(filePath, fileName);
    strcat(filePath, ".csv");
    
    filePath[len] = toupper(filePath[len]);
    
    
    new_file->fp = fopen(filePath, "w+");
    new_file->fileName = calloc(strlen(name), sizeof(char*));
    strcpy(new_file->fileName, name);
    new_file->headerNdx = 0;
    new_file->arrayDepth = 0;
    new_file->numCols = 0;
    
    free(filePath);
    return new_file;
}

//gets the length of the headers if we were to concatenate all headers together
//into one string
int getHeadersLength(HeaderNode *first) {
    HeaderNode *header = first;
    int len = 0;
    int numHeaders = 0;
    
    while (header != NULL) {
        len += strlen(header->headerName);
        numHeaders++;
        header = header->nextHeader;
    }
    
    return len + numHeaders;
}

void strArrayToCSV(FILE *fp, char **arr, size_t arrLen)
{
    size_t len, i;
    char *buffer;
    
    len = arrLen;
    for(i=0; i<arrLen; i++)
    {
        if(arr[i] != NULL) {
            //printf("okaaaaaaaaaaaaaaaa: %s\n", arr[i]);
            len += strlen(arr[i]);
        }
    }
    
    buffer = calloc(len + 1, sizeof(char));
    
    for(i=0; i<arrLen; i++)
    {
        if(i>0)
        	strcat(buffer, ",");
        if(arr[i] != NULL) {
        	strcat(buffer, arr[i]);
        }
    }
    strcat(buffer, "\n");

    fputs(buffer, fp);
    
    free(buffer);
}

//this function writes all the headers to a specified file
void writeHeadersToCSV(FILE *fp, HeaderNode *first) {
    size_t len = getHeadersLength(first);
    char *buffer;
    HeaderNode *header = first;
    
    buffer = calloc(len + 1, sizeof(char));
    
    while (header != NULL) {
        if (header->headerName != NULL) {
            strcat(buffer, header->headerName);
        }
        
        if (header->nextHeader != NULL) {
            strcat(buffer, ",");
        }
        header = header->nextHeader;
    }
    
    strcat(buffer, "\n");
    fputs(buffer, fp);
    free(buffer);
}

//this function writes all of the headers to every file
void writeHeaders(DataFiles *df) {
    DataFile *file = df->main;
    
    while (file != NULL) {
        writeHeadersToCSV(file->fp, file->firstHeader);
        file = file->nextFile;
    }
}

void freeStrArray(char **arr, size_t arrLen)
{
    size_t i;
    for(i=0; i<arrLen; i++)
    {
        if(arr[i] != NULL) {
        	free(arr[i]);
            arr[i] = NULL;
        }
    }
}

int writeStringToArr(char **data, HeaderNode *firstNode, size_t numCols, const char *key, char *str)
{
    size_t i = 0, col = -1;
    
    HeaderNode *temp = firstNode;
    while(temp) {
        if(strcmp(temp->headerName, key) == 0)
        {
            col = i;
            break;
        }
        i++;
        temp = temp->nextHeader;
    }

    if(col == -1)
    {
        fprintf(stderr, "ERROR: can't find key \"%s\" in header\n", key);
        return 6;
    }
    
    data[col] = calloc(strlen(str) + 1, sizeof(char));
    strcpy(data[col], str);
    
    return 0;
}


void convertJSONtoString(json_t* value, char* buffer) {
    if(json_is_string(value))
    {
        sprintf(buffer,"\"%s\"",json_string_value(value));
    }
    else if(json_is_integer(value))
    {
        sprintf(buffer,"%lld",json_integer_value(value));
    }
    else if(json_is_real(value))
    {
        sprintf(buffer,"%f",json_real_value(value));
    }
    else if(json_is_true(value))
    {
        strcpy(buffer,"true");
    }
    else if(json_is_false(value))
    {
        strcpy(buffer,"false");
    }
    else if(json_is_null(value))
    {
        strcpy(buffer,"");
    }
}

int writeValueToArr(char **data, HeaderNode *firstNode, size_t numCols, const char *key, json_t *value)
{
    char buffer[1024];
    convertJSONtoString(value, buffer);
    return writeStringToArr(data, firstNode, numCols, key, buffer);
}

int writePkToArr(char **data, json_t *value, int index) {
    char buffer[1024];
    convertJSONtoString(value, buffer);
    data[index] = calloc(strlen(buffer) + 1, sizeof(char));
    strcpy(data[index], buffer);
    return 0;
}

DataFile* getDataFileByName(DataFile *df, const char *name){
    if(df == NULL) {
        return NULL;
    }
    else if(strcmp(df->fileName, name) == 0)
        return df;
    else
        return getDataFileByName(df->nextFile, name);
    
}

//This function adds a header to the headers object in the datafile
void addHeader(DataFile *file, const char *key) {
    HeaderNode *headerNode = calloc(1, sizeof(HeaderNode));
    file->numCols++; 
    headerNode->headerName = calloc(strlen(key) + 1, sizeof(char));
    strcpy(headerNode->headerName, key);
    
    if (file->firstHeader == NULL) {
        file->firstHeader = headerNode;
        file->lastHeader = headerNode;
    }
    else {
        file->lastHeader->nextHeader = headerNode;
        file->lastHeader = headerNode;
    }
}

//raw speed
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
     and adjust this final return as well. */
    return 10;
}

//gets new datafile
DataFile* getNewDataFile(DataFiles *dataFiles, DataFile *dataFile, const char *key, int outer) {
    const char *fileName = NULL;
    char *tempFileName = NULL;
    DataFile *file;
    
    if (outer == 1) {
        fileName = key;
    }
    else {
        tempFileName = calloc(strlen(dataFile->fileName) + strlen(key) + 1, sizeof(char));
        strcpy(tempFileName, dataFile->fileName);
        strcat(tempFileName, key);
        tempFileName[strlen(dataFile->fileName)] = toupper(tempFileName[strlen(dataFile->fileName)]);
        fileName = tempFileName;
    }
    
    file = buildNewDataFile(dataFiles, key, fileName);
    
    if (tempFileName != NULL) {
        free(tempFileName);
    }
    
    return file;
}

json_t* getNewPKHeader(json_t* primaryKey, char *headerName) {
    json_t* newPK = json_array();
    json_array_extend(newPK, primaryKey);
    json_array_append_new(newPK, json_string(headerName));
    return newPK;
}

//This function recursively determines all headers needed and writes them to CSV files
void parseHeaders(json_t *record, DataFiles *dataFiles, char *fileName, json_t *primaryKey, int outer) {
    json_t *value, *val, *firstRecord, *newPK;
    const char *key;
    size_t index;
    char *positionHeader, *pkName;
    DataFile *dataFile = getDataFileByName(dataFiles->main, fileName);
    DataFile *file;
    
    if (json_array_size(primaryKey) == 0) {
        pkName = malloc(snprintf(NULL, 0, "%s%s", dataFiles->name, "Id") + 1);
        sprintf(pkName, "%s%s", dataFiles->name, "Id");
        json_array_append_new(primaryKey, json_string(pkName));
        free(pkName);
    }
    
    //if we're looking at an object
    if (json_is_object(record)) {
        json_object_foreach(record, key, value) {
            if (json_is_array(value)) {
                file = getNewDataFile(dataFiles, dataFile, key, outer);

                //add position header
                positionHeader = calloc(8 + numPlaces(file->arrayDepth) + 1, sizeof(char));
                sprintf(positionHeader, "%s%d", "position", file->arrayDepth++);
                newPK = getNewPKHeader(primaryKey, positionHeader);
                free(positionHeader);
                
                json_array_foreach(newPK, index, val) {
                    addHeader(file, json_string_value(val));
                }
                
                parseHeaders(value, dataFiles, file->fileName, newPK, 0);
            }
            //if record is object handle file creation
            //then recurse through for sub objects
            else if (json_is_object(value)) {
                file = getNewDataFile(dataFiles, dataFile, key, outer);
                
                //add pks to new file if not outer file
                json_array_foreach(primaryKey, index, val) {
                    addHeader(file, json_string_value(val));
                }
                
                parseHeaders(value, dataFiles, file->fileName, primaryKey, 0);
            }
            //if just regular value
            else {
                addHeader(dataFile, key);
            }
        }
    }
    else if (json_is_array(record)) {
        firstRecord = json_array_get(record, 0);
        
        if (json_is_array(firstRecord)) {
            positionHeader = calloc(8 + numPlaces(dataFile->arrayDepth) + 1, sizeof(char));
            sprintf(positionHeader, "%s%d", "position", dataFile->arrayDepth++);
            newPK = getNewPKHeader(primaryKey, positionHeader);
            free(positionHeader);
            
            parseHeaders(firstRecord, dataFiles, fileName, newPK, 0);
        }
        else if (json_is_object(firstRecord)) {
            parseHeaders(firstRecord, dataFiles, fileName, primaryKey, 0);
        }
        else {
            addHeader(dataFile, "value");
        }
    }
    else {
        if (json_is_string(record)) {
            printf("record: %s\n", json_string_value(record));
        }
    }
}

json_t* getNewPK(json_t* primaryKey, size_t indx) {
    json_t* newPK = json_array();
    json_array_extend(newPK, primaryKey);
    json_array_append_new(newPK, json_integer(indx));
    return newPK;
}

void printPK(json_t* pk) {
    size_t index;
    json_t *value;
    json_array_foreach(pk, index, value) {
        printf("pk%zu: %" JSON_INTEGER_FORMAT ", ", index, json_integer_value(value));
    }
    printf("\n");
}

//inserting data
int parseJSON(json_t *record, DataFiles *dataFiles, json_t *primaryKey, const char* tableName) {
    json_t *value, *newPK = NULL;
    size_t index;
    const char *key;
    int outer = 0;
    //char *positionHeader;
    //DataFile *dataFile = getDataFileByName(dataFiles->main, fileName);
    DataFile *file = getDataFileByName(dataFiles->main, tableName);

    if(json_is_object(record)) {
        if(json_array_size(primaryKey) == 0) {
            outer = 1;
            json_array_append_new(primaryKey, json_object_get(record, "id"));
        } else {
            outer = 0;
        }
        json_object_foreach(record, key, value) {
            if(json_is_object(value)) {
                parseJSON(value, dataFiles, primaryKey, key);
            } else if(json_is_array(value)) {
                parseJSON(value, dataFiles, primaryKey, key);
            } else {
                //printf("1WRITING TO FILE %s for key %s numCols: %d\n",tableName, key, file->numCols);
                //printPK(primaryKey);
                //write

                writeValueToArr(file->child_data, file->firstHeader, file->numCols, key, value);
            }
        }
    } else if(json_is_array(record)) { //json is array
        json_array_foreach(record, index, value) {
            newPK = getNewPK(primaryKey, index);
            parseJSON(value, dataFiles, newPK, tableName);

            //major hack alert?
            if(index == json_array_size(record)-1) {
                return 0;
            }
        }

    } else {
                //printf("2WRITING TO FILE %s for key %s numCols: %d\n",tableName, "value", file->numCols);
        //atomic values in array
        writeValueToArr(file->child_data, file->firstHeader, file->numCols, "value", record);
    }

    //lets use newPK only
    if(!newPK) {
        newPK = primaryKey;
    }

    //write PK's to array
    json_array_foreach(newPK, index, value) {
        if(outer == 0) {
            writePkToArr(file->child_data, value, index); 
        }
    }

    //write data to file 
    strArrayToCSV(file->fp, file->child_data, file->numCols);
    //free str array
    /*int i;
    for(i = 0; i < file->numCols; i++) {
        file->child_data[i] = 0;
    }*/
    freeStrArray(file->child_data, file->numCols);
    return 0;
}

void createChildData(DataFiles* df) {
    DataFile* temp = df->main;
    while(temp) {
        temp->child_data = calloc(temp->numCols, sizeof(char*));
        temp = temp->nextFile;
    }
}

void printDataFile(DataFile *dataFile) {
    HeaderNode *header = dataFile->firstHeader;
    printf("Filename: %s\n", dataFile->fileName);

    printf("Headers: \n");

    while (header != NULL) {
        printf("%s ", header->headerName);
        header = header->nextHeader;
    }
    printf("\n");
}

void printDataFiles(DataFiles *dataFiles) {
    DataFile *df = dataFiles->main;

    while (df != NULL) {
        printDataFile(df);
        printf("\n");

        df = df->nextFile;
    }
}

int interpRecJson(json_t *json, DataFiles *dataFiles) {
    json_t *record;
    size_t i;
    
    if(!json_is_array(json))
    {
        fprintf(stderr, "ERROR: root element is not an array\n");
        return 4;
    }
    
    for (i = 0; i < json_array_size(json); i++) {
        record = json_array_get(json, i);
        
        if(!json_is_object(record))
        {
            fprintf(stderr, "ERROR: first level element is not an object\n");
            return 5;
        }
        
        //looking at first object
        //make headers
        if (i == 0) {
            parseHeaders(record, dataFiles, dataFiles->main->fileName, json_array(), 1);
            writeHeaders(dataFiles);
            createChildData(dataFiles);
            //printDataFiles(dataFiles);
        }
        
        //add data in
        parseJSON(record, dataFiles, json_array(), dataFiles->main->fileName);
    }
    
    return 0;
}

//Initial build of datafiles object
DataFiles* buildDataFileStruct(char *inputFile)
{
    char *output_path, *ptr, *filename, *name;
    DataFiles *dataFiles;
    
    //Get output_path, filename, & name
    output_path = calloc(strlen(inputFile) + 1, sizeof(char));
    filename = calloc(strlen(inputFile) + 7, sizeof(char));
    name = calloc(strlen(inputFile) + 1, sizeof(char));
    
    ptr = strrchr(inputFile, '/');
    if(ptr == NULL) {
        strcpy(output_path, "./");
        strcpy(name, inputFile);
    }
    else {
        strncpy(output_path, inputFile, strlen(inputFile)-strlen(ptr)+1);
        ptr++;
        strcpy(name, ptr);
    }
    
    ptr = strstr(name, ".");
    
    if(ptr != NULL)
    	strcpy(ptr,"");
    
    name[0] = toupper(name[0]);
    
    strcpy(filename, output_path);
    strcat(filename, name);
    strcat(filename, ".csv");
    
    
    //build dataFiles
    dataFiles = calloc(1, sizeof(DataFiles));
    dataFiles->outputPath = output_path;
    dataFiles->name = name;
    dataFiles->nameLen = strlen(name);
    
    //build main dataFile
    dataFiles->main = calloc(1, sizeof(DataFile));
    dataFiles->main->fp = fopen(filename, "w+");
    dataFiles->main->fileName = filename;
    dataFiles->main->headerNdx = 0;
    
    return dataFiles;
}

void freeHeaders(DataFile *df) {
    HeaderNode *header = df->firstHeader, *temp;
    
    while (header != NULL) {
        temp = header;
        header = header->nextHeader;
        
        free(temp->headerName);
        free(temp);
    }
}

//This function cleans up the datafile at the end
void cleanUpDataFile(DataFile *df)
{
    if(df->nextFile != NULL)
    	cleanUpDataFile(df->nextFile);
    
    free(df->child_data);
    free(df->fileName);
    fclose(df->fp);
    
    freeHeaders(df);
    free(df);
}

int main(int argc, char *argv[])
{
    FILE *fp;
    json_t *root, *temp = NULL;
    json_error_t error;
    int eof = 0, ret;
    DataFiles *dataFiles;
    
    if(argc < 2)
    {
        fprintf(stderr, "ERROR: This program requires an agrument that contains the path to the JSON file to process.\n");
        return 1;
    }
    
    //open src file
    fp = fopen(argv[1], "r");
    if(!fp)
    {
        fprintf(stderr, "ERROR: Could not open JSON file.\n");
        return 2;
    }
    
    //build output data file stuct
    dataFiles = buildDataFileStruct(argv[1]);
    
    //read in JSON
    temp = json_loadf(fp, JSON_DISABLE_EOF_CHECK, &error);
    if(!temp)
    {
        fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
        fclose(fp);
        return 3;
    }
    
    if(json_is_array(temp))
        root = temp;
    else
    {
        root = json_array();
        json_array_append_new(root, temp);
        
        while(!eof)
        {
            temp = json_loadf(fp, JSON_DISABLE_EOF_CHECK, &error);
            
            if(temp)
                json_array_append_new(root, temp);
            else
                eof = 1;
        }
    }
    
    //Process JSON
    //outputJsonType(root);
//    ret = interpJson(root, dataFiles);
    ret = interpRecJson(root, dataFiles);
    
    //Clean-up
    cleanUpDataFile(dataFiles->main);
    free(dataFiles->outputPath);
    free(dataFiles->name);
    free(dataFiles);
    json_decref(root);
    fclose(fp);
    
    report_space();
    
    return ret;
}
