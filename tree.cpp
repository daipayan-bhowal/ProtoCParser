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
