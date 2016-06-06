/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "FLOPPY_parser.y" /* yacc.c:339  */

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
#include <stdlib.h>

int yyerror(YYLTYPE* llocp, FLOPPYOutput ** result, yyscan_t scanner, const char *msg) {
	FLOPPYOutput *fo = new FLOPPYOutput();
	fo->isValid = false;
   
   *result = fo;
	return 0;
}


#line 90 "FLOPPY_parser.cpp" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* In a future release of Bison, this section will be replaced
   by #include "FLOPPY_parser.h".  */
#ifndef YY_YY_FLOPPY_PARSER_H_INCLUDED
# define YY_YY_FLOPPY_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 24 "FLOPPY_parser.y" /* yacc.c:355  */

#include "../FLOPPY_statements/statements.h"
#include "../FLOPPYOutput.h"
#ifndef YYtypeDEF_YY_SCANNER_T
#define YYtypeDEF_YY_SCANNER_T
typedef void* yyscan_t;
#endif

#line 129 "FLOPPY_parser.cpp" /* yacc.c:355  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    FLOPPY_INTVAL = 258,
    FLOPPY_FLOATVAL = 259,
    FLOPPY_ID = 260,
    FLOPPY_STRING = 261,
    FLOPPY_CREATE = 262,
    FLOPPY_TABLE = 263,
    FLOPPY_VOLATILE = 264,
    FLOPPY_PRIMARY = 265,
    FLOPPY_FOREIGN = 266,
    FLOPPY_KEY = 267,
    FLOPPY_REFERENCES = 268,
    FLOPPY_INDEX = 269,
    FLOPPY_ONLY = 270,
    FLOPPY_SPLIT = 271,
    FLOPPY_DROP = 272,
    FLOPPY_ON = 273,
    FLOPPY_INTO = 274,
    FLOPPY_VALUES = 275,
    FLOPPY_DELETE = 276,
    FLOPPY_INSERT = 277,
    FLOPPY_SELECT = 278,
    FLOPPY_FROM = 279,
    FLOPPY_WHERE = 280,
    FLOPPY_UPDATE = 281,
    FLOPPY_SET = 282,
    FLOPPY_GROUP = 283,
    FLOPPY_BY = 284,
    FLOPPY_HAVING = 285,
    FLOPPY_ORDER = 286,
    FLOPPY_LIMIT = 287,
    FLOPPY_DISTINCT = 288,
    FLOPPY_COUNT = 289,
    FLOPPY_AVERAGE = 290,
    FLOPPY_MAX = 291,
    FLOPPY_MIN = 292,
    FLOPPY_SUM = 293,
    FLOPPY_NULL = 294,
    FLOPPY_AS = 295,
    FLOPPY_NOT = 296,
    FLOPPY_MOD = 297,
    FLOPPY_LE = 298,
    FLOPPY_GE = 299,
    FLOPPY_NE = 300,
    FLOPPY_TRUE = 301,
    FLOPPY_FALSE = 302,
    FLOPPY_AND = 303,
    FLOPPY_INT = 304,
    FLOPPY_FLOAT = 305,
    FLOPPY_BOOLEAN = 306,
    FLOPPY_DATETIME = 307,
    FLOPPY_VARCHAR = 308
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 44 "FLOPPY_parser.y" /* yacc.c:355  */

	int64_t ival;
	float fval;
   bool bval;
   char *sval;

	FLOPPYStatement *statement;
   FLOPPYCreateTableStatement *create_table_statement;
   FLOPPYDropTableStatement *drop_table_statement;
   FLOPPYCreateIndexStatement *create_index_statement;
   FLOPPYDropIndexStatement *drop_index_statement;
   FLOPPYInsertStatement *insert_statement;
   FLOPPYDeleteStatement *delete_statement;
   FLOPPYUpdateStatement *update_statement;
   FLOPPYSelectStatement *select_statement;
   FLOPPYForeignKey *foreign_key;
   FLOPPYSelectItem *select_item;
   FLOPPYTableSpec *table_spec;
   FLOPPYGroupBy *group_by;

   std::vector<char *> *str_vec;
   std::vector<FLOPPYCreateColumn *> *create_column_vec;
   std::vector<FLOPPYForeignKey *> *foreign_key_vec;
   std::vector<FLOPPYValue *> *value_vec;
   std::vector<FLOPPYSelectItem *> *select_item_vec;
   std::vector<FLOPPYTableSpec *> *table_spec_vec;
   CreateTableAdditionalFunctionality *flags;

   FLOPPYCreateColumn *create_column;
   FLOPPYPrimaryKey *primary_key;

   FLOPPYValue *value;
   FLOPPYNode *node;

#line 230 "FLOPPY_parser.cpp" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



int yyparse (FLOPPYOutput** result, yyscan_t scanner);

