/**** 'Made in India' by Daipayan *****/
#include <stdio.h>
#include <stdlib.h>
#include "CLexer.h"
#include "compile_t.h"
//#include "debug.h"
#include "tree.h"
#include "bool_t.h"
//#include "util.h"
//#include "p_codegen.h"

#include "Brutal.h"



void IsParseFailed(const char source_func[], int line)
{
    printf("%s: error: token:%s tok pos:%d at Parser func:%s line:%d \n", __FILE__, getTokenString()->str, getTokenPos(), source_func, line);
    exit(0);

}
void FailOnEOF(const char source_func[], int line)
{
    if (lookahead() == EOF)
    {
        IsParseFailed(source_func, line);
    }
}

bool_t checkNextTokenIsPrim()
{
    int token = getCurrentToken();
    if (token == ID ||
        token == STROBJ ||
        token == HEXADECI_CONST ||
        token == OCTAL_CONST ||
        token == USIGN_INT_CONST ||
        token == INT_CONST ||
        token == LONG_INT_CONST ||
        token == F_CONST ||
        token == LONG_DOUBLE_CONST ||
        token == E_F_CONST ||
        token == CHAR_CONST ||
        token == ESCAPE_SEQ_CONST
        )
    {
        printf("error: wrong expression, primary expr cannot come after primary expr!\n");
        return True;
    }
}

bool_t checkNextTokenIsUnary()
{
    int token = getCurrentToken();
    if (token == '~' ||
        token == '!' ||
        token == SIZEOF
        )
    {
        printf("error: wrong expression, unary operator cannot come after primary expr!\n");
        return True;
    }

}
bool_t checkNextTokenIsUnknownChar()
{
    int token = getCurrentToken();
    if (token == '@' ||
        token == '#' ||
        token == '$' ||
        token == '`'
        )
    {
		printf("error: wrong expression, illegal symbols found %c !\n", token);
        return True;
    }
    return False;
}


