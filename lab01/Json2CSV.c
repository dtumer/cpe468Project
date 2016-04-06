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

DataFile* buildNewDataFile(DataFiles *dataFiles, const char *name)
{
    char *filePath;
    DataFile *new_file = dataFiles->main;
    size_t len;
    
    while(new_file->nextFile != NULL)
        new_file = new_file->nextFile;
    
    new_file->nextFile = calloc(1, sizeof(DataFile));
    new_file = new_file->nextFile;
    
    filePath = calloc(strlen(dataFiles->outputPath) + strlen(dataFiles->name) + strlen(name) + 4, sizeof(char*));
    strcpy(filePath, dataFiles->outputPath);
    strcat(filePath, dataFiles->name);
    len = strlen(filePath);
    strcat(filePath, name);
    strcat(filePath, ".csv");
    
    filePath[len] = toupper(filePath[len]);
    
    
    new_file->fp = fopen(filePath, "w+");
    new_file->fileName = calloc(strlen(name), sizeof(char*));
    strcpy(new_file->fileName, name);
    new_file->headerNdx = 0;
    new_file->arrayDepth = 0;
    
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
    for(size_t i=0; i<arrLen; i++)
    {
        if(arr[i] != NULL)
        	free(arr[i]);
    }
    free(arr);
}