#endif /* !YY_YY_FLOPPY_PARSER_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 260 "FLOPPY_parser.cpp" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  26
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   253

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  66
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  41
/* YYNRULES -- Number of rules.  */
#define YYNRULES  107
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  230

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   308

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      61,    62,    59,    57,    65,    58,    63,    60,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    64,
      56,    54,    55,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   138,   138,   145,   146,   147,   148,   149,   150,   151,
     152,   165,   180,   184,   193,   197,   205,   209,   216,   220,
     227,   236,   240,   247,   254,   258,   264,   272,   275,   278,
     281,   284,   290,   293,   298,   309,   321,   334,   347,   356,
     360,   367,   368,   372,   379,   383,   387,   391,   395,   407,
     416,   417,   423,   428,   436,   442,   448,   454,   460,   466,
     472,   477,   485,   486,   492,   498,   504,   510,   516,   524,
     529,   533,   539,   545,   551,   557,   563,   577,   599,   617,
     621,   624,   628,   635,   639,   644,   650,   656,   662,   668,
     674,   682,   686,   693,   698,   703,   711,   712,   716,   719,
     725,   731,   737,   740,   746,   749,   755,   758
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INTVAL", "FLOATVAL", "ID", "STRING",
  "CREATE", "TABLE", "VOLATILE", "PRIMARY", "FOREIGN", "KEY", "REFERENCES",
  "INDEX", "ONLY", "SPLIT", "DROP", "ON", "INTO", "VALUES", "DELETE",
  "INSERT", "SELECT", "FROM", "WHERE", "UPDATE", "SET", "GROUP", "BY",
  "HAVING", "ORDER", "LIMIT", "DISTINCT", "COUNT", "AVERAGE", "MAX", "MIN",
  "SUM", "NULL", "AS", "NOT", "MOD", "LE", "GE", "NE", "TRUE", "FALSE",
  "AND", "INT", "FLOAT", "BOOLEAN", "DATETIME", "VARCHAR", "'='", "'>'",
  "'<'", "'+'", "'-'", "'*'", "'/'", "'('", "')'", "'.'", "';'", "','",
  "$accept", "input", "statement", "create_table_statement",
  "opt_volatile", "opt_index_only", "opt_split", "column_def_commalist",
  "column_def", "attribute_list", "primary_key", "opt_foreign_key_list",
  "foreign_key", "column_type", "opt_column_size", "int_literal",
  "drop_table_statement", "create_index_statement", "drop_index_statement",
  "insert_statement", "value_list", "value", "constant",
  "delete_statement", "condition", "atomic_condition", "expression",
  "atomic_expression", "update_statement", "select_statement",
  "star_or_select_item_list", "select_item_list", "select_item",
  "table_spec_list", "table_spec", "opt_distinct", "opt_where",
  "opt_group_by", "opt_having", "opt_order_by", "opt_limit", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,    61,    62,    60,    43,    45,    42,
      47,    40,    41,    46,    59,    44
};
# endif

#define YYPACT_NINF -111

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-111)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     141,    -6,     1,   -18,    60,   -16,    73,    81,    27,  -111,
    -111,  -111,  -111,  -111,  -111,  -111,  -111,   107,   109,   112,
     128,   129,   131,  -111,    59,   110,  -111,  -111,    77,   120,
    -111,   121,   132,   134,    96,    95,    99,   100,   104,   105,
    -111,   148,   108,  -111,   169,   111,   114,   172,   173,    22,
     118,   182,     8,   183,   184,   185,   186,   187,    85,   139,
     180,  -111,   190,   135,  -111,  -111,  -111,  -111,  -111,   136,
     137,   138,   140,   142,  -111,    22,  -111,  -111,   197,    22,
    -111,   154,  -111,   126,  -111,    26,  -111,   143,   144,   145,
     146,   147,   149,    -2,     6,  -111,  -111,    16,   189,    75,
     150,  -111,   205,    11,   207,   208,   209,   211,  -111,  -111,
     -38,    87,    22,    16,    16,    16,    16,    16,    16,    16,
      16,    16,    16,    16,  -111,  -111,   -51,  -111,  -111,  -111,
    -111,  -111,  -111,  -111,  -111,  -111,   212,    22,   187,   191,
      16,    50,   153,  -111,  -111,  -111,  -111,  -111,   159,     2,
    -111,    23,   160,   161,   162,   163,   164,   165,  -111,  -111,
    -111,  -111,   -24,   -24,   -24,   -24,   -24,   -24,    40,    40,
    -111,  -111,  -111,    26,  -111,   154,  -111,   192,   198,    93,
      22,   214,  -111,   225,  -111,   219,  -111,  -111,  -111,   227,
    -111,  -111,  -111,  -111,  -111,  -111,  -111,   205,   204,   202,
     154,  -111,  -111,   174,   176,    39,  -111,    46,   205,   232,
    -111,  -111,   205,  -111,   228,    22,  -111,   175,  -111,    41,
     226,  -111,   154,  -111,   181,   205,    51,   230,   236,  -111
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,    97,     0,     0,     0,     3,
       4,     5,     6,     7,     8,     9,    10,     0,     0,     0,
       0,     0,     0,    96,     0,     0,     1,     2,    13,     0,
      35,     0,     0,     0,    83,     0,     0,     0,     0,     0,
      79,     0,    80,    81,     0,    15,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    12,     0,     0,    37,    44,    46,    69,    47,     0,
       0,     0,     0,     0,    48,     0,    60,    61,     0,     0,
      70,    49,    50,     0,    62,     0,    84,     0,     0,     0,
       0,     0,     0,    93,    99,    91,    82,     0,     0,     0,
       0,    18,     0,     0,     0,     0,     0,     0,    52,    45,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    42,    43,     0,    39,    41,    85,
      90,    86,    87,    88,    89,    94,     0,     0,     0,   101,
       0,     0,    17,    27,    28,    31,    30,    29,    33,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    53,    68,
      51,    67,    57,    58,    59,    56,    55,    54,    63,    64,
      65,    66,    38,     0,    95,    98,    92,     0,   105,     0,
       0,     0,    14,     0,    20,     0,    19,    25,    36,     0,
      75,    76,    74,    72,    71,    73,    40,     0,     0,   107,
      77,    16,    34,     0,     0,     0,    22,   103,     0,     0,
      78,    32,     0,    11,     0,     0,   100,   104,   106,     0,
       0,    24,   102,    23,     0,     0,     0,     0,     0,    26
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -111,  -111,  -111,  -111,  -111,  -111,  -111,  -111,    97,  -110,
    -111,  -111,  -111,  -111,  -111,  -111,  -111,  -111,  -111,  -111,
    -111,    71,   -84,  -111,   -75,  -111,   -74,  -111,  -111,  -111,
    -111,  -111,   193,  -111,   115,  -111,  -111,  -111,  -111,  -111,
    -111
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     7,     8,     9,    46,    61,   182,   100,   101,   151,
     187,   205,   221,   148,   184,   203,    10,    11,    12,    13,
     126,   127,    80,    14,    81,    82,    83,    84,    15,    16,
      41,    42,    43,    94,    95,    24,   139,   178,   216,   199,
     210
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
     108,   128,    17,   135,   110,   111,    21,    99,    18,    19,
     112,   172,   185,    87,   173,    20,   152,    23,   113,    65,
      66,    67,    68,   141,   158,    65,    66,    67,    68,    65,
      66,   137,    68,   120,   121,   122,   123,   160,   136,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
      69,    70,    71,    72,    73,    74,    69,    70,    71,    72,
      73,    74,   175,    75,    34,    74,   179,    88,    76,    77,
     153,   138,   124,   125,    78,   180,   215,   140,    25,    22,
      78,    26,   113,    79,    78,   188,    45,   207,   189,   128,
      34,    27,   113,    35,    36,    37,    38,    39,   217,   122,
     123,   213,   219,   223,   214,   200,   189,   120,   121,   122,
     123,   189,    28,   227,    29,   226,   189,    30,    40,    35,
      36,    37,    38,    39,   143,   144,   145,   146,   147,   113,
     114,   115,   116,    31,    32,   113,    33,    44,    47,    48,
     222,   117,   118,   119,   120,   121,   122,   123,     1,   159,
     120,   121,   122,   123,    50,   159,    52,    49,     2,    51,
      53,    54,     3,     4,     5,    55,    56,     6,   113,   114,
     115,   116,    57,    58,    59,    62,    60,    63,    64,    85,
     117,   118,   119,   120,   121,   122,   123,    86,    89,    90,
      91,    92,    93,    97,    98,    99,   102,   103,   104,   105,
     109,   106,   112,   107,   142,   129,   130,   131,   132,   133,
     150,   134,   154,   155,   156,   149,   157,   174,   181,   177,
     183,   197,   190,   191,   192,   193,   194,   195,   202,   198,
     201,   204,   206,   208,   209,   218,   211,   212,   224,   220,
     189,   229,   225,   228,   196,     0,   186,     0,     0,     0,
       0,    96,     0,   176
};

