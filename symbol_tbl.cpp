#include "symbol_tbl.h"
Symbol symb_tbl[MAX_NO_OF_SYMB];

void init()
{
    int symb_index;
    for (symb_index = 0; symb_index < MAX_NO_OF_SYMB; symb_index++)
    {
        symb_tbl[symb_index].filled = EMPTY;
    }

}

void fill(Symbol* s, CompositeType c, ScopeType scopeType, string_t scope_or_composite_name, DclType simple_type, string_t var_name, DclType initValueType, string_t StoreValue)
{ 
    if (s != NULL)
    {
        s->compType = c;
        s->scopeType = scopeType;
        s->scope_name = scope_or_composite_name;
        s->simple_type = simple_type;
        s->var_name = var_name;
        s->initValueType = initValueType;
        s->StoreValue = StoreValue;
    }
    else
    {
        printf("error: Symbol table ptr is NULL, cannot be filled !\n");
        exit(0);
    }


}
Symbol* insert_entry_from_front(symbol_index* updateIndx, CompositeType c, ScopeType scopeType, string_t scope_name, DclType simple_type, string_t var_name, DclType initValueType, string_t StoreValue)
{
    symbol_index insertion_symb_index = 0;
    while (insertion_symb_index < MAX_NO_OF_SYMB)
    {
        if (symb_tbl[insertion_symb_index].filled == EMPTY)
        {
            *updateIndx = insertion_symb_index;
            break;
        }
        insertion_symb_index++;
    }
    if (insertion_symb_index >= MAX_NO_OF_SYMB)
    {
        printf("error: Symbol table size has reached it's limit\n");
        exit(0);
    }
    fill(&symb_tbl[insertion_symb_index], c, scopeType, scope_name, simple_type, var_name, initValueType, StoreValue);

    return &symb_tbl[insertion_symb_index];
}
Symbol* insert_entry(CompositeType c, ScopeType scopeType, string_t scope_name, DclType simple_type, string_t var_name, DclType initValueType, string_t StoreValue)
{
    static symbol_index insertion_symb_index = 0;
    int tempIndex;
    Symbol* front = NULL;
    while (insertion_symb_index < MAX_NO_OF_SYMB)
    {
        if (symb_tbl[insertion_symb_index].filled == EMPTY)
        {
            break;
        }
        insertion_symb_index++;
    }
    /* If index has crossed the max limit check empty entries from start of the table and store the new index */
    if (insertion_symb_index >= MAX_NO_OF_SYMB)
    {
        front = insert_entry_from_front(&tempIndex, c, scopeType, scope_name, simple_type, var_name, initValueType, StoreValue);
        insertion_symb_index = tempIndex; // store & update the new index
        return front;
    }
    setOrGetCurrentSymbolIndex(True,&insertion_symb_index);
    return &symb_tbl[insertion_symb_index];
}

void setOrGetCurrentSymbolIndex(bool_t SetElseGet,symbol_index *i)
{
    static symbol_index insertion_symb_index;
    if (SetElseGet == True)
    {
        insertion_symb_index = *i;
    }
    else
    {
        *i = insertion_symb_index;
    }
}



Symbol* find_byname(string_t symbolname)
{
    symbol_index symb_index;
    for (symb_index = 0; symb_index < MAX_NO_OF_SYMB; symb_index++)
    {
        if (compare(symb_tbl[symb_index].var_name,symbolname) == 0)
        {
            return &symb_tbl[symb_index];
        }
    }
    return NULL;
}

Symbol* getSymbol_byindex(symbol_index indx)
{
    return &symb_tbl[indx];
}

bool_t update_scope_lines(Symbol *s,int start,int end)
{
    bool_t ret = False;
    if (start != NA)
    {
        s->start_scope_line = start;
        ret = True;
    }
    if (end != NA)
    {
        s->end_scope_line = end;
        ret = True;
    }
    return ret;
}
bool_t update_parent_index(Symbol* s, symbol_index parent)
{
    bool_t ret = False;
    if (parent < 0 || ret >= MAX_NO_OF_SYMB)
        return ret;
    s->parent_index = parent;
    return True;
}

symbol_index find_index_byname(string_t symbolname)
{
    symbol_index symb_index;
    for (symb_index = 0; symb_index < MAX_NO_OF_SYMB; symb_index++)
    {
        if (compare(symb_tbl[symb_index].var_name, symbolname) == 0)
        {
            return symb_index;
        }
    }
    return EMPTY;
}

void clean_all_entry()
{
    symbol_index symb_index;
    for (symb_index = 0; symb_index < MAX_NO_OF_SYMB; symb_index++)
    {
            symb_tbl[symb_index].filled = EMPTY;
            symb_tbl[symb_index].scopeType = (ScopeType)NA;
            symb_tbl[symb_index].scope_name = string_const("");
            symb_tbl[symb_index].simple_type = (DclType)NA;
            symb_tbl[symb_index].initValueType = (DclType)NA;

    }
}

bool_t remove_entry(Symbol* s)
{
    symbol_index symb_index;
    for (symb_index = 0; symb_index < MAX_NO_OF_SYMB; symb_index++)
    {
        if (&symb_tbl[symb_index] == s)
        {
            symb_tbl[symb_index].filled = EMPTY;
            symb_tbl[symb_index].scopeType = (ScopeType)NA;
            symb_tbl[symb_index].scope_name = string_const("");
            symb_tbl[symb_index].simple_type = (DclType)NA;
            symb_tbl[symb_index].initValueType = (DclType)NA;
            return True;
        }
    }

    return False;
}