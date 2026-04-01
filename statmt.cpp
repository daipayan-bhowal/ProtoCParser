#include "tree.h"

TreeNode* statement();
TreeNode* expression_statement();
TreeNode* switch_body();

/* Based on K & R The C Programming language book modified grammer --> README.md
case_list:
    DEFAULT ':' statement
	CASE expression ':' statement case_list	
switch_body:
	'{' CASE expression ':' statement case_list '}'
statement:
	 switch '(' expression ')' switch_body
  */
TreeNode* case_list()
{
	TreeNode* t_cases = NULL;
	int tok = getCurrentToken();
	if (tok == DEFAULT)
	{
		    t_cases = newStmtNode(Default);
			checkEOF();
			tok = getNextToken();
			if (tok == ':')
			{
				checkEOF();
				getNextToken();
				t_cases->child[0] = statement();
				return t_cases;
			}
			else
			{
				printf("error: expected ':' at end of default label !\n");
				exit(0);
			}
	}
	else if (tok == CASE)
	{
		t_cases = newStmtNode(Case);
		checkEOF();
		getNextToken();
		t_cases->child[0] = expression();
		tok = getCurrentToken();
		if (tok == ':')
		{
			checkEOF();
			getNextToken();
			t_cases->child[1] = statement();
			t_cases->child[2] = case_list();
			return t_cases;
		}
		else
		{
			printf("error: expected ':' at end of case label !\n");
			exit(0);
		}
	}

	return t_cases;
}

TreeNode* switch_body()
{
	TreeNode* t_case = NULL;
	int tok = getCurrentToken();
	if (tok == '{')
	{
		checkEOF();
		tok = getNextToken();
	}
	if (tok == CASE ||
		tok == DEFAULT)
	{
		if (tok == CASE)
		{
			t_case = newStmtNode(Case);
			checkEOF();
		    getNextToken();
			t_case->child[0]= expression();
			tok = getCurrentToken();
			if (tok == ':')
			{
				checkEOF();
				getNextToken();
				t_case->child[1] = statement();
			}
			else
			{
				printf("error: expected ':' at end of case label !\n");
				exit(0);
			}
			t_case->child[2] = case_list();
		}
	}
	if ((tok = getCurrentToken()) == '}')
	{
		checkEOF();
		tok = getNextToken();
	}
	else
	{
		printf("error: expected '}' at end of switch !\n");
		exit(0);
	}

	return t_case;
}
/*
expression_statement:
	';'
	expression ';'

*/

TreeNode* expression_statement()
{
	TreeNode* t;
	int tok = getCurrentToken();
	if (tok == ';')
	{
		checkEOF();
		getNextToken();
		return NULL;
	}
	else
	{
		t= expression();
		tok = getCurrentToken();
		if (tok == ';')
		{
			checkEOF();
			getNextToken();
			return t;
		}
		else
		{
			printf("error: expected ';' at end of expression statement !\n");
			exit(0);
		}

	}


}

/* Based on K & R The C Programming language book modified grammer -->  
 statement:
     ;
	 IDENTIFIER : statement   // labeled statement
	 '{' declaration_list statement '}'   // compound statement
	 SWITCH '(' expression ')' switch_statement
     IF '(' expression ')' statement ELSE statement
	 WHILE '(' expression ')' statement
	 DO statement WHILE '(' expression ')' ';'
	 FOR '(' expression_statement expression_statement ')' statement
	 FOR '(' expression_statement expression_statement expression ')' statement
*/ 