int writeStringToArr(char **data, char **headers, size_t numCols, const char *key, char *str)
{
    size_t i, col = -1;
    
    for(i=0; i<numCols; i++)
    {
        if(strcmp(headers[i], key) == 0)
        {
            col = i;
            break;
        }
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

int writeValueToArr(char **data, char **headers, size_t numCols, const char *key, json_t *value)
{
    char buffer[1024];
    
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
    
    return writeStringToArr(data, headers, numCols, key, buffer);
}

DataFile* getDataFileByName(DataFile *df, const char *name){
    if(df == NULL)
        return NULL;
    else if(strcmp(df->fileName, name) == 0)
        return df;
    else
        return getDataFileByName(df->nextFile, name);
    
}

//void printDataFile(DataFile *dataFile) {
//    HeaderNode *header = dataFile->firstHeader;
//    printf("Filename: %s\n", dataFile->fileName);
//    
//    printf("Headers: \n");
//    
//    while (header != NULL) {
//        printf("%s ", header->headerName);
//        header = header->nextHeader;
//    }
//    printf("\n");
//}
//
//void printDataFiles(DataFiles *dataFiles) {
//    DataFile *df = dataFiles->main;
//    
//    while (df != NULL) {
//        printDataFile(df);
//        printf("\n");
//        
//        df = df->nextFile;
//    }
//}

//This function adds a header to the headers object in the datafile
void addHeader(DataFile *file, const char *key) {
    HeaderNode *headerNode = calloc(1, sizeof(HeaderNode));
    
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
    
    file = buildNewDataFile(dataFiles, fileName);
    
    if (tempFileName != NULL) {
        free(tempFileName);
    }
    
    return file;
}

//This function recursively determines all headers needed and writes them to CSV files
void parseHeaders(json_t *record, DataFiles *dataFiles, char *fileName, char *pkName, int outer) {
    json_t *value, *firstRecord;
    const char *key;
    char *positionHeader;
    DataFile *dataFile = getDataFileByName(dataFiles->main, fileName);
    DataFile *file;
    
    //if we're looking at an object
    if (json_is_object(record)) {
        json_object_foreach(record, key, value) {
            if (json_is_array(value)) {
                file = getNewDataFile(dataFiles, dataFile, key, outer);

                addHeader(file, pkName);
                positionHeader = calloc(8 + numPlaces(file->arrayDepth) + 1, sizeof(char));
                sprintf(positionHeader, "%s%d", "position", file->arrayDepth++);
                addHeader(file, positionHeader);
                free(positionHeader);

                parseHeaders(value, dataFiles, file->fileName, pkName, 0);
            }
            //if record is object handle file creation
            //then recurse through for sub objects
            else if (json_is_object(value)) {
                file = getNewDataFile(dataFiles, dataFile, key, outer);
                
                addHeader(file, pkName);
                
                parseHeaders(value, dataFiles, file->fileName, pkName, 0);
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
            addHeader(dataFile, positionHeader);
            free(positionHeader);
            
            parseHeaders(firstRecord, dataFiles, fileName, pkName, 0);
        }
        else if(json_is_object(firstRecord)) {
            parseHeaders(firstRecord, dataFiles, fileName, pkName, 0);
        }
        else {
            addHeader(dataFile, "value");
        }
    }
}

//inserting data
int parseJSON(json_t *record, DataFiles *dataFiles, size_t recordNum) {
//    DataFile *main_file = dataFiles->main;

    return 0;
}


int interpRecJson(json_t *json, DataFiles *dataFiles) {
    json_t *record;
    size_t i;
    char *pkName;
    
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
            pkName = malloc(snprintf(NULL, 0, "%s%s", dataFiles->name, "Id") + 1);
            sprintf(pkName, "%s%s", dataFiles->name, "Id");
            
            parseHeaders(record, dataFiles, dataFiles->main->fileName, pkName, 1);
            writeHeaders(dataFiles);
            
            free(pkName);
        }
        
        //add data in
        parseJSON(record, dataFiles, i);
    }
    
    return 0;
}

int interpJson(json_t *json, DataFiles *dataFiles)
{
    json_t *record, *value, *child_value, *array_value;
    const char *key, *child_key;
    DataFile *file, *main_file = dataFiles->main;
    char **data, **child_data, *idName, buffer[1024], idStr[1024];
    size_t i, j, k, idNameSize;
    
    idNameSize = dataFiles->nameLen + 3;
    idName = calloc(idNameSize, sizeof(char));
    strcpy(idName, dataFiles->name);
    strcat(idName, "Id");
    
    if(!json_is_array(json))
    {
        fprintf(stderr, "ERROR: root element is not an array\n");
        return 4;
    }
    
    for(i = 0; i < json_array_size(json); i++)
    {
        //printf("%zu\n", i);
        record = json_array_get(json, i);
        
        if(!json_is_object(record))
        {
            fprintf(stderr, "ERROR: first level element is not an object\n");
            return 5;
        }
        
        if(i == 0)
        {
            main_file->numCols = json_object_size(record);
           	main_file->headers = calloc(main_file->numCols, sizeof(char*));
            
            j=0;
            json_object_foreach(record, key, value)
            {
                if(json_is_array(value))
                {
                	main_file->numCols--;
                    file = buildNewDataFile(dataFiles, key);
                    
                    array_value = json_array_get(value, 0);
                    
                    if(json_is_object(array_value))
                    {
                        file->numCols = json_object_size(array_value) + 2;
                        file->headers = calloc(file->numCols, sizeof(char*));
                        
                        file->headers[0] = calloc(idNameSize, sizeof(char));
                        strcpy(file->headers[0], idName);
                        
                        file->headers[1] = calloc(9, sizeof(char));
                        strcpy(file->headers[1], "position");
                        
                        k = 2;
                        json_object_foreach(array_value, child_key, child_value)
                        {
                            file->headers[k] = calloc(strlen(child_key) + 1, sizeof(char));
                            strcpy(file->headers[k], child_key);
                            k++;
                        }
                    }
                    // Potentially expand out to arrays of arrays
                    else
                    {
                        file->numCols = 3;
                        file->headers = calloc(3, sizeof(char*));
                        
                        file->headers[0] = calloc(idNameSize, sizeof(char));
                        strcpy(file->headers[0], idName);
                        
                        file->headers[1] = calloc(9, sizeof(char));
                        strcpy(file->headers[1], "position");
                        
                        file->headers[2] = calloc(6, sizeof(char));
                        strcpy(file->headers[2], "value");
                        
                        
                    }
//                    strArrayToCSV(file->fp, file->headers, file->numCols);
                }
                else if(json_is_object(value))
                {
					main_file->numCols--;
                    file = buildNewDataFile(dataFiles, key);
                    
                    file->numCols = json_object_size(value) + 1;
                    file->headers = calloc(file->numCols, sizeof(char*));
                    
                    file->headers[0] = calloc(idNameSize, sizeof(char));
                    strcpy(file->headers[0], idName);
                    
                    k = 1;
                    json_object_foreach(value, child_key, child_value)
                    {
                        file->headers[k] = calloc(strlen(child_key) + 1, sizeof(char));
                        strcpy(file->headers[k], child_key);
                        k++;
                    }
                    
//                    strArrayToCSV(file->fp, file->headers, file->numCols);
                }
                else
                {
                    main_file->headers[j] = calloc(strlen(key) + 1, sizeof(char));
                    strcpy(main_file->headers[j], key);
                    j++;
                }
                
            }
//            strArrayToCSV(main_file->fp, main_file->headers, main_file->numCols);
        }
        
        json_object_foreach(record, key, value) {
            if(strcmp(key, "id") == 0)
            {
                sprintf(idStr,"%lld", json_integer_value(value));
                break;
            }
        }
        
        data = calloc(main_file->numCols, sizeof(char*));
        
        json_object_foreach(record, key, value)
        {
            if(json_is_array(value))
            {
                file = getDataFileByName(main_file, key);
                
                for(j = 0; j < json_array_size(value); j++)
                {
                    child_data = calloc(file->numCols, sizeof(char*));
                    
                    writeStringToArr(child_data, file->headers, file->numCols, idName, idStr);
                    
                    sprintf(buffer,"%zu", j);
                    writeStringToArr(child_data, file->headers, file->numCols, "position", buffer);
                    
                    array_value = json_array_get(value, j);
                	
                    if(json_is_object(array_value))
                    {
                        json_object_foreach(array_value, child_key, child_value)
                        {
                            writeValueToArr(child_data, file->headers, file->numCols, child_key, child_value);
                        }
                    }
                    else
                    {
                        writeValueToArr(child_data, file->headers, file->numCols, "value", array_value);
                    }
                
                	
//                    strArrayToCSV(file->fp, child_data, file->numCols);
                    freeStrArray(child_data, file->numCols);
                }
            }
            else if(json_is_object(value))
            {
                file = getDataFileByName(main_file, key);
                
                child_data = calloc(file->numCols, sizeof(char*));
                
                writeStringToArr(child_data, file->headers, file->numCols, idName, idStr);
                
                json_object_foreach(value, child_key, child_value)
                {
                    writeValueToArr(child_data, file->headers, file->numCols, child_key, child_value);
                }
                
//                strArrayToCSV(file->fp, child_data, file->numCols);
                freeStrArray(child_data, file->numCols);
            }
            else
            {
                writeValueToArr(data, main_file->headers, main_file->numCols, key, value);
            }
        }
        
//        strArrayToCSV(main_file->fp, data, main_file->numCols);
        freeStrArray(data, main_file->numCols);
    }
    
    free(idName);
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


