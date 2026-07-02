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
#line 4 "misc/grammar.y"

#include "helper.hpp"

int yylex(void);
void yyerror(const char *message);

#line 78 "misc/grammar.tab.c"

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

#include "grammar.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_LITERAL = 3,                    /* LITERAL  */
  YYSYMBOL_SYMBOL = 4,                     /* SYMBOL  */
  YYSYMBOL_STRING = 5,                     /* STRING  */
  YYSYMBOL_GPRX = 6,                       /* GPRX  */
  YYSYMBOL_CSRX = 7,                       /* CSRX  */
  YYSYMBOL_GLOBAL = 8,                     /* GLOBAL  */
  YYSYMBOL_EXTERN = 9,                     /* EXTERN  */
  YYSYMBOL_SECTION = 10,                   /* SECTION  */
  YYSYMBOL_WORD = 11,                      /* WORD  */
  YYSYMBOL_SKIP = 12,                      /* SKIP  */
  YYSYMBOL_ASCII = 13,                     /* ASCII  */
  YYSYMBOL_EQU = 14,                       /* EQU  */
  YYSYMBOL_END = 15,                       /* END  */
  YYSYMBOL_HALT = 16,                      /* HALT  */
  YYSYMBOL_INT = 17,                       /* INT  */
  YYSYMBOL_IRET = 18,                      /* IRET  */
  YYSYMBOL_CALL = 19,                      /* CALL  */
  YYSYMBOL_RET = 20,                       /* RET  */
  YYSYMBOL_JMP = 21,                       /* JMP  */
  YYSYMBOL_BEQ = 22,                       /* BEQ  */
  YYSYMBOL_BNE = 23,                       /* BNE  */
  YYSYMBOL_BGT = 24,                       /* BGT  */
  YYSYMBOL_PUSH = 25,                      /* PUSH  */
  YYSYMBOL_POP = 26,                       /* POP  */
  YYSYMBOL_XCHG = 27,                      /* XCHG  */
  YYSYMBOL_ADD = 28,                       /* ADD  */
  YYSYMBOL_SUB = 29,                       /* SUB  */
  YYSYMBOL_MUL = 30,                       /* MUL  */
  YYSYMBOL_DIV = 31,                       /* DIV  */
  YYSYMBOL_NOT = 32,                       /* NOT  */
  YYSYMBOL_AND = 33,                       /* AND  */
  YYSYMBOL_OR = 34,                        /* OR  */
  YYSYMBOL_XOR = 35,                       /* XOR  */
  YYSYMBOL_SHL = 36,                       /* SHL  */
  YYSYMBOL_SHR = 37,                       /* SHR  */
  YYSYMBOL_LD = 38,                        /* LD  */
  YYSYMBOL_ST = 39,                        /* ST  */
  YYSYMBOL_CSRRD = 40,                     /* CSRRD  */
  YYSYMBOL_CSRWR = 41,                     /* CSRWR  */
  YYSYMBOL_WHITESPACE = 42,                /* WHITESPACE  */
  YYSYMBOL_NEWLINE = 43,                   /* NEWLINE  */
  YYSYMBOL_DOT = 44,                       /* DOT  */
  YYSYMBOL_COMMA = 45,                     /* COMMA  */
  YYSYMBOL_PERCENT = 46,                   /* PERCENT  */
  YYSYMBOL_RBRACK = 47,                    /* RBRACK  */
  YYSYMBOL_LBRACK = 48,                    /* LBRACK  */
  YYSYMBOL_DOLLAR = 49,                    /* DOLLAR  */
  YYSYMBOL_COLON = 50,                     /* COLON  */
  YYSYMBOL_PLUS = 51,                      /* PLUS  */
  YYSYMBOL_MINUS = 52,                     /* MINUS  */
  YYSYMBOL_MULT = 53,                      /* MULT  */
  YYSYMBOL_DIVISION = 54,                  /* DIVISION  */
  YYSYMBOL_COMMENT = 55,                   /* COMMENT  */
  YYSYMBOL_UMINUS = 56,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 57,                  /* $accept  */
  YYSYMBOL_input = 58,                     /* input  */
  YYSYMBOL_line_with_comment = 59,         /* line_with_comment  */
  YYSYMBOL_line = 60,                      /* line  */
  YYSYMBOL_label = 61,                     /* label  */
  YYSYMBOL_asm_directives = 62,            /* asm_directives  */
  YYSYMBOL_symbol_list = 63,               /* symbol_list  */
  YYSYMBOL_sym_lit_list = 64,              /* sym_lit_list  */
  YYSYMBOL_expr = 65,                      /* expr  */
  YYSYMBOL_symbol_name = 66,               /* symbol_name  */
  YYSYMBOL_asm_instructions = 67,          /* asm_instructions  */
  YYSYMBOL_gpr_12 = 68,                    /* gpr_12  */
  YYSYMBOL_gpr_1 = 69,                     /* gpr_1  */
  YYSYMBOL_csr_1 = 70,                     /* csr_1  */
  YYSYMBOL_branchOp = 71,                  /* branchOp  */
  YYSYMBOL_arithmeticOp = 72,              /* arithmeticOp  */
  YYSYMBOL_operandJmp = 73,                /* operandJmp  */
  YYSYMBOL_operandLS = 74                  /* operandLS  */
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

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

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
# define YYCOPY_NEEDED 1
#endif /* 1 */

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
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   643

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  57
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  18
/* YYNRULES -- Number of rules.  */
#define YYNRULES  111
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  190

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   311


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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    69,    69,    70,    74,    78,    79,    80,    81,    82,
      83,    87,    93,    96,    99,   102,   105,   108,   111,   114,
     120,   123,   130,   133,   136,   140,   147,   151,   155,   159,
     163,   167,   171,   175,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   222,   225,   228,   231,   234,   237,   240,   243,   246,
     249,   252,   255,   258,   261,   264,   270,   277,   283,   289,
     290,   291,   295,   296,   297,   298,   299,   300,   301,   302,
     303,   304,   308,   311,   317,   320,   323,   326,   329,   332,
     335,   338
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  static const char *const yy_sname[] =
  {
  "end of file", "error", "invalid token", "LITERAL", "SYMBOL", "STRING",
  "GPRX", "CSRX", "GLOBAL", "EXTERN", "SECTION", "WORD", "SKIP", "ASCII",
  "EQU", "END", "HALT", "INT", "IRET", "CALL", "RET", "JMP", "BEQ", "BNE",
  "BGT", "PUSH", "POP", "XCHG", "ADD", "SUB", "MUL", "DIV", "NOT", "AND",
  "OR", "XOR", "SHL", "SHR", "LD", "ST", "CSRRD", "CSRWR", "WHITESPACE",
  "NEWLINE", "DOT", "COMMA", "PERCENT", "RBRACK", "LBRACK", "DOLLAR",
  "COLON", "PLUS", "MINUS", "MULT", "DIVISION", "COMMENT", "UMINUS",
  "$accept", "input", "line_with_comment", "line", "label",
  "asm_directives", "symbol_list", "sym_lit_list", "expr", "symbol_name",
  "asm_instructions", "gpr_12", "gpr_1", "csr_1", "branchOp",
  "arithmeticOp", "operandJmp", "operandLS", YY_NULLPTR
  };
  return yy_sname[yysymbol];
}
#endif