TreeNode* primary_expression(bool_t* IsPrim)
{
    //  int k=current_ptr;
    printf("frame: %s tok:%d tok_str:%s tok_pos:%d\n", __func__, getCurrentToken(), getTokenString()->str, getTokenPos());
    TreeNode* t = NULL;
    int offset = 0;
    int token = getCurrentToken();

    if (token == HEXADECI_CONST ||
        token == OCTAL_CONST ||
        token == USIGN_INT_CONST ||
        token == INT_CONST ||
        token == LONG_INT_CONST ||
        token == F_CONST ||
        token == LONG_DOUBLE_CONST ||
        token == E_F_CONST ||
        token == CHAR_CONST ||
        token == ESCAPE_SEQ_CONST
        )
    {
        *IsPrim = True;
        //  printTok_len(offset);
        if (token == HEXADECI_CONST)
        {
            t = newExpNode(ICONST);
            t->attrib.ival = ConvertHextoDec(getTokenString());
        }
        else if (token == OCTAL_CONST)
        {
            t = newExpNode(ICONST);
            t->attrib.ival = ConvertOcttoDec(getTokenString());
        }
        else if (token == USIGN_INT_CONST || token == LONG_INT_CONST || token == INT_CONST)
        {
            t = newExpNode(ICONST);
            t->attrib.ival = ConvertStrtoDec(getTokenString());
        }
        else if (token == F_CONST || token == LONG_DOUBLE_CONST)
        {
            t = newExpNode(FCONST);
            t->attrib.fval = ConvertStrtoDouble(getTokenString());
        }
        else if (token == E_F_CONST)
        {
            t = newExpNode(FCONST);
            t->attrib.fval = ConvertStrtoDouble(getTokenString());

        }
        else if (token == CHAR_CONST || token == ESCAPE_SEQ_CONST)
        {
            t = newExpNode(CHARCONST);
            t->attrib.ch = ConvertStrtoASCII(getTokenString());
        }
        //white_spaces();
        if (lookahead() == EOF)
            return t;
        getNextToken();
        if (checkNextTokenIsPrim() == True ||
            checkNextTokenIsUnary() == True ||
            checkNextTokenIsUnknownChar() == True
            )
            _exit(0);
    }
    else if (token == ID)
    {
        //  printf("%s %d\n", Buffer + current_ptr, current_ptr);
     //   printTok_len(offset);
        t = newExpNode(IDEN);
        *IsPrim = True;
        //white_spaces();
        if (lookahead() == EOF)
            return t;
        getNextToken();
        if (checkNextTokenIsPrim() == True ||
            checkNextTokenIsUnary() == True ||
            checkNextTokenIsUnknownChar() == True
            )
            _exit(0);

    }
    else if (token == STROBJ)
    {
        t = newExpNode(STRLIT);
        //StrCopy(&c, current_ptr, offset);
        t->attrib.strval = getTokenString();
        *IsPrim = True;
        //white_spaces();
        if (lookahead() == EOF)
            return t;
        getNextToken();
        if (checkNextTokenIsPrim() == True ||     // next token should not be prime, unary or unknown chars
            checkNextTokenIsUnary() == True ||      // if it's one of them then illegal i.e.  x = "hello"~
            checkNextTokenIsUnknownChar() == True
            )
            _exit(0);
    }
    else if (token == '(')
    {
        checkEOF();
        t = expression();


        //white_spaces();
        if ((token = getCurrentToken()) != ')')
        {
            printf("error: expected ')' !\n");
            _exit(0);
        }
        if (t != NULL)
        {
            *IsPrim = True;
        }
        if (lookahead() == EOF)
            return t;

    }
    /*else if ((t = expression()) == NULL)
    {
        IsParseFailed(__func__, __LINE__);
        return NULL;
    }*/
    return t;
}
TreeNode* expression()
{
    printf("frame: %s tok:%d tok_str:%s\n", __func__, getCurrentToken(), getTokenString()->str);
    TreeNode* t;
    TreeNode* t2;
    //white_spaces();
    t = assignment_expression();

    //white_spaces();
    int tok = getCurrentToken();
    if (tok == ',')
    {

        t2 = assignment_expression();

        return t2;
    }
    return t;
}
TreeNode* assignment_expression()
{
    printf("frame: %s tok:%d tok_str:%s\n", __func__, getCurrentToken(), getTokenString()->str);
    TreeNode* t;
    TreeNode* t2;
    //white_spaces();

    t = conditional_expression();
    int tok = getCurrentToken();
    //white_spaces();
    if (tok == '=')
    {
        t2 = newExpNode(OP);

        t2->child[0] = t;
        t2->attrib.op = OP_ASSIGN;
        t = t2;
        //white_spaces();
        checkEOF();
        getNextToken();
        t->child[1] = assignment_expression();

    }
    else if (tok == MUL_ASSIGN)
    {
        t2 = newExpNode(OP);
        t2->child[0] = t;
        t2->attrib.op = OP_MULTEQ;
        t = t2;

        checkEOF();
        //white_spaces();
        getNextToken();
        t->child[1] = assignment_expression();
    }
    else if (tok == DIV_ASSIGN)
    {
        t2 = newExpNode(OP);
        t2->child[0] = t;
        t2->attrib.op = OP_DIVEQ;
        t = t2;

        checkEOF();
        //white_spaces();
        getNextToken();
        t->child[1] = assignment_expression();
    }
    else if (tok == MOD_ASSIGN)
    {
        t2 = newExpNode(OP);
        t2->child[0] = t;
        t2->attrib.op = OP_MODEQ;
        t = t2;

        checkEOF();
        getNextToken();
        //white_spaces();
        t->child[1] = assignment_expression();

    }
    else if (tok == ADD_ASSIGN)
    {
        t2 = newExpNode(OP);
        t2->child[0] = t;
        t2->attrib.op = OP_PLUSEQ;
        t = t2;

        checkEOF();
        getNextToken();
        //white_spaces();
        t->child[1] = assignment_expression();

    }
    else if (tok == SUB_ASSIGN)
    {
        t2 = newExpNode(OP);
        t2->child[0] = t;
        t2->attrib.op = OP_MINUSEQ;
        t = t2;

        checkEOF();
        getNextToken();
        //white_spaces();
        t->child[1] = assignment_expression();

    }
    else if (tok == AND_ASSIGN)
    {
        t2 = newExpNode(OP);
        t2->child[0] = t;
        t2->attrib.op = OP_ANDEQ;
        t = t2;

        checkEOF();
        getNextToken();
        //white_spaces();
        t->child[1] = assignment_expression();

    }
    else if (tok == XOR_ASSIGN)
    {
        t2 = newExpNode(OP);
        t2->child[0] = t;
        t2->attrib.op = OP_XOREQ;
        t = t2;

        checkEOF();
        getNextToken();
        //white_spaces();
        t->child[1] = assignment_expression();

    }
    else if (tok == OR_ASSIGN)
    {
        t2 = newExpNode(OP);
        t2->child[0] = t;
        t2->attrib.op = OP_OREQ;
        t = t2;

        checkEOF();
        getNextToken();
        //white_spaces();
        t->child[1] = assignment_expression();
    }
    else if (tok == LEFT_ASSIGN_OP)
    {
        t2 = newExpNode(OP);
        t2->child[0] = t;
        t2->attrib.op = OP_LSFTEQ;
        t = t2;

        checkEOF();
        getNextToken();
        //white_spaces();
        t->child[1] = assignment_expression();

    }
    else if (tok == RIGHT_ASSIGN_OP)
    {
        t2 = newExpNode(OP);
        t2->child[0] = t;
        t2->attrib.op = OP_RSFTEQ;
        t = t2;

        checkEOF();
        getNextToken();
        //white_spaces();
        t->child[1] = assignment_expression();

    }

    return t;
}

