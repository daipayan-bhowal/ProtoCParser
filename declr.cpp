#include "tree.h"
#include "CLexer.h"

void initializer_list();



int check_storage_class(bool_t* isStorage, int* count_stor)
{
	int tok = getCurrentToken();
		int sclass = -1;
		if (tok == EXTERN ||
			tok == STATIC ||
			tok == AUTO ||
			tok == TYPEDEF ||
			tok == REGISTER)
		{
			sclass = tok;
			*isStorage = True;
			*count_stor++;
			checkEOF();
			getNextToken();
			return sclass;
		}
		else
		{
			return -1;
		}

}

int check_type_qualifier(bool_t* isTypQual, int *count_qual)
{
	int tok = getCurrentToken();
	int qual = -1;
	if (tok == CONST ||
		tok == VOLATILE)
	{
		qual = tok;
		*isTypQual = True;
		*count_qual++;
		checkEOF();
		getNextToken();
		return qual;
	}
	else
	{
		return -1;
	}

}

int check_type_sign(bool_t* isTypSign, int *count_signed)
{
	int tok = getCurrentToken();
	int sign = -1;
	if (tok == SIGNED ||
		tok == UNSIGNED)
	{
		sign = tok;
		*isTypSign = True;
		*count_signed++;
		checkEOF();
		getNextToken();
		return sign;
	}
	else
	{
		return -1;
	}
}

int check_type_specifier(bool_t* isTypSpecf, int *count_spef)
{
	int tok = getCurrentToken();
	int type = -1;
	if (tok == INT ||
		tok == LONG ||
		tok == FLOAT ||
		tok == DOUBLE ||
		tok == CHAR   ||
		tok == SHORT ||
		tok == REGISTER)
	{
		type = tok;
		*isTypSpecf = True;
		*count_spef++;
		checkEOF();
		getNextToken();
		return type;
	}
	else
	{
		return -1;
	}

}
/*
declaration_specifiers
	: storage_class_specifier
	| storage_class_specifier declaration_specifiers
	| type_specifier
	| type_specifier declaration_specifiers
	| type_qualifier
	| type_qualifier declaration_specifiers
*/

void declaration_specifiers()
{
	bool_t isStorage = False;
	bool_t isTypeSign = False;
	bool_t isTypeSpecf = False;
	bool_t isTypeQual = False;
	int typ_sign = -1;
	int stor_class = -1;
	int typ_qual = -1;
	int typ_specf = -1;
	int count_signed = 0;
	int count_stor = 0;
	int count_qual = 0;
	int count_spef = 0;

	while (
		((stor_class = check_storage_class(&isStorage, &count_stor)) != -1) ||
		((typ_qual = check_type_qualifier(&isTypeQual, &count_qual)) != -1) ||
		((typ_sign = check_type_sign(&isTypeSign, &count_signed)) != -1) ||
		((typ_specf = check_type_specifier(&isTypeSpecf, &count_spef)) != -1)
		)
	{
		if (count_stor > 1)
		{
			printf("error: There can be a single class storage in declaration");
			exit(0);
		}
		else if (count_qual > 1)
		{
			printf("error: There can be a single type qualifier in declaration");
			exit(0);
		}
		else if (count_signed > 1)
		{
			printf("error: There can be a single signed/unsigned in declaration");
			exit(0);
		}
		else if (count_spef > 1)
		{
			printf("error: There can be a single type specifier in declaration");
			exit(0);
		}

	}

	if (isStorage == False && isTypeQual == False && isTypeSpecf == False)
	{
		printf("error: No type defined in declaration");
		exit(0);
	}
	else if (isStorage == True && isTypeQual == True && isTypeSpecf == False)
	{
		printf("error: type specifier is missing !\n");
		exit(0);
	}
	else if (isStorage == True && isTypeSpecf == False)
	{
		printf("error: Storage class defined without type specifier");
		exit(0);
	}
	else if (isTypeQual == True && isTypeSpecf == False)
	{
		printf("error: Type qualifier defined without type specifier");
		exit(0);
	}
	else if (isStorage == True && isTypeSpecf == True)
	{
	}
	else if (isTypeQual == True && isTypeSpecf == True)
	{
	}
	else if (isStorage == True && isTypeQual == True && isTypeSpecf == True)
	{
	}
	else if (isStorage == False && isTypeQual == False && isTypeSpecf == True)
	{
	}
	else if (isTypeSpecf == True)
	{

	}
	else
	{
		printf("error: declaration syntax is not coorect !\n");
		exit(0);
	}
	return;
}


