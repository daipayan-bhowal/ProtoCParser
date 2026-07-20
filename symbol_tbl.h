#include "tree.h"
#pragma once
typedef int symbol_index;
#define MAX_NO_OF_SYMB 4096
#define NO_PARENT -255
#define EMPTY 0
#define FILLED 1
typedef struct symbol_table {
    char filled;
    CompositeType compType;
    symbol_index Composite_members[48];
    ScopeType scopeType;
    string_t scope_name;
    DclType simple_type;
    string_t var_name;
    int start_scope_line;
    int end_scope_line;
    DclType initValueType;
    string_t StoreValue;
    symbol_index parent_index;
} Symbol;



Symbol* insert_entry(CompositeType c, ScopeType scopeType, string_t scope_name, DclType simple_type, string_t var_name, DclType initValueType, string_t StoreValue);

void setOrGetCurrentSymbolIndex(bool_t SetElseGet, symbol_index* i);

bool_t update_parent_index(Symbol* s, symbol_index parent);

Symbol* struct_or_union_start(bool_t* IsStruct, bool_t* IsUnion, bool_t* IsDecl, bool_t* IsTypedef);

bool_t declaration_specifiers(Symbol* s);

Symbol* getSymbol_byindex(symbol_index indx);