static const yytype_int16 yycheck[] =
{
      75,    85,     8,     5,    79,    79,    24,     5,    14,     8,
      48,    62,    10,     5,    65,    14,     5,    33,    42,     3,
       4,     5,     6,    97,    62,     3,     4,     5,     6,     3,
       4,    25,     6,    57,    58,    59,    60,   112,    40,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
      34,    35,    36,    37,    38,    39,    34,    35,    36,    37,
      38,    39,   137,    41,     5,    39,   140,    59,    46,    47,
      59,    65,    46,    47,    58,    25,    30,    61,     5,    19,
      58,     0,    42,    61,    58,    62,     9,   197,    65,   173,
       5,    64,    42,    34,    35,    36,    37,    38,   208,    59,
      60,    62,   212,    62,    65,   180,    65,    57,    58,    59,
      60,    65,     5,    62,     5,   225,    65,     5,    59,    34,
      35,    36,    37,    38,    49,    50,    51,    52,    53,    42,
      43,    44,    45,     5,     5,    42,     5,    27,    18,    18,
     215,    54,    55,    56,    57,    58,    59,    60,     7,    62,
      57,    58,    59,    60,    20,    62,    61,    25,    17,    63,
      61,    61,    21,    22,    23,    61,    61,    26,    42,    43,
      44,    45,    24,    65,     5,    61,    65,     5,     5,    61,
      54,    55,    56,    57,    58,    59,    60,     5,     5,     5,
       5,     5,     5,    54,    14,     5,    61,    61,    61,    61,
       3,    61,    48,    61,    15,    62,    62,    62,    62,    62,
       5,    62,     5,     5,     5,    65,     5,     5,    65,    28,
      61,    29,    62,    62,    62,    62,    62,    62,     3,    31,
      16,    12,     5,    29,    32,     3,    62,    61,    12,    11,
      65,     5,    61,    13,   173,    -1,   149,    -1,    -1,    -1,
      -1,    58,    -1,   138
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     7,    17,    21,    22,    23,    26,    67,    68,    69,
      82,    83,    84,    85,    89,    94,    95,     8,    14,     8,
      14,    24,    19,    33,   101,     5,     0,    64,     5,     5,
       5,     5,     5,     5,     5,    34,    35,    36,    37,    38,
      59,    96,    97,    98,    27,     9,    70,    18,    18,    25,
      20,    63,    61,    61,    61,    61,    61,    24,    65,     5,
      65,    71,    61,     5,     5,     3,     4,     5,     6,    34,
      35,    36,    37,    38,    39,    41,    46,    47,    58,    61,
      88,    90,    91,    92,    93,    61,     5,     5,    59,     5,
       5,     5,     5,     5,    99,   100,    98,    54,    14,     5,
      73,    74,    61,    61,    61,    61,    61,    61,    90,     3,
      90,    92,    48,    42,    43,    44,    45,    54,    55,    56,
      57,    58,    59,    60,    46,    47,    86,    87,    88,    62,
      62,    62,    62,    62,    62,     5,    40,    25,    65,   102,
      61,    92,    15,    49,    50,    51,    52,    53,    79,    65,
       5,    75,     5,    59,     5,     5,     5,     5,    62,    62,
      90,    92,    92,    92,    92,    92,    92,    92,    92,    92,
      92,    92,    62,    65,     5,    90,   100,    28,   103,    92,
      25,    65,    72,    61,    80,    10,    74,    76,    62,    65,
      62,    62,    62,    62,    62,    62,    87,    29,    31,   105,
      90,    16,     3,    81,    12,    77,     5,    75,    29,    32,
     106,    62,    61,    62,    65,    30,   104,    75,     3,    75,
      11,    78,    90,    62,    12,    61,    75,    62,    13,     5
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    66,    67,    68,    68,    68,    68,    68,    68,    68,
      68,    69,    70,    70,    71,    71,    72,    72,    73,    73,
      74,    75,    75,    76,    77,    77,    78,    79,    79,    79,
      79,    79,    80,    80,    81,    82,    83,    84,    85,    86,
      86,    87,    87,    87,    88,    88,    88,    88,    88,    89,
      90,    90,    90,    90,    91,    91,    91,    91,    91,    91,
      91,    91,    92,    92,    92,    92,    92,    92,    92,    93,
      93,    93,    93,    93,    93,    93,    93,    94,    95,    96,
      96,    97,    97,    98,    98,    98,    98,    98,    98,    98,
      98,    99,    99,   100,   100,   100,   101,   101,   102,   102,
     103,   103,   104,   104,   105,   105,   106,   106
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,    10,     2,     0,     4,     0,     2,     0,     1,     3,
       3,     1,     3,     5,     3,     0,     7,     1,     1,     1,
       1,     1,     3,     0,     1,     3,     8,     5,     7,     1,
       3,     1,     1,     1,     1,     2,     1,     1,     1,     5,
       1,     3,     2,     3,     3,     3,     3,     3,     3,     3,
       1,     1,     1,     3,     3,     3,     3,     3,     3,     1,
       1,     4,     4,     4,     4,     4,     4,     8,     9,     1,
       1,     1,     3,     1,     3,     4,     4,     4,     4,     4,
       4,     1,     3,     1,     2,     3,     1,     0,     2,     0,
       4,     0,     2,     0,     3,     0,     2,     0
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (&yylloc, result, scanner, YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  unsigned res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location, result, scanner); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, FLOPPYOutput** result, yyscan_t scanner)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yylocationp);
  YYUSE (result);
  YYUSE (scanner);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, FLOPPYOutput** result, yyscan_t scanner)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, result, scanner);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, FLOPPYOutput** result, yyscan_t scanner)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       , result, scanner);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, result, scanner); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, FLOPPYOutput** result, yyscan_t scanner)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (result);
  YYUSE (scanner);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/*----------.
| yyparse.  |
`----------*/

