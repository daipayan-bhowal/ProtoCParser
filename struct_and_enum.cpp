#include "tree.h"

void struct_declarator(string_t scope)
{
    ComplxNode* ctree;
    TreeNode* t;
    if (tok == ':')
    {
        t=constant_expression();
    }
    else
    {
        ctree = declarator();
        if (tok == ':')
        {
            t=constant_expression();
        }
    }
    // scope can be a structure/union, functional(local), global
}


void struct_declarator_list(string_t scope)
{
    struct_declarator(scope);
    if (tok == ',')
    {
        struct_declarator_list(scope);
    }
}

void struct_declaration(bool_t *IsStruct, string_t scope)
{

    specifier_qualifier_list();
    struct_declarator_list(scope);
    int tok = getCurrentToken();
    if (tok != ';')
    {
        *IsStruct = False;
    }
    else
    {

    }
}


void struct_member_declaration_list(bool_t* isStruct, string_t scope)
{
    bool_t isLocalStruct = False;
    while (tok != '}')
    {
        struct_declaration(&isLocalStruct, scope);
        if (isLocalStruct == False)
        {

        }
    }
    if (tok == '}' && isLocalStruct == True)
    {

    }
    else
    {
        printf("error: ");
    }
}


void struct_or_union_start(bool_t *IsStruct, bool_t *IsEnum, bool_t *IsDecl)
{
    if (tok == STRUCT)
    {
        if (tok == ID)
        {
            scope = getTokenString();
            if (tok == '{')
            {
                struct_member_declaration_list(&isStruct, scope);
            }
            else
            {
                *IsDecl = True;
                return;
            }
        }
        else if (tok == '{')
        {
            struct_member_declaration_list(&isStruct);
            *IsDecl = False;
        }
    }
    else if (tok == UNION)
    {
        if (tok == ID)
        {
            if (tok == '{')
            {
                struct_member_declaration_list();
            }
            else
            {
                return;
            }
        }
        else if (tok == '{')
        {
            struct_member_declaration_list();
        }

    }
    return;

}

void enumerator()
{
    TreeNode* t;
    if (tok == ID)
    {
        if (tok == '=')
        {
            t=constant_expression();
        }
    }
    else
    {
        printf("error: Identifier exprected !\n");
        exit(0);
    }
}

void enumerator_list()
{
    enumerator();
    if (tok == ',')
    {
        enumerator_list();
    }
}

void enum_start()
{
    if (tok == ENUM)
    {
        if (tok == '{')
        {
            enumerator_list();
        }
        else
        {
            if (tok == ID)
            {
                if (tok == '{')
                {
                    enumerator_list();
                }
                else
                {
                    return;
                }
            }
            else
            {
                printf("error: Identifier exprected !\n");
                exit(0);
            }
        }
    }
}