#define YYPACT_NINF (-48)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-102)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -48,   223,   -48,   -48,   -48,   -48,   -48,   -48,   -48,   -48,
     -48,   -48,   -48,   -48,   -36,   -34,   -33,   262,   -32,   262,
     -30,   -20,   -19,   -17,   -17,   -16,   -15,   -14,    26,    27,
     -17,    28,    29,    31,    32,    33,   179,   -17,    34,   -17,
      10,   -48,   -31,   599,   -37,   -48,    35,    35,   -48,   -48,
     -48,   -48,   -48,   -48,   -48,   -48,   -48,   -48,   -48,   -48,
     -48,   -48,   -48,   -48,   -48,   -48,   -48,   -48,   -48,   -48,
     -48,   -48,   -48,   -48,   -48,   -48,   -48,   -48,     8,   -48,
     -48,   -48,   -48,    22,    37,   301,   -48,    40,    41,    69,
      42,    43,   573,   573,   573,   340,    92,    93,   573,   -48,
     -48,   -48,   -48,   -48,   -48,   262,   -48,   262,   -48,   -48,
     -48,   -17,   -17,   -48,   -48,   -48,   -48,   -48,   -17,   -48,
     -48,   -48,   -48,   -48,   179,   -17,    34,   -17,    10,   -48,
     -48,    94,    54,   -48,   -48,   -48,    95,   -48,   -48,   -17,
     179,   -48,   -17,    34,    57,   -48,    57,   -48,   -48,    59,
     -48,   -48,   -48,    61,   -48,    62,   262,   -45,   -48,   -48,
     -48,   -48,   573,   379,    30,    63,   -48,   -48,   418,   -48,
     -48,   -48,   -48,   457,   -47,   -48,   102,    64,    65,   -48,
     -48,   496,   535,   -48,   -48,   -48,   -48,   -48,   -48,   -48
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     5,     1,    34,    36,    35,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
       0,     3,     0,     8,     0,     7,     0,     0,   102,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,   103,    75,    76,     0,    78,
      79,    80,   106,     0,     0,     0,   107,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    19,
       6,     4,    71,    72,    73,     0,    74,     0,    90,    89,
      91,     0,     0,    92,    93,    94,    95,    96,     0,    97,
      98,    99,   100,   101,     0,     0,     0,     0,     0,    10,
      11,     0,     0,    81,    87,   108,     0,   104,   105,     0,
       0,    88,     0,     0,    12,    20,    13,    14,    22,    15,
      23,    16,    17,     0,     9,     0,     0,     0,    82,    83,
      84,    85,     0,     0,     0,     0,    77,   109,     0,    21,
      25,    24,    26,     0,    18,    27,     0,     0,     0,    32,
      33,     0,     0,    86,   110,   111,    28,    29,    30,    31
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -48,   -48,   -48,   -48,   -48,   -13,    17,   -48,   -48,    -1,
      68,    67,   -22,   -27,   -48,   -48,   -18,   -21
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,    41,    42,    43,   100,   144,   149,   174,    75,
      45,   132,    79,    90,    46,    47,    76,    87
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      44,    77,    80,   167,   181,   182,   168,   -71,    81,   -72,
     -73,   -74,   101,   130,   134,    88,   -90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   -89,   -91,   135,    78,
     -92,   -93,   -94,   172,     3,    86,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,   -95,   -96,   -97,   -98,   141,   -99,  -100,  -101,
      89,   131,   173,   136,   138,   139,   140,   142,   143,    77,
      80,   145,   145,   147,   150,   151,    81,   153,   152,   156,
     155,   157,   162,    88,   163,    91,   164,   165,   183,   176,
     146,   129,   184,   185,   133,   154,   161,   158,     0,   159,
     160,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   166,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   169,   171,   175,     0,     0,     0,   178,     0,     0,
       0,     0,   180,     0,     0,     0,     0,     0,     0,     0,
     187,   189,    82,     3,     0,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,     0,     0,     2,     0,    83,    84,     3,    85,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    48,     3,    40,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,   137,     3,     0,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,   148,     3,     0,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,   170,     3,     0,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,   177,     3,     0,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
     179,     3,     0,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,   186,
       3,     0,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,   188,     3,
       0,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,     3,     0,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,     0,     0,   128
};