int
yyparse (FLOPPYOutput** result, yyscan_t scanner)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yyls1, yysize * sizeof (*yylsp),
                    &yystacksize);

        yyls = yyls1;
        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex (&yylval, &yylloc, scanner);
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 138 "FLOPPY_parser.y" /* yacc.c:1661  */
    { 
      *result = new FLOPPYOutput((yyvsp[-1].statement)); 
      (*result)->isValid = true;
   }
#line 1615 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 3:
#line 145 "FLOPPY_parser.y" /* yacc.c:1661  */
    { (yyval.statement) = (yyvsp[0].create_table_statement); }
#line 1621 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 4:
#line 146 "FLOPPY_parser.y" /* yacc.c:1661  */
    { (yyval.statement) = (yyvsp[0].drop_table_statement); }
#line 1627 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 5:
#line 147 "FLOPPY_parser.y" /* yacc.c:1661  */
    { (yyval.statement) = (yyvsp[0].create_index_statement); }
#line 1633 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 6:
#line 148 "FLOPPY_parser.y" /* yacc.c:1661  */
    { (yyval.statement) = (yyvsp[0].drop_index_statement); }
#line 1639 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 7:
#line 149 "FLOPPY_parser.y" /* yacc.c:1661  */
    { (yyval.statement) = (yyvsp[0].insert_statement); }
#line 1645 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 8:
#line 150 "FLOPPY_parser.y" /* yacc.c:1661  */
    { (yyval.statement) = (yyvsp[0].delete_statement); }
#line 1651 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 9:
#line 151 "FLOPPY_parser.y" /* yacc.c:1661  */
    { (yyval.statement) = (yyvsp[0].update_statement); }
#line 1657 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 10:
#line 152 "FLOPPY_parser.y" /* yacc.c:1661  */
    { (yyval.statement) = (yyvsp[0].select_statement); }
#line 1663 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 11:
#line 169 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
               (yyval.create_table_statement) = new FLOPPYCreateTableStatement();
               (yyval.create_table_statement)->tableName = (yyvsp[-7].sval);
               (yyval.create_table_statement)->flags = (yyvsp[-6].flags);
               (yyval.create_table_statement)->columns = (yyvsp[-4].create_column_vec);
               (yyval.create_table_statement)->pk = (yyvsp[-2].primary_key);
               (yyval.create_table_statement)->fk = (yyvsp[-1].foreign_key_vec);
            }
#line 1676 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 12:
#line 180 "FLOPPY_parser.y" /* yacc.c:1661  */
    { 
         (yyval.flags) = (yyvsp[0].flags);
         (yyval.flags)->volatileFlag = true;
      }
#line 1685 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 13:
#line 184 "FLOPPY_parser.y" /* yacc.c:1661  */
    { 
         (yyval.flags) = new CreateTableAdditionalFunctionality(); 
         (yyval.flags)->volatileFlag = false;
         (yyval.flags)->indexOnlyFlag = false;
         (yyval.flags)->splitFlag = false;
      }
#line 1696 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 14:
#line 193 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.flags) = (yyvsp[0].flags);
         (yyval.flags)->indexOnlyFlag = true;
      }
#line 1705 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 15:
#line 197 "FLOPPY_parser.y" /* yacc.c:1661  */
    { 
         (yyval.flags) = new CreateTableAdditionalFunctionality(); 
         (yyval.flags)->indexOnlyFlag = false;
         (yyval.flags)->splitFlag = false;
      }
