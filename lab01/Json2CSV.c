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
    
    free(filePath);
    return new_file;
}

void strArrayToCSV(FILE *fp, char **arr, size_t arrLen)
{
    size_t len;
    char *buffer;
    
    len = arrLen;
    for(size_t i=0; i<arrLen; i++)
    {
        if(arr[i] != NULL)
            len += strlen(arr[i]);
    }
    
    buffer = calloc(len + 1, sizeof(char));
    
    for(size_t i=0; i<arrLen; i++)
    {
        if(i>0)
        	strcat(buffer, ",");
        if(arr[i] != NULL)
        	strcat(buffer, arr[i]);
    }
    strcat(buffer, "\n");

    fputs(buffer, fp);
    
    free(buffer);
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

void printDataFile(DataFile *dataFile) {
    printf("Filename: %s\n", dataFile->fileName);
    
    printf("HEADERS: \n");
    for (int i = 0; i < dataFile->numCols; i++) {
        printf("%s\n", dataFile->headers[i]);
    }
}

void printDataFiles(DataFiles *dataFiles) {
    printDataFile(dataFiles->main);
    printf("Output path: %s\n", dataFiles->outputPath);
    printf("Name: %s\n", dataFiles->name);
    printf("\n");
}

void parseHeaders(json_t *record, DataFiles *dataFiles, char *fileName, json_t *primaryKey) {
    json_t *value;
    const char *key;
    DataFile *dataFile = getDataFileByName(dataFiles->main, fileName);
    int headerNdx = 0;
    
    //if we're looking at an object
    if (json_is_object(record)) {
        //getting id to pass down to child data files
        if (primaryKey == NULL) {
            primaryKey = json_object();
            json_object_set_new(primaryKey, "id", json_object_get(record, "id"));
        }
        
        json_object_foreach(record, key, value) {
            if (json_is_array(value)) {

            }
            //if record is object handle file creation
            //then recurse through for sub objects
            else if (json_is_object(value)) {

            }
            //if just regular value
            else {
                headerNdx = dataFile->headerNdx;
                dataFile->headers[headerNdx] = calloc(strlen(key) + 1, sizeof(char));
                strcpy(dataFile->headers[headerNdx], key);
                dataFile->headerNdx++;
            }
        }
    }
    else if (json_is_array(record)) {
        json_object_foreach(record, key, value) {
            if (json_is_array(value)) {
//                parseHeaders(value, dataFiles, primaryKey);
            }
            else if (json_is_object(value)) {
//                (value, dataFiles, primaryKey);
            }
            else {
//                main_file->headers[j] = calloc(strlen(key) + 1, sizeof(char));
//                strcpy(main_file->headers[j], key);
//                j++;
            }
        }
    }
    else {
        
    }
}

//inserting data
int parseJSON(json_t *record, DataFiles *dataFiles, size_t recordNum) {
    DataFile *main_file = dataFiles->main;

    return 0;
}


int interpRecJson(json_t *json, DataFiles *dataFiles) {
    json_t *record;
    DataFile *main_file = dataFiles->main;
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
            main_file->numCols = json_object_size(record);
            main_file->headers = calloc(main_file->numCols, sizeof(char*));
            
            parseHeaders(record, dataFiles, dataFiles->main->fileName, NULL);
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
                    strArrayToCSV(file->fp, file->headers, file->numCols);
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
                    
                    strArrayToCSV(file->fp, file->headers, file->numCols);
                }
                else
                {
                    main_file->headers[j] = calloc(strlen(key) + 1, sizeof(char));
                    strcpy(main_file->headers[j], key);
                    j++;
                }
                
            }
            strArrayToCSV(main_file->fp, main_file->headers, main_file->numCols);
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
                
                	
                    strArrayToCSV(file->fp, child_data, file->numCols);
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
                
                strArrayToCSV(file->fp, child_data, file->numCols);
                freeStrArray(child_data, file->numCols);
            }
            else
            {
                writeValueToArr(data, main_file->headers, main_file->numCols, key, value);
            }
        }
        
        strArrayToCSV(main_file->fp, data, main_file->numCols);
        freeStrArray(data, main_file->numCols);
    }
    
    free(idName);
    return 0;
}

void outputJsonType(json_t *json)
{
    if(json_is_array(json))
    {
        printf("elm is an array\n");
        
        for(size_t i = 0; i < json_array_size(json); i++)
        {
            outputJsonType(json_array_get(json, i));
        }
        printf("\tend array\n");
    }
    else if(json_is_object(json))
    {
        printf("elm is an object\n");
        
        const char *key;
        json_t *value;
        
        json_object_foreach(json, key, value) {
            printf("key: %s\n", key);
            outputJsonType(value);
        }
        
        printf("\tend object\n");
    }
    else if(json_is_string(json))
    {
    	printf("elm is string: %s\n", json_string_value(json));
    }
    else if(json_is_integer(json))
    {
        printf("elm is int: %lld\n", json_integer_value(json));
    }
    else if(json_is_real(json))
    {
        printf("elm is real: %f\n", json_real_value(json));
    }
    else if(json_is_true(json))
    {
        printf("elm is true\n");
    }
    else if(json_is_false(json))
    {
        printf("elm is false\n");
    }
    else if(json_is_null(json))
    {
        printf("elm is null\n");
    }
    
}

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

void cleanUpDataFile(DataFile *df)
{
    if(df->nextFile != NULL)
    	cleanUpDataFile(df->nextFile);
    
    free(df->fileName);
    fclose(df->fp);
    
    for(size_t i=0; i<df->numCols; i++)
    {
        free(df->headers[i]);
    }
    if(df->headers != NULL)
	    free(df->headers);
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


