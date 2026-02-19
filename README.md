# Proto C Parser
This is a simple handwritten C parser for expression as grammer given in the book "The C Programming Language" by Brian W. Kernighan and Dennis M. Ritchie,<br>
and also coding style is inspired from "Compiler Construction: Principles and Practice" by Kenneth C. Louden.

## Prerequisite
Visual Studio 2022 Community 

## Lexer 

Lexer for C is adopted from my DELex repo: 
https://github.com/daipayan-bhowal/DELex

## Usage

create a file in C:\Mywork\prat.c<br>
prat.c will have only 1 line of C expression, similarly create prat2.c, prat3.c & prat4.c<br>

prat.c
-------
x / y % z + y

   

grammer is defined in expr.cpp

## Original C grammer is:-

primary-expression:<br>
    identifier\
    constant\
    string-literal\
    ( expression )<br>


postfix-expression:<br>
    primary-expression\
    postfix-expression [ expression ]\
    postfix-expression ( argument-expression-list_opt )\
    postfix-expression . identifier\
    postfix-expression -> identifier\
    postfix-expression ++\
    postfix-expression --<br>


unary-expression:<br>
    postfix-expression\
    ++ unary-expression\
    -- unary-expression\
    unary-operator cast-expression\
    sizeof unary-expression\
    sizeof ( type-name )<br>

unary-operator: one of<br>
    &  *  +  -  ~  !

cast-expression:<br>
    unary-expression\
    ( type-name ) cast-expression\

multiplicative-expression:<br>
    cast-expression\
    multiplicative-expression * cast-expression\
    multiplicative-expression / cast-expression\
    multiplicative-expression % cast-expression<br>

additive-expression:<br>
    multiplicative-expression\
    additive-expression + multiplicative-expression\
    additive-expression - multiplicative-expression<br>

shift-expression:<br>
    additive-expression\
    shift-expression << additive-expression\
    shift-expression >> additive-expression<br>

relational-expression:<br>
    shift-expression\
    relational-expression < shift-expression\
    relational-expression > shift-expression\
    relational-expression <= shift-expression\
    relational-expression >= shift-expression<br>

equality-expression:<br>
    relational-expression\
    equality-expression == relational-expression\
    equality-expression != relational-expression<br>

AND-expression:<br>
    equality-expression\
    AND-expression & equality-expression<br>

exclusive-OR-expression:<br>
    AND-expression\
    exclusive-OR-expression ^ AND-expression<br>

inclusive-OR-expression:<br>
    exclusive-OR-expression\
    inclusive-OR-expression | exclusive-OR-expression<br>

logical-AND-expression:<br>
    inclusive-OR-expression\
    logical-AND-expression && inclusive-OR-expression<br>

logical-OR-expression:<br>
    logical-AND-expression\
    logical-OR-expression || logical-AND-expression<br>

conditional-expression:<br>
    logical-OR-expression\
    logical-OR-expression ? expression : conditional-expression<br>

assignment-expression:<br>
    conditional-expression\
    unary-expression assignment-operator assignment-expression<br>

assignment-operator: one of<br>
    =  *=  /=  %=  +=  -=  <<=  >>=  &=  ^=  |=<br>

expression:<br>
    assignment-expression\
    expression , assignment-expression<br>





## Modified Grammer in our parser is:-

primary-expression:<br>
    identifier<br>
    constant<br>
    string-literal<br>
    ( expression )<br>

postfix-expression:<br>
    primary-expression<br>
    primary-expression [ expression ]<br>
    primary-expression ( argument-expression-list_opt )<br>
    primary-expression . identifier<br>
    primary-expression -> identifier<br>
    primary-expression ++<br>
    primary-expression --<br>

unary-expression:<br>
    postfix-expression<br> 
    ++ expression  // expression() is called for a infinite loop so that parser doesn't terminate until EOF<br> 
    -- expression    // expression() is called for a infinite loop so that parser doesn't terminate until EOF<br> 
    unary-operator expression   // expression() is called for a infinite loop so that parser doesn't terminate until EOF<br> 
    ( type-name ) expression // cast-expression merged into unary-expression due to cyclic reference<br>
    sizeof expression<br> 
    sizeof ( type-name )<br>

multiplicative-expression:<br>
    unary-expression<br>
    unary-expression * multiplicative-expression<br>
    unary-expression / multiplicative-expression<br>
    unary-expression % multiplicative-expression<br>

additive-expression:<br>
    multiplicative-expression<br>
    multiplicative-expression + additive-expression<br>
    multiplicative-expression - additive-expression<br>

shift-expression:<br>
    additive-expression<br>
    additive-expression << shift-expression<br> 
    additive-expression >> shift-expression<br>

relational-expression:<br>
    shift-expression<br>
    shift-expression < relational-expression<br>
    shift-expression > relational-expression<br>
    shift-expression <= relational-expression<br>
    shift-expression >= relational-expression<br>

equality-expression:<br>
    relational-expression<br>
    relational-expression == equality-expression<br>
    relational-expression != equality-expression<br>

AND-expression:<br>
    equality-expression<br>
    equality-expression & AND-expression<br>

exclusive-OR-expression:<br>
    AND-expression<br>
    AND-expression ^ exclusive-OR-expression<br>

inclusive-OR-expression:<br>
    exclusive-OR-expression<br>
    exclusive-OR-expression | inclusive-OR-expression<br>

logical-AND-expression:<br>
    inclusive-OR-expression<br>
    inclusive-OR-expression && logical-AND-expression<br>

logical-OR-expression:<br>
    logical-AND-expression<br>
    logical-AND-expression || logical-OR-expression<br>

conditional-expression:<br>
    logical-OR-expression<br>
    logical-OR-expression ? expression : conditional-expression<br>

assignment-expression:<br>
    conditional-expression<br>
    conditional-expression assignment-operator assignment-expression<br>

assignment-operator: one of<br>
    =  *=  /=  %=  +=  -=  <<=  >>=  &=  ^=  |=<br>

expression:<br>
    assignment-expression , expression<br>