#line 1715 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 16:
#line 205 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.flags) = new CreateTableAdditionalFunctionality(); 
         (yyval.flags)->splitFlag = true;
      }
#line 1724 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 17:
#line 209 "FLOPPY_parser.y" /* yacc.c:1661  */
    { 
         (yyval.flags) = new CreateTableAdditionalFunctionality(); 
         (yyval.flags)->splitFlag = false;
      }
#line 1733 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 18:
#line 216 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.create_column_vec) = new std::vector<FLOPPYCreateColumn *>(); 
         (yyval.create_column_vec)->push_back((yyvsp[0].create_column)); 
      }
#line 1742 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 19:
#line 220 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyvsp[-2].create_column_vec)->push_back((yyvsp[0].create_column));
         (yyval.create_column_vec) = (yyvsp[-2].create_column_vec);
      }
#line 1751 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 20:
#line 227 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.create_column) = new FLOPPYCreateColumn();
         (yyval.create_column)->name = (yyvsp[-2].sval);
         (yyval.create_column)->type = (ColumnType) (yyvsp[-1].ival);
         (yyval.create_column)->size = (yyvsp[0].ival);
      }
#line 1762 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 21:
#line 236 "FLOPPY_parser.y" /* yacc.c:1661  */
    { 
         (yyval.str_vec) = new std::vector<char *>(); 
         (yyval.str_vec)->push_back((yyvsp[0].sval)); 
      }
#line 1771 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 22:
#line 240 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyvsp[-2].str_vec)->push_back((yyvsp[0].sval));
         (yyval.str_vec) = (yyvsp[-2].str_vec);
      }
#line 1780 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 23:
#line 247 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.primary_key) = new FLOPPYPrimaryKey();
         (yyval.primary_key)->attributes = (yyvsp[-1].str_vec);
      }
#line 1789 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 24:
#line 254 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.foreign_key_vec) = (yyvsp[-2].foreign_key_vec);
         (yyval.foreign_key_vec)->push_back((yyvsp[0].foreign_key));
      }
#line 1798 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 25:
#line 258 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.foreign_key_vec) = new std::vector<FLOPPYForeignKey *>();
      }
#line 1806 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 26:
#line 264 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.foreign_key) = new FLOPPYForeignKey();
         (yyval.foreign_key)->attributes = (yyvsp[-3].str_vec);
         (yyval.foreign_key)->refTableName = (yyvsp[0].sval);
      }
#line 1816 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 27:
#line 272 "FLOPPY_parser.y" /* yacc.c:1661  */
    { 
         (yyval.ival) = ColumnType::INT; 
      }
#line 1824 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 28:
#line 275 "FLOPPY_parser.y" /* yacc.c:1661  */
    { 
         (yyval.ival) = ColumnType::FLOAT; 
      }
#line 1832 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 29:
#line 278 "FLOPPY_parser.y" /* yacc.c:1661  */
    { 
         (yyval.ival) = ColumnType::VARCHAR; 
      }
#line 1840 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 30:
#line 281 "FLOPPY_parser.y" /* yacc.c:1661  */
    { 
         (yyval.ival) = ColumnType::DATETIME; 
      }
#line 1848 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 31:
#line 284 "FLOPPY_parser.y" /* yacc.c:1661  */
    { 
         (yyval.ival) = ColumnType::BOOLEAN; 
      }
#line 1856 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 32:
#line 290 "FLOPPY_parser.y" /* yacc.c:1661  */
    { 
      (yyval.ival) = (yyvsp[-1].ival); 
   }
#line 1864 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 33:
#line 293 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
      (yyval.ival) = 0;
   }
#line 1872 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 34:
#line 298 "FLOPPY_parser.y" /* yacc.c:1661  */
    { 
         (yyval.ival) = (yyvsp[0].ival); 
      }
#line 1880 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 35:
#line 309 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.drop_table_statement) = new FLOPPYDropTableStatement();
         (yyval.drop_table_statement)->table = (yyvsp[0].sval);
      }
#line 1889 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 36:
#line 322 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
      (yyval.create_index_statement) = new FLOPPYCreateIndexStatement();
      (yyval.create_index_statement)->name = (yyvsp[-5].sval);
      (yyval.create_index_statement)->tableReference = (yyvsp[-3].sval);
      (yyval.create_index_statement)->attributes = (yyvsp[-1].str_vec);
   }
#line 1900 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 37:
#line 334 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.drop_index_statement) = new FLOPPYDropIndexStatement();
         (yyval.drop_index_statement)->indexName = (yyvsp[-2].sval);
         (yyval.drop_index_statement)->tableName = (yyvsp[0].sval);
      }
#line 1910 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 38:
#line 348 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.insert_statement) = new FLOPPYInsertStatement();
         (yyval.insert_statement)->name = (yyvsp[-4].sval);
         (yyval.insert_statement)->values = (yyvsp[-1].value_vec);
      }
#line 1920 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 39:
#line 356 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.value_vec) = new std::vector<FLOPPYValue *>();
         (yyval.value_vec)->push_back((yyvsp[0].value));
      }
#line 1929 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 40:
#line 360 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.value_vec) = (yyvsp[-2].value_vec);
         (yyval.value_vec)->push_back((yyvsp[0].value));
      }
#line 1938 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 41:
#line 367 "FLOPPY_parser.y" /* yacc.c:1661  */
    { (yyval.value) = (yyvsp[0].value); }
#line 1944 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 42:
#line 368 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.value) = new FLOPPYValue(BooleanValue);
         (yyval.value)->bVal = true;
      }
#line 1953 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 43:
#line 372 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.value) = new FLOPPYValue(BooleanValue);
         (yyval.value)->bVal = false;
      }
