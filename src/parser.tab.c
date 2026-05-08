/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 11 "grammar/parser.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

/* ── External declarations ── */
extern int yylex(void);
extern int yyline;
extern FILE *yyin;

/* ── The root of the AST ── */
ASTNode *ast_root = NULL;

/* ── Error handling ── */
int syntax_error_count = 0;

void yyerror(const char *msg) {
    fprintf(stderr, "Syntax Error at line %d: %s\n", yyline, msg);
    syntax_error_count++;
}

/* ── Helper: allocate param/arg arrays ── */
static ASTNode **alloc_node_array(int capacity) {
    return (ASTNode **)calloc(capacity, sizeof(ASTNode *));
}

#line 99 "src/parser.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_INT = 3,                        /* INT  */
  YYSYMBOL_FLOAT = 4,                      /* FLOAT  */
  YYSYMBOL_CHAR = 5,                       /* CHAR  */
  YYSYMBOL_VOID = 6,                       /* VOID  */
  YYSYMBOL_IDENTIFIER = 7,                 /* IDENTIFIER  */
  YYSYMBOL_STRING_LITERAL = 8,             /* STRING_LITERAL  */
  YYSYMBOL_INTEGER_LITERAL = 9,            /* INTEGER_LITERAL  */
  YYSYMBOL_FLOAT_LITERAL = 10,             /* FLOAT_LITERAL  */
  YYSYMBOL_CHAR_LITERAL = 11,              /* CHAR_LITERAL  */
  YYSYMBOL_IF = 12,                        /* IF  */
  YYSYMBOL_ELSE = 13,                      /* ELSE  */
  YYSYMBOL_WHILE = 14,                     /* WHILE  */
  YYSYMBOL_FOR = 15,                       /* FOR  */
  YYSYMBOL_RETURN = 16,                    /* RETURN  */
  YYSYMBOL_PRINTF = 17,                    /* PRINTF  */
  YYSYMBOL_PLUS = 18,                      /* PLUS  */
  YYSYMBOL_MINUS = 19,                     /* MINUS  */
  YYSYMBOL_MULTIPLY = 20,                  /* MULTIPLY  */
  YYSYMBOL_DIVIDE = 21,                    /* DIVIDE  */
  YYSYMBOL_MODULO = 22,                    /* MODULO  */
  YYSYMBOL_ASSIGN = 23,                    /* ASSIGN  */
  YYSYMBOL_EQ = 24,                        /* EQ  */
  YYSYMBOL_NEQ = 25,                       /* NEQ  */
  YYSYMBOL_LT = 26,                        /* LT  */
  YYSYMBOL_GT = 27,                        /* GT  */
  YYSYMBOL_LTE = 28,                       /* LTE  */
  YYSYMBOL_GTE = 29,                       /* GTE  */
  YYSYMBOL_AND = 30,                       /* AND  */
  YYSYMBOL_OR = 31,                        /* OR  */
  YYSYMBOL_NOT = 32,                       /* NOT  */
  YYSYMBOL_SEMICOLON = 33,                 /* SEMICOLON  */
  YYSYMBOL_COMMA = 34,                     /* COMMA  */
  YYSYMBOL_LPAREN = 35,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 36,                    /* RPAREN  */
  YYSYMBOL_LBRACE = 37,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 38,                    /* RBRACE  */
  YYSYMBOL_LOWER_THAN_ELSE = 39,           /* LOWER_THAN_ELSE  */
  YYSYMBOL_UMINUS = 40,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 41,                  /* $accept  */
  YYSYMBOL_program = 42,                   /* program  */
  YYSYMBOL_declaration_list = 43,          /* declaration_list  */
  YYSYMBOL_declaration = 44,               /* declaration  */
  YYSYMBOL_var_declaration = 45,           /* var_declaration  */
  YYSYMBOL_type_specifier = 46,            /* type_specifier  */
  YYSYMBOL_fun_declaration = 47,           /* fun_declaration  */
  YYSYMBOL_param_list = 48,                /* param_list  */
  YYSYMBOL_param = 49,                     /* param  */
  YYSYMBOL_compound_stmt = 50,             /* compound_stmt  */
  YYSYMBOL_statement_list = 51,            /* statement_list  */
  YYSYMBOL_statement = 52,                 /* statement  */
  YYSYMBOL_expression_stmt = 53,           /* expression_stmt  */
  YYSYMBOL_selection_stmt = 54,            /* selection_stmt  */
  YYSYMBOL_iteration_stmt = 55,            /* iteration_stmt  */
  YYSYMBOL_return_stmt = 56,               /* return_stmt  */
  YYSYMBOL_printf_stmt = 57,               /* printf_stmt  */
  YYSYMBOL_expression = 58,                /* expression  */
  YYSYMBOL_assignment_expr = 59,           /* assignment_expr  */
  YYSYMBOL_or_expr = 60,                   /* or_expr  */
  YYSYMBOL_and_expr = 61,                  /* and_expr  */
  YYSYMBOL_equality_expr = 62,             /* equality_expr  */
  YYSYMBOL_relational_expr = 63,           /* relational_expr  */
  YYSYMBOL_additive_expr = 64,             /* additive_expr  */
  YYSYMBOL_multiplicative_expr = 65,       /* multiplicative_expr  */
  YYSYMBOL_unary_expr = 66,                /* unary_expr  */
  YYSYMBOL_primary_expr = 67,              /* primary_expr  */
  YYSYMBOL_arg_list = 68                   /* arg_list  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

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


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
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

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

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
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  11
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   227

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  41
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  28
/* YYNRULES -- Number of rules.  */
#define YYNRULES  73
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  136

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   295


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      35,    36,    37,    38,    39,    40
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   108,   108,   118,   123,   131,   132,   137,   141,   149,
     150,   151,   152,   157,   161,   169,   174,   181,   189,   196,
     203,   208,   216,   217,   218,   219,   220,   221,   222,   227,
     230,   237,   240,   247,   250,   259,   262,   269,   273,   284,
     285,   290,   298,   299,   306,   307,   314,   315,   318,   325,
     326,   329,   332,   335,   342,   343,   346,   353,   354,   357,
     360,   367,   368,   371,   378,   381,   384,   387,   391,   395,
     399,   403,   410,   415
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "INT", "FLOAT", "CHAR",
  "VOID", "IDENTIFIER", "STRING_LITERAL", "INTEGER_LITERAL",
  "FLOAT_LITERAL", "CHAR_LITERAL", "IF", "ELSE", "WHILE", "FOR", "RETURN",
  "PRINTF", "PLUS", "MINUS", "MULTIPLY", "DIVIDE", "MODULO", "ASSIGN",
  "EQ", "NEQ", "LT", "GT", "LTE", "GTE", "AND", "OR", "NOT", "SEMICOLON",
  "COMMA", "LPAREN", "RPAREN", "LBRACE", "RBRACE", "LOWER_THAN_ELSE",
  "UMINUS", "$accept", "program", "declaration_list", "declaration",
  "var_declaration", "type_specifier", "fun_declaration", "param_list",
  "param", "compound_stmt", "statement_list", "statement",
  "expression_stmt", "selection_stmt", "iteration_stmt", "return_stmt",
  "printf_stmt", "expression", "assignment_expr", "or_expr", "and_expr",
  "equality_expr", "relational_expr", "additive_expr",
  "multiplicative_expr", "unary_expr", "primary_expr", "arg_list", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-99)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      76,   -99,   -99,   -99,   -99,    27,    76,   -99,   -99,    22,
     -99,   -99,   -99,   -17,   178,   -99,    17,   -11,   -99,   -99,
     -99,   -99,   192,   192,   178,    41,   -99,    52,    74,    11,
      58,    43,    -7,   -99,   -99,    68,    99,   -25,   -99,   178,
      57,    72,   -99,   -99,    84,   -99,   192,   192,   192,   192,
     192,   192,   192,   192,   192,   192,   192,   192,   192,    40,
     -99,   -99,    76,    68,   -99,   -99,   -99,    24,   -99,    74,
      11,    58,    58,    43,    43,    43,    43,    -7,    -7,   -99,
     -99,   -99,    90,    92,    93,   158,    94,   -99,   -99,   -99,
     123,   -99,   107,   -99,   -99,   -99,   -99,   -99,   -99,    98,
     -99,   -99,   178,   -99,   178,   178,   163,   -99,   100,   124,
       9,   -99,   -99,   -99,   -99,   101,   102,   163,   -99,    64,
     143,   143,   178,   178,   103,   121,   -99,   105,    65,   -99,
     143,   143,   110,   -99,   -99,   -99
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     9,    10,    11,    12,     0,     2,     3,     5,     0,
       6,     1,     4,     0,     0,     7,     0,    68,    67,    64,
      65,    66,     0,     0,     0,     0,    39,    40,    42,    44,
      46,    49,    54,    57,    61,     0,     0,     0,    15,     0,
       0,    68,    62,    63,     0,     8,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      14,    17,     0,     0,    41,    70,    72,     0,    71,    43,
      45,    47,    48,    50,    51,    52,    53,    55,    56,    58,
      59,    60,     0,     0,     0,     0,     0,    30,    19,    22,
       0,    24,     0,    20,    23,    25,    26,    27,    28,     0,
      16,    13,     0,    69,     0,     0,     0,    36,     0,     0,
       0,    18,    21,    29,    73,     0,     0,     0,    35,     0,
       0,     0,     0,     0,     0,    31,    33,     0,     0,    37,
       0,     0,     0,    32,    34,    38
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -99,   -99,   -99,   129,    28,     1,   -99,   -99,   117,   -30,
     -99,   -90,   -98,   -99,   -99,   -99,   -99,   -14,   -99,   -99,
     115,   109,    21,    44,    48,   -19,   -99,    60
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     5,     6,     7,    89,    90,    10,    37,    38,    91,
      92,    93,    94,    95,    96,    97,    98,    99,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    67
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      25,     9,   112,    42,    43,    60,    14,     9,   117,    62,
      44,    63,    39,    56,    57,    58,    15,    36,    16,   122,
       1,     2,     3,     4,    40,    64,    66,    11,     8,    13,
     125,   126,    14,   101,     8,    48,    49,    79,    80,    81,
     133,   134,    15,     1,     2,     3,     4,    17,    18,    19,
      20,    21,    82,    35,    83,    84,    85,    86,   102,    22,
     103,    54,    55,    36,    17,    18,    19,    20,    21,    71,
      72,   108,    23,    87,    45,    24,    22,    59,    88,     1,
       2,     3,     4,    46,    50,    51,    52,    53,   114,    23,
     115,   116,    24,    65,    73,    74,    75,    76,   123,   102,
     124,   132,    77,    78,    47,    59,    61,    40,   127,    66,
       1,     2,     3,     4,    17,    18,    19,    20,    21,    82,
      68,    83,    84,    85,    86,   104,    22,   105,   106,   109,
     110,   113,   119,   118,   130,    12,   129,   120,   121,    23,
      87,   131,    24,   135,    59,   111,     1,     2,     3,     4,
      17,    18,    19,    20,    21,    82,    70,    83,    84,    85,
      86,    69,    22,     0,     0,    17,    18,    19,    20,    21,
      17,    18,    19,    20,    21,    23,    87,    22,    24,   100,
      59,     0,    22,   128,     0,    17,    18,    19,    20,    21,
      23,   107,     0,    24,     0,    23,    87,    22,    24,    41,
      18,    19,    20,    21,     0,     0,     0,     0,     0,     0,
      23,    22,     0,    24,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    23,     0,     0,    24
};

