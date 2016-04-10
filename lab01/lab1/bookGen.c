//
//  bookGen.c
//  lab01
//
//  Created by Deniz Tumer on 4/3/16.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jansson.h"

//get the name of a book
json_t* getBookName() {
    char* strings[50];
    strings[0] = "Merry Stories and Funny Pictures";
    strings[1] = "Pride and Prejudice";
    strings[2] = "Alice's Adventures in Wonderland";
    strings[3] = "Moby Dick; Or, The Whale";
    strings[4] = "Adventures of Huckleberry Finn";
    strings[5] = "Sales Talk";
    strings[6] = "A Tale of Two Cities";
    strings[7] = "The Importance of Being Earnest: A Trivial Comedy for Serious People";
    strings[8] = "A Matter of Protocol";
    strings[9] = "The Adventures of Tom Sawyer";
    strings[10] = "Frankenstein; Or, The Modern Prometheus";
    strings[11] = "Narrative of the Life of Frederick Douglass, an American Slave";
    strings[12] = "The Adventures of Sherlock Holmes";
    strings[13] = "Dracula";
    strings[14] = "Emma";
    strings[15] = "Mystery of the Chinese Ring";
    strings[16] = "A Fall of Glass";
    strings[17] = "The Kama Sutra of Vatsyayana";
    strings[18] = "Ulysses";
    strings[19] = "The Picture of Dorian Gray";
    strings[20] = "Metamorphosis";
    strings[21] = "Great Expectations";
    strings[22] = "Leaves of Grass";
    strings[23] = "War and Peace";
    strings[24] = "The Yellow Wallpaper";
    strings[25] = "Solid Solution";
    strings[26] = "Tractatus Logico-Philosophicus";
    strings[27] = "The Romance of Lust";
    strings[28] = "Grimms' Fairy Tales";
    strings[29] = "A Doll's House : a play";
    strings[30] = "Les Miserables";
    strings[31] = "Candide";
    strings[32] = "Dubliners";
    strings[33] = "Jane Eyre: An Autobiography";
    strings[34] = "A Modest Proposal";
    strings[35] = "The Count of Monte Cristo";
    strings[36] = "Sense and Sensibility";
    strings[37] = "The Jungle Book";
    strings[38] = "Treasure Island";
    strings[39] = "The Sorceress";
    strings[40] = "The Divine Comedy";
    strings[41] = "Wuthering Heights";
    strings[42] = "The Silent Call";
    strings[43] = "The Strange Case of Dr. Jekyll and Mr. Hyde";
    strings[44] = "My Secret Life, Volumes I. to III.";
    strings[45] = "Max und Moritz: Eine Bubengeschichte in sieben Streichen";
    strings[46] = "Peter Pan";
    strings[47] = "Leviathan";
    strings[48] = "Heart of Darkness";
    strings[49] = "Harry Potter and the Sorcerer's Stone";
    
    return json_string(strings[rand() % 50]);
}

// get first name of author
json_t* getFirstName() {
    char* strings[50];
    strings[0] = "James";
    strings[1] = "John";
    strings[2] = "Robert";
    strings[3] = "Michael";
    strings[4] = "William";
    strings[5] = "Mary";
    strings[6] = "David";
    strings[7] = "Richard";
    strings[8] = "Joseph";
    strings[9] = "Charles";
    strings[10] = "Thomas";
    strings[11] = "Christopher";
    strings[12] = "Daniel";
    strings[13] = "Patricia";
    strings[14] = "Matthew";
    strings[15] = "Jennifer";
    strings[16] = "Elizabeth";
    strings[17] = "Linda";
    strings[18] = "Barbara";
    strings[19] = "Donald";
    strings[20] = "Anthony";
    strings[21] = "Mark";
    strings[22] = "Paul";
    strings[23] = "Steven";
    strings[24] = "George";
    strings[25] = "Kenneth";
    strings[26] = "Andrew";
    strings[27] = "Edward";
    strings[28] = "Joshua";
    strings[29] = "Brian";
    strings[30] = "Kevin";
    strings[31] = "Susan";
    strings[32] = "Ronald";
    strings[33] = "Margaret";
    strings[34] = "Timothy";
    strings[35] = "Jessica";
    strings[36] = "Jason";
    strings[37] = "Sarah";
    strings[38] = "Dorothy";
    strings[39] = "Karen";
    strings[40] = "Nancy";
    strings[41] = "Betty";
    strings[42] = "Jeffrey";
    strings[43] = "Lisa";
    strings[44] = "Ryan";
    strings[45] = "Gary";
    strings[46] = "Sandra";
    strings[47] = "Nicholas";
    strings[48] = "Eric";
    strings[49] = "Jacob";
    
    return json_string(strings[rand() % 50]);
}

// get last name of author
json_t* getLastName() {
    char* strings[20];
    strings[0] = "Smith";
    strings[1] = "Johnson";
    strings[2] = "Williams";
    strings[3] = "Brown";
    strings[4] = "Jones";
    strings[5] = "Miller";
    strings[6] = "Davis";
    strings[7] = "Garcia";
    strings[8] = "Rodriguez";
    strings[9] = "Wilson";
    strings[10] = "Martinez";
    strings[11] = "Anderson";
    strings[12] = "Taylor";
    strings[13] = "Thomas";
    strings[14] = "Hernandez";
    strings[15] = "Moore";
    strings[16] = "Martin";
    strings[17] = "Jackson";
    strings[18] = "Thompson";
    strings[19] = "White";
    
    return json_string(strings[rand() % 20]);
}