TreeNode* conditional_expression()
{
    printf("frame: %s tok:%d tok_str:%s\n", __func__, getCurrentToken(), getTokenString()->str);
    TreeNode* t = NULL,*t2= NULL,*t3= NULL;
    t3 = logical_OR_expression();

    //white_spaces();
    int tok = getCurrentToken();
    if (tok == '?')
    {
        t = newStmtNode(If);
        t2 = newStmtNode(Else);

        t->child[0] = t3;

        //white_spaces();
        checkEOF();
        getNextToken();
        t->child[1] = expression();

        //white_spaces();

        if ((tok = getCurrentToken()) != ':')
        {
            printf("error: expected '?' \n");
            _exit(0);
        }

        checkEOF();
        getNextToken();
        //white_spaces();
        t2->child[0] = conditional_expression();
        //t2->child[0] = t3;
        t->child[2] = t2;
        
        return t;

    }
    return t3;
}
TreeNode* logical_OR_expression()
{
    printf("frame: %s tok:%d tok_str:%s\n", __func__, getCurrentToken(), getTokenString()->str);
    TreeNode* t;
    TreeNode* t2;
    t = logical_AND_expression();

    //white_spaces();
    int tok = getCurrentToken();
    if (tok == OR_OR_OP)
    {
        t2 = newExpNode(OP);
        t2->child[0] = t;
        t2->attrib.op = OP_LOGOR;
        t = t2;

        checkEOF();
        getNextToken();
        t->child[1] = logical_OR_expression();

    }
    return t;
}

TreeNode* logical_AND_expression()
{
    printf("frame: %s tok:%d tok_str:%s\n", __func__, getCurrentToken(), getTokenString()->str);
    TreeNode* t;
    TreeNode* t2;

    t = inclusive_OR_expression();
    int tok = getCurrentToken();
    if (tok == AND_AND_OP)
    {
        t2 = newExpNode(OP);
        t2->child[0] = t;
        t2->attrib.op = OP_LOGAND;
        t = t2;

        checkEOF();
        getNextToken();

        t2 = logical_AND_expression();
        t->child[1] = t2;

    }
    return t;
}

TreeNode* inclusive_OR_expression()
{
    printf("frame: %s tok:%d tok_str:%s\n", __func__, getCurrentToken(), getTokenString()->str);
    TreeNode* t;
    TreeNode* t2;
    t = exclusive_OR_expression();
    int tok = getCurrentToken();
    if (tok == '|')
    {
        t2 = newExpNode(OP);
        t2->child[0] = t;
        t2->attrib.op = OP_OR;
        t = t2;

        checkEOF();
        getNextToken();
        t->child[1] = inclusive_OR_expression();
    }
    return t;
}

TreeNode* exclusive_OR_expression()
{
    printf("frame: %s tok:%d tok_str:%s\n", __func__, getCurrentToken(), getTokenString()->str);
    TreeNode* t;
    TreeNode* t2;
    t = AND_expression();

    int tok = getCurrentToken();
    if (tok == '^')
    {
        t2 = newExpNode(OP);
        t2->child[0] = t;
        t2->attrib.op = OP_XOR;
        t = t2;

        checkEOF();
        getNextToken();
        t->child[1] = exclusive_OR_expression();
    }
    return t;
}

TreeNode* AND_expression()
{
    printf("frame: %s tok:%d tok_str:%s\n", __func__, getCurrentToken(), getTokenString()->str);
    TreeNode* t;
    TreeNode* t2;
    t = equality_expression();

    //white_spaces();
    int tok = getCurrentToken();
    if (tok == '&')
    {
        t2 = newExpNode(OP);
        t2->child[0] = t;
        t2->attrib.op = OP_AND;
        t = t2;

        checkEOF();
        getNextToken();
        t->child[1] = AND_expression();
    }
    return t;
}