/*   
direct_declarator
	: '[' constant_expression ']'
	| '[' ']'
	| '(' parameter_type_list ')'
	| '(' ')'
	| IDENTIFIER ';'
	| IDENTIFIER '[' constant_expression ']' direct_declarator
	| IDENTIFIER '[' ']' direct_declarator
	| IDENTIFIER '(' parameter_type_list ')' direct_declarator
	| IDENTIFIER '(' identifier_list ')' direct_declarator
	| IDENTIFIER '(' ')' direct_declarator
	| '(' pointer direct_declarator ')' direct_declarator

*/
void direct_declarator()
{
	int tok = getCurrentToken();
	bool_t isOpenBraces = False;
	bool_t isOpenArrayBrack = False;
	bool_t isTypeQual = False;
	int count_id = 0;
	int count_qual = 0;
	if (tok == ID || tok == '(' || tok == '[')
	{
		if (tok == ID)
		{
			checkEOF();
			getNextToken();
			count_id++;
			if (tok == ';')
				return;
		}


		if (tok == '(')
		{
			checkEOF();
			getNextToken();
			while (tok == '*')
			{
				checkEOF();
				getNextToken();
				while (check_type_qualifier(&isTypeQual, &count_qual) != -1)
				{
					checkEOF();
					getNextToken();
					//type_qualifier_list();
					 //check for more type qualifiers
				}

			}
			if (tok == ')')
			{
				direct_declarator();
				checkEOF();
				getNextToken();
				return;
			}

		}
		else if (tok == '[')
		{
			checkEOF();
			getNextToken();
			if (tok == ']')
			{
				checkEOF();
				getNextToken();
				return;
			}
			else
			{
				expression();
				if (tok != ']')
				{
					printf("error: expected ']' !\n");
					exit(0);
				}
				else
				{
					checkEOF();
					getNextToken();
				}
			}
		}
	}

	while (count_id > 1 && tok != '(' && tok != '[' && tok != ')' && tok != ']')
	{
		checkEOF();
		getNextToken();
		if (tok == '(' && isOpenBraces == False)
		{
			isOpenBraces = True;
			checkEOF();
			getNextToken();
			if (tok == ')')
			{
				isOpenBraces = False;
				checkEOF();
				getNextToken();
			}
			else
			{
				while (tok == ID || tok == ELIPPSIS)
				{
					checkEOF();
					getNextToken();
					if (tok == ',')
					{
						checkEOF();
						getNextToken();
					}
					else
						break;

				}
				if (tok != ')')
				{
					printf("error: expected ')' !\n");
					exit(0);
				}
				else
				{
					isOpenArrayBrack = False;
					checkEOF();
					getNextToken();
				}

			}

		}
		else if (tok == '[' && isOpenArrayBrack == False)
		{
			isOpenArrayBrack = True;
			checkEOF();
			getNextToken();
			if (tok == ']')
			{
				isOpenArrayBrack = False;
				checkEOF();
				getNextToken();

				
			}
			else
			{
				expression();
				if (tok != ']')
				{
					printf("error: expected ']' !\n");
					exit(0);
				}
				else
				{
					isOpenArrayBrack = False;
					checkEOF();
					getNextToken();
				}
			}
	
		}
	}
	if (count_id < 1)
	{
		printf("error: expected identifier !\n");
		exit(0);
	}
}
/*
pointer
	: '*'
	| '*' type_qualifier_list
	| '*' pointer
	| '*' type_qualifier_list pointer
  declarator
	: pointer direct_declarator
	| direct_declarator
  */

void declarator()
{
	int tok = getCurrentToken();
	bool_t isTypeQual = False;
	int count_qual = 0;
	if (tok == '*')
	{
		while (check_type_qualifier(&isTypeQual, &count_qual) || tok == '*')
		{
			checkEOF();
			getNextToken();
			//type_qualifier_list();
			 //check for more type qualifiers
		}
		
	}
	direct_declarator();
}