//gets city name
json_t* getCity() {
    char* strings[10];
    strings[0] = "San Luis Obispo";
    strings[1] = "Los Angeles";
    strings[2] = "New York City";
    strings[3] = "Quebec City";
    strings[4] = "San Francisco";
    strings[5] = "Davis";
    strings[6] = "Ann Arbor";
    strings[7] = "Philidelphia";
    strings[8] = "San Diego";
    strings[9] = "Springfield";
    
    return json_string(strings[rand() % 10]);
}

//gets a random json_ingeter with the given range
json_t* getRandJSONInt(int range) {
    return json_integer(rand() % range);
}

//gets address
json_t* getAddress() {
    json_t *address = json_object();
    
    json_object_set_new(address, "number", getRandJSONInt(1000));
    json_object_set_new(address, "city", getCity());
    
    return address;
}

// gets author's name
json_t* getAuthor(int id) {
    json_t *author = json_object();
    
    json_object_set_new(author, "firstName", getFirstName());
    json_object_set_new(author, "lastName", getLastName());
    
    if (id == 1 || rand() % 5 > 0) {
        json_object_set_new(author, "address", getAddress());
    }
    
    return author;
}

// gets publisher name
json_t* getPublisher() {
    char *strings[10];
    
    strings[0] = "Pearson";
    strings[1] = "ThomsonReuters";
    strings[2] = "RELX Group";
    strings[3] = "Wolters Kluwer";
    strings[4] = "Penguin Random House";
    strings[5] = "Phoenix";
    strings[6] = "China South";
    strings[7] = "Hachette Livre";
    strings[8] = "Scholastic";
    strings[9] = "Harper Collins";
    
    return json_string(strings[rand() % 10]);
}

//gets number of pages of the book
json_t* getNumPages() {
    return getRandJSONInt(1000);
}

//gets the edition of the book
json_t* getEdition() {
    return getRandJSONInt(10);
}

//gets the genres of the book
json_t* getGenres(int recordId) {
    char* strings[10];
    strings[0] = "Horror";
    strings[1] = "Mystery";
    strings[2] = "Crime";
    strings[3] = "Romance";
    strings[4] = "Fantasy";
    strings[5] = "Non-Fiction";
    strings[6] = "Biography";
    strings[7] = "Action";
    strings[8] = "Adventure";
    strings[9] = "Comedy";
    
    int numGenres = rand() % 3;
    json_t *genres = json_array();
    
    if (recordId == 1 && numGenres == 0) {
        numGenres = 1;
    }
    
    for (int i = 0; i < numGenres; i++) {
        json_array_append_new(genres, json_string(strings[rand() % 10]));
    }
    
    return genres;
}

//creates a character object
json_t* createCharacter() {
    json_t* character = json_object();
    
    json_object_set_new(character, "firstName", getFirstName());
    json_object_set_new(character, "lastName", getLastName());
    
    return character;
}

//gets character information
json_t* getCharacters(int recordId) {
    int numCharacters = rand() % 15;
    json_t* characters = json_array();
    
    if (recordId == 1 && numCharacters == 0) {
        numCharacters = 1;
    }
    
    for (int i = 0; i < numCharacters; i++) {
        json_array_append_new(characters, createCharacter());
    }
    
    return characters;
}

// creates an individual record with specified id. Tries to randomize data.
json_t* createRecord(int id) {
    json_t* record = json_object();
    
    json_object_set_new(record, "id", json_integer(id));
    json_object_set_new(record, "bookName", getBookName());
    json_object_set_new(record, "author", getAuthor(id));
    
    if (id == 1 || rand() % 5 > 0) {
        json_object_set_new(record, "publisher", getPublisher());
    }
    
    json_object_set_new(record, "numPages", getNumPages());
    
    if (id == 1 || rand() % 5 > 0) {
        json_object_set_new(record, "edition", getEdition());
    }
    
    json_object_set_new(record, "genres", getGenres(id));
    
    if (id == 1 || rand() % 5 > 0) {
        json_object_set_new(record, "characters", getCharacters(id));
    }
    
    return record;
}

// creates the json tree and inserts individual records into it.
json_t* generateJSON(int numRecords) {
    json_t* root = json_array();
    
    for (int i = 0; i < numRecords; i++) {
        json_array_append(root, createRecord(i + 1));
    }
    
    return root;
}

// Main runner
int main(int argc, char *argv[]) {
    int numRecords = 0;
    char *outputFile = "Books.json";
    
    // Throw error if not enough arguments
    if(argc < 2)
    {
        fprintf(stderr, "ERROR: This program requires an agrument that contains the number of records to return and an optional path to an output file.\n");
        return 1;
    }
    
    // If there's more than 1 argument the second argument will be the output file name
    if (argc > 2) {
        outputFile = argv[2];
    }
    
    numRecords = atoi(argv[1]);
    
    json_t *root = generateJSON(numRecords);
    
    if(json_dump_file(root, outputFile, JSON_INDENT(1)))
    {
        fprintf(stderr, "ERROR: json_dump_file error.\n");
        return 3;
    }
    
    json_decref(root);
    return 0;
}