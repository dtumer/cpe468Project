//
//  courseGen.c
//  lab01
//
//  Created by Jonathon Elfar on 4/3/16.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jansson.h"

json_t* getCourseTitle() {
    char* strings[50] = {
        "Personal Software Process",
        "Individual Software Design and Development",
        "Software Engineering II",
        "Introduction to Computer Security",
        "Cryptography Engineering",
        "Music Programming",
        "Computer System Administration",
        "Introduction to Database Systems",
        "Database Modeling, Design and Implementation",
        "Introduction to Distributed Computing",
        "Game Design",
        "Interactive Entertainment Engineering",
        "Special Problems",
        "Software Requirements Engineering",
        "Software Construction",
        "Software Deployment",
        "Current Topics in Software Engineering",
        "Software Evaluation",
        "Autonomous Mobile Robotics",
        "Applied Parallel Computing",
        "Software Security",
        "Current Topics in Computer Security",
        "Introduction to Object Oriented Design Using Graphical User Interfaces",
        "Mobile Application Development",
        "Dynamic Web Development",
        "Bioinformatics Algorithms",
        "Implementation of Operating Systems",
        "Current Topics in Computer Systems",
        "Introduction to Computer Networks",
        "Advanced Computer Networks",
        "Knowledge Discovery from Data",
        "Database Management Systems Implementation",
        "Introduction to Computer Graphics",
        "Advanced Rendering Techniques",
        "Computer Animation",
        "Real-Time 3D Computer Graphics Software",
        "Scientific and Information Visualization",
        "Current Topics in Computer Graphics",
        "Computer Graphics Seminar",
        "Artificial Intelligence",
        "Knowledge Based Systems",
        "Current Topics in Human-Computer Interaction",
        "User-Centered Interface Design and Development",
        "Autonomous Robot Navigation",
        "Human-Computer Interaction Theory and Design",
        "Current Topics in Artificial Intelligence",
        "Selected Advanced Topics",
        "Selected Advanced Laboratory",
        "Software Engineering I",
        "Software Engineering II"
    };

    return json_string(strings[rand() % 50]);
}

json_t* getCourseTitleAbbrev() {
    char* strings[3] = { "CSC", "CPE", "CSC/CPE" };
    return json_string(strings[rand() % 3]);
}

json_t* getCourseNumber() {
    char* strings[50] = {
        "301",
        "305",
        "309",
        "321",
        "323",
        "344",
        "358",
        "365",
        "366",
        "369",
        "371",
        "378",
        "400",
        "402",
        "405",
        "406",
        "409",
        "410",
        "416",
        "419",
        "424",
        "429",
        "435",
        "436",
        "437",
        "448",
        "454",
        "458",
        "464",
        "465",
        "466",
        "468",
        "471",
        "473",
        "474",
        "476",
        "477",
        "478",
        "479",
        "480",
        "481",
        "483",
        "484",
        "485",
        "486",
        "489",
        "490",
        "496",
        "508",
        "509"
    };
    return json_string(strings[rand() % 50]);
}

json_t* getAbbreviatedCourse() {
    json_t *course = json_object();
    json_object_set_new(course, "courseTitleAbbrev", getCourseTitleAbbrev());
    json_object_set_new(course, "courseIdAbbrev", getCourseNumber());
    return course;
}

json_t* getDaysAndTime() {
    char *strings[16] = {
        "MWF 9-10AM",
        "MWF 10-11AM",
        "MWF 12-1PM",
        "MWF 1-2PM",
        "MWF 2-4PM",
        "MWF 3-5PM",
        "TR 9-11AM",
        "TR 12-1PM",
        "TR 2-4PM",
        "TR 4-7PM",
        "MW 2-4PM",
        "MW 10-12PM",
        "MTWR 10-11AM",
        "MTWR 2-3PM",
        "MTRF 4-5PM",
        "MTRF 7-8PM"
    };
    return json_string(strings[rand() % 16]);
}

