#include "tree.h"
#define _CRT_SECURE_NO_WARNINGS
#include "symbol_tbl.h"
/*
struct_declarator
    : declarator
    | ':' constant_expression
    | declarator ':' constant_expression
*/
void struct_declarator(string_t scope, symbol_index parent)
{
    ComplxNode* ctree;
    TreeNode* t;
    int tok = getCurrentToken();
    if (tok == ':')
    {
        t=constant_expression();
    }
    else
    {
        ctree = declarator();
        insert_entry(NotAComposite, STRUCT_MEM, scope, ctree->categ.return_type, ctree->categ.Identifier, NotADclType, string_const(""));
        tok = getCurrentToken();
        if (tok == ':')
        {
            t=constant_expression();
        }
    }
    // scope can be a structure/union, functional(local), global
}
/*
struct_declarator_list
    : struct_declarator
    | struct_declarator ',' struct_declarator_list 
    ;
*/

void struct_declarator_list(string_t scope, symbol_index parent)
{
    struct_declarator(scope, parent);
    int tok = getCurrentToken();
    if (tok == ',')
    {
        struct_declarator_list(scope, parent);
    }
}
/*
struct_declaration
    : specifier_qualifier_list struct_declarator_list ';'
    ;
*/

void struct_declaration(string_t scope, symbol_index parent)
{

    specifier_qualifier_list();
    struct_declarator_list(scope, parent);
}
/*
struct_member_declaration_list
    : struct_declaration '}'
    | struct_declaration ';' struct_member_declaration_list
    ;
*/

void struct_member_declaration_list(bool_t* isStruct, string_t scope, symbol_index parent)
{
    bool_t isLocalStruct = False;
    int tok = getCurrentToken();
    Symbol* s;
    struct_declaration(scope, parent);
    tok = getCurrentToken();
    if (tok == ';')
    {
        getNextToken();
        struct_member_declaration_list(isStruct, scope, parent);
    }
    else if (tok == '}')
    {
            
            tok = getNextToken();
            if (tok == ID)
            {
                s = getSymbol_byindex(parent);
                if (s->scopeType == GLOBAL)
                    s->var_name = getTokenString();
            }
            else if(tok == ';')
            {
                *isStruct = True;
                return;
            }
            else
            {
                printf("error: expected identifier or ';' \n");
                *isStruct = False;
                return;
            }

    }
    else
    {
        printf("error: expected '}' ");
        *isStruct = False;
        return;
    }
}
/*
struct_or_union_specifier
    : struct_or_union IDENTIFIER '{' struct_member_declaration_list
    | struct_or_union '{' struct_member_declaration_list 
    | struct_or_union IDENTIFIER
    ;
 */
Symbol* struct_or_union_start(bool_t *IsStruct, bool_t *IsUnion, bool_t *IsDecl, bool_t *IsTypedef)
{
    int tok = getCurrentToken();
    string_t scope = string_const("@Global");
    string_t scope2 = string_const("@Global");
    symbol_index si;
    Symbol* s = NULL;
    if (tok == STRUCT)
    {
        tok = getNextToken();
        if (tok == ID)
        {
            scope = getTokenString();
            tok = getNextToken();
            if (tok == '{')
            {
                s = insert_entry(Struct, GLOBAL, scope, NotADclType, scope, NotADclType, string_const(""));
                setOrGetCurrentSymbolIndex(False, &si);
                struct_member_declaration_list(IsStruct, scope, si);

            }
            else if(tok == ';')
            {
                *IsDecl = True;
                return insert_entry(Struct, GLOBAL, scope, StructDcl, scope, NotADclType, string_const(""));
            }
            else if (tok == ID)
            {
                scope = getTokenString();
                tok = getNextToken();
                if (tok == ';')
                {
                    *IsDecl = True;
                    return insert_entry(Struct, GLOBAL, scope, StructDcl, scope, NotADclType, string_const(""));
                }
                else
                {
                    printf("error: invalid character %s", scope->str);
                    *IsStruct = False;
                    return NULL;
                }
                
            }
            else
            {
                printf("error: invalid character %s", getTokenString()->str);
                *IsStruct = False;
                return NULL;
            }
        }
        else if (tok == '{')
        {
            setOrGetCurrentSymbolIndex(False, &si);
            struct_member_declaration_list(IsStruct, scope, si);
            *IsDecl = False;
        }
    }
    else if (tok == UNION)
    {
        tok = getNextToken();
        if (tok == ID)
        {
            scope = getTokenString();
            s = insert_entry(Union, GLOBAL, scope, NotADclType, scope, NotADclType, string_const(""));
            tok = getNextToken();
            if (tok == '{')
            {
                setOrGetCurrentSymbolIndex(False, &si);
                struct_member_declaration_list(IsUnion, scope, si);
            }
            else
            {
                *IsDecl = True;
                return (s=insert_entry(Union, GLOBAL, scope, NotADclType, scope, NotADclType, string_const("")));
            }
        }
        else if (tok == '{')
        {
            setOrGetCurrentSymbolIndex(False, &si);
            struct_member_declaration_list(IsUnion, scope, si);
        }

    }
    return s;

}

void enumerator(symbol_index parent, int value)
{
    TreeNode* t;
    int tok = getCurrentToken();
    string_t tok_name;
    Symbol* s;
    char v[20];
    _itoa_s(value, v, 10);
    string_t enum_value = string_const(v);
    if (tok == ID)
    {
        tok_name = getTokenString();
        s = insert_entry(NotAComposite, GLOBAL, string_const("Global"), Int, tok_name, Int, enum_value);
        update_parent_index(s, parent);
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

void enumerator_list(symbol_index parent)
{
    static int value = 0;
    enumerator(parent, value++);
    int tok = getCurrentToken();
    if (tok == ',')
    {
        enumerator_list(parent);
    }
    value = 0;
}

void enum_start()
{
    int tok = getCurrentToken();
    string_t tok_name = string_const("@Global");
    symbol_index si;
    Symbol* s;
    if (tok == ENUM)
    {
        tok = getNextToken();
        if (tok == '{')
        {
            s = insert_entry(Enum, GLOBAL, string_const("@Global"), NotADclType, tok_name, NotADclType, string_const(""));
            setOrGetCurrentSymbolIndex(False,&si);
            enumerator_list(si);
        }
        else
        {
            if (tok == ID)
            {
                tok_name = getTokenString();

                if (tok == '{')
                {
                    s = insert_entry(Enum, GLOBAL, string_const("Global"), NotADclType, tok_name, NotADclType, string_const(""));
                    setOrGetCurrentSymbolIndex(False, &si);
                    enumerator_list(si);
                }
                else
                {
                    s = insert_entry(Enum, GLOBAL, string_const("Global"), EnumDcl, tok_name, NotADclType, string_const(""));
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