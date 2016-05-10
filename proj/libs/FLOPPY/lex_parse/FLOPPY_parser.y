%{
/* 
 * FLOPPY Parser
 * For CPE468 Spring 2016 (Dekhtyar)
 * Written by Andrew Wang
 */
#include "FLOPPY_parser.h"
#include "FLOPPY_lexer.h"

#include <vector>
#include <string>
#include <stdio.h>

int yyerror(YYLTYPE* llocp, FLOPPYOutput ** result, yyscan_t scanner, const char *msg) {
	FLOPPYOutput *fo = new FLOPPYOutput();
	fo->isValid = false;
   
   *result = fo;
	return 0;
}

%}
%code requires {
#include "../FLOPPY_statements/statements.h"
#include "../FLOPPYOutput.h"
#ifndef YYtypeDEF_YY_SCANNER_T
#define YYtypeDEF_YY_SCANNER_T
typedef void* yyscan_t;
#endif
}

%output  "FLOPPY_parser.cpp"
%defines "FLOPPY_parser.h"

%define api.pure full
%define api.token.prefix {FLOPPY_}
%define parse.error verbose
%locations
%lex-param   { yyscan_t scanner }
%parse-param { FLOPPYOutput** result }
%parse-param { yyscan_t scanner }

%union {
	int64_t ival;
   bool bval;
   char *sval;

	FLOPPYStatement *statement;
   FLOPPYCreateTableStatement *create_table_statement;
   FLOPPYForeignKey *foreign_key;

   std::vector<char *> *str_vec;
   std::vector<FLOPPYCreateColumn *> *create_column_vec;
   std::vector<FLOPPYForeignKey *> *foreign_key_vec;

   FLOPPYCreateColumn *create_column;
   FLOPPYPrimaryKey *primary_key;
}

%token <ival> INTVAL
%token <sval> ID

%token CREATE TABLE VOLATILE PRIMARY FOREIGN KEY REFERENCES
%token INT FLOAT BOOLEAN DATETIME VARCHAR

%type <bval>                     opt_volatile
%type <ival>                     int_literal opt_column_size column_type

%type <statement> 	            statement 
%type <create_table_statement>   create_statement

%type <create_column>            column_def

%type <primary_key>              primary_key
%type <foreign_key>              foreign_key

%type <create_column_vec>        column_def_commalist
%type <foreign_key_vec>          opt_foreign_key_list
%type <str_vec>                  attribute_list

%%
input:
	statement ';' { 
      *result = new FLOPPYOutput($1); 
      (*result)->isValid = true;
   }
	;

statement:
	   create_statement { $$ = $1; }
	;

/******************************
 * Create Statement
 * CREATE TABLE <Table> [VOLATILE] (
 *    <Attribute> <Type>,
 *    ...
 *    <Attribute> <Type>,
 *    PRIMARY KEY (<Attribute>[, <Attribute>]*)
 *    [, FOREIGN KEY (<Attribute[, <Attribute>]*) REFERENCES <TableName>]*
 ******************************/
create_statement:
      CREATE TABLE ID opt_volatile '(' 
         column_def_commalist ','
         primary_key
         opt_foreign_key_list
      ')'   {
               $$ = new FLOPPYCreateTableStatement();
               $$->tableName = $3;
               $$->volatileFlag = $4;
               $$->pk = $8;
               $$->fk = $9;
            }
	;

opt_volatile:
		VOLATILE    { $$ = true; }
	|	/* empty */ { $$ = false; }
	;

column_def_commalist:
      column_def  {
         $$ = new std::vector<FLOPPYCreateColumn *>(); 
         $$->push_back($1); 
      }
   |  column_def_commalist ',' column_def {
         $1->push_back($3);
         $$ = $1;
      }
	;

column_def:
      ID column_type opt_column_size {
         $$ = new FLOPPYCreateColumn();
         $$->name = $1;
         $$->type = (ColumnType) $2;
         $$->size = $3;
      }
	; 
attribute_list:
      ID { 
         $$ = new std::vector<char *>(); 
         $$->push_back($1); 
      }
   |  attribute_list ',' ID {
         $1->push_back($3);
         $$ = $1;
      }
   ;

primary_key:
      PRIMARY KEY '(' attribute_list ')' {
         $$ = new FLOPPYPrimaryKey();
         $$->attributes = $4;
      }
   ;

opt_foreign_key_list:
      opt_foreign_key_list ',' foreign_key {
         $$ = $1;
         $$->push_back($3);
      }
	|	/* empty */ {
         $$ = new std::vector<FLOPPYForeignKey *>();
      }
   ;

foreign_key:
      FOREIGN KEY '(' attribute_list ')' REFERENCES ID {
         $$ = new FLOPPYForeignKey();
         $$->attributes = $4;
         $$->refTableName = $7;
      }
   ;

column_type:
      INT { 
         $$ = ColumnType::INT; 
      }
   |  FLOAT { 
         $$ = ColumnType::FLOAT; 
      }
   |  VARCHAR { 
         $$ = ColumnType::VARCHAR; 
      }
   |  DATETIME { 
         $$ = ColumnType::DATETIME; 
      }
   |  BOOLEAN { 
         $$ = ColumnType::BOOLEAN; 
      }
	;

opt_column_size:
   '(' int_literal ')' { 
      $$ = $2; 
   }
	|	/* empty */ {
      $$ = 0;
   }

int_literal:
      INTVAL { 
         $$ = 1; 
      }
   ;
%%
