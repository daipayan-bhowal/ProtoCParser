# Proto C Parser
This is a simple C parser for expression as grammer in the book "The C Programming Language" by Brian W. Kernighan and Dennis M. Ritchie.

grammer is defined in expr.cpp

Original C grammer is:-

primary-expression:
    identifier
    constant
    string-literal
    ( expression )


postfix-expression:
    primary-expression
    postfix-expression [ expression ]
    postfix-expression ( argument-expression-list_opt )
    postfix-expression . identifier
    postfix-expression -> identifier
    postfix-expression ++
    postfix-expression --


unary-expression:
    postfix-expression
    ++ unary-expression
    -- unary-expression
    unary-operator cast-expression
    sizeof unary-expression
    sizeof ( type-name )

unary-operator: one of
    &  *  +  -  ~  !

cast-expression:
    unary-expression
    ( type-name ) cast-expression

multiplicative-expression:
    cast-expression
    multiplicative-expression * cast-expression
    multiplicative-expression / cast-expression
    multiplicative-expression % cast-expression

additive-expression:
    multiplicative-expression
    additive-expression + multiplicative-expression
    additive-expression - multiplicative-expression

shift-expression:
    additive-expression
    shift-expression << additive-expression
    shift-expression >> additive-expression

relational-expression:
    shift-expression
    relational-expression < shift-expression
    relational-expression > shift-expression
    relational-expression <= shift-expression
    relational-expression >= shift-expression

equality-expression:
    relational-expression
    equality-expression == relational-expression
    equality-expression != relational-expression

AND-expression:
    equality-expression
    AND-expression & equality-expression

exclusive-OR-expression:
    AND-expression
    exclusive-OR-expression ^ AND-expression

inclusive-OR-expression:
    exclusive-OR-expression
    inclusive-OR-expression | exclusive-OR-expression

logical-AND-expression:
    inclusive-OR-expression
    logical-AND-expression && inclusive-OR-expression

logical-OR-expression:
    logical-AND-expression
    logical-OR-expression || logical-AND-expression

conditional-expression:
    logical-OR-expression
    logical-OR-expression ? expression : conditional-expression

assignment-expression:
    conditional-expression
    unary-expression assignment-operator assignment-expression

assignment-operator: one of
    =  *=  /=  %=  +=  -=  <<=  >>=  &=  ^=  |=

expression:
    assignment-expression
    expression , assignment-expression





Modified Grammer in our parser is:-

primary-expression:
    identifier
    constant
    string-literal
    ( expression )

postfix-expression: 
    primary-expression
    primary-expression [ expression ]
    primary-expression ( argument-expression-list_opt )
    primary-expression . identifier
    primary-expression -> identifier
    primary-expression ++
    primary-expression --

unary-expression:
    postfix-expression
    ++ expression
    -- expression
    unary-operator expression
    ( type-name ) expression // cast-expression merged into unary-expression due to cyclic reference
    sizeof expression
    sizeof ( type-name )

multiplicative-expression:
    unary-expression
    unary-expression * multiplicative-expression
    unary-expression / multiplicative-expression
    unary-expression % multiplicative-expression

additive-expression:
    multiplicative-expression
    multiplicative-expression + additive-expression 
    multiplicative-expression - additive-expression

shift-expression:
    additive-expression
    additive-expression << shift-expression 
    additive-expression >> shift-expression

relational-expression:
    shift-expression
    shift-expression < relational-expression
    shift-expression > relational-expression
    shift-expression <= relational-expression
    shift-expression >= relational-expression

equality-expression:
    relational-expression
    relational-expression == equality-expression 
    relational-expression != equality-expression

AND-expression:
    equality-expression
    equality-expression & AND-expression

exclusive-OR-expression:
    AND-expression
    AND-expression ^ exclusive-OR-expression

inclusive-OR-expression:
    exclusive-OR-expression
    exclusive-OR-expression | inclusive-OR-expression

logical-AND-expression:
    inclusive-OR-expression
    inclusive-OR-expression && logical-AND-expression

logical-OR-expression:
    logical-AND-expression
    logical-AND-expression || logical-OR-expression

conditional-expression:
    logical-OR-expression
    logical-OR-expression ? expression : conditional-expression

assignment-expression:
    conditional-expression
    conditional-expression assignment-operator assignment-expression

assignment-operator: one of
    =  *=  /=  %=  +=  -=  <<=  >>=  &=  ^=  |=

expression:
    assignment-expression , expression
