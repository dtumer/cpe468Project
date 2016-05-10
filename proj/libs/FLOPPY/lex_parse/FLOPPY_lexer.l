/*
 * FLOPPY Lexer
 *
 * For CPE 468 Spring 2016 (Dekhtyar)
 * Written by Andrew Wang
 */
%{
#include "FLOPPY_parser.h"
#include <stdio.h>
%}

/* Remove stupid y.def.h naming to be constant FLOPPY_ */
%option header-file="FLOPPY_lexer.h"
%option outfile="FLOPPY_lexer.cpp"

/* Flags to make it work */
%option reentrant
%option bison-bridge
%option never-interactive
%option batch
%option case-insensitive
%option bison-locations 
%option warn
%option noyywrap
%option nounput

/* Create Comment and BlockComment States */
%s COMMENT 
%s BLOCKCOMMENT 
%%
--                      BEGIN(COMMENT);
<COMMENT>[^\n]*         /* skip anything thats in a comment */;
<COMMENT>\n             BEGIN(INITIAL);

\/\*                    BEGIN(BLOCKCOMMENT);
<BLOCKCOMMENT>.|\n      /* skip anything thats in a comment block */;
<BLOCKCOMMENT>\*\/      BEGIN(INITIAL);

[ \r\t\n]+              /* skip isspace() */;

CREATE                  { return FLOPPY_CREATE; }
TABLE                   { return FLOPPY_TABLE; }
VOLATILE                { return FLOPPY_VOLATILE; }
PRIMARY                 { return FLOPPY_PRIMARY; }
FOREIGN                 { return FLOPPY_FOREIGN; }
KEY                     { return FLOPPY_KEY; }
REFERENCES              { return FLOPPY_REFERENCES; }
INT|INTEGER             { return FLOPPY_INT; }
FLOAT                   { return FLOPPY_FLOAT; }
BOOLEAN                 { return FLOPPY_BOOLEAN; }
DATETIME                { return FLOPPY_DATETIME; }
VARCHAR                 { return FLOPPY_VARCHAR; }

[(){},;]                { return yytext[0]; }


[0-9]+                  {
                           yylval->ival = atol(yytext);
                           return FLOPPY_INTVAL;
                        }

[A-Za-z][A-Za-z0-9_]*   {
                           yylval->sval = strdup(yytext);
                           return FLOPPY_ID;
                        }


.                       { 
                           fprintf(stderr, "FLOPPY Lex Error Unknown Token: %c\n", yytext[0]); 
                           return 0;
                        }
%%
int yyerror(const char *errmsg) {
    fprintf(stderr, "[FLOPPY-Lexer-Error] %s\n", errmsg);
    return 0;
}
