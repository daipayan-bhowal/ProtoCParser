#include "tree.h"
#include "CLexer.h"
#include "symbol_tbl.h"

TreeNode* body() // compound statement and body are the same thing in C language
{
	TreeNode* t;
	int tok = getCurrentToken();
	bool_t IsDcl = False;
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
		if (MaybeDcl() == True)
		{
			newTypeNode(declaration(&IsDcl));
		}

		t = statement();
		/*if (tok == '}')
		{
			checkEOF();
			getNextToken();
			return t;
		}
		else
		{
			printf("error: expected '}' at end of compound statement !\n");
			_exit(0);
		}*/
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
 */

void func_declare_parameter_list(DclType* d, int *no_of_params)
{
	bool_t IsDclSpecf = False;
	static int counter = 0;
	Symbol s;
	int tok = getCurrentToken();
	if (tok == ')')
	{
		checkEOF();
		getNextToken();
		return;
	}

	IsDclSpecf = declaration_specifiers(&s);
	if (tok != ','
		&& tok != ')'
		&& IsDclSpecf == False)
	{
		printf("error: function declaration is incomplete !");
		_exit(0);
	}
 	else if (IsDclSpecf == False)
	{
		return;
	}

	declarator();
	if (tok == ',')
	{
		checkEOF();
		getNextToken();
		counter++;
		func_declare_parameter_list(d, &counter);
		*no_of_params = counter;
	}
	else if (tok == ELIPPSIS)
	{
		checkEOF();
		getNextToken();
		if (tok != ')')
		{
			printf("error: expected ')' at end of parameter list !");
			_exit(0);
		}
		else
		{
			checkEOF();
			getNextToken();
			counter++;
			*no_of_params = counter;
			counter = 0;
			return;
		}
	}


}
void func_defination_parameter_list(DclType *d, int *no_params)
{
	bool_t IsDclSpecf = False;
	int tok = getCurrentToken();
	Symbol* s = NULL;
	if (tok == ')')
	{
		checkEOF();
		getNextToken();
		return;
	}
	IsDclSpecf = declaration_specifiers(s);
	if (IsDclSpecf == False)
	{
		return;
	}
	declarator();
	tok = getCurrentToken();
	if (tok == ')')
	{
		checkEOF();
		//getNextToken();
		return;
	}
	else if (tok == ',')
	{
		checkEOF();
		getNextToken();
		*no_params++;
		func_defination_parameter_list(d, no_params);
	}
	else if (tok == ELIPPSIS)
	{
		checkEOF();
		getNextToken();
		if (tok != ')')
		{
			printf("error: expected ')' at end of parameter list !");
			_exit(0);
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
	Symbol s;
	int tok = getCurrentToken();
	bool_t IsDclSpecf = declaration_specifiers(&s);
	if (IsDclSpecf == False)
	{
		*isFuncDefin = False;
		*isFuncDeclr = False;
		return;
	}
	declarator();
	tok = getCurrentToken();
	if (tok == ';')  // complex function declaration
	{
		checkEOF();
		getNextToken();
		*isFuncDeclr = True;
		
	}
	else if(tok == '{')
	{
		//func_defination_parameter_list();
		body();
		*isFuncDefin = True;
	}

	return;

}

bool_t start_function()
{
	bool_t isFuncDefin = False;
	bool_t isFuncDeclr = False;
	bool_t isFuncProtoTyp = False;
	func_defin_or_decl(&isFuncDefin, &isFuncDeclr);
	if (isFuncDeclr == True && isFuncDefin == True)
	{
		printf("error: function cannot be both declared and defined at the same time !");
		return False;
	}
	else if (isFuncDeclr == False && isFuncDefin == False)
	{
		printf("error: function declaration or defination is expected !");
		return False;
	}
	else // if (isFuncDeclr == True || isFuncDefin == True) , either one of them is true
	{
		return True;
	}
	return False;
}