static const yytype_int16 yycheck[] =
{
      14,     0,    92,    22,    23,    35,    23,     6,   106,    34,
      24,    36,    23,    20,    21,    22,    33,    16,    35,   117,
       3,     4,     5,     6,    35,    39,    40,     0,     0,     7,
     120,   121,    23,    63,     6,    24,    25,    56,    57,    58,
     130,   131,    33,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    36,    14,    15,    16,    17,    34,    19,
      36,    18,    19,    62,     7,     8,     9,    10,    11,    48,
      49,    85,    32,    33,    33,    35,    19,    37,    38,     3,
       4,     5,     6,    31,    26,    27,    28,    29,   102,    32,
     104,   105,    35,    36,    50,    51,    52,    53,    34,    34,
      36,    36,    54,    55,    30,    37,     7,    35,   122,   123,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      36,    14,    15,    16,    17,    35,    19,    35,    35,    35,
       7,    33,     8,    33,    13,     6,    33,    36,    36,    32,
      33,    36,    35,    33,    37,    38,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    47,    14,    15,    16,
      17,    46,    19,    -1,    -1,     7,     8,     9,    10,    11,
       7,     8,     9,    10,    11,    32,    33,    19,    35,    62,
      37,    -1,    19,   123,    -1,     7,     8,     9,    10,    11,
      32,    33,    -1,    35,    -1,    32,    33,    19,    35,     7,
       8,     9,    10,    11,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    19,    -1,    35,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    35
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     6,    42,    43,    44,    45,    46,
      47,     0,    44,     7,    23,    33,    35,     7,     8,     9,
      10,    11,    19,    32,    35,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    36,    46,    48,    49,    23,
      35,     7,    66,    66,    58,    33,    31,    30,    24,    25,
      26,    27,    28,    29,    18,    19,    20,    21,    22,    37,
      50,     7,    34,    36,    58,    36,    58,    68,    36,    61,
      62,    63,    63,    64,    64,    64,    64,    65,    65,    66,
      66,    66,    12,    14,    15,    16,    17,    33,    38,    45,
      46,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      49,    50,    34,    36,    35,    35,    35,    33,    58,    35,
       7,    38,    52,    33,    58,    58,    58,    53,    33,     8,
      36,    36,    53,    34,    36,    52,    52,    58,    68,    33,
      13,    36,    36,    52,    52,    33
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    41,    42,    43,    43,    44,    44,    45,    45,    46,
      46,    46,    46,    47,    47,    48,    48,    49,    50,    50,
      51,    51,    52,    52,    52,    52,    52,    52,    52,    53,
      53,    54,    54,    55,    55,    56,    56,    57,    57,    58,
      58,    59,    60,    60,    61,    61,    62,    62,    62,    63,
      63,    63,    63,    63,    64,    64,    64,    65,    65,    65,
      65,    66,    66,    66,    67,    67,    67,    67,    67,    67,
      67,    67,    68,    68
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     2,     1,     1,     3,     5,     1,
       1,     1,     1,     6,     5,     1,     3,     2,     3,     2,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     2,
       1,     5,     7,     5,     7,     3,     2,     5,     7,     1,
       1,     3,     1,     3,     1,     3,     1,     3,     3,     1,
       3,     3,     3,     3,     1,     3,     3,     1,     3,     3,
       3,     1,     2,     2,     1,     1,     1,     1,     1,     4,
       3,     3,     1,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
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
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


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




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
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


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: declaration_list  */
#line 108 "grammar/parser.y"
                       {
        ast_root = create_program_node(1);
        for (int i = 0; i < (yyvsp[0].node_list).count; i++) {
            program_add_declaration(ast_root, (yyvsp[0].node_list).nodes[i]);
        }
        free((yyvsp[0].node_list).nodes);
    }
#line 1269 "src/parser.tab.c"
    break;

  case 3: /* declaration_list: declaration  */
#line 118 "grammar/parser.y"
                  {
        (yyval.node_list).nodes = alloc_node_array(MAX_CHILDREN);
        (yyval.node_list).count = 0;
        (yyval.node_list).nodes[(yyval.node_list).count++] = (yyvsp[0].node);
    }
#line 1279 "src/parser.tab.c"
    break;

  case 4: /* declaration_list: declaration_list declaration  */
#line 123 "grammar/parser.y"
                                   {
        (yyval.node_list) = (yyvsp[-1].node_list);
        (yyval.node_list).nodes[(yyval.node_list).count++] = (yyvsp[0].node);
    }
#line 1288 "src/parser.tab.c"
    break;

  case 5: /* declaration: var_declaration  */
#line 131 "grammar/parser.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 1294 "src/parser.tab.c"
    break;

  case 6: /* declaration: fun_declaration  */
#line 132 "grammar/parser.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 1300 "src/parser.tab.c"
    break;

  case 7: /* var_declaration: type_specifier IDENTIFIER SEMICOLON  */
#line 137 "grammar/parser.y"
                                          {
        (yyval.node) = create_var_decl_node((yyvsp[-2].str), (yyvsp[-1].str), NULL, yyline);
        free((yyvsp[-2].str)); free((yyvsp[-1].str));
    }
#line 1309 "src/parser.tab.c"
    break;

  case 8: /* var_declaration: type_specifier IDENTIFIER ASSIGN expression SEMICOLON  */
#line 141 "grammar/parser.y"
                                                            {
        (yyval.node) = create_var_decl_node((yyvsp[-4].str), (yyvsp[-3].str), (yyvsp[-1].node), yyline);
        free((yyvsp[-4].str)); free((yyvsp[-3].str));
    }
#line 1318 "src/parser.tab.c"
    break;

  case 9: /* type_specifier: INT  */
#line 149 "grammar/parser.y"
             { (yyval.str) = strdup("int"); }
#line 1324 "src/parser.tab.c"
    break;

  case 10: /* type_specifier: FLOAT  */
#line 150 "grammar/parser.y"
             { (yyval.str) = strdup("float"); }
#line 1330 "src/parser.tab.c"
    break;

  case 11: /* type_specifier: CHAR  */
#line 151 "grammar/parser.y"
             { (yyval.str) = strdup("char"); }
#line 1336 "src/parser.tab.c"
    break;

  case 12: /* type_specifier: VOID  */
#line 152 "grammar/parser.y"
             { (yyval.str) = strdup("void"); }
#line 1342 "src/parser.tab.c"
    break;

  case 13: /* fun_declaration: type_specifier IDENTIFIER LPAREN param_list RPAREN compound_stmt  */
#line 157 "grammar/parser.y"
                                                                       {
        (yyval.node) = create_fun_decl_node((yyvsp[-5].str), (yyvsp[-4].str), (yyvsp[-2].node_list).nodes, (yyvsp[-2].node_list).count, (yyvsp[0].node), yyline);
        free((yyvsp[-5].str)); free((yyvsp[-4].str));
    }
#line 1351 "src/parser.tab.c"
    break;

  case 14: /* fun_declaration: type_specifier IDENTIFIER LPAREN RPAREN compound_stmt  */
#line 161 "grammar/parser.y"
                                                            {
        (yyval.node) = create_fun_decl_node((yyvsp[-4].str), (yyvsp[-3].str), NULL, 0, (yyvsp[0].node), yyline);
        free((yyvsp[-4].str)); free((yyvsp[-3].str));
    }
#line 1360 "src/parser.tab.c"
    break;

  case 15: /* param_list: param  */
#line 169 "grammar/parser.y"
            {
        (yyval.node_list).nodes = alloc_node_array(MAX_PARAMS);
        (yyval.node_list).count = 0;
        (yyval.node_list).nodes[(yyval.node_list).count++] = (yyvsp[0].node);
    }
#line 1370 "src/parser.tab.c"
    break;

  case 16: /* param_list: param_list COMMA param  */
#line 174 "grammar/parser.y"
                             {
        (yyval.node_list) = (yyvsp[-2].node_list);
        (yyval.node_list).nodes[(yyval.node_list).count++] = (yyvsp[0].node);
    }
#line 1379 "src/parser.tab.c"
    break;

  case 17: /* param: type_specifier IDENTIFIER  */
#line 181 "grammar/parser.y"
                                {
        (yyval.node) = create_param_node((yyvsp[-1].str), (yyvsp[0].str), yyline);
        free((yyvsp[-1].str)); free((yyvsp[0].str));
    }
#line 1388 "src/parser.tab.c"
    break;

  case 18: /* compound_stmt: LBRACE statement_list RBRACE  */
#line 189 "grammar/parser.y"
                                   {
        (yyval.node) = create_compound_node(yyline);
        for (int i = 0; i < (yyvsp[-1].node_list).count; i++) {
            compound_add_item((yyval.node), (yyvsp[-1].node_list).nodes[i]);
        }
        free((yyvsp[-1].node_list).nodes);
    }
#line 1400 "src/parser.tab.c"
    break;

  case 19: /* compound_stmt: LBRACE RBRACE  */
#line 196 "grammar/parser.y"
                    {
        (yyval.node) = create_compound_node(yyline);
    }
#line 1408 "src/parser.tab.c"
    break;

  case 20: /* statement_list: statement  */
#line 203 "grammar/parser.y"
                {
        (yyval.node_list).nodes = alloc_node_array(MAX_CHILDREN);
        (yyval.node_list).count = 0;
        if ((yyvsp[0].node)) (yyval.node_list).nodes[(yyval.node_list).count++] = (yyvsp[0].node);
    }
#line 1418 "src/parser.tab.c"
    break;

  case 21: /* statement_list: statement_list statement  */
#line 208 "grammar/parser.y"
                               {
        (yyval.node_list) = (yyvsp[-1].node_list);
        if ((yyvsp[0].node)) (yyval.node_list).nodes[(yyval.node_list).count++] = (yyvsp[0].node);
    }
#line 1427 "src/parser.tab.c"
    break;

  case 22: /* statement: var_declaration  */
#line 216 "grammar/parser.y"
                          { (yyval.node) = (yyvsp[0].node); }
#line 1433 "src/parser.tab.c"
    break;

  case 23: /* statement: expression_stmt  */
#line 217 "grammar/parser.y"
                          { (yyval.node) = (yyvsp[0].node); }
#line 1439 "src/parser.tab.c"
    break;

  case 24: /* statement: compound_stmt  */
#line 218 "grammar/parser.y"
                          { (yyval.node) = (yyvsp[0].node); }
#line 1445 "src/parser.tab.c"
    break;

  case 25: /* statement: selection_stmt  */
#line 219 "grammar/parser.y"
                          { (yyval.node) = (yyvsp[0].node); }
#line 1451 "src/parser.tab.c"
    break;

  case 26: /* statement: iteration_stmt  */
#line 220 "grammar/parser.y"
                          { (yyval.node) = (yyvsp[0].node); }
#line 1457 "src/parser.tab.c"
    break;

  case 27: /* statement: return_stmt  */
#line 221 "grammar/parser.y"
                          { (yyval.node) = (yyvsp[0].node); }
#line 1463 "src/parser.tab.c"
    break;

  case 28: /* statement: printf_stmt  */
#line 222 "grammar/parser.y"
                          { (yyval.node) = (yyvsp[0].node); }
#line 1469 "src/parser.tab.c"
    break;

  case 29: /* expression_stmt: expression SEMICOLON  */
#line 227 "grammar/parser.y"
                           {
        (yyval.node) = create_expr_stmt_node((yyvsp[-1].node), yyline);
    }
#line 1477 "src/parser.tab.c"
    break;

  case 30: /* expression_stmt: SEMICOLON  */
#line 230 "grammar/parser.y"
                {
        (yyval.node) = create_expr_stmt_node(NULL, yyline);
    }
#line 1485 "src/parser.tab.c"
    break;

  case 31: /* selection_stmt: IF LPAREN expression RPAREN statement  */
#line 237 "grammar/parser.y"
                                                                  {
        (yyval.node) = create_if_node((yyvsp[-2].node), (yyvsp[0].node), NULL, yyline);
    }
#line 1493 "src/parser.tab.c"
    break;

  case 32: /* selection_stmt: IF LPAREN expression RPAREN statement ELSE statement  */
#line 240 "grammar/parser.y"
                                                           {
        (yyval.node) = create_if_node((yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[0].node), yyline);
    }
#line 1501 "src/parser.tab.c"
    break;

  case 33: /* iteration_stmt: WHILE LPAREN expression RPAREN statement  */
#line 247 "grammar/parser.y"
                                               {
        (yyval.node) = create_while_node((yyvsp[-2].node), (yyvsp[0].node), yyline);
    }
#line 1509 "src/parser.tab.c"
    break;

  case 34: /* iteration_stmt: FOR LPAREN expression_stmt expression_stmt expression RPAREN statement  */
#line 250 "grammar/parser.y"
                                                                             {
        ASTNode *init = (yyvsp[-4].node)->data.expr_stmt.expr;
        ASTNode *cond = (yyvsp[-3].node)->data.expr_stmt.expr;
        (yyval.node) = create_for_node(init, cond, (yyvsp[-2].node), (yyvsp[0].node), yyline);
    }
#line 1519 "src/parser.tab.c"
    break;

  case 35: /* return_stmt: RETURN expression SEMICOLON  */
#line 259 "grammar/parser.y"
                                  {
        (yyval.node) = create_return_node((yyvsp[-1].node), yyline);
    }
#line 1527 "src/parser.tab.c"
    break;

  case 36: /* return_stmt: RETURN SEMICOLON  */
#line 262 "grammar/parser.y"
                       {
        (yyval.node) = create_return_node(NULL, yyline);
    }
#line 1535 "src/parser.tab.c"
    break;

  case 37: /* printf_stmt: PRINTF LPAREN STRING_LITERAL RPAREN SEMICOLON  */
#line 269 "grammar/parser.y"
                                                    {
        (yyval.node) = create_printf_node((yyvsp[-2].str), NULL, 0, yyline);
        free((yyvsp[-2].str));
    }
#line 1544 "src/parser.tab.c"
    break;

  case 38: /* printf_stmt: PRINTF LPAREN STRING_LITERAL COMMA arg_list RPAREN SEMICOLON  */
#line 273 "grammar/parser.y"
                                                                   {
        (yyval.node) = create_printf_node((yyvsp[-4].str), (yyvsp[-2].node_list).nodes, (yyvsp[-2].node_list).count, yyline);
        free((yyvsp[-4].str));
    }
#line 1553 "src/parser.tab.c"
    break;

  case 39: /* expression: assignment_expr  */
#line 284 "grammar/parser.y"
                          { (yyval.node) = (yyvsp[0].node); }
#line 1559 "src/parser.tab.c"
    break;

  case 40: /* expression: or_expr  */
#line 285 "grammar/parser.y"
                          { (yyval.node) = (yyvsp[0].node); }
#line 1565 "src/parser.tab.c"
    break;

  case 41: /* assignment_expr: IDENTIFIER ASSIGN expression  */
#line 290 "grammar/parser.y"
                                   {
        (yyval.node) = create_assign_node((yyvsp[-2].str), (yyvsp[0].node), yyline);
        free((yyvsp[-2].str));
    }
#line 1574 "src/parser.tab.c"
    break;

  case 42: /* or_expr: and_expr  */
#line 298 "grammar/parser.y"
                          { (yyval.node) = (yyvsp[0].node); }
#line 1580 "src/parser.tab.c"
    break;

  case 43: /* or_expr: or_expr OR and_expr  */
#line 299 "grammar/parser.y"
                          {
        (yyval.node) = create_binary_node("||", (yyvsp[-2].node), (yyvsp[0].node), yyline);
    }
#line 1588 "src/parser.tab.c"
    break;

  case 44: /* and_expr: equality_expr  */
#line 306 "grammar/parser.y"
                          { (yyval.node) = (yyvsp[0].node); }
#line 1594 "src/parser.tab.c"
    break;

  case 45: /* and_expr: and_expr AND equality_expr  */
#line 307 "grammar/parser.y"
                                 {
        (yyval.node) = create_binary_node("&&", (yyvsp[-2].node), (yyvsp[0].node), yyline);
    }
#line 1602 "src/parser.tab.c"
    break;

  case 46: /* equality_expr: relational_expr  */
#line 314 "grammar/parser.y"
                          { (yyval.node) = (yyvsp[0].node); }
#line 1608 "src/parser.tab.c"
    break;

  case 47: /* equality_expr: equality_expr EQ relational_expr  */
#line 315 "grammar/parser.y"
                                       {
        (yyval.node) = create_binary_node("==", (yyvsp[-2].node), (yyvsp[0].node), yyline);
    }
#line 1616 "src/parser.tab.c"
    break;

  case 48: /* equality_expr: equality_expr NEQ relational_expr  */
#line 318 "grammar/parser.y"
                                        {
        (yyval.node) = create_binary_node("!=", (yyvsp[-2].node), (yyvsp[0].node), yyline);
    }
#line 1624 "src/parser.tab.c"
    break;

  case 49: /* relational_expr: additive_expr  */
#line 325 "grammar/parser.y"
                          { (yyval.node) = (yyvsp[0].node); }
#line 1630 "src/parser.tab.c"
    break;

  case 50: /* relational_expr: relational_expr LT additive_expr  */
#line 326 "grammar/parser.y"
                                       {
        (yyval.node) = create_binary_node("<", (yyvsp[-2].node), (yyvsp[0].node), yyline);
    }
#line 1638 "src/parser.tab.c"
    break;

  case 51: /* relational_expr: relational_expr GT additive_expr  */
#line 329 "grammar/parser.y"
                                       {
        (yyval.node) = create_binary_node(">", (yyvsp[-2].node), (yyvsp[0].node), yyline);
    }
#line 1646 "src/parser.tab.c"
    break;

  case 52: /* relational_expr: relational_expr LTE additive_expr  */
#line 332 "grammar/parser.y"
                                        {
        (yyval.node) = create_binary_node("<=", (yyvsp[-2].node), (yyvsp[0].node), yyline);
    }
#line 1654 "src/parser.tab.c"
    break;

  case 53: /* relational_expr: relational_expr GTE additive_expr  */
#line 335 "grammar/parser.y"
                                        {
        (yyval.node) = create_binary_node(">=", (yyvsp[-2].node), (yyvsp[0].node), yyline);
    }
#line 1662 "src/parser.tab.c"
    break;

  case 54: /* additive_expr: multiplicative_expr  */
#line 342 "grammar/parser.y"
                          { (yyval.node) = (yyvsp[0].node); }
#line 1668 "src/parser.tab.c"
    break;

  case 55: /* additive_expr: additive_expr PLUS multiplicative_expr  */
#line 343 "grammar/parser.y"
                                             {
        (yyval.node) = create_binary_node("+", (yyvsp[-2].node), (yyvsp[0].node), yyline);
    }
#line 1676 "src/parser.tab.c"
    break;

  case 56: /* additive_expr: additive_expr MINUS multiplicative_expr  */
#line 346 "grammar/parser.y"
                                              {
        (yyval.node) = create_binary_node("-", (yyvsp[-2].node), (yyvsp[0].node), yyline);
    }
#line 1684 "src/parser.tab.c"
    break;

  case 57: /* multiplicative_expr: unary_expr  */
#line 353 "grammar/parser.y"
                          { (yyval.node) = (yyvsp[0].node); }
#line 1690 "src/parser.tab.c"
    break;

  case 58: /* multiplicative_expr: multiplicative_expr MULTIPLY unary_expr  */
#line 354 "grammar/parser.y"
                                              {
        (yyval.node) = create_binary_node("*", (yyvsp[-2].node), (yyvsp[0].node), yyline);
    }
#line 1698 "src/parser.tab.c"
    break;

  case 59: /* multiplicative_expr: multiplicative_expr DIVIDE unary_expr  */
#line 357 "grammar/parser.y"
                                            {
        (yyval.node) = create_binary_node("/", (yyvsp[-2].node), (yyvsp[0].node), yyline);
    }
#line 1706 "src/parser.tab.c"
    break;

  case 60: /* multiplicative_expr: multiplicative_expr MODULO unary_expr  */
#line 360 "grammar/parser.y"
                                            {
        (yyval.node) = create_binary_node("%", (yyvsp[-2].node), (yyvsp[0].node), yyline);
    }
#line 1714 "src/parser.tab.c"
    break;

  case 61: /* unary_expr: primary_expr  */
#line 367 "grammar/parser.y"
                          { (yyval.node) = (yyvsp[0].node); }
#line 1720 "src/parser.tab.c"
    break;

  case 62: /* unary_expr: MINUS unary_expr  */
#line 368 "grammar/parser.y"
                                    {
        (yyval.node) = create_unary_node("-", (yyvsp[0].node), yyline);
    }
#line 1728 "src/parser.tab.c"
    break;

  case 63: /* unary_expr: NOT unary_expr  */
#line 371 "grammar/parser.y"
                     {
        (yyval.node) = create_unary_node("!", (yyvsp[0].node), yyline);
    }
#line 1736 "src/parser.tab.c"
    break;

  case 64: /* primary_expr: INTEGER_LITERAL  */
#line 378 "grammar/parser.y"
                      {
        (yyval.node) = create_int_literal_node((yyvsp[0].ival), yyline);
    }
#line 1744 "src/parser.tab.c"
    break;

  case 65: /* primary_expr: FLOAT_LITERAL  */
#line 381 "grammar/parser.y"
                    {
        (yyval.node) = create_float_literal_node((yyvsp[0].fval), yyline);
    }
#line 1752 "src/parser.tab.c"
    break;

  case 66: /* primary_expr: CHAR_LITERAL  */
#line 384 "grammar/parser.y"
                   {
        (yyval.node) = create_char_literal_node((yyvsp[0].cval), yyline);
    }
#line 1760 "src/parser.tab.c"
    break;

  case 67: /* primary_expr: STRING_LITERAL  */
#line 387 "grammar/parser.y"
                     {
        (yyval.node) = create_string_literal_node((yyvsp[0].str), yyline);
        free((yyvsp[0].str));
    }
#line 1769 "src/parser.tab.c"
    break;

  case 68: /* primary_expr: IDENTIFIER  */
#line 391 "grammar/parser.y"
                 {
        (yyval.node) = create_identifier_node((yyvsp[0].str), yyline);
        free((yyvsp[0].str));
    }
#line 1778 "src/parser.tab.c"
    break;

  case 69: /* primary_expr: IDENTIFIER LPAREN arg_list RPAREN  */
#line 395 "grammar/parser.y"
                                        {
        (yyval.node) = create_func_call_node((yyvsp[-3].str), (yyvsp[-1].node_list).nodes, (yyvsp[-1].node_list).count, yyline);
        free((yyvsp[-3].str));
    }
#line 1787 "src/parser.tab.c"
    break;

  case 70: /* primary_expr: IDENTIFIER LPAREN RPAREN  */
#line 399 "grammar/parser.y"
                               {
        (yyval.node) = create_func_call_node((yyvsp[-2].str), NULL, 0, yyline);
        free((yyvsp[-2].str));
    }
#line 1796 "src/parser.tab.c"
    break;

  case 71: /* primary_expr: LPAREN expression RPAREN  */
#line 403 "grammar/parser.y"
                               {
        (yyval.node) = (yyvsp[-1].node);
    }
#line 1804 "src/parser.tab.c"
    break;

  case 72: /* arg_list: expression  */
#line 410 "grammar/parser.y"
                 {
        (yyval.node_list).nodes = alloc_node_array(MAX_ARGS);
        (yyval.node_list).count = 0;
        (yyval.node_list).nodes[(yyval.node_list).count++] = (yyvsp[0].node);
    }
#line 1814 "src/parser.tab.c"
    break;

  case 73: /* arg_list: arg_list COMMA expression  */
#line 415 "grammar/parser.y"
                                {
        (yyval.node_list) = (yyvsp[-2].node_list);
        (yyval.node_list).nodes[(yyval.node_list).count++] = (yyvsp[0].node);
    }
#line 1823 "src/parser.tab.c"
    break;


#line 1827 "src/parser.tab.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

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
                      yytoken, &yylval);
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

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

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 421 "grammar/parser.y"