#line 1962 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 44:
#line 379 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.value) = new FLOPPYValue(IntValue);
         (yyval.value)->iVal = (yyvsp[0].ival);
      }
#line 1971 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 45:
#line 383 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.value) = new FLOPPYValue(IntValue);
         (yyval.value)->iVal = -(yyvsp[0].ival);
      }
#line 1980 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 46:
#line 387 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.value) = new FLOPPYValue(FloatValue);
         (yyval.value)->fVal = (yyvsp[0].fval);
      }
#line 1989 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 47:
#line 391 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.value) = new FLOPPYValue(StringValue);
         (yyval.value)->sVal = (yyvsp[0].sval);
      }
#line 1998 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 48:
#line 395 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.value) = new FLOPPYValue(NullValue);
         (yyval.value)->sVal = NULL;
      }
#line 2007 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 49:
#line 408 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.delete_statement) = new FLOPPYDeleteStatement();
         (yyval.delete_statement)->name = (yyvsp[-2].sval);
         (yyval.delete_statement)->where = (yyvsp[0].node);
      }
#line 2017 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 50:
#line 416 "FLOPPY_parser.y" /* yacc.c:1661  */
    { (yyval.node) = (yyvsp[0].node); }
#line 2023 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 51:
#line 417 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.node) = new FLOPPYNode(ConditionNode);
         (yyval.node)->node.left = (yyvsp[-2].node);
         (yyval.node)->node.op = FLOPPYNodeOperator::AndOperator;
         (yyval.node)->node.right= (yyvsp[0].node);
      }
#line 2034 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 52:
#line 423 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.node) = new FLOPPYNode(ConditionNode);
         (yyval.node)->node.left = (yyvsp[0].node);
         (yyval.node)->node.op = FLOPPYNodeOperator::NotOperator;
      }
#line 2044 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 53:
#line 428 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.node) = new FLOPPYNode(ConditionNode);
         (yyval.node)->node.left = (yyvsp[-1].node);
         (yyval.node)->node.op = FLOPPYNodeOperator::ParenthesisOperator;
   }
#line 2054 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 54:
#line 436 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.node) = new FLOPPYNode(ConditionNode);
         (yyval.node)->node.left = (yyvsp[-2].node);
         (yyval.node)->node.op = FLOPPYNodeOperator::LessThanOperator;
         (yyval.node)->node.right= (yyvsp[0].node);
      }
#line 2065 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 55:
#line 442 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.node) = new FLOPPYNode(ConditionNode);
         (yyval.node)->node.left = (yyvsp[-2].node);
         (yyval.node)->node.op = FLOPPYNodeOperator::GreaterThanOperator;
         (yyval.node)->node.right= (yyvsp[0].node);
      }
#line 2076 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 56:
#line 448 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.node) = new FLOPPYNode(ConditionNode);
         (yyval.node)->node.left = (yyvsp[-2].node);
         (yyval.node)->node.op = FLOPPYNodeOperator::EqualOperator;
         (yyval.node)->node.right= (yyvsp[0].node);
      }
#line 2087 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 57:
#line 454 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.node) = new FLOPPYNode(ConditionNode);
         (yyval.node)->node.left = (yyvsp[-2].node);
         (yyval.node)->node.op = FLOPPYNodeOperator::LessThanEqualOperator;
         (yyval.node)->node.right= (yyvsp[0].node);
      }
#line 2098 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 58:
#line 460 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.node) = new FLOPPYNode(ConditionNode);
         (yyval.node)->node.left = (yyvsp[-2].node);
         (yyval.node)->node.op = FLOPPYNodeOperator::GreaterThanEqualOperator;
         (yyval.node)->node.right= (yyvsp[0].node);
      }
#line 2109 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 59:
#line 466 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.node) = new FLOPPYNode(ConditionNode);
         (yyval.node)->node.left = (yyvsp[-2].node);
         (yyval.node)->node.op = FLOPPYNodeOperator::NotEqualOperator;
         (yyval.node)->node.right= (yyvsp[0].node);
      }
#line 2120 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 60:
#line 472 "FLOPPY_parser.y" /* yacc.c:1661  */
    { 
         (yyval.node) = new FLOPPYNode(ValueNode); 
         (yyval.node)->value = new FLOPPYValue(BooleanValue);
         (yyval.node)->value->bVal = true;
      }
#line 2130 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 61:
#line 477 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.node) = new FLOPPYNode(ValueNode); 
         (yyval.node)->value = new FLOPPYValue(BooleanValue);
         (yyval.node)->value->bVal = false;
      }
#line 2140 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 62:
#line 485 "FLOPPY_parser.y" /* yacc.c:1661  */
    { (yyval.node) = (yyvsp[0].node); }
#line 2146 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 63:
#line 486 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.node) = new FLOPPYNode(ExpressionNode);
         (yyval.node)->node.left = (yyvsp[-2].node);
         (yyval.node)->node.op = FLOPPYNodeOperator::PlusOperator;
         (yyval.node)->node.right= (yyvsp[0].node);
      }
#line 2157 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 64:
#line 492 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.node) = new FLOPPYNode(ExpressionNode);
         (yyval.node)->node.left = (yyvsp[-2].node);
         (yyval.node)->node.op = FLOPPYNodeOperator::MinusOperator;
         (yyval.node)->node.right= (yyvsp[0].node);
      }
#line 2168 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 65:
#line 498 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.node) = new FLOPPYNode(ExpressionNode);
         (yyval.node)->node.left = (yyvsp[-2].node);
         (yyval.node)->node.op = FLOPPYNodeOperator::TimesOperator;
         (yyval.node)->node.right= (yyvsp[0].node);
      }
#line 2179 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 66:
#line 504 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.node) = new FLOPPYNode(ExpressionNode);
         (yyval.node)->node.left = (yyvsp[-2].node);
         (yyval.node)->node.op = FLOPPYNodeOperator::DivideOperator;
         (yyval.node)->node.right= (yyvsp[0].node);
      }