void initializer()
{
	int tok = getCurrentToken();
	if (tok == '{')
	{
		checkEOF();
		getNextToken();
		initializer_list();
		if (tok != '}')
		{
			printf("error: expected '}' !\n");
			exit(0);
		}
	}
	else
	{
		assignment_expression();
	}
}

void initializer_list()
{
	int tok = getCurrentToken();
	initializer();
	if (tok == ',')
	{
		checkEOF();
		getNextToken();
		initializer_list();
	}
}



void init_declarator()
{
	declarator();
	int tok = getCurrentToken();
	if (tok == '=')
	{
		checkEOF();
		getNextToken();
		initializer();
	}
}

void init_declarator_list()
{ 
	
	init_declarator();
	int tok = getCurrentToken();
	if (tok == ',')
	{
		checkEOF();
		getNextToken();
		init_declarator_list();
	}
}
/*
  declaration
	: declaration_specifiers ';'
	| declaration_specifiers init_declarator_list ';'
*/

void declaration(bool_t *IsDecl)
{
	int tok = getCurrentToken();
	declaration_specifiers();
	if (tok != ';')
	{
		init_declarator_list();	
		tok = getCurrentToken();
		if (tok != ';')
		{
			printf("error: expected ';' at end !\n");
			exit(0);
		}
		else
		{
			*IsDecl = True;
			return;
		}
	}
	else
	{
		*IsDecl = True;
		return;
	}
}
/* 
   direct_abstract_declarator
	: '(' pointer direct_abstract_declarator ')'
	|  '[' ']' direct_abstract_declarator
	| '[' constant_expression ']' direct_abstract_declarator 
	|  '(' ')' direct_abstract_declarator
	|  '(' parameter_type_list ')' direct_abstract_declarator
	;
*/

void direct_abstract_declarator()
{
	bool_t isTypeQual = False;
	int tok = getCurrentToken();
	int count_qual = 0;
	while (tok == '(' || tok == '[' || tok == ']' || tok == ')')
	{

		if (tok == '(')
		{
			checkEOF();
			getNextToken();
			if (tok == '*')
			{
				while (tok == '*')
				{
					checkEOF();
					getNextToken();
					while (check_type_qualifier(&isTypeQual, &count_qual))
					{
						checkEOF();
						getNextToken();
						//type_qualifier_list();
						 //check for more type qualifiers
					}

				}
				if (tok != ')')
				{
					printf("error: expected ')' !\n");
					exit(0);
				}
				return;
			}
			else if (tok == ')')
			{
				checkEOF();
				getNextToken();
			}
			else 
			{
				//parameter_type_list(); TO DO
				if (tok == ')')
				{
					checkEOF();
					getNextToken();

				}
				else
				{
					printf("error: expected ')' !\n");
					exit(0);
				}
				return;
			}

		}
		else if (tok == '[')
		{
			checkEOF();
			getNextToken();
			if (tok != ']')
				expression();
			if (tok != ']')
			{
				printf("error: expected ']' !\n");
				exit(0);
			}
			else {
				checkEOF();
				getNextToken();
			}
		}
	}
}


void abstract_declarator()
{
	bool_t isTypeQual = False;
	int tok = getCurrentToken();
	int count_qual = 0;
	if (tok == '*')
	{
		checkEOF();
		getNextToken();
		while (check_type_qualifier(&isTypeQual, &count_qual))
		{
			checkEOF();
			getNextToken();
			//type_qualifier_list();
			 //check for more type qualifiers
		}
		
	}
	direct_abstract_declarator();
}

void specifier_qualifier_list()
{
	bool_t isTypeQual = False;
	bool_t isTypeSpecf = False;
	int count_qual = 0;
	int count_typespef = 0;
	int typ_qual = check_type_qualifier(&isTypeQual, &count_qual);
	int typ_specf = check_type_specifier(&isTypeSpecf, &count_typespef);
	if (isTypeQual == False && isTypeSpecf == False)
	{
		printf("error: No type defined in declaration");
		exit(0);
	}
	else if (isTypeQual == True && isTypeSpecf == False)
	{
		printf("error: Type qualifier defined without type specifier");
		exit(0);
	}
	else if (isTypeSpecf == True)
	{
	}
	else if (isTypeQual == True && isTypeSpecf == True)
	{
	}
}

void type_name()
{
	specifier_qualifier_list();
	abstract_declarator();
}