char* getFirstName()
{
    char* strings[50] = {
        "James",
        "John",
        "Robert",
        "Michael",
        "William",
        "Mary",
        "David",
        "Richard",
        "Joseph",
        "Charles",
        "Thomas",
        "Christopher",
        "Daniel",
        "Patricia",
        "Matthew",
        "Jennifer",
        "Elizabeth",
        "Linda",
        "Barbara",
        "Donald",
        "Anthony",
        "Mark",
        "Paul",
        "Steven",
        "George",
        "Kenneth",
        "Andrew",
        "Edward",
        "Joshua",
        "Brian",
        "Kevin",
        "Susan",
        "Ronald",
        "Margaret",
        "Timothy",
        "Jessica",
        "Jason",
        "Sarah",
        "Dorothy",
        "Karen",
        "Nancy",
        "Betty",
        "Jeffrey",
        "Lisa",
        "Ryan",
        "Gary",
        "Sandra",
        "Nicholas",
        "Eric",
        "Jacob"
    };
    
    return strings[rand() % 50];
}

char* getLastName()
{
    char* strings[20] = {
        "Smith",
        "Johnson",
        "Williams",
        "Brown",
        "Jones",
        "Miller",
        "Davis",
        "Garcia",
        "Rodriguez",
        "Wilson",
        "Martinez",
        "Anderson",
        "Taylor",
        "Thomas",
        "Hernandez",
        "Moore",
        "Martin",
        "Jackson",
        "Thompson",
        "White"
    };
    
    return strings[rand() % 20];
}

json_t* getMajor() {
    char* strings[2] = {
        "CS",
        "CPE"
    };
    return json_string(strings[rand() % 2]);
}

json_t* createStudent() {
    json_t *student = json_object();
    
    json_t *nameObject = json_object();
    json_object_set_new(nameObject, "firstName", json_string(getFirstName()));
    json_object_set_new(nameObject, "lastName", json_string(getLastName()));

    json_object_set_new(student, "name", nameObject);
    json_object_set_new(student, "major", getMajor());
    
    return student;
}

json_t* getStudents(int recordNum) {
    int numStudents = rand() % 25;
    json_t *students = json_array();

    if(recordNum == 1 && numStudents == 0) {
        numStudents = 1;
    }

    for(int i = 0; i < numStudents; i++) {
        json_array_append_new(students, createStudent());
    }
    
    return students;
}

//Course: ID, course title, {course abrv:course #}, [time/days] offered, [{students}]
//Student: [name, major]
json_t* createRecord(int recordNum)
{
    json_t *record = json_object();
    
    json_object_set_new(record, "id", json_integer(recordNum));
    json_object_set_new(record, "courseTitle", getCourseTitle());
    json_object_set_new(record, "courseAbbrev", getAbbreviatedCourse());
    
    //if(recordNum == 1 || (rand() % 10) > 0)
        //json_object_set_new(record, "writer", getWriter());

    if(recordNum == 1 || (rand() % 10) > 0) {
        json_object_set_new(record, "daysAndTime", getDaysAndTime());
    }
    json_object_set_new(record, "students", getStudents(recordNum));
    
    
    return record;
}

json_t* createJSON(int numRecords) {
    json_t *root = json_array();
    
    for(int i = 0; i < numRecords; i++) {
        json_array_append_new(root, createRecord(i+1));
    }
    
    return root;
}

int main(int argc, char *argv[]) {
    int numRecords = 0;
    char *outputFile = "./courses.json";
    
    if(argc < 2) {
        fprintf(stderr, "ERROR: This program requires an agrument that contains the number of records to return and an optional path to an output file.\n");
        return 1;
    }
    
    numRecords = atoi(argv[1]);
    
    if(numRecords == 0) {
        fprintf(stderr, "ERROR: Argument is not a number greater than zero.\n");
        return 2;
    }
    
    if(argc > 2) {
        outputFile = argv[2];
    }
    
    json_t *root = createJSON(numRecords);
    
    if(json_dump_file(root, outputFile, JSON_INDENT(1))) {
        fprintf(stderr, "ERROR: json_dump_file error.\n");
        return 3;
    }
    
    json_decref(root);
    return 0;
}
