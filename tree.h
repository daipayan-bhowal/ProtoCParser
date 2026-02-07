#pragma once
#pragma once
#include "CLexer.h"
#include "bool_t.h"
#define MAX_CHILD 3

typedef enum
{
    OP_PLUS, // '+'
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
    OP_COMMA
} OpTokenType;


typedef enum
{
    If,
    While,
    Switch,
    Do,
    For
} StmtType;

typedef enum
{
    OP,  // Operator
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
    Expr
}NodeType;

typedef enum
{
    Void,
    Char,
    Short,
    Int,
    Long,
    Float,
    Double,
    Unsign
} DclType;


struct Tree
{
    struct Tree* child[MAX_CHILD];
    // need to add sibling for future
    NodeType nodeType;
    union {
        StmtType stmt;
        ExpType exp;
    } Type;
    union {
        OpTokenType op;
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
TreeNode* postfix_expression(bool_t* IsPost);
TreeNode* argument_expression_list();
