#include "tree.h"

struct symbol_table {
    bool_t IsCompositeType; 
    CompositeType c;
    struct symbol_table* Composite_members[48];
    ScopeType scopeType;
    string_t scope_name;
    DclType simple_type;
    int start_scope_line;
    int end_scope_line;
    DclType valueType;
    string_t StoreValue;
} Symbol;