TreeNode* equality_expression()
{
    printf("frame: %s tok:%d tok_str:%s\n", __func__, getCurrentToken(), getTokenString()->str);
    TreeNode* t;
    TreeNode* t2;
    t = relational_expression();
    //white_spaces();
    int tok = getCurrentToken();
    if (tok == EQ_EQ_COND)
    {
        t2 = newExpNode(OP);
        t2->child[0] = t;
        t2->attrib.op = OP_EQ;
        t = t2;

        checkEOF();
        getNextToken();
        t->child[1] = equality_expression();
    }
    else if (tok == NOT_ASSIGN)
    {
        t2 = newExpNode(OP);
        t2->child[0] = t;
        t2->attrib.op = OP_NOTEQ;
        t = t2;

        checkEOF();
        getNextToken();
        t->child[1] = equality_expression();
    }
    return t;
}

TreeNode* relational_expression()
{
    printf("frame: %s tok:%d tok_str:%s\n", __func__, getCurrentToken(), getTokenString()->str);
    TreeNode* t;
    TreeNode* t2;
    t = shift_expression();

    //white_spaces();
    int tok = getCurrentToken();
    if (tok == '<')
    {
        t2 = newExpNode(OP);
        t2->child[0] = t;
        t2->attrib.op = OP_LT;
        t = t2;

        checkEOF();
        getNextToken();
        t->child[1] = relational_expression();
    }
    else if (tok == '>')
    {
        t2 = newExpNode(OP);
        t2->child[0] = t;
        t2->attrib.op = OP_GT;
        t = t2;

        checkEOF();
        getNextToken();
        t->child[1] = relational_expression();

    }
    else if (tok == LESR_EQ_OP)
    {
        t2 = newExpNode(OP);
        t2->child[0] = t;
        t2->attrib.op = OP_LTE;
        t = t2;

        checkEOF();
        getNextToken();
        t->child[1] = relational_expression();

    }
    else if (tok == GRT_EQ_OP)
    {
        t2 = newExpNode(OP);
        t2->child[0] = t;
        t2->attrib.op = OP_GTE;
        t = t2;

        checkEOF();
        getNextToken();
        t->child[1] = relational_expression();

    }
    return t;
}

TreeNode* shift_expression()
{
    printf("frame: %s tok:%d tok_str:%s\n", __func__, getCurrentToken(), getTokenString()->str);
    TreeNode* t;
    TreeNode* t2;
    t = additive_expression();

    //white_spaces();
    int tok = getCurrentToken();
    if (tok == LEFT_OP)
    {
        t2 = newExpNode(OP);
        t2->child[0] = t;
        t2->attrib.op = OP_LSFT;
        t = t2;

        checkEOF();
        getNextToken();
        t->child[1] = shift_expression();
    }
    else if (tok == RIGHT_OP)
    {
        t2 = newExpNode(OP);
        t2->child[0] = t;
        t2->attrib.op = OP_RSFT;
        t = t2;

        checkEOF();
        getNextToken();
        t->child[1] = shift_expression();
    }
    return t;
}

TreeNode* additive_expression()
{
    printf("frame: %s tok:%d tok_str:%s\n", __func__, getCurrentToken(), getTokenString()->str);
    TreeNode* t;
    TreeNode* t2;
    t = multiplicative_expression();

    //white_spaces();
    int tok = getCurrentToken();
    if (tok == '+')
    {
        t2 = newExpNode(OP);
        t2->child[0] = t;
        t2->attrib.op = OP_PLUS;
        t = t2;

        checkEOF();
        getNextToken();
        t->child[1] = additive_expression();

    }
    else if (tok == '-')
    {
        t2 = newExpNode(OP);
        t2->child[0] = t;
        t2->attrib.op = OP_MINUS;
        t = t2;

        checkEOF();
        getNextToken();
        t->child[1] = additive_expression();
    }
    return t;
}

TreeNode* multiplicative_expression()
{
    printf("frame: %s tok:%d tok_str:%s\n", __func__, getCurrentToken(), getTokenString()->str);
    TreeNode* t;
    TreeNode* t2;
    t = unary_expression();

    int tok = getCurrentToken();
    if (tok == '*' || tok == '/' || tok == '%')
    {
        if (tok == '*')
        {
            t2 = newExpNode(OP);
            t2->child[0] = t;
            t2->attrib.op = OP_MULT;
            t = t2;

            checkEOF();
            getNextToken();
            t->child[1] = multiplicative_expression();
        }
        else if (tok == '/')
        {
            t2 = newExpNode(OP);
            t2->child[0] = t;
            t2->attrib.op = OP_DIV;
            t = t2;

            checkEOF();
            getNextToken();
            t->child[1] = multiplicative_expression();

        }
        else if (tok == '%')
        {
            t2 = newExpNode(OP);
            t2->child[0] = t;
            t2->attrib.op = OP_MOD;
            t = t2;

            checkEOF();
            getNextToken();
            t->child[1] = multiplicative_expression();
        }
    }
    return t;
}