#line 2190 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 67:
#line 510 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.node) = new FLOPPYNode(ExpressionNode);
         (yyval.node)->node.left = (yyvsp[-2].node);
         (yyval.node)->node.op = FLOPPYNodeOperator::ModOperator;
         (yyval.node)->node.right= (yyvsp[0].node);
      }
#line 2201 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 68:
#line 516 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.node) = new FLOPPYNode(ExpressionNode);
         (yyval.node)->node.op = FLOPPYNodeOperator::ParenthesisOperator;
         (yyval.node)->node.left = (yyvsp[-1].node);
      }
#line 2211 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 69:
#line 524 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.node) = new FLOPPYNode(ValueNode);
         (yyval.node)->value = new FLOPPYValue(AttributeValue);
         (yyval.node)->value->sVal = (yyvsp[0].sval);
      }
#line 2221 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 70:
#line 529 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.node) = new FLOPPYNode(ValueNode);
         (yyval.node)->value = (yyvsp[0].value);
      }
#line 2230 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 71:
#line 533 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.node) = new FLOPPYNode(AggregateNode);
         (yyval.node)->aggregate.op = FLOPPYAggregateOperator::MinAggregate;
         (yyval.node)->aggregate.value = new FLOPPYValue(AttributeValue);
         (yyval.node)->aggregate.value->sVal = (yyvsp[-1].sval);
      }
#line 2241 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 72:
#line 539 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.node) = new FLOPPYNode(AggregateNode);
         (yyval.node)->aggregate.op = FLOPPYAggregateOperator::MaxAggregate;
         (yyval.node)->aggregate.value = new FLOPPYValue(AttributeValue);
         (yyval.node)->aggregate.value->sVal = (yyvsp[-1].sval);
      }
#line 2252 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 73:
#line 545 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.node) = new FLOPPYNode(AggregateNode);
         (yyval.node)->aggregate.op = FLOPPYAggregateOperator::SumAggregate;
         (yyval.node)->aggregate.value = new FLOPPYValue(AttributeValue);
         (yyval.node)->aggregate.value->sVal = (yyvsp[-1].sval);
      }
#line 2263 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 74:
#line 551 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.node) = new FLOPPYNode(AggregateNode);
         (yyval.node)->aggregate.op = FLOPPYAggregateOperator::AverageAggregate;
         (yyval.node)->aggregate.value = new FLOPPYValue(AttributeValue);
         (yyval.node)->aggregate.value->sVal = (yyvsp[-1].sval);
      }
#line 2274 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 75:
#line 557 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.node) = new FLOPPYNode(AggregateNode);
         (yyval.node)->aggregate.op = FLOPPYAggregateOperator::CountAggregate;
         (yyval.node)->aggregate.value = new FLOPPYValue(AttributeValue);
         (yyval.node)->aggregate.value->sVal = (yyvsp[-1].sval);
      }
#line 2285 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 76:
#line 563 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.node) = new FLOPPYNode(AggregateNode);
         (yyval.node)->aggregate.op = FLOPPYAggregateOperator::CountStarAggregate;
         (yyval.node)->aggregate.value = NULL;
      }
#line 2295 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 77:
#line 579 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.update_statement) = new FLOPPYUpdateStatement();
         (yyval.update_statement)->tableName = (yyvsp[-6].sval);
         (yyval.update_statement)->attributeName = (yyvsp[-4].sval);
         (yyval.update_statement)->attributeExpression= (yyvsp[-2].node);
         (yyval.update_statement)->whereExpression = (yyvsp[0].node);
      }
#line 2307 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 78:
#line 604 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.select_statement) = new FLOPPYSelectStatement();
         (yyval.select_statement)->distinct = (yyvsp[-7].bval);
         (yyval.select_statement)->selectItems = (yyvsp[-6].select_item_vec);
         (yyval.select_statement)->tableSpecs = (yyvsp[-4].table_spec_vec);
         (yyval.select_statement)->whereCondition = (yyvsp[-3].node);
         (yyval.select_statement)->groupBy = (yyvsp[-2].group_by);
         (yyval.select_statement)->orderBys = (yyvsp[-1].str_vec);
         (yyval.select_statement)->limit= (yyvsp[0].ival);
      }
#line 2322 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 79:
#line 617 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.select_item_vec) = new std::vector<FLOPPYSelectItem *>();
         (yyval.select_item_vec)->push_back(new FLOPPYSelectItem(FLOPPYSelectItemType::StarType));
      }
#line 2331 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 80:
#line 621 "FLOPPY_parser.y" /* yacc.c:1661  */
    { (yyval.select_item_vec) = (yyvsp[0].select_item_vec); }
#line 2337 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 81:
#line 624 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.select_item_vec) = new std::vector<FLOPPYSelectItem *>(); 
         (yyval.select_item_vec)->push_back((yyvsp[0].select_item));
      }
#line 2346 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 82:
#line 628 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.select_item_vec) = (yyvsp[-2].select_item_vec);
         (yyval.select_item_vec)->push_back((yyvsp[0].select_item));
      }
#line 2355 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 83:
#line 635 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.select_item) = new FLOPPYSelectItem(FLOPPYSelectItemType::AttributeType);
         (yyval.select_item)->attribute = (yyvsp[0].sval);
      }
#line 2364 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 84:
#line 639 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.select_item) = new FLOPPYSelectItem(FLOPPYSelectItemType::TableAttributeType);
         (yyval.select_item)->tableAttribute.tableName = (yyvsp[-2].sval);
         (yyval.select_item)->tableAttribute.attribute = (yyvsp[0].sval);
      }