static const yytype_int16 yycheck[] =
{
       1,    19,    24,    48,    51,    52,    51,    43,    30,    43,
      43,    43,    43,    50,     6,    37,    46,    39,     8,     9,
      10,    11,    12,    13,    14,    15,    46,    46,     6,    46,
      46,    46,    46,     3,     4,    36,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    46,    46,    46,    46,     7,    46,    46,    46,
      46,    46,    52,    46,    85,    45,    45,    45,    45,   107,
     112,    92,    93,    94,    95,     3,   118,    98,     5,    45,
       6,     6,    45,   125,    45,   127,    45,    45,     6,    46,
      93,    43,    48,    48,    47,   128,   143,   139,    -1,   140,
     142,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,   140,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,    -1,
      -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     181,   182,     3,     4,    -1,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    -1,    -1,     0,    -1,    46,    47,     4,    49,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,     3,     4,    44,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,     3,     4,    -1,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,     3,     4,    -1,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,     3,     4,    -1,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,     3,     4,    -1,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
       3,     4,    -1,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,     3,
       4,    -1,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,     3,     4,
      -1,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,     4,    -1,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    -1,    -1,    44
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    58,     0,     4,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      44,    59,    60,    61,    66,    67,    71,    72,     3,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    66,    73,    73,    46,    69,
      69,    69,     3,    46,    47,    49,    66,    74,    69,    46,
      70,    69,     8,     9,    10,    11,    12,    13,    14,    15,
      62,    43,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    44,    67,
      50,    46,    68,    68,     6,     6,    46,     3,    66,    45,
      45,     7,    45,    45,    63,    66,    63,    66,     3,    64,
      66,     3,     5,    66,    62,     6,    45,     6,    69,    74,
      69,    70,    45,    45,    45,    45,    73,    48,    51,    66,
       3,    66,     3,    52,    65,    66,    46,     3,    66,     3,
      66,    51,    52,     6,    48,    48,     3,    66,     3,    66
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    57,    58,    58,    59,    60,    60,    60,    60,    60,
      60,    61,    62,    62,    62,    62,    62,    62,    62,    62,
      63,    63,    64,    64,    64,    64,    65,    65,    65,    65,
      65,    65,    65,    65,    66,    66,    66,    66,    66,    66,
      66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
      66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
      66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
      66,    67,    67,    67,    67,    67,    67,    67,    67,    67,
      67,    67,    67,    67,    67,    67,    68,    69,    70,    71,
      71,    71,    72,    72,    72,    72,    72,    72,    72,    72,
      72,    72,    73,    73,    74,    74,    74,    74,    74,    74,
      74,    74
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     0,     2,     1,     1,     3,
       2,     2,     2,     2,     2,     2,     2,     2,     4,     1,
       1,     3,     1,     1,     3,     3,     1,     1,     3,     3,
       3,     3,     2,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     2,     4,     2,     2,
       2,     2,     4,     4,     4,     4,     5,     2,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     1,     1,     2,     4,
       6,     6
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
        YY_LAC_DISCARD ("YYBACKUP");                              \
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


/* Given a state stack such that *YYBOTTOM is its bottom, such that
   *YYTOP is either its top or is YYTOP_EMPTY to indicate an empty
   stack, and such that *YYCAPACITY is the maximum number of elements it
   can hold without a reallocation, make sure there is enough room to
   store YYADD more elements.  If not, allocate a new stack using
   YYSTACK_ALLOC, copy the existing elements, and adjust *YYBOTTOM,
   *YYTOP, and *YYCAPACITY to reflect the new capacity and memory
   location.  If *YYBOTTOM != YYBOTTOM_NO_FREE, then free the old stack
   using YYSTACK_FREE.  Return 0 if successful or if no reallocation is
   required.  Return YYENOMEM if memory is exhausted.  */
static int
yy_lac_stack_realloc (YYPTRDIFF_T *yycapacity, YYPTRDIFF_T yyadd,
#if YYDEBUG
                      char const *yydebug_prefix,
                      char const *yydebug_suffix,
#endif
                      yy_state_t **yybottom,
                      yy_state_t *yybottom_no_free,
                      yy_state_t **yytop, yy_state_t *yytop_empty)
{
  YYPTRDIFF_T yysize_old =
    *yytop == yytop_empty ? 0 : *yytop - *yybottom + 1;
  YYPTRDIFF_T yysize_new = yysize_old + yyadd;
  if (*yycapacity < yysize_new)
    {
      YYPTRDIFF_T yyalloc = 2 * yysize_new;
      yy_state_t *yybottom_new;
      /* Use YYMAXDEPTH for maximum stack size given that the stack
         should never need to grow larger than the main state stack
         needs to grow without LAC.  */
      if (YYMAXDEPTH < yysize_new)
        {
          YYDPRINTF ((stderr, "%smax size exceeded%s", yydebug_prefix,
                      yydebug_suffix));
          return YYENOMEM;
        }
      if (YYMAXDEPTH < yyalloc)
        yyalloc = YYMAXDEPTH;
      yybottom_new =
        YY_CAST (yy_state_t *,
                 YYSTACK_ALLOC (YY_CAST (YYSIZE_T,
                                         yyalloc * YYSIZEOF (*yybottom_new))));
      if (!yybottom_new)
        {
          YYDPRINTF ((stderr, "%srealloc failed%s", yydebug_prefix,
                      yydebug_suffix));
          return YYENOMEM;
        }
      if (*yytop != yytop_empty)
        {
          YYCOPY (yybottom_new, *yybottom, yysize_old);
          *yytop = yybottom_new + (yysize_old - 1);
        }
      if (*yybottom != yybottom_no_free)
        YYSTACK_FREE (*yybottom);
      *yybottom = yybottom_new;
      *yycapacity = yyalloc;
    }
  return 0;
}

/* Establish the initial context for the current lookahead if no initial
   context is currently established.

   We define a context as a snapshot of the parser stacks.  We define
   the initial context for a lookahead as the context in which the
   parser initially examines that lookahead in order to select a
   syntactic action.  Thus, if the lookahead eventually proves
   syntactically unacceptable (possibly in a later context reached via a
   series of reductions), the initial context can be used to determine
   the exact set of tokens that would be syntactically acceptable in the
   lookahead's place.  Moreover, it is the context after which any
   further semantic actions would be erroneous because they would be
   determined by a syntactically unacceptable token.

   YY_LAC_ESTABLISH should be invoked when a reduction is about to be
   performed in an inconsistent state (which, for the purposes of LAC,
   includes consistent states that don't know they're consistent because
   their default reductions have been disabled).  Iff there is a
   lookahead token, it should also be invoked before reporting a syntax
   error.  This latter case is for the sake of the debugging output.

   For parse.lac=full, the implementation of YY_LAC_ESTABLISH is as
   follows.  If no initial context is currently established for the
   current lookahead, then check if that lookahead can eventually be
   shifted if syntactic actions continue from the current context.
   Report a syntax error if it cannot.  */
#define YY_LAC_ESTABLISH                                                \
do {                                                                    \
  if (!yy_lac_established)                                              \
    {                                                                   \
      YYDPRINTF ((stderr,                                               \
                  "LAC: initial context established for %s\n",          \
                  yysymbol_name (yytoken)));                            \
      yy_lac_established = 1;                                           \
      switch (yy_lac (yyesa, &yyes, &yyes_capacity, yyssp, yytoken))    \
        {                                                               \
        case YYENOMEM:                                                  \
          YYNOMEM;                                                      \
        case 1:                                                         \
          goto yyerrlab;                                                \
        }                                                               \
    }                                                                   \
} while (0)

/* Discard any previous initial lookahead context because of Event,
   which may be a lookahead change or an invalidation of the currently
   established initial context for the current lookahead.

   The most common example of a lookahead change is a shift.  An example
   of both cases is syntax error recovery.  That is, a syntax error
   occurs when the lookahead is syntactically erroneous for the
   currently established initial context, so error recovery manipulates
   the parser stacks to try to find a new initial context in which the
   current lookahead is syntactically acceptable.  If it fails to find
   such a context, it discards the lookahead.  */
#if YYDEBUG
# define YY_LAC_DISCARD(Event)                                           \
do {                                                                     \
  if (yy_lac_established)                                                \
    {                                                                    \
      YYDPRINTF ((stderr, "LAC: initial context discarded due to "       \
                  Event "\n"));                                          \
      yy_lac_established = 0;                                            \
    }                                                                    \
} while (0)
#else
# define YY_LAC_DISCARD(Event) yy_lac_established = 0
#endif

/* Given the stack whose top is *YYSSP, return 0 iff YYTOKEN can
   eventually (after perhaps some reductions) be shifted, return 1 if
   not, or return YYENOMEM if memory is exhausted.  As preconditions and
   postconditions: *YYES_CAPACITY is the allocated size of the array to
   which *YYES points, and either *YYES = YYESA or *YYES points to an
   array allocated with YYSTACK_ALLOC.  yy_lac may overwrite the
   contents of either array, alter *YYES and *YYES_CAPACITY, and free
   any old *YYES other than YYESA.  */
static int
yy_lac (yy_state_t *yyesa, yy_state_t **yyes,
        YYPTRDIFF_T *yyes_capacity, yy_state_t *yyssp, yysymbol_kind_t yytoken)
{
  yy_state_t *yyes_prev = yyssp;
  yy_state_t *yyesp = yyes_prev;
  /* Reduce until we encounter a shift and thereby accept the token.  */
  YYDPRINTF ((stderr, "LAC: checking lookahead %s:", yysymbol_name (yytoken)));
  if (yytoken == YYSYMBOL_YYUNDEF)
    {
      YYDPRINTF ((stderr, " Always Err\n"));
      return 1;
    }
  while (1)
    {
      int yyrule = yypact[+*yyesp];
      if (yypact_value_is_default (yyrule)
          || (yyrule += yytoken) < 0 || YYLAST < yyrule
          || yycheck[yyrule] != yytoken)
        {
          /* Use the default action.  */
          yyrule = yydefact[+*yyesp];
          if (yyrule == 0)
            {
              YYDPRINTF ((stderr, " Err\n"));
              return 1;
            }
        }
      else
        {
          /* Use the action from yytable.  */
          yyrule = yytable[yyrule];
          if (yytable_value_is_error (yyrule))
            {
              YYDPRINTF ((stderr, " Err\n"));
              return 1;
            }
          if (0 < yyrule)
            {
              YYDPRINTF ((stderr, " S%d\n", yyrule));
              return 0;
            }
          yyrule = -yyrule;
        }
      /* By now we know we have to simulate a reduce.  */
      YYDPRINTF ((stderr, " R%d", yyrule - 1));
      {
        /* Pop the corresponding number of values from the stack.  */
        YYPTRDIFF_T yylen = yyr2[yyrule];
        /* First pop from the LAC stack as many tokens as possible.  */
        if (yyesp != yyes_prev)
          {
            YYPTRDIFF_T yysize = yyesp - *yyes + 1;
            if (yylen < yysize)
              {
                yyesp -= yylen;
                yylen = 0;
              }
            else
              {
                yyesp = yyes_prev;
                yylen -= yysize;
              }
          }
        /* Only afterwards look at the main stack.  */
        if (yylen)
          yyesp = yyes_prev -= yylen;
      }
      /* Push the resulting state of the reduction.  */
      {
        yy_state_fast_t yystate;
        {
          const int yylhs = yyr1[yyrule] - YYNTOKENS;
          const int yyi = yypgoto[yylhs] + *yyesp;
          yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyesp
                     ? yytable[yyi]
                     : yydefgoto[yylhs]);
        }
        if (yyesp == yyes_prev)
          {
            yyesp = *yyes;
            YY_IGNORE_USELESS_CAST_BEGIN
            *yyesp = YY_CAST (yy_state_t, yystate);
            YY_IGNORE_USELESS_CAST_END
          }
        else
          {
            if (yy_lac_stack_realloc (yyes_capacity, 1,
#if YYDEBUG
                                      " (", ")",
#endif
                                      yyes, yyesa, &yyesp, yyes_prev))
              {
                YYDPRINTF ((stderr, "\n"));
                return YYENOMEM;
              }
            YY_IGNORE_USELESS_CAST_BEGIN
            *++yyesp = YY_CAST (yy_state_t, yystate);
            YY_IGNORE_USELESS_CAST_END
          }
        YYDPRINTF ((stderr, " G%d", yystate));
      }
    }
}