TreeNode* unary_expression()
{
    printf("frame: %s tok:%d tok_str:%s tok_pos:%d\n", __func__, getCurrentToken(), getTokenString()->str, getTokenPos());
    TreeNode* t = NULL;
    TreeNode* t2 = NULL;
    //white_spaces();
    //  printf("%s:%d %d\n", __func__,__LINE__, getch());
    //   printf("%s:%d %c\n", __func__,__LINE__, getch());
    bool_t IsPost = False;
    bool_t IsPrim = False;
    int tok = getCurrentToken(), tok1, tok2;
    if (tok == '(')
    {
        //white_spaces();
        // fetch type name
        // check_type(token);
        //white_spaces();
        checkEOF();
        getNextToken();
        tok1 = getCurrentToken();
        if (tok1 == SIGNED ||
            tok1 == UNSIGNED ||
            tok1 == CONST
            )
        {
            checkEOF();
            getNextToken();
            tok2 = getCurrentToken();
            switch (tok2)
            {
            case INT:
                t2 = newExpNode(OP);
                if (tok1 == SIGNED)
                    t2->attrib.op = OP_SINT_CAST;
                else if (tok1 == UNSIGNED)
                    t2->attrib.op = OP_UINT_CAST;
                else if (tok1 == CONST)
                    t2->attrib.op = OP_CINT_CAST;
                //t2->child[1] = expression();
                break;
            case LONG:
                t2 = newExpNode(OP);

                if (tok1 == SIGNED)
                    t2->attrib.op = OP_SLONG_CAST;
                else if (tok1 == UNSIGNED)
                    t2->attrib.op = OP_ULONG_CAST;
                else if (tok1 == CONST)
                    t2->attrib.op = OP_CLONG_CAST;
                //t2->child[1] = expression();
                break;
            case SHORT:
                t2 = newExpNode(OP);
                if (tok1 == SIGNED)
                    t2->attrib.op = OP_SSHORT_CAST;
                else if (tok1 == UNSIGNED)
                    t2->attrib.op = OP_USHORT_CAST;
                else if (tok1 == CONST)
                    t2->attrib.op = OP_CSHORT_CAST;
                //t2->child[1] = expression();
                break;

            case CHAR:
                t2 = newExpNode(OP);
                if (tok1 == SIGNED)
                    t2->attrib.op = OP_SSHORT_CAST;
                else if (tok1 == UNSIGNED)
                    t2->attrib.op = OP_USHORT_CAST;
                else if (tok1 == CONST)
                    t2->attrib.op = OP_CSHORT_CAST;
                //t2->child[1] = expression();
                break;
            default:
                IsParseFailed(__func__, __LINE__);
                break;


            }
            getNextToken();

        }
        else if (tok1 == INT ||
            tok1 == FLOAT ||
            tok1 == LONG ||
            tok1 == DOUBLE ||
            tok1 == SHORT ||
            tok1 == CHAR
            )
        {

            switch (tok1)
            {
            case INT:
                t2 = newExpNode(OP);
                t2->attrib.op = OP_SINT_CAST;
                checkEOF();
                //t2->child[1] = expression();
                break;

            case FLOAT:
                t2 = newExpNode(OP);
                t2->attrib.op = OP_FLOAT_CAST;
                checkEOF();
                //t2->child[1] = expression();

                break;

            case LONG:
                t2 = newExpNode(OP);
                t2->attrib.op = OP_SLONG_CAST;
                checkEOF();
                //t2->child[1] = expression();
                break;

            case DOUBLE:
                t2 = newExpNode(OP);
                t2->attrib.op = OP_DOUBLE_CAST;
                checkEOF();
                //t2->child[1] = expression();

                break;

            case SHORT:
                t2 = newExpNode(OP);
                t2->attrib.op = OP_SSHORT_CAST;
                checkEOF();
                //t2->child[1] = expression();
                break;

            case CHAR:
                t2 = newExpNode(OP);
                t2->attrib.op = OP_CHAR_CAST;
                checkEOF();
                //t2->child[1] = expression();
                break;
            }

            getNextToken();
        }
        else
        {

            t = argument_expression_list();
            if (t != NULL)
            {
                tok = getCurrentToken();
                if (tok != ')')
                {
                    printf("error: expected ')' !\n");
                    _exit(0);
                }
                return t;
            }
            else
            {
                printf("error: expected cast operator, not:%s !", getTokenString());
                _exit(0);
            }

        }

        tok = getCurrentToken();
        if (tok != ')')
        {
            printf("error: expected ')' !\n");
            _exit(0);
        }
        else
        {   
            getNextToken();
            if (t2 != NULL)
            {
                TreeNode *temp = primary_expression(&IsPrim);
                if (IsPrim == True)
                    t2->child[0] = temp;
                else
                    t2->child[0] = expression();
            }
        }
        return t2;

    }
    else if (tok == INC_OP)
    {
        t2 = newExpNode(OP);
        t2->attrib.op = OP_PREINCR;
        t = t2;
        checkEOF();
        getNextToken();
        if ((t->child[0] = expression()) == NULL)
        {
            printf("error: No postfix id found !\n");
            IsParseFailed(__func__, __LINE__);
        }
        t->child[1] = NULL;
        return t;
    }
    if (tok == DEC_OP)
    {
        t2 = newExpNode(OP);
        t2->attrib.op = OP_PREDECR;
        t = t2;
        t->child[1] = NULL;
        checkEOF();
        getNextToken();
        if ((t->child[0] = expression()) == NULL)
        {
            printf("error: No postfix id found !\n");
            IsParseFailed(__func__, __LINE__);
        }

        return t;

    }
    else if (tok == '+')
    {
        t2 = newExpNode(OP);
        t2->attrib.op = OP_UPLUS;
        t = t2;
        checkEOF();
        getNextToken();

        if ((t2->child[0] = expression()) == NULL)
        {
            printf("error: No postfix id found , tok:%d!\n", tok);
            IsParseFailed(__func__, __LINE__);
        }

        t->child[1] = NULL;
        return t;
    }
    else  if (tok == '-')
    {
        t2 = newExpNode(OP);
        t2->attrib.op = OP_UMINUS;
        t = t2;
        checkEOF();
        getNextToken();
        if ((t2->child[0] = expression()) == NULL)
        {
            printf("error: No postfix id found !\n");
            IsParseFailed(__func__, __LINE__);
        }

        t->child[1] = NULL;
        return t;

    }
    else if (tok == '&')
    {
        t2 = newExpNode(OP);
        t2->attrib.op = OP_ADDROF;
        t = t2;
        checkEOF();
        getNextToken();
        if ((t2->child[0] = expression()) == NULL)
        {
            printf("error: No postfix id found !\n");
            IsParseFailed(__func__, __LINE__);
        }

        t->child[1] = NULL;
        return t;

    }
    else if (tok == '*')
    {
        t2 = newExpNode(OP);
        t2->attrib.op = OP_DEREF;
        t = t2;
        checkEOF();
        getNextToken();
        if ((t2->child[0] = expression()) == NULL)
        {
            printf("error: No postfix id found !\n");
            IsParseFailed(__func__, __LINE__);
        }

        t->child[1] = NULL;
        return t;

    }
    else if (tok == '~')
    {
        t2 = newExpNode(OP);
        t2->attrib.op = OP_COMPL;
        t = t2;

        checkEOF();
        getNextToken();
        if ((t2->child[0] = expression()) == NULL)
        {
            printf("error: No postfix id found !\n");
            IsParseFailed(__func__, __LINE__);
        }

        t->child[1] = NULL;
        return t;

    }
    else if (tok == '!')
    {
        //          printf("%s:%d %c\n", __func__,__LINE__, getch());
        //exp=cast_expression(); 
        t2 = newExpNode(OP);
        t2->attrib.op = OP_NOT;
        t = t2;
        checkEOF();
        getNextToken();
        if ((t2->child[0] = expression()) == NULL)
        {
            printf("error: No postfix id found !\n");
            IsParseFailed(__func__, __LINE__);
        }

        t->child[1] = NULL;
        return t;

    }
    else if (tok == SIZEOF)
    {

        t2 = newExpNode(OP);
        t2->attrib.op = OP_SZ;
        t = t2;
        checkEOF();
        getNextToken();
        if (tok == '(')
        {
            //type_specifiers();
            checkEOF();
            getNextToken();
            t->child[0] = expression();
            //white_spaces();
            tok = getCurrentToken();
            if (tok != ')')
            {
                printf("error: sizeof doesn't have ')' !");
                IsParseFailed(__func__, __LINE__);
            }
            return t;
        }
        else
        {
            checkEOF();
            if ((t->child[0] = expression()) == NULL)
            {
                printf("error: sizeof doesn't have operand !");
                _exit(0);
            }
            return t;
        }

    }
    else
    {
        // printf("frame: %s tok:%d tok_str:%s\n", __func__, getCurrentToken(), getTokenString()->str);
        //checkEOF();
        t = postfix_expression(&IsPost, &IsPrim);
        if (IsPost == True || IsPrim == True)
            return t;
        else
            return NULL;
    }
    //  printf("%s:%d %c\n", __func__,__LINE__, getch());
    return t;
}

