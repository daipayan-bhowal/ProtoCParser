#include "tree.h"
#include "CLexer.h"

TreeNode* body() // compound statement and body are the same thing in C language
{
	TreeNode* t;
	int tok = getCurrentToken();
	if (tok == '{')
	{
		checkEOF();
		getNextToken();
		if (tok == '}')
		{
			checkEOF();
			getNextToken();
			return NULL;
		}
		declaration();
		t = statement();
		if (tok == '}')
		{
			checkEOF();
			getNextToken();
			return t;
		}
		else
		{
			printf("error: expected '}' at end of compound statement !\n");
			exit(0);
		}
	}
}

/* 
parameter_type_list
	: parameter_list
	| parameter_list ',' ELLIPSIS
	; */

/* 
parameter_list
	: parameter_declaration
	| parameter_list ',' parameter_declaration
	; */


/*
parameter_declaration
	: declaration_specifiers declarator
	| declaration_specifiers abstract_declarator
	| declaration_specifiers */

void func_declare_parameter_list()
{
	int tok = getCurrentToken();
	if (tok == ')')
	{
		checkEOF();
		getNextToken();
		return;
	}
	else
	{
		printf("error: function declaration is incomplete !");
		exit(0);
	}
	declaration_specifiers();
	abstract_declarator();
	if (tok == ',')
	{
		checkEOF();
		getNextToken();
		func_declare_parameter_list();
	}
	else if (tok == ELIPPSIS)
	{
		checkEOF();
		getNextToken();
		if (tok != ')')
		{
			printf("error: expected ')' at end of parameter list !");
			exit(0);
		}
		else
		{
			checkEOF();
			getNextToken();
			return;
		}
	}


}
void func_defination_parameter_list()
{
	int tok = getCurrentToken();
	if (tok == ')')
	{
		checkEOF();
		getNextToken();
		return;
	}
	else
	{
		printf("error: function defination argument is incomplete !");
		exit(0);
	}
	declaration_specifiers();
	declarator();
	if (tok == ',')
	{
		checkEOF();
		getNextToken();
		func_defination_parameter_list();
	}
	else if (tok == ELIPPSIS)
	{
		checkEOF();
		getNextToken();
		if (tok != ')')
		{
			printf("error: expected ')' at end of parameter list !");
			exit(0);
		}
		else
		{
			checkEOF();
			getNextToken();
			return;
		}
	}
}

void func_defin_or_decl(bool_t * isFuncDefin , bool_t  * isFuncDeclr)
{
	int tok = getCurrentToken();
	declaration_specifiers();
	declarator();
	if (tok == ';')  // complex function declaration
	{
		checkEOF();
		getNextToken();
		*isFuncDeclr = True;
		
	}
	else if(tok == '(')
	{
		func_defination_parameter_list();
		body();
		*isFuncDefin = True;
	}

	return;

}

void start_function()
{
	bool_t isFuncDefin = False;
	bool_t isFuncDeclr = False;
	func_defin_or_decl(&isFuncDefin, &isFuncDeclr);
	if (isFuncDeclr == True && isFuncDefin == True)
	{
		printf("error: function cannot be both declared and defined at the same time !");
		exit(0);
	}
	else if (isFuncDeclr == False && isFuncDefin == False)
	{
		printf("error: function declaration or defination is expected !");
		exit(0);
	}

}