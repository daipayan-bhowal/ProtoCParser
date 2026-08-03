#pragma once
#pragma once
#include "CLexer.h"
#include "bool_t.h"
#define MAX_CHILD 3
#define NA -1
#define _CRT_SECURE_NO_WARNINGS

typedef enum
{
    OP_PLUS = 1, // '+'
    OP_PLUSEQ, // '+='
    OP_MINUS, // '-'
    OP_MINUSEQ, // '-'
    OP_MULT, // '*'
    OP_MULTEQ, // '*='
    OP_DIV, // '/'
    OP_DIVEQ, // '/='
    OP_MOD, // '%'
    OP_MODEQ, // '%='
    OP_RSFT, // >>
    OP_RSFTEQ, // >>
    OP_LSFT,  // <<
    OP_LSFTEQ,  // <<
    OP_ASSIGN, // =
    OP_EQ, // ==
    OP_NOTEQ,  // != 
    OP_LT, // <
    OP_GT, // >
    OP_LTE, // <=
    OP_GTE, // >=
    OP_OR, // |
    OP_OREQ, // |=
    OP_AND, // &
    OP_ANDEQ, // '&='
    OP_XOR, // ^
    OP_XOREQ, // ^=
    OP_NEG, // ~
    OP_ADDROF, // &
    OP_LOGOR, // ||
    OP_LOGAND, // &&
    OP_PREINCR, // ++i
    OP_PREDECR, // --i
    OP_UPLUS, // +5
    OP_UMINUS, // -5
    OP_DEREF, // *ptr
    OP_COMPL,  // ~
    OP_NOT, // !
    OP_SZ,  // sizeof
    OP_ARRAY, // [ ]
    OP_POSTINCR, // i++
    OP_POSTDECR,  // i--
    OP_UINT_CAST, // unsigned int
    OP_SINT_CAST, // signed int
    OP_CINT_CAST, // const int
    OP_SLONG_CAST,
    OP_ULONG_CAST,
    OP_CLONG_CAST,
    OP_SSHORT_CAST,
    OP_USHORT_CAST,
    OP_CSHORT_CAST,
    OP_FLOAT_CAST,
    OP_CHAR_CAST,
    OP_DOUBLE_CAST,
    OP_COMMA,
    OP_ARROW_PTR,
    OP_DOT,
    OP_CALLER,  // function caller , i.e  x = f(i) + g(i);
	OP_TERN // for ternary operator ?
} OpTokenType;

typedef enum
{
    INTEGER = 1,
    FPOINT,
    STRING,
    CHARAC

} OperandType;

typedef enum
{
    If = 1,
    Else,
    While,
    Switch,
    Do,
    For,
    Label,
    Goto,
    Break,
    Case,
    Return,
    Default,
    Continue
} StmtType;

typedef enum
{
    OP = 1,  // Operator
    IDEN,  // Identifier
    ICONST, // Integer Const
    FCONST,  // Float COnst
    CHARCONST,
    STRLIT,
    CALLEXP   // function caller
} ExpType;

typedef enum
{
    Stmt,
    Expr,
    ComplxType,
    FuncDef
}NodeType;

typedef enum
{
    Void,
    Char,  /* i.e. c = 'a'; */
    Short, /* i.e. short s = 12; */
    Int,
    Long,
    Float,
    Double,
    Unsign,
    StructDcl,
    EnumDcl,
    UnionDcl,
    NotADclType = NA /* if init value is not available i.e int i;*/
} DclType;

typedef enum
{
    Struct,
    Enum,
    Union,
    NotAComposite = NA /* if it's not a composite type */
} CompositeType;

typedef enum
{
    GLOBAL,
    LOCAL,
    FUNC,
    EXTERNAL,
    STATC,
    STRUCT_MEM,
    UNION_MEM
} ScopeType;

typedef enum
{
    EXPR_ASSIGN,
    ARRAY_OF,  // '[' ']'
    POINTER_OF,     // '*'
    PARAMTYPE,    // parameter of decl
   // OPEN_BRACK,   // '('
   // CLOSE_BRACK,  // ')'
    FUNC_DEF,
    FUNC_DCL,
    RETURNTYPE,
    IDENTIFIER,
    CONST_TYP_QUAL,
    VOLAT_TYP_QUAL
} ComplNodetype;

struct Tree;

struct ComplType
{
    ComplNodetype subCompntComplx;
    struct Tree* array_size;
    union {
        DclType return_type;
        DclType params[50];
        string_t Identifier;
    } categ;
    struct ComplType* Complx_child[2];
};

typedef struct ComplType ComplxNode;


struct Tree
{
    struct Tree* child[MAX_CHILD];
    // need to add sibling for future
    NodeType nodeType;
    union {
        StmtType stmt;
        ExpType exp;
        ComplType* ctyp;
    } Type;
    union {
        OpTokenType op;
        OperandType operd;
        string_t operand;
        long ival; // for storing integer in decimal form
        double fval;  // for storing floating point number
        string_t strval;  // for storing string literal
        char ch;  // for storing a character
    } attrib;

    DclType dType;     // for future need to add var for typechecking
};

typedef struct Tree TreeNode;

TreeNode* newStmtNode(StmtType tp);
TreeNode* newExpNode(ExpType tp);
TreeNode* newTypeNode(ComplxNode* tp);

ComplxNode* newSubDeclNode(ComplNodetype tp, TreeNode *t);
void printTree(TreeNode* t);
void debugOp(TreeNode *t);
void debugDecl(ComplxNode* c);

void IsParseFailed(const char source_func[], int line);
void FailOnEOF(const char source_func[], int line);
#define checkEOF() FailOnEOF(__func__,__LINE__)
#define getNextToken() _getNextToken(__func__,__LINE__)

/* Based on K & R C book modified grammer */
TreeNode* primary_expression(bool_t* IsPrim);
TreeNode* expression();
TreeNode* assignment_expression();
TreeNode* conditional_expression();
TreeNode* logical_OR_expression();
TreeNode* logical_AND_expression();
TreeNode* inclusive_OR_expression();
TreeNode* exclusive_OR_expression();
TreeNode* AND_expression();
TreeNode* equality_expression();
TreeNode* relational_expression();
TreeNode* shift_expression();
TreeNode* additive_expression();
TreeNode* multiplicative_expression();
TreeNode* cast_expression();
TreeNode* unary_expression();
TreeNode* postfix_expression(bool_t* IsPost, bool_t* IsPrim);
TreeNode* argument_expression_list();

bool_t MaybeDcl();
ComplxNode* declaration(bool_t *);
ComplxNode* declarator();
ComplxNode* direct_declarator();
TreeNode* statement();
TreeNode* constant_expression();

void specifier_qualifier_list();
void func_defination_parameter_list(DclType *d,int *no_of_params);
void func_declare_parameter_list(DclType* d, int* no_of_params);
bool_t start_function();