/* 
postfix_expression_dash:
      '[' expression ']' postfix_expression_dash
    | '(' argument_expression_list ')' postfix_expression_dash
    | '.' IDENTIFIER postfix_expression_dash
    | '->' IDENTIFIER postfix_expression_dash
    | INC_OP postfix_expression_dash
    | DEC_OP postfix_expression_dash

postfix_expression
    : primary_expression
    | primary_expression '[' expression ']' postfix_expression_dash
    | primary_expression(' argument_expression_list ')' postfix_expression_dash
    | primary_expression '.' IDENTIFIER postfix_expression_dash
    | primary_expression '->' IDENTIFIER postfix_expression_dash
    | primary_expression INC_OP postfix_expression_dash
    | primary_expression DEC_OP postfix_expression_dash
*/

TreeNode* postfix_expression_dash(bool_t *IsPost, OpTokenType parentNodeType)
{
    TreeNode* t2 = NULL,* t=NULL;
    int tok = getCurrentToken();
    if (tok == '[')
    {
            *IsPost = True;
            t2 = newExpNode(OP);
            
            //  if(t->attrib.op == INT )
            t2->attrib.op = OP_ARRAY;
            checkEOF();
            getNextToken();
            t2->child[0] = expression();
            //white_spaces();
            if ((tok = getCurrentToken()) != ']')
            {
                IsParseFailed(__func__, __LINE__);
            }
            t2->child[1] = postfix_expression_dash(IsPost, OP_ARRAY);
            return t2;
            
     }
     else if (tok == '(') // Function caller parsing for expr like f(1)+f(3)
        {
            *IsPost = True;
            t2 = newExpNode(CALLEXP);
            // modification may be required for attrib
            checkEOF();
            getNextToken();
            t2->child[0] = argument_expression_list(); // Need to modify in future
            t2->child[1] = postfix_expression_dash(IsPost, OP_CALLER);
            return t2;
        }
     else if(tok == '.')
        {
            *IsPost = True;
            t2 = newExpNode(OP);
            t2->attrib.op = OP_DOT;
            checkEOF();
            getNextToken();
            if(tok == ID)
			{
                t2->child[0] = newExpNode(IDEN);
                t2->child[1] = postfix_expression_dash(IsPost, OP_DOT);
				return t2;
			}
			else
			{
				IsParseFailed(__func__, __LINE__);
			}
            

        }
      else if(tok == MEM_ARROW)
        {
            *IsPost = True;
            t2 = newExpNode(OP);
            t2->attrib.op = OP_ARROW_PTR;
            checkEOF();
            getNextToken();
            
            if (tok == ID)
            {
				t2->child[0] = newExpNode(IDEN);
                t2->child[1] = postfix_expression_dash(IsPost, OP_ARROW_PTR);
                return t2;
            }
            else
            {
                IsParseFailed(__func__, __LINE__);
            }
            

        } 
      else if (tok == INC_OP)
        {
            if (parentNodeType == OP_POSTINCR || parentNodeType == OP_POSTDECR)
		    {
		       printf("error: two consecutive post increment or decrement operators are not allowed !\n");
			   IsParseFailed(__func__, __LINE__);
               _exit(0);
            }
            *IsPost = True;
            t2 = newExpNode(OP);
            
            t2->attrib.op = OP_POSTINCR;
            // checkEOF(); don't fail on EOF for post increment and decrement as it can be last token in the file
            t2->child[0] = NULL;
            if (lookahead() == EOF)
                return t2;
            getNextToken();
            t2->child[1] = postfix_expression_dash(IsPost, OP_POSTINCR);

        }
      else if (tok == DEC_OP)
        {
            if (parentNodeType == OP_POSTINCR || parentNodeType == OP_POSTDECR)
            {
               printf("error: two consecutive post increment or decrement operators are not allowed !\n");
               IsParseFailed(__func__, __LINE__);
               _exit(0);
            }
            *IsPost = True;
            t2 = newExpNode(OP);
            t2->attrib.op = OP_POSTDECR;
            // checkEOF(); don't fail on EOF for post increment and decrement as it can be last token in the file
            t2->child[0] = NULL;
            if (lookahead() == EOF)
                return t2;
            getNextToken();
            t2->child[1] = postfix_expression_dash(IsPost, OP_POSTDECR);
        }
    return t2;

}