#line 2374 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 85:
#line 644 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.select_item) = new FLOPPYSelectItem(FLOPPYSelectItemType::AggregateType);
         (yyval.select_item)->aggregate.op = FLOPPYAggregateOperator::CountAggregate;
         (yyval.select_item)->aggregate.value = new FLOPPYValue(ValueType::AttributeValue);
         (yyval.select_item)->aggregate.value->sVal = (yyvsp[-1].sval);
      }
#line 2385 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 86:
#line 650 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.select_item) = new FLOPPYSelectItem(FLOPPYSelectItemType::AggregateType);
         (yyval.select_item)->aggregate.op = FLOPPYAggregateOperator::AverageAggregate;
         (yyval.select_item)->aggregate.value = new FLOPPYValue(ValueType::AttributeValue);
         (yyval.select_item)->aggregate.value->sVal = (yyvsp[-1].sval);
      }
#line 2396 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 87:
#line 656 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.select_item) = new FLOPPYSelectItem(FLOPPYSelectItemType::AggregateType);
         (yyval.select_item)->aggregate.op = FLOPPYAggregateOperator::MaxAggregate;
         (yyval.select_item)->aggregate.value = new FLOPPYValue(ValueType::AttributeValue);
         (yyval.select_item)->aggregate.value->sVal = (yyvsp[-1].sval);
      }
#line 2407 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 88:
#line 662 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.select_item) = new FLOPPYSelectItem(FLOPPYSelectItemType::AggregateType);
         (yyval.select_item)->aggregate.op = FLOPPYAggregateOperator::MinAggregate;
         (yyval.select_item)->aggregate.value = new FLOPPYValue(ValueType::AttributeValue);
         (yyval.select_item)->aggregate.value->sVal = (yyvsp[-1].sval);
      }
#line 2418 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 89:
#line 668 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.select_item) = new FLOPPYSelectItem(FLOPPYSelectItemType::AggregateType);
         (yyval.select_item)->aggregate.op = FLOPPYAggregateOperator::SumAggregate;
         (yyval.select_item)->aggregate.value = new FLOPPYValue(ValueType::AttributeValue);
         (yyval.select_item)->aggregate.value->sVal = (yyvsp[-1].sval);
      }
#line 2429 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 90:
#line 674 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.select_item) = new FLOPPYSelectItem(FLOPPYSelectItemType::AggregateType);
         (yyval.select_item)->aggregate.op = FLOPPYAggregateOperator::CountStarAggregate;
         (yyval.select_item)->aggregate.value = NULL;
      }
#line 2439 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 91:
#line 682 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.table_spec_vec) = new std::vector<FLOPPYTableSpec *>();
         (yyval.table_spec_vec)->push_back((yyvsp[0].table_spec));
      }
#line 2448 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 92:
#line 686 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.table_spec_vec) = (yyvsp[-2].table_spec_vec);
         (yyval.table_spec_vec)->push_back((yyvsp[0].table_spec));
      }
#line 2457 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 93:
#line 693 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.table_spec) = new FLOPPYTableSpec();
         (yyval.table_spec)->tableName = (yyvsp[0].sval);
         (yyval.table_spec)->alias = NULL;
      }
#line 2467 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 94:
#line 698 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.table_spec) = new FLOPPYTableSpec();
         (yyval.table_spec)->tableName = (yyvsp[-1].sval);
         (yyval.table_spec)->alias = (yyvsp[0].sval);
      }
#line 2477 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 95:
#line 703 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.table_spec) = new FLOPPYTableSpec();
         (yyval.table_spec)->tableName = (yyvsp[-2].sval);
         (yyval.table_spec)->alias = (yyvsp[0].sval);
      }
#line 2487 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 96:
#line 711 "FLOPPY_parser.y" /* yacc.c:1661  */
    { (yyval.bval) = true; }
#line 2493 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 97:
#line 712 "FLOPPY_parser.y" /* yacc.c:1661  */
    { (yyval.bval) = false; }
#line 2499 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 98:
#line 716 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.node) = (yyvsp[0].node);
      }
#line 2507 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 99:
#line 719 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.node) = NULL;
      }
#line 2515 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 100:
#line 726 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.group_by) = new FLOPPYGroupBy();
         (yyval.group_by)->groupByAttributes = (yyvsp[-1].str_vec);
         (yyval.group_by)->havingCondition = (yyvsp[0].node);
      }
#line 2525 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 101:
#line 731 "FLOPPY_parser.y" /* yacc.c:1661  */
    { 
         (yyval.group_by) = NULL;
      }
#line 2533 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 102:
#line 737 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.node) = (yyvsp[0].node);
      }
#line 2541 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 103:
#line 740 "FLOPPY_parser.y" /* yacc.c:1661  */
    { 
         (yyval.node) = NULL;
      }
#line 2549 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 104:
#line 746 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.str_vec) = (yyvsp[0].str_vec);
      }
#line 2557 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 105:
#line 749 "FLOPPY_parser.y" /* yacc.c:1661  */
    { 
         (yyval.str_vec) = NULL;
      }
#line 2565 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 106:
#line 755 "FLOPPY_parser.y" /* yacc.c:1661  */
    {
         (yyval.ival) = (yyvsp[0].ival);
      }
#line 2573 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;

  case 107:
#line 758 "FLOPPY_parser.y" /* yacc.c:1661  */
    { 
         (yyval.ival) = -1;
      }
#line 2581 "FLOPPY_parser.cpp" /* yacc.c:1661  */
    break;


#line 2585 "FLOPPY_parser.cpp" /* yacc.c:1661  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (&yylloc, result, scanner, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (&yylloc, result, scanner, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc, result, scanner);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[1] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp, result, scanner);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, result, scanner, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, result, scanner);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yylsp, result, scanner);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 763 "FLOPPY_parser.y" /* yacc.c:1906  */
