#include <stdlib.h>
#include "tree.h"
TreeNode* newStmtNode(StmtType tp)
{
    int i;
    TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
    for (i = 0; i < MAX_CHILD; i++)
    {
        t->child[i] = NULL;
    }
    t->Type.stmt = tp;
    t->nodeType = Stmt;
    // Future addition needed
    return t;
}
TreeNode* newExpNode(ExpType tp)
{
    int i;
    TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
    for (i = 0; i < MAX_CHILD; i++)
    {
        t->child[i] = NULL;
    }
    t->Type.exp = tp;
    t->nodeType = Expr;
    // Future addition needed
    return t;
}

void debugOp(OpTokenType op)
{
    switch (op)
    {
    case OP_PLUS: // '+'
        printf("OP_PLUS\n");
        break;
    case OP_PLUSEQ: // '+='
        printf("OP_PLUSEQ\n");
        break;
    case OP_MINUS: // '-'
        printf("OP_MINUS\n");
        break;
    case OP_MINUSEQ: // '-'
        printf("OP_MINUSEQ\n");
        break;
    case OP_MULT: // '*'
        printf("OP_MULT\n");
        break;
    case OP_MULTEQ: // '*='
        printf("OP_MULTEQ\n");
        break;
    case OP_DIV: // '/'
        printf("OP_DIV\n");
        break;
    case OP_DIVEQ: // '/='
        printf("OP_DIVEQ\n");
        break;
    case OP_MOD: // '%'
        printf("OP_MOD\n");
        break;
    case OP_MODEQ: // '%='
        printf("OP_MODEQ\n");
        break;
    case OP_RSFT: // >>
        printf("OP_RSFT\n");
        break;
    case OP_RSFTEQ: // >>
        printf("OP_RSFTEQ\n");
        break;
    case OP_LSFT:  // <<
        printf("OP_LSFT\n");
        break;
    case OP_LSFTEQ:  // <<
        printf("OP_LSFTEQ\n");
        break;
    case OP_ASSIGN: // =
        printf("OP_ASSIGN\n");
        break;
    case OP_EQ: // ==
        printf("OP_EQ\n");
        break;
    case OP_NOTEQ:  // !=
        printf("OP_NOTEQ\n");
        break;
    case OP_LT: // <
        printf("OP_LT\n");
        break;
    case OP_GT: // >
        printf("OP_GT\n");
        break;
    case OP_LTE: // <=
        printf("OP_LTE\n");
        break;
    case OP_GTE: // >=
        printf("OP_GTE\n");
        break;
    case OP_OR: // |
        printf("OP_OR\n");
        break;
    case OP_OREQ: // |=
        printf("OP_OREQ\n");
        break;
    case OP_AND: // &
        printf("OP_AND\n");
        break;
    case OP_ANDEQ: // '&='
        printf("OP_ANDEQ\n");
        break;
    case OP_XOR: // ^
        printf("OP_XOR\n");
        break;
    case OP_XOREQ: // ^=
        printf("OP_XOREQ\n");
        break;
    case OP_NEG: // ~
        printf("OP_NEG\n");
        break;
    case OP_ADDROF: // &
        printf("OP_ADDROF\n");
        break;
    case OP_LOGOR: // ||
        printf("OP_LOGOR\n");
        break;
    case OP_LOGAND: // &&
        printf("OP_LOGAND\n");
        break;
    case OP_PREINCR: // ++i
        printf("OP_PREINCR\n");
        break;
    case OP_PREDECR: // --i
        printf("OP_PREDECR\n");
        break;
    case OP_UPLUS: // +5
        printf("OP_UPLUS\n");
        break;
    case OP_UMINUS: // -5
        printf("OP_UMINUS\n");
        break;
    case OP_DEREF: // *ptr
        printf("OP_DEREF\n");
        break;
    case OP_COMPL:  // ~
        printf("OP_COMPL\n");
        break;
    case OP_NOT: // !
        printf("OP_NOT\n");
        break;
    case OP_SZ:  // sizeof
        printf("OP_SZ\n");
        break;
    case OP_ARRAY: // [ ]
        printf("OP_ARRAY\n");
        break;
    case OP_POSTINCR: // i++
        printf("OP_POSTINCR\n");
        break;
    case OP_POSTDECR:  // i--
        printf("OP_POSTDECR\n");
        break;
    case OP_UINT_CAST: // unsigned int
        printf("OP_UINT_CAST\n");
        break;
    case OP_SINT_CAST: // signed int
        printf("OP_SINT_CAST\n");
        break;
    case OP_CINT_CAST: // const int
        printf("OP_CINT_CAST\n");
        break;
    case OP_SLONG_CAST:
        printf("OP_SLONG_CAST\n");
        break;
    case OP_ULONG_CAST:
        break;
    case OP_CLONG_CAST:
        break;
    case OP_SSHORT_CAST:
        break;
    case OP_USHORT_CAST:
        break;
    case OP_CSHORT_CAST:
        break;
    case OP_FLOAT_CAST:
        break;
    case OP_CHAR_CAST:
        break;
    case OP_DOUBLE_CAST:
        break;
    case OP_COMMA:
        printf("OP_COMMA\n");
        break;
    }
}

void printTree(TreeNode* t)
{
    int i;
    if (t == NULL)
    {
        return;
    }
	debugOp(t->attrib.op);
    for (i = 0; i < 3; i++)
    {
        printTree(t->child[i]);
    }

}
