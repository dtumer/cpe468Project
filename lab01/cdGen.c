//
//  cdGen.c
//  lab01
//
//  Created by Scott Tucker on 3/31/16.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jansson.h"

json_t* getFirstName()
{
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

json_t* getLastName()
{
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

json_t* getSongName()
{
    char* strings[180];
    strings[0] = "Little Drummer Boy";
    strings[1] = "(Puting My) Heart On The Line";
    strings[2] = "A Little Bit Of Ecstacy";
    strings[3] = "A New Hope";
    strings[4] = "Adam's Song";
    strings[5] = "After The Dance";
    strings[6] = "Aliens Exist";
    strings[7] = "ALIVE / THE PRIME TIME OF YOUR LIFE (LIVE)";
    strings[8] = "All I Want To Be (Is By Your Side)";
    strings[9] = "All Or Nothing";
    strings[10] = "All The Small Things";
    strings[11] = "Alone With You";
    strings[12] = "American Saturday Night";
    strings[13] = "Anthem";
    strings[14] = "Any Colour You Like";
    strings[15] = "Anything Like Me";
    strings[16] = "Anywhere With You";
    strings[17] = "Apple Pie Moonshine";
    strings[18] = "Apple Shampoo";
    strings[19] = "AROUND THE WORLD";
    strings[20] = "Baby (Somethin's Happening)";
    strings[21] = "Baby, I Love Your Way";
    strings[22] = "Back To The Future [Hidden Track]";
    strings[23] = "Bamboogie";
    strings[24] = "Barefoot Blue Jean Night";
    strings[25] = "Be My Lover";
    strings[26] = "Beautiful Life";
    strings[27] = "Beer";
    strings[28] = "Big Willie Style";
    strings[29] = "Blessed";
    strings[30] = "Boring";
    strings[31] = "Brain Damage";
    strings[32] = "Can I Get A Witness";
    strings[33] = "Candy";
    strings[34] = "Careless Whisper";
    strings[35] = "Carol Of The Bells";
    strings[36] = "Catch All The Fish";
    strings[37] = "Changes";
    strings[38] = "Chasing Forever";
    strings[39] = "Cheap Sunglasses";
    strings[40] = "Concrete Angel";
    strings[41] = "Cry";
    strings[42] = "DA FUNK (LIVE)";
    strings[43] = "Da Ya Think I'm Sexy";
    strings[44] = "Dammit";
    strings[45] = "Degenerate";
    strings[46] = "Dick Lips";
    strings[47] = "Disco Inferno";
    strings[48] = "Distant Lover";
    strings[49] = "Do You";
    strings[50] = "Do You Remember (Feat. Sean Paul & Lil Jon)";
    strings[51] = "Don't Say Nothin'";
    strings[52] = "Doobie Wah";
    strings[53] = "Doubleback";
    strings[54] = "Down";
    strings[55] = "Dysentery Gary";
    strings[56] = "Eclipse";
    strings[57] = "Emo";
    strings[58] = "Enthused";
    strings[59] = "Everybody's Here";
    strings[60] = "FACE TO FACE / HARDER, BETTER, FASTER, STRONGER (LIVE)";
    strings[61] = "Fire";
    strings[62] = "Gettin' Jiggy Wit It";
    strings[63] = "Gimme All Your Lovin'";
    strings[64] = "Give It Up";
    strings[65] = "God Rest Ye Merry Gentlemen";
    strings[66] = "Going Away To College";
    strings[67] = "Got Me Under Pressure";
    strings[68] = "Gun Love";
    strings[69] = "Have Yourself A Merry Little Christmas";
    strings[70] = "Heaven";
    strings[71] = "How Sweet It Is (To Be Loved By You)";
    strings[72] = "I Can't Stand It No More";
    strings[73] = "I Heard It Through The Grapevine";
    strings[74] = "I Hope That's Me";
    strings[75] = "I Love You";
    strings[76] = "I Loved You";
    strings[77] = "I Wana Go To The Sun";
    strings[78] = "I Want You";
    strings[79] = "I'll Give You Monet";
    strings[80] = "I'm Bad, I'm Nationwide";
    strings[81] = "I'm In You";
    strings[82] = "I'm Sorry";
    strings[83] = "If I Ain't Got You";
    strings[84] = "Insomia";
    strings[85] = "Intro";
    strings[86] = "It Came Upon A Midnight Clear";
    strings[87] = "It's A Plain Shame";
    strings[88] = "It's All Good";
    strings[89] = "Jet Airliner";
    strings[90] = "Josie";
    strings[91] = "Just The Two Of Us";
    strings[92] = "Keepin' It Country";
    strings[93] = "La Grange";
    strings[94] = "Legs";
    strings[95] = "Lemmings";
    strings[96] = "Lights Off";
    strings[97] = "Lines On My Face";
    strings[98] = "Love Like This (Eternity)";
    strings[99] = "Make That Money";
    strings[100] = "Men In Black";
    strings[101] = "Mercy Mercy Me (The Ecology)";
    strings[102] = "Miami";
    strings[103] = "Money";
    strings[104] = "Mutt";
    strings[105] = "My Head's In Mississippi";
    strings[106] = "No";
    strings[107] = "Nobody Feelin' No Pain";
    strings[108] = "Not Now";
    strings[109] = "O Tannenbaum";
    strings[110] = "OH YEAH / TECHNOLOGIC (LIVE)";
    strings[111] = "Oh Yeah, You're Gone";
    strings[112] = "On The Run";
    strings[113] = "ONE MORE TIME / AERODYNAMIC (LIVE)";
    strings[114] = "Pathetic";
    strings[115] = "Pearl Necklace";
    strings[116] = "Planet Of Woman";
    strings[117] = "Pop Muzik";
    strings[118] = "Ride It";
    strings[119] = "ROBOT ROCK (LIVE)";
    strings[120] = "Rough Boy";
    strings[121] = "Santa Claus Is Comin' To Town";
    strings[122] = "Settin' The World On Fire";
    strings[123] = "Sharp Dressed Man";
    strings[124] = "She's Her Own Woman";
    strings[125] = "SHORT CIRCUIT (LIVE)";
    strings[126] = "Show Me The Way";
    strings[127] = "Signed, Sealed, Delivered I'm Yours";
    strings[128] = "Silent Night";
    strings[129] = "Silver Bells";
    strings[130] = "Sleeping Bag";
    strings[131] = "Sleigh Ride";
    strings[132] = "Some Children See Him";
    strings[133] = "Speak To Me/Breathe";
    strings[134] = "Stay";
    strings[135] = "STEAM MACHINE (LIVE)";
    strings[136] = "Stuck In The Middle (Feat. Jared Cotter)";
    strings[137] = "SUPERHEROES / HUMAN AFTER ALL (LIVE)";
    strings[138] = "Take The Money And Run";
    strings[139] = "TEACHERS (EXTENDED MIX)";
    strings[140] = "TELEVISION RULES THE NATION / CRESCENDOLLS (LIVE)";
    strings[141] = "THE BRAINWASHER / ROLLIN' & SCRATCHIN' (LIVE)";
    strings[142] = "The Christmas Song";
    strings[143] = "The Great Gig In The Sky";
    strings[144] = "The Journey Of Your Life";
    strings[145] = "The One That Got Away";
    strings[146] = "The Pants";
    strings[147] = "The Party Song";
    strings[148] = "Then";
    strings[149] = "There You Are";
    strings[150] = "This Christmas";
    strings[151] = "This Is Your Night";
    strings[152] = "Time";
    strings[153] = "TOO LONG (LIVE)";
    strings[154] = "Trouble Man";
    strings[155] = "Tube Snake Boogie";
    strings[156] = "Tush";
    strings[157] = "Untitled";
    strings[158] = "Us And Them";
    strings[159] = "Viva Las Vegas";
    strings[160] = "Voyeur";
    strings[161] = "Waggy";
    strings[162] = "Want You Bad";
    strings[163] = "War";
    strings[164] = "Water";
    strings[165] = "We Are The Champions";
    strings[166] = "Welcome To The Future";
    strings[167] = "Welcome to the Jungle";
    strings[168] = "What Child Is This?";
    strings[169] = "What Is Love";
    strings[170] = "What's Going On";
    strings[171] = "What's My Age Again?";
    strings[172] = "Where Do You Go";
    strings[173] = "Where Would You Be";
    strings[174] = "White Christmas";
    strings[175] = "Wide Awake";
    strings[176] = "Wild Angels";
    strings[177] = "Y'All Know";
    strings[178] = "Yes Yes Y'All";
    strings[179] = "You Do The Math";
    
    return json_string(strings[rand() % 180]);
}

json_t* getTrackDuration()
{
    char buffer[6];
    
    sprintf(buffer, "%d:%02d", rand() % 14, rand() % 61);
    
    return json_string(buffer);
}

json_t* createTrack(int trackNum)
{
    json_t *track = json_object();
    
    json_object_set_new(track, "trackNumber", json_integer(trackNum));
    json_object_set_new(track, "name", getSongName());
    json_object_set_new(track, "duration", getTrackDuration());
    
    return track;
}

json_t* getTracks(int recordNum)
{
    int numTracks = rand() % 21;
    json_t *tracks = json_array();
    
    if(recordNum == 0 && numTracks == 0)
        numTracks = 1;
    
    for(int i=0; i<numTracks; i++)
    {
        json_array_append_new(tracks, createTrack(i+1));
    }
    
    return tracks;
}

json_t* getAlbumName()
{
    char* strings[20];
    strings[0] = "A Christmas Collection";
    strings[1] = "A Night At The Roxbury";
    strings[2] = "All Or Nothing";
    strings[3] = "American Saturday Night";
    strings[4] = "Around the World";
    strings[5] = "Barefoot Blue Jean Night";
    strings[6] = "Big Willie Style";
    strings[7] = "Dark Side Of The Moon";
    strings[8] = "Dude Ranch";
    strings[9] = "Enema of the State";
    strings[10] = "Greatest Hits";
    strings[11] = "Late Registration";
    strings[12] = "Money, Power & Respect";
    strings[13] = "Ocean Avenue";
    strings[14] = "Riser";
    strings[15] = "Stadium Arcadium";
    strings[16] = "The Eminem Show";
    strings[17] = "The Graduate";
    strings[18] = "We Can't Dance";
    strings[19] = "Wheelhouse";
    
    return json_string(strings[rand() % 20]);
}

json_t* getArtist()
{
    char* strings[20];
    strings[0] = "Andre Nickatina";
    strings[1] = "Jack Johnson";
    strings[2] = "Daft Punk";
    strings[3] = "Atmosphere";
    strings[4] = "Ray Charles";
    strings[5] = "Deadmau5";
    strings[6] = "Blink-182";
    strings[7] = "Kanye West";
    strings[8] = "Eminem";
    strings[9] = "Dierks Bentley";
    strings[10] = "Michael Jackson";
    strings[11] = "The Eagles";
    strings[12] = "Chris Brown";
    strings[13] = "Nickelback";
    strings[14] = "John Mellencamp";
    strings[15] = "Diana Ross";
    strings[16] = "Will Smith";
    strings[17] = "U2";
    strings[18] = "Josh Groban";
    strings[19] = "The Police";
    
    return json_string(strings[rand() % 20]);
}

json_t* getWriter()
{
    json_t *writer = json_object();
    
    json_object_set_new(writer, "firstName", getFirstName());
    json_object_set_new(writer, "lastName", getLastName());
    
    return writer;
}

json_t* getBandMembers(int recordNum)
{
    int numBandMembers = rand() % 6;
    json_t *bandMembers = json_array();
    
    if(recordNum == 0 && numBandMembers == 0)
        numBandMembers = 1;
    
    for(int i=0; i<numBandMembers; i++)
    {
        json_array_append_new(bandMembers, getFirstName());
    }
    
    return bandMembers;
}


json_t* createRecord(int recordNum)
{
    json_t *record = json_object();
    
    json_object_set_new(record, "id", json_integer(recordNum));
    json_object_set_new(record, "albumName", getAlbumName());
    
    if(recordNum == 1 || (rand() % 10) > 0)
    	json_object_set_new(record, "artist", getArtist());
    
    if(recordNum == 1 || (rand() % 10) > 0)
        json_object_set_new(record, "writer", getWriter());
    
    json_object_set_new(record, "bandMembers", getBandMembers(recordNum));
    json_object_set_new(record, "tracks", getTracks(recordNum));
    
    
    return record;
}

json_t* createJSON(int numRecords)
{
    json_t *root = json_array();
    
    for(int i=0; i<numRecords; i++)
    {
        json_array_append_new(root, createRecord(i+1));
    }
    
    return root;
}

int main(int argc, char *argv[])
{
    int numRecords = 0;
    char *outputFile = "./CDs.json";
    
    if(argc < 2)
    {
        fprintf(stderr, "ERROR: This program requires an agrument that contains the number of records to return and an optional path to an output file.\n");
        return 1;
    }
    
    numRecords = atoi(argv[1]);
    
    if(numRecords == 0)
    {
        fprintf(stderr, "ERROR: Argument is not a number greater than zero.\n");
        return 2;
    }
    
    if(argc > 2)
        outputFile = argv[2];
    
    json_t *root = createJSON(numRecords);
    
    if(json_dump_file(root, outputFile, JSON_INDENT(1)))
    {
        fprintf(stderr, "ERROR: json_dump_file error.\n");
        return 3;
    }
    
    json_decref(root);
    return 0;
}