/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yy_state_t *yyesa;
  yy_state_t **yyes;
  YYPTRDIFF_T *yyes_capacity;
  yysymbol_kind_t yytoken;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;

  int yyx;
  for (yyx = 0; yyx < YYNTOKENS; ++yyx)
    {
      yysymbol_kind_t yysym = YY_CAST (yysymbol_kind_t, yyx);
      if (yysym != YYSYMBOL_YYerror && yysym != YYSYMBOL_YYUNDEF)
        switch (yy_lac (yyctx->yyesa, yyctx->yyes, yyctx->yyes_capacity, yyctx->yyssp, yysym))
          {
          case YYENOMEM:
            return YYENOMEM;
          case 1:
            continue;
          default:
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = yysym;
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
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
# endif
#endif



static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
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
       In the first two cases, it might appear that the current syntax
       error should have been detected in the previous state when yy_lac
       was invoked.  However, at that time, there might have been a
       different syntax error that discarded a different initial context
       during error recovery, leaving behind the current lookahead.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      YYDPRINTF ((stderr, "Constructing syntax error message\n"));
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else if (yyn == 0)
        YYDPRINTF ((stderr, "No expected tokens.\n"));
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.  In order to see if a particular token T is a
   valid looakhead, invoke yy_lac (YYESA, YYES, YYES_CAPACITY, YYSSP, T).

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store or if
   yy_lac returned YYENOMEM.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yystrlen (yysymbol_name (yyarg[yyi]));
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
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
          yyp = yystpcpy (yyp, yysymbol_name (yyarg[yyi++]));
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


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

    yy_state_t yyesa[20];
    yy_state_t *yyes = yyesa;
    YYPTRDIFF_T yyes_capacity = 20 < YYMAXDEPTH ? 20 : YYMAXDEPTH;

  /* Whether LAC context is established.  A Boolean.  */
  int yy_lac_established = 0;
  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

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
    {
      YY_LAC_ESTABLISH;
      goto yydefault;
    }
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      YY_LAC_ESTABLISH;
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
  YY_LAC_DISCARD ("shift");
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
  {
    int yychar_backup = yychar;
    switch (yyn)
      {
  case 11: /* label: symbol_name COLON  */
#line 87 "misc/grammar.y"
                          {
            parser_add_label((yyvsp[-1].symbol));
        }
#line 1814 "misc/grammar.tab.c"
    break;

  case 12: /* asm_directives: GLOBAL symbol_list  */
#line 93 "misc/grammar.y"
                           {
            parser_add_symbol_list_directive(ASM_DIR_GLOBAL, (yyvsp[0].symbol_list));
        }
#line 1822 "misc/grammar.tab.c"
    break;

  case 13: /* asm_directives: EXTERN symbol_list  */
#line 96 "misc/grammar.y"
                           {
            parser_add_symbol_list_directive(ASM_DIR_EXTERN, (yyvsp[0].symbol_list));
        }
#line 1830 "misc/grammar.tab.c"
    break;

  case 14: /* asm_directives: SECTION symbol_name  */
#line 99 "misc/grammar.y"
                            {
            parser_add_section_directive((yyvsp[0].symbol));
        }
#line 1838 "misc/grammar.tab.c"
    break;

  case 15: /* asm_directives: WORD sym_lit_list  */
#line 102 "misc/grammar.y"
                          {
            parser_add_word_directive((yyvsp[0].sym_lit_list));
        }
#line 1846 "misc/grammar.tab.c"
    break;

  case 16: /* asm_directives: SKIP LITERAL  */
#line 105 "misc/grammar.y"
                     {
            parser_add_skip_directive((yyvsp[0].literal));
        }
#line 1854 "misc/grammar.tab.c"
    break;

  case 17: /* asm_directives: ASCII STRING  */
#line 108 "misc/grammar.y"
                     {
            parser_add_ascii_directive((yyvsp[0].string));
        }
#line 1862 "misc/grammar.tab.c"
    break;

  case 18: /* asm_directives: EQU symbol_name COMMA expr  */
#line 111 "misc/grammar.y"
                                   {
            parser_add_equ_directive((yyvsp[-2].symbol), (yyvsp[0].expr));
        }
#line 1870 "misc/grammar.tab.c"
    break;

  case 19: /* asm_directives: END  */
#line 114 "misc/grammar.y"
            {
            parser_add_end_directive();
        }
#line 1878 "misc/grammar.tab.c"
    break;

  case 20: /* symbol_list: symbol_name  */
#line 120 "misc/grammar.y"
                    {
            (yyval.symbol_list) = parser_new_symbol_list((yyvsp[0].symbol));
        }
#line 1886 "misc/grammar.tab.c"
    break;

  case 21: /* symbol_list: symbol_list COMMA symbol_name  */
#line 123 "misc/grammar.y"
                                      {
            parser_add_symbol_to_list((yyvsp[-2].symbol_list), (yyvsp[0].symbol));
            (yyval.symbol_list) = (yyvsp[-2].symbol_list);
        }
#line 1895 "misc/grammar.tab.c"
    break;

  case 22: /* sym_lit_list: LITERAL  */
#line 130 "misc/grammar.y"
                {
            (yyval.sym_lit_list) = parser_new_sym_lit_list(new_lit_sl((yyvsp[0].literal)));
        }
#line 1903 "misc/grammar.tab.c"
    break;

  case 23: /* sym_lit_list: symbol_name  */
#line 133 "misc/grammar.y"
                    {
            (yyval.sym_lit_list) = parser_new_sym_lit_list(new_sym_sl((yyvsp[0].symbol)));
        }
#line 1911 "misc/grammar.tab.c"
    break;

  case 24: /* sym_lit_list: sym_lit_list COMMA symbol_name  */
#line 136 "misc/grammar.y"
                                       {
            parser_add_sym_lit_to_list((yyvsp[-2].sym_lit_list), new_sym_sl((yyvsp[0].symbol)));
            (yyval.sym_lit_list) = (yyvsp[-2].sym_lit_list);
        }
#line 1920 "misc/grammar.tab.c"
    break;

  case 25: /* sym_lit_list: sym_lit_list COMMA LITERAL  */
#line 140 "misc/grammar.y"
                                   {
            parser_add_sym_lit_to_list((yyvsp[-2].sym_lit_list), new_lit_sl((yyvsp[0].literal)));
            (yyval.sym_lit_list) = (yyvsp[-2].sym_lit_list);
        }
#line 1929 "misc/grammar.tab.c"
    break;

  case 26: /* expr: LITERAL  */
#line 147 "misc/grammar.y"
                {
            (yyval.expr) = new_expression();
            add_literal_to_expression((yyval.expr), (yyvsp[0].literal));
        }
#line 1938 "misc/grammar.tab.c"
    break;

  case 27: /* expr: symbol_name  */
#line 151 "misc/grammar.y"
                    {
            (yyval.expr) = new_expression();
            add_symbol_to_expression((yyval.expr), (yyvsp[0].symbol), 1);
        }
#line 1947 "misc/grammar.tab.c"
    break;

  case 28: /* expr: expr PLUS LITERAL  */
#line 155 "misc/grammar.y"
                          {
            (yyval.expr) = (yyvsp[-2].expr);
            add_literal_to_expression((yyval.expr), (yyvsp[0].literal));
        }
#line 1956 "misc/grammar.tab.c"
    break;

  case 29: /* expr: expr PLUS symbol_name  */
#line 159 "misc/grammar.y"
                              {
            (yyval.expr) = (yyvsp[-2].expr);
            add_symbol_to_expression((yyval.expr), (yyvsp[0].symbol), 1);
        }
#line 1965 "misc/grammar.tab.c"
    break;

  case 30: /* expr: expr MINUS LITERAL  */
#line 163 "misc/grammar.y"
                           {
            (yyval.expr) = (yyvsp[-2].expr);
            add_literal_to_expression((yyval.expr), -(yyvsp[0].literal));
        }
#line 1974 "misc/grammar.tab.c"
    break;

  case 31: /* expr: expr MINUS symbol_name  */
#line 167 "misc/grammar.y"
                               {
            (yyval.expr) = (yyvsp[-2].expr);
            add_symbol_to_expression((yyval.expr), (yyvsp[0].symbol), -1);
        }
#line 1983 "misc/grammar.tab.c"
    break;

  case 32: /* expr: MINUS LITERAL  */
#line 171 "misc/grammar.y"
                                   {
            (yyval.expr) = new_expression();
            add_literal_to_expression((yyval.expr), -(yyvsp[0].literal));
        }
#line 1992 "misc/grammar.tab.c"
    break;

  case 33: /* expr: MINUS symbol_name  */
#line 175 "misc/grammar.y"
                                       {
            (yyval.expr) = new_expression();
            add_symbol_to_expression((yyval.expr), (yyvsp[0].symbol), -1);
        }
#line 2001 "misc/grammar.tab.c"
    break;

  case 34: /* symbol_name: SYMBOL  */
#line 182 "misc/grammar.y"
               { (yyval.symbol) = (yyvsp[0].symbol); }
#line 2007 "misc/grammar.tab.c"
    break;

  case 35: /* symbol_name: CSRX  */
#line 183 "misc/grammar.y"
             { (yyval.symbol) = (yyvsp[0].reg); }
#line 2013 "misc/grammar.tab.c"
    break;

  case 36: /* symbol_name: GPRX  */
#line 184 "misc/grammar.y"
             { (yyval.symbol) = (yyvsp[0].reg); }
#line 2019 "misc/grammar.tab.c"
    break;

  case 37: /* symbol_name: GLOBAL  */
#line 185 "misc/grammar.y"
               { (yyval.symbol) = parser_copy_symbol("global"); }
#line 2025 "misc/grammar.tab.c"
    break;

  case 38: /* symbol_name: EXTERN  */
#line 186 "misc/grammar.y"
               { (yyval.symbol) = parser_copy_symbol("extern"); }
#line 2031 "misc/grammar.tab.c"
    break;

  case 39: /* symbol_name: SECTION  */
#line 187 "misc/grammar.y"
                { (yyval.symbol) = parser_copy_symbol("section"); }
#line 2037 "misc/grammar.tab.c"
    break;

  case 40: /* symbol_name: WORD  */
#line 188 "misc/grammar.y"
             { (yyval.symbol) = parser_copy_symbol("word"); }
#line 2043 "misc/grammar.tab.c"
    break;

  case 41: /* symbol_name: SKIP  */
#line 189 "misc/grammar.y"
             { (yyval.symbol) = parser_copy_symbol("skip"); }
#line 2049 "misc/grammar.tab.c"
    break;

  case 42: /* symbol_name: ASCII  */
#line 190 "misc/grammar.y"
              { (yyval.symbol) = parser_copy_symbol("ascii"); }
#line 2055 "misc/grammar.tab.c"
    break;

  case 43: /* symbol_name: EQU  */
#line 191 "misc/grammar.y"
            { (yyval.symbol) = parser_copy_symbol("equ"); }
#line 2061 "misc/grammar.tab.c"
    break;

  case 44: /* symbol_name: END  */
#line 192 "misc/grammar.y"
            { (yyval.symbol) = parser_copy_symbol("end"); }
#line 2067 "misc/grammar.tab.c"
    break;

  case 45: /* symbol_name: HALT  */
#line 193 "misc/grammar.y"
             { (yyval.symbol) = parser_copy_symbol("halt"); }
#line 2073 "misc/grammar.tab.c"
    break;

  case 46: /* symbol_name: INT  */
#line 194 "misc/grammar.y"
            { (yyval.symbol) = parser_copy_symbol("int"); }
#line 2079 "misc/grammar.tab.c"
    break;

  case 47: /* symbol_name: IRET  */
#line 195 "misc/grammar.y"
             { (yyval.symbol) = parser_copy_symbol("iret"); }
#line 2085 "misc/grammar.tab.c"
    break;

  case 48: /* symbol_name: CALL  */
#line 196 "misc/grammar.y"
             { (yyval.symbol) = parser_copy_symbol("call"); }
#line 2091 "misc/grammar.tab.c"
    break;

  case 49: /* symbol_name: RET  */
#line 197 "misc/grammar.y"
            { (yyval.symbol) = parser_copy_symbol("ret"); }
#line 2097 "misc/grammar.tab.c"
    break;

  case 50: /* symbol_name: JMP  */
#line 198 "misc/grammar.y"
            { (yyval.symbol) = parser_copy_symbol("jmp"); }
#line 2103 "misc/grammar.tab.c"
    break;

  case 51: /* symbol_name: BEQ  */
#line 199 "misc/grammar.y"
            { (yyval.symbol) = parser_copy_symbol("beq"); }
#line 2109 "misc/grammar.tab.c"
    break;

  case 52: /* symbol_name: BNE  */
#line 200 "misc/grammar.y"
            { (yyval.symbol) = parser_copy_symbol("bne"); }
#line 2115 "misc/grammar.tab.c"
    break;

  case 53: /* symbol_name: BGT  */
#line 201 "misc/grammar.y"
            { (yyval.symbol) = parser_copy_symbol("bgt"); }
#line 2121 "misc/grammar.tab.c"
    break;

  case 54: /* symbol_name: PUSH  */
#line 202 "misc/grammar.y"
             { (yyval.symbol) = parser_copy_symbol("push"); }
#line 2127 "misc/grammar.tab.c"
    break;

  case 55: /* symbol_name: POP  */
#line 203 "misc/grammar.y"
            { (yyval.symbol) = parser_copy_symbol("pop"); }
#line 2133 "misc/grammar.tab.c"
    break;

  case 56: /* symbol_name: XCHG  */
#line 204 "misc/grammar.y"
             { (yyval.symbol) = parser_copy_symbol("xchg"); }
#line 2139 "misc/grammar.tab.c"
    break;

  case 57: /* symbol_name: ADD  */
#line 205 "misc/grammar.y"
            { (yyval.symbol) = parser_copy_symbol("add"); }
#line 2145 "misc/grammar.tab.c"
    break;

  case 58: /* symbol_name: SUB  */
#line 206 "misc/grammar.y"
            { (yyval.symbol) = parser_copy_symbol("sub"); }
#line 2151 "misc/grammar.tab.c"
    break;

  case 59: /* symbol_name: MUL  */
#line 207 "misc/grammar.y"
            { (yyval.symbol) = parser_copy_symbol("mul"); }
#line 2157 "misc/grammar.tab.c"
    break;

  case 60: /* symbol_name: DIV  */
#line 208 "misc/grammar.y"
            { (yyval.symbol) = parser_copy_symbol("div"); }
#line 2163 "misc/grammar.tab.c"
    break;

  case 61: /* symbol_name: NOT  */
#line 209 "misc/grammar.y"
            { (yyval.symbol) = parser_copy_symbol("not"); }
#line 2169 "misc/grammar.tab.c"
    break;

  case 62: /* symbol_name: AND  */
#line 210 "misc/grammar.y"
            { (yyval.symbol) = parser_copy_symbol("and"); }
#line 2175 "misc/grammar.tab.c"
    break;

  case 63: /* symbol_name: OR  */
#line 211 "misc/grammar.y"
           { (yyval.symbol) = parser_copy_symbol("or"); }
#line 2181 "misc/grammar.tab.c"
    break;

  case 64: /* symbol_name: XOR  */
#line 212 "misc/grammar.y"
            { (yyval.symbol) = parser_copy_symbol("xor"); }
#line 2187 "misc/grammar.tab.c"
    break;

  case 65: /* symbol_name: SHL  */
#line 213 "misc/grammar.y"
            { (yyval.symbol) = parser_copy_symbol("shl"); }
#line 2193 "misc/grammar.tab.c"
    break;

  case 66: /* symbol_name: SHR  */
#line 214 "misc/grammar.y"
            { (yyval.symbol) = parser_copy_symbol("shr"); }
#line 2199 "misc/grammar.tab.c"
    break;

  case 67: /* symbol_name: LD  */
#line 215 "misc/grammar.y"
           { (yyval.symbol) = parser_copy_symbol("ld"); }
#line 2205 "misc/grammar.tab.c"
    break;

  case 68: /* symbol_name: ST  */
#line 216 "misc/grammar.y"
           { (yyval.symbol) = parser_copy_symbol("st"); }
#line 2211 "misc/grammar.tab.c"
    break;

  case 69: /* symbol_name: CSRRD  */
#line 217 "misc/grammar.y"
              { (yyval.symbol) = parser_copy_symbol("csrrd"); }
#line 2217 "misc/grammar.tab.c"
    break;

  case 70: /* symbol_name: CSRWR  */
#line 218 "misc/grammar.y"
              { (yyval.symbol) = parser_copy_symbol("csrwr"); }
#line 2223 "misc/grammar.tab.c"
    break;

  case 71: /* asm_instructions: HALT  */
#line 222 "misc/grammar.y"
             {
            parser_add_no_operand_instruction(ASM_INSTR_HALT);
        }
#line 2231 "misc/grammar.tab.c"
    break;

  case 72: /* asm_instructions: INT  */
#line 225 "misc/grammar.y"
            {
            parser_add_no_operand_instruction(ASM_INSTR_INT);
        }
#line 2239 "misc/grammar.tab.c"
    break;

  case 73: /* asm_instructions: IRET  */
#line 228 "misc/grammar.y"
             {
            parser_add_no_operand_instruction(ASM_INSTR_IRET);
        }
#line 2247 "misc/grammar.tab.c"
    break;

  case 74: /* asm_instructions: RET  */
#line 231 "misc/grammar.y"
            {
            parser_add_no_operand_instruction(ASM_INSTR_RET);
        }
#line 2255 "misc/grammar.tab.c"
    break;

  case 75: /* asm_instructions: CALL operandJmp  */
#line 234 "misc/grammar.y"
                        {
            parser_add_jump_instruction(ASM_INSTR_CALL, (yyvsp[0].jmp_operand));
        }
#line 2263 "misc/grammar.tab.c"
    break;

  case 76: /* asm_instructions: JMP operandJmp  */
#line 237 "misc/grammar.y"
                       {
            parser_add_jump_instruction(ASM_INSTR_JMP, (yyvsp[0].jmp_operand));
        }
#line 2271 "misc/grammar.tab.c"
    break;

  case 77: /* asm_instructions: branchOp gpr_12 COMMA operandJmp  */
#line 240 "misc/grammar.y"
                                         {
            parser_add_branch_instruction((yyvsp[-3].instruction), (yyvsp[-2].gprs), (yyvsp[0].jmp_operand));
        }
#line 2279 "misc/grammar.tab.c"
    break;

  case 78: /* asm_instructions: PUSH gpr_1  */
#line 243 "misc/grammar.y"
                   {
            parser_add_single_gpr_instruction(ASM_INSTR_PUSH, (yyvsp[0].reg_id));
        }
#line 2287 "misc/grammar.tab.c"
    break;

  case 79: /* asm_instructions: POP gpr_1  */
#line 246 "misc/grammar.y"
                  {
            parser_add_single_gpr_instruction(ASM_INSTR_POP, (yyvsp[0].reg_id));
        }
#line 2295 "misc/grammar.tab.c"
    break;

  case 80: /* asm_instructions: NOT gpr_1  */
#line 249 "misc/grammar.y"
                  {
            parser_add_single_gpr_instruction(ASM_INSTR_NOT, (yyvsp[0].reg_id));
        }
#line 2303 "misc/grammar.tab.c"
    break;

  case 81: /* asm_instructions: arithmeticOp gpr_12  */
#line 252 "misc/grammar.y"
                            {
            parser_add_two_gpr_instruction((yyvsp[-1].instruction), (yyvsp[0].gprs));
        }
#line 2311 "misc/grammar.tab.c"
    break;

  case 82: /* asm_instructions: LD operandLS COMMA gpr_1  */
#line 255 "misc/grammar.y"
                                 {
            parser_add_ld_instruction((yyvsp[-2].ls_operand), (yyvsp[0].reg_id));
        }
#line 2319 "misc/grammar.tab.c"
    break;

  case 83: /* asm_instructions: ST gpr_1 COMMA operandLS  */
#line 258 "misc/grammar.y"
                                 {
            parser_add_st_instruction((yyvsp[-2].reg_id), (yyvsp[0].ls_operand));
        }
#line 2327 "misc/grammar.tab.c"
    break;

  case 84: /* asm_instructions: CSRRD csr_1 COMMA gpr_1  */
#line 261 "misc/grammar.y"
                                {
            parser_add_csrrd_instruction((yyvsp[-2].reg_id), (yyvsp[0].reg_id));
        }
#line 2335 "misc/grammar.tab.c"
    break;

  case 85: /* asm_instructions: CSRWR gpr_1 COMMA csr_1  */
#line 264 "misc/grammar.y"
                                {
            parser_add_csrwr_instruction((yyvsp[-2].reg_id), (yyvsp[0].reg_id));
        }
#line 2343 "misc/grammar.tab.c"
    break;

  case 86: /* gpr_12: PERCENT GPRX COMMA PERCENT GPRX  */
#line 270 "misc/grammar.y"
                                        {
            (yyval.gprs).reg1 = reg_from_name((yyvsp[-3].reg));
            (yyval.gprs).reg2 = reg_from_name((yyvsp[0].reg));
        }
#line 2352 "misc/grammar.tab.c"
    break;

  case 87: /* gpr_1: PERCENT GPRX  */
#line 277 "misc/grammar.y"
                     {
            (yyval.reg_id) = reg_from_name((yyvsp[0].reg));
        }
#line 2360 "misc/grammar.tab.c"
    break;

  case 88: /* csr_1: PERCENT CSRX  */
#line 283 "misc/grammar.y"
                     {
            (yyval.reg_id) = reg_from_name((yyvsp[0].reg));
        }
#line 2368 "misc/grammar.tab.c"
    break;

  case 89: /* branchOp: BNE  */
#line 289 "misc/grammar.y"
            { (yyval.instruction) = ASM_INSTR_BNE; }
#line 2374 "misc/grammar.tab.c"
    break;

  case 90: /* branchOp: BEQ  */
#line 290 "misc/grammar.y"
            { (yyval.instruction) = ASM_INSTR_BEQ; }
#line 2380 "misc/grammar.tab.c"
    break;

  case 91: /* branchOp: BGT  */
#line 291 "misc/grammar.y"
            { (yyval.instruction) = ASM_INSTR_BGT; }
#line 2386 "misc/grammar.tab.c"
    break;

  case 92: /* arithmeticOp: XCHG  */
#line 295 "misc/grammar.y"
             { (yyval.instruction) = ASM_INSTR_XCHG; }
#line 2392 "misc/grammar.tab.c"
    break;

  case 93: /* arithmeticOp: ADD  */
#line 296 "misc/grammar.y"
            { (yyval.instruction) = ASM_INSTR_ADD; }
#line 2398 "misc/grammar.tab.c"
    break;

  case 94: /* arithmeticOp: SUB  */
#line 297 "misc/grammar.y"
            { (yyval.instruction) = ASM_INSTR_SUB; }
#line 2404 "misc/grammar.tab.c"
    break;

  case 95: /* arithmeticOp: MUL  */
#line 298 "misc/grammar.y"
            { (yyval.instruction) = ASM_INSTR_MUL; }
#line 2410 "misc/grammar.tab.c"
    break;

  case 96: /* arithmeticOp: DIV  */
#line 299 "misc/grammar.y"
            { (yyval.instruction) = ASM_INSTR_DIV; }
#line 2416 "misc/grammar.tab.c"
    break;

  case 97: /* arithmeticOp: AND  */
#line 300 "misc/grammar.y"
            { (yyval.instruction) = ASM_INSTR_AND; }
#line 2422 "misc/grammar.tab.c"
    break;

  case 98: /* arithmeticOp: OR  */
#line 301 "misc/grammar.y"
           { (yyval.instruction) = ASM_INSTR_OR; }
#line 2428 "misc/grammar.tab.c"
    break;

  case 99: /* arithmeticOp: XOR  */
#line 302 "misc/grammar.y"
            { (yyval.instruction) = ASM_INSTR_XOR; }
#line 2434 "misc/grammar.tab.c"
    break;

  case 100: /* arithmeticOp: SHL  */
#line 303 "misc/grammar.y"
            { (yyval.instruction) = ASM_INSTR_SHL; }
#line 2440 "misc/grammar.tab.c"
    break;

  case 101: /* arithmeticOp: SHR  */
#line 304 "misc/grammar.y"
            { (yyval.instruction) = ASM_INSTR_SHR; }
#line 2446 "misc/grammar.tab.c"
    break;

  case 102: /* operandJmp: LITERAL  */
#line 308 "misc/grammar.y"
                {
            (yyval.jmp_operand) = parser_new_jmp_literal((yyvsp[0].literal));
        }
#line 2454 "misc/grammar.tab.c"
    break;

  case 103: /* operandJmp: symbol_name  */
#line 311 "misc/grammar.y"
                    {
            (yyval.jmp_operand) = parser_new_jmp_symbol((yyvsp[0].symbol));
        }
#line 2462 "misc/grammar.tab.c"
    break;

  case 104: /* operandLS: DOLLAR LITERAL  */
#line 317 "misc/grammar.y"
                       {
            (yyval.ls_operand) = parser_new_ls_operand(ASM_OP_LS_IMM_LIT, (yyvsp[0].literal), NULL, ASM_REG_R0);
        }
#line 2470 "misc/grammar.tab.c"
    break;

  case 105: /* operandLS: DOLLAR symbol_name  */
#line 320 "misc/grammar.y"
                           {
            (yyval.ls_operand) = parser_new_ls_operand(ASM_OP_LS_IMM_SYM, 0, (yyvsp[0].symbol), ASM_REG_R0);
        }
#line 2478 "misc/grammar.tab.c"
    break;

  case 106: /* operandLS: LITERAL  */
#line 323 "misc/grammar.y"
                {
            (yyval.ls_operand) = parser_new_ls_operand(ASM_OP_LS_MEM_LIT, (yyvsp[0].literal), NULL, ASM_REG_R0);
        }
#line 2486 "misc/grammar.tab.c"
    break;

  case 107: /* operandLS: symbol_name  */
#line 326 "misc/grammar.y"
                    {
            (yyval.ls_operand) = parser_new_ls_operand(ASM_OP_LS_MEM_SYM, 0, (yyvsp[0].symbol), ASM_REG_R0);
        }
#line 2494 "misc/grammar.tab.c"
    break;

  case 108: /* operandLS: PERCENT GPRX  */
#line 329 "misc/grammar.y"
                     {
            (yyval.ls_operand) = parser_new_ls_operand(ASM_OP_LS_REG, 0, NULL, reg_from_name((yyvsp[0].reg)));
        }
#line 2502 "misc/grammar.tab.c"
    break;

  case 109: /* operandLS: RBRACK PERCENT GPRX LBRACK  */
#line 332 "misc/grammar.y"
                                   {
            (yyval.ls_operand) = parser_new_ls_operand(ASM_OP_LS_REG_MEM, 0, NULL, reg_from_name((yyvsp[-1].reg)));
        }
#line 2510 "misc/grammar.tab.c"
    break;

  case 110: /* operandLS: RBRACK PERCENT GPRX PLUS LITERAL LBRACK  */
#line 335 "misc/grammar.y"
                                                {
            (yyval.ls_operand) = parser_new_ls_operand(ASM_OP_LS_REG_MEM_LIT, (yyvsp[-1].literal), NULL, reg_from_name((yyvsp[-3].reg)));
        }
#line 2518 "misc/grammar.tab.c"
    break;

  case 111: /* operandLS: RBRACK PERCENT GPRX PLUS symbol_name LBRACK  */
#line 338 "misc/grammar.y"
                                                    {
            (yyval.ls_operand) = parser_new_ls_operand(ASM_OP_LS_REG_MEM_SYM, 0, (yyvsp[-1].symbol), reg_from_name((yyvsp[-3].reg)));
        }
#line 2526 "misc/grammar.tab.c"
    break;


#line 2530 "misc/grammar.tab.c"

        default: break;
      }
    if (yychar_backup != yychar)
      YY_LAC_DISCARD ("yychar change");
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
      {
        yypcontext_t yyctx
          = {yyssp, yyesa, &yyes, &yyes_capacity, yytoken};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        if (yychar != YYEMPTY)
          YY_LAC_ESTABLISH;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
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

  /* If the stack popping above didn't lose the initial context for the
     current lookahead token, the shift below will for sure.  */
  YY_LAC_DISCARD ("error recovery");

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
  if (yyes != yyesa)
    YYSTACK_FREE (yyes);
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 343 "misc/grammar.y"