TreeNode* postfix_expression(bool_t* IsPost, bool_t* IsPrim_g)
{
    printf("frame: %s tok:%d tok_str:%s\n", __func__, getCurrentToken(), getTokenString()->str);
    //white_spaces();
    TreeNode* t;
    TreeNode* t2;
    int tok;
    bool_t IsPrim = False;
    t = primary_expression(&IsPrim);
	*IsPrim_g = IsPrim;

    //white_spaces();
    //   return TRUE; 
        tok = getCurrentToken();
        if (tok == '[')
        {
            *IsPost = True;
            t2 = newExpNode(OP);
            t2->child[0] = t;
            //  if(t->attrib.op == INT )
            t2->attrib.op = OP_ARRAY;
            checkEOF();
            getNextToken();
            t2->child[1] = expression();
            //white_spaces();
            if ((tok = getCurrentToken()) != ']')
            {
                IsParseFailed(__func__, __LINE__);
            }
			t2->child[2] = postfix_expression_dash(IsPost, OP_ARRAY);
            return t2;
        }
        else if (tok == '(') // Function caller parsing for expr like f(1)+f(3)
        {
            *IsPost = True;
            t2 = newExpNode(CALLEXP);
            t2->child[0] = t;
            t2->attrib.op = OP_CALLER;
            // modification may be required for attrib
            checkEOF();
            getNextToken();
            t2->child[1] = argument_expression_list(); // Need to modify in future
			t2->child[2] = postfix_expression_dash(IsPost, OP_CALLER);
            return t2;
        }
        else if (tok == '.')
        {
            *IsPost = True;
            t2 = newExpNode(OP);
            t2->child[0] = t;
            t2->attrib.op = OP_DOT;
            checkEOF();
            getNextToken();
            if (tok == ID)
            {
                t2->child[1] = newExpNode(IDEN);
                t2->child[2] = postfix_expression_dash(IsPost, OP_DOT);
                return t2;
            }
            else
            {
                IsParseFailed(__func__, __LINE__);
            }


        }
        else if (tok == MEM_ARROW)
        {
            *IsPost = True;
            t2 = newExpNode(OP);
            t2->child[0] = t;
            t2->attrib.op = OP_ARROW_PTR;
            checkEOF();
            getNextToken();

            if (tok == ID)
            {
                t2->child[1] = newExpNode(IDEN);
                t2->child[2] = postfix_expression_dash(IsPost, OP_ARROW_PTR);
                return t2;
            }
            else
            {
                IsParseFailed(__func__, __LINE__);
            }


        }
        else if (tok == INC_OP)
        {
            *IsPost = True;
            t2 = newExpNode(OP);
            t2->child[0] = t;
            t2->attrib.op = OP_POSTINCR;
            // checkEOF(); don't fail on EOF for post increment and decrement as it can be last token in the file
            t2->child[1] = NULL;
            if (lookahead() == EOF)
                return t2;
            getNextToken();
            t2->child[2] = postfix_expression_dash(IsPost, OP_POSTINCR);
            return t2;

        }
        else if (tok == DEC_OP)
        {
            *IsPost = True;
            t2 = newExpNode(OP);
            t2->child[0] = t;
            t2->attrib.op = OP_POSTDECR;
            // checkEOF(); don't fail on EOF for post increment and decrement as it can be last token in the file
            t2->child[1] = NULL;
            if (lookahead() == EOF)
                return t2;
            getNextToken();
            t2->child[2] = postfix_expression_dash(IsPost, OP_POSTDECR);
            return t2;
        }
        else if (IsPrim == True)
        {
            
                return t;
        }
    
    return NULL;
}

TreeNode* argument_expression_list()
{
    printf("frame: %s tok:%d tok_str:%s\n", __func__, getCurrentToken(), getTokenString()->str);
    TreeNode* t;
    TreeNode* t2;
    t = assignment_expression();
    //white_spaces();
    int tok = getCurrentToken();
    if (tok == ',')
    {
        getNextToken();
        t2 = argument_expression_list();

    }
    return t;
}

/**** 'Made in India' by Daipayan *****/