TreeNode* statement()
{
	TreeNode* t= NULL;
	bool_t IsDcl = False;
	int tok = getCurrentToken();
	if (tok == ';')
	{
		checkEOF();
		getNextToken();
		return NULL;
	}
	else if (tok == DO)
	{
		checkEOF();
		tok = getNextToken();
		t = newStmtNode(Do);
		t->child[0] = statement();
		tok = getCurrentToken();
		if (tok == WHILE)
		{
			checkEOF();
			tok = getNextToken();
			if (tok == '(')
			{
				checkEOF();
				getNextToken();
				t->child[1] = expression();
				tok = getCurrentToken();
				if (tok == ')')
				{
					checkEOF();
					tok = getNextToken();
					if (tok == ';')
					{
						checkEOF();
						getNextToken();
						return t;
					}
					else
					{
						printf("error: expected ';' at end of do-while statement !\n");
						exit(0);
					}
				}
				else
				{
					printf("error: expected ')' !\n");
					exit(0);
				}
			}
			else
			{
				printf("error: expected '(' !\n");
				exit(0);
			}
		}
		else
		{
			printf("error: expected 'while' at end of do-while statement !\n");
			exit(0);
		}
	}
	else if (tok == ID)
	{
		checkEOF();
		getNextToken();
		if (tok == ':')
		{
			checkEOF();
			getNextToken();
			t = newStmtNode(Label);
			t->child[0] = statement();
		}		

	}
	else if (tok == '{')
	{
		checkEOF();
		tok = getNextToken();		
		if (tok == '}')
		{
			checkEOF();
			getNextToken();
            return NULL;
		}
		else if(MaybeDcl() == True)
		{
			declaration(&IsDcl);
		}
		
		t = statement();
		tok = getCurrentToken();
		if (tok == '}')
		{
			if(lookahead() == EOF)
				return t;
            getNextToken();
			return t;
		}
		else
		{
			printf("error: expected '}' at end of compound statement !\n");
			exit(0);
		}
	}
	else if (tok == SWITCH)
	{
		t = newStmtNode(Switch);
		checkEOF();
		tok = getNextToken();
		if (tok == '(')
		{
			checkEOF();
			getNextToken();
			t->child[0] = expression();
			tok = getCurrentToken();
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
		}

		t->child[1] = switch_body();
	}
	else if (tok == IF)
	{
		checkEOF();
		tok = getNextToken();
		t = newStmtNode(If);
		if (tok == '(')
		{
			checkEOF();
			getNextToken();
			t->child[0] = expression();
			tok = getCurrentToken();
			if (tok == ')')
			{
				checkEOF();
				tok = getNextToken();
				t->child[1] = statement();
				tok = getCurrentToken();
				if (tok == ELSE)
				{
					checkEOF();
					tok = getNextToken();
					t->child[2] = statement();
				}
			}
			else
			{
				printf("error: expected ')' !\n");
				exit(0);
			}
		}
		else
		{
			printf("error: expected '(' !\n");
			exit(0);
		}
	}
	else if (tok == FOR)
	{
		checkEOF();
		tok = getNextToken();
		if (tok == '(')
		{
			checkEOF();
			tok = getNextToken();
			expression_statement();
			t = newStmtNode(For);
			t->child[0] = expression_statement();
			if (tok != ')')
			{
				expression();
				tok = getCurrentToken();
				if (tok != ')')
				{
					printf("error: expected ')' !\n");
					exit(0);
				}
				else
				{
					checkEOF();
					getNextToken();
					t->child[1] = statement();
					return t;
				}
			}
			else
			{
				checkEOF();
				getNextToken();
				t->child[1] = statement();
				return t;
			}
		}


	}
	else if (tok == WHILE)
	{
		checkEOF();
		tok = getNextToken();
		if (tok == '(')
		{
			checkEOF();
			getNextToken();
			t = newStmtNode(While);
			t->child[0] = expression();
			tok = getCurrentToken();
			if (tok == ')')
			{
				checkEOF();
				getNextToken();
				t->child[1] = statement();
				return t;
			}
			else
			{
				printf("error: expected ')' !\n");
				exit(0);
			}
		}
		else
		{
			printf("error: expected '(' !\n");
			exit(0);
		}
	}
	else if (tok == GOTO)
	{
		checkEOF();
		tok = getNextToken();
		if (tok == ID)
		{
			checkEOF();
			tok = getNextToken();
			if (tok == ';')
			{
				checkEOF();
				getNextToken();
				t = newStmtNode(Goto);
				return t;
			}
			else
			{
				printf("error: expected ';' at end of goto statement !\n");
				exit(0);
			}
		}
		else
		{
			printf("error: expected label name after 'goto' !\n");
			exit(0);
		}
	}
	else if(tok == CONTINUE)
	{
		checkEOF();
		tok = getNextToken();
		if (tok == ';')
		{
			checkEOF();
			getNextToken();
			t = newStmtNode(Continue);
			return t;
		}
		else
		{
			printf("error: expected ';' at end of continue statement !\n");
			exit(0);
		}
	}
	else if (tok == BREAK)
	{
		checkEOF();
		tok = getNextToken();
		if (tok == ';')
		{
			checkEOF();
			getNextToken();
			t = newStmtNode(Break);
			return t;
		}
		else
		{
			printf("error: expected ';' at end of break statement !\n");
			exit(0);
		}
	}
	else if (tok == RETURN)
	{
		checkEOF();
		getNextToken();
		t = newStmtNode(Return);
		t->child[0] = expression_statement();
	}
	else
	{
		t = expression_statement();
	}
	return t;
}