#include "tree.h"

TreeNode* statement();
TreeNode* expression_statement();
TreeNode* switch_statement();

/* Based on K & R The C Programming language book modified grammer --> README.md
switch_statement:
	CASE expression ':' statement
	DEFAULT ':' statement
  */
TreeNode* switch_statement()
{
	int tok = getCurrentToken();
	while (tok == CASE ||
		tok == DEFAULT)
	{

		if (tok == CASE)
		{
			checkEOF();
			getNextToken();
			expression();
			if (tok == ':')
			{
				checkEOF();
				getNextToken();
				statement();
			}
			else
			{
				printf("error: expected ':' at end of case label !\n");
				exit(0);
			}
        }
		else if (tok == DEFAULT)
		{

			checkEOF();
			getNextToken();
			if (tok == ':')
			{
				checkEOF();
				getNextToken();
				statement();
			}
			else
			{
				printf("error: expected ':' at end of default label !\n");
				exit(0);
			}
		}
	}


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
	int tok = getCurrentToken();
	if (tok == ID)
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
	else if (tok == SWITCH)
	{
		checkEOF();
		getNextToken();
		switch_statement();
	}
	else if (tok == IF)
	{
		checkEOF();
		getNextToken();
		t = newStmtNode(If);
		if (tok == '(')
		{
			checkEOF();
			getNextToken();
			t->child[0] = expression();
			if (tok == ')')
			{
				checkEOF();
				getNextToken();
				t->child[1] = statement();
				if (tok == ELSE)
				{
					checkEOF();
					getNextToken();
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
		getNextToken();
		if (tok == '(')
		{
			checkEOF();
			getNextToken();
			expression_statement();
			t = newStmtNode(For);
			t->child[0] = expression_statement();
			if (tok != ')')
			{
				expression();
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
				}
			}
			else
			{
				checkEOF();
				getCurrentToken();
				t->child[1] = statement();
				return t;
			}
		}


	}
	else if (tok == WHILE)
	{
		checkEOF();
		getNextToken();
		if (tok == '(')
		{
			checkEOF();
			getNextToken();
			expression();
			if (tok == ')')
			{
				checkEOF();
				getNextToken();
				statement();
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
		getNextToken();
		if (tok == ID)
		{
			checkEOF();
			getNextToken();
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
		getNextToken();
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
		getNextToken();
		if (tok == ';')
		{
			checkEOF();
			getNextToken();
			t = newStmtNode(Break);
			return t;
		}
		else
		{
			printf("error: expected ';' at end of continue statement !\n");
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