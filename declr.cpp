#include "tree.h"
#include "CLexer.h"
#include "symbol_tbl.h"

ComplxNode* initializer_list();

bool_t MaybeDcl()
{
	int tok = getCurrentToken();
	if (tok == INT ||
		tok == LONG ||
		tok == FLOAT ||
		tok == DOUBLE ||
		tok == CHAR ||
		tok == SHORT ||
		tok == VOID ||
		tok == SIGNED ||
		tok == UNSIGNED ||
		tok == CONST ||
		tok == VOLATILE ||
		tok == EXTERN ||
		tok == STATIC ||
		tok == AUTO ||
		tok == TYPEDEF ||
		tok == REGISTER ||
		tok == ID)
		return True;

	return False;
}

#define setParent(parent, current_node) {   \
	if (parent == NULL) \
	{                    \
		parent = current_node;  \
	}                    \
}   

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
		tok == REGISTER ||
		tok == VOID)
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

bool_t declaration_specifiers(Symbol* sym)
{
	bool_t IsDclSpef = False;
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

	bool_t IsTypSpef = False;
	bool_t IsStruct = False;
	bool_t IsUnion = False;
	bool_t IsStructOrUnionDecl = False;
	int tok = getCurrentToken();
	if (tok == TYPEDEF && (lookahead() == STRUCT || lookahead() == UNION))
	{
		IsTypSpef = True;
		getNextToken();
		struct_or_union_start(&IsStruct, &IsUnion, &IsStructOrUnionDecl, &IsTypSpef);
	}
	else if (tok == STRUCT)
	{
		struct_or_union_start(&IsStruct, &IsUnion, &IsStructOrUnionDecl, &IsTypSpef);
	}

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
			//_exit(0);
		}
		else
		{
			//sym->
		}
		
		if (count_qual > 2)
		{
			printf("error: There can be a single type qualifier in declaration");
			//_exit(0);
		}
		
		if (count_signed > 1)
		{
			printf("error: There can be a single signed/unsigned in declaration");
			//_exit(0);
		}
	   
		if (count_spef > 1)
		{
			printf("error: There can be a single type specifier in declaration");
			//_exit(0);
		}

	}

	if (isStorage == False && isTypeQual == False && isTypeSpecf == False)
	{
		printf("error: No type defined in declaration");
		//_exit(0);
	}
	else if (isStorage == True && isTypeQual == True && isTypeSpecf == False)
	{
		printf("error: type specifier is missing !\n");
		//_exit(0);
	}
	else if (isStorage == True && isTypeSpecf == False)
	{
		printf("error: Storage class defined without type specifier");
		//_exit(0);
	}
	else if (isTypeQual == True && isTypeSpecf == False)
	{
		printf("error: Type qualifier defined without type specifier");
		//_exit(0);
	}
	else if (isStorage == True && isTypeSpecf == True)
	{
		return True;
	}
	else if (isTypeQual == True && isTypeSpecf == True)
	{
		return True;
	}
	else if (isStorage == True && isTypeQual == True && isTypeSpecf == True)
	{
		return True;
	}
	else if (isStorage == False && isTypeQual == False && isTypeSpecf == True)
	{
		return True;
	}
	else if (isTypeSpecf == True)
	{
		return True;
	}
	else
	{
		printf("error: declaration syntax is not coorect !\n");
		_exit(0);
	}
	return False;
}

void type_specifier_list(bool_t *IsTypSpefList)
{
	int tok = getCurrentToken();
	int count_spef = 0;
	bool_t isTypeSpecf = False;
	*IsTypSpefList = False;
	if(tok == check_type_specifier(&isTypeSpecf, &count_spef))
	{
		if (isTypeSpecf == True)
		*IsTypSpefList = True;
	}
	if (tok == ',' && isTypeSpecf == True)
	{
		checkEOF();
		getNextToken();
		type_specifier_list(IsTypSpefList);
	}

}

/*   
direct_declarator_dash
	: ';'
	| '(' ')'
	|  '[' ']'
	| '[' constant_expression ']'
	| '(' type_specifier_list ')'
	| '[' constant_expression ']' direct_declarator_dash
	| '[' ']' direct_declarator_dash
	| '(' type_specifier_list ')' direct_declarator_dash
	| '(' parameter_list ')' direct_declarator_dash
	| '(' ')' direct_declarator_dash
	| '(' pointer direct_declarator ')' direct_declarator_dash
*/

ComplxNode* direct_declarator_dash(int *count_id)
{
	ComplxNode *c = NULL,*c2= NULL, *prev = NULL, *parent = NULL;
	TreeNode* tNode;
	DclType d[10];
	bool_t isTypeQual = False;
	int count_qual = 0;
	int tok = getCurrentToken();
	bool_t IsDcl = False;
	bool_t IsTypSpef = False;
	if (tok == ';')
	{
		return NULL;
	}
	if (tok == '[')
	{
		
		checkEOF();
		getNextToken();
		if (tok == ']')
		{
			c = newSubDeclNode(POINTER_OF, NULL); // 2nd param null means 0 size
			setParent(parent,c);
			checkEOF();
			getNextToken();
			c2= direct_declarator_dash(count_id);
			c->Complx_child[1] = c2;
			return c;
		}
		else
		{
			tNode=constant_expression();
			
			if (tok != ']')
			{
				printf("error: expected ']' !\n");
				_exit(0);
			}
			else
			{
				c = newSubDeclNode(ARRAY_OF, tNode);
				setParent(parent, c);
				c->array_size = tNode;
				setParent(parent, c);
				checkEOF();
				getNextToken();
			}
			c2=direct_declarator_dash(count_id);
			c->Complx_child[1] = c2;
			return c;
		}
		

	}
	else if (tok == '(')
	{
		checkEOF();
		getNextToken();
		tok = getCurrentToken();
		if (tok == ')')
		{
			checkEOF();
			getNextToken();
			c = newSubDeclNode(FUNC_DCL, NULL);	
			setParent(parent, c);
			c2=direct_declarator_dash(count_id);
			c->Complx_child[0] = c2;
			return parent;
		}
		else if (tok == '*')
		{
			while (tok == '*')
			{
				c = newSubDeclNode(POINTER_OF, NULL);
				setParent(parent, c);
				if (prev)
				{
					prev->Complx_child[1] = c;
				}
				prev = c;
				checkEOF();
				tok = getNextToken();
			}
			if (tok == ID)
			{
				prev = c;
				c = newSubDeclNode(IDENTIFIER, NULL);
				setParent(parent, c);
				if (prev != NULL)
				{
					prev->Complx_child[0] = c;
				}
				prev = c;
				checkEOF();
				tok = getNextToken();
				c = direct_declarator();
				prev->Complx_child[0] = c;
				return prev;
			}
			else if (check_type_qualifier(&isTypeQual, &count_qual) != -1)
			{
				c = newSubDeclNode(PARAMTYPE, NULL);
				setParent(parent, c);
				checkEOF();
				getNextToken();
				//type_qualifier_list();
				 //check for more type qualifiers
			}

		}
		
		if (tok == ')')
		{

			checkEOF();
			getNextToken();
			c2=direct_declarator_dash(count_id);
			return c2;
		}
		else 
		{

			  if (lookahead() == ',' || lookahead() == ')')
			  {
				  c = newSubDeclNode(FUNC_DCL, NULL);
				  setParent(parent, c);
				  type_specifier_list(&IsTypSpef);
				  tok = getCurrentToken();
				  if (IsTypSpef == True && tok == ')')
				  {
					  c = newSubDeclNode(PARAMTYPE, NULL);
					  setParent(parent, c);
					  checkEOF();
					  getNextToken();
					  c2=direct_declarator_dash(count_id);
					  return c2;
				  }
			  }
			  else
			  {
				  c = newSubDeclNode(FUNC_DEF, NULL);
				  setParent(parent, c);
				  c2 = newSubDeclNode(PARAMTYPE, NULL);
				  c->Complx_child[1] = c2;
				  func_defination_parameter_list(d,0);
			  }
			

		}

		
	}
}
/*
direct_declarator
	:
	|  '[' ']' direct_declarator_dash
	| '[' constant_expression ']' direct_declarator_dash
	|  '(' ')' direct_declarator_dash
	|  '(' type_specifier_list ')' direct_declarator_dash
	| IDENTIFIER
	| IDENTIFIER '[' constant_expression ']' direct_declarator_dash
	| IDENTIFIER '[' ']' direct_declarator_dash
	| IDENTIFIER '(' parameter_list ')' direct_declarator_dash
	| IDENTIFIER '(' type_specifier_list ')' direct_declarator_dash
	| IDENTIFIER '(' ')' direct_declarator_dash
	| '(' pointer direct_declarator ')' direct_declarator_dash
*/
ComplxNode* direct_declarator()
{
	ComplxNode* c = NULL,* c2 = NULL, *c3 = NULL, *prev = NULL, * parent = NULL;
	DclType d[50] = { NotADclType };
	int params = 0;
	TreeNode* tNode = NULL;
	int tok = getCurrentToken();
	bool_t isOpenBracket = False;
	bool_t isOpenBraces = False;
	bool_t isOpenArrayBrack = False;
	bool_t isTypeQual = False;
	bool_t isTypeSpecf = False;
	bool_t IsDcl = False;
	int count_id = 0;
	int count_qual = 0;
	int count_spef = 0;
	int qual_tok= NA;
	int typ_tok = NA;

	if (tok == ID || tok == '(' || tok == '[')
	{
		if (tok == ID)
		{
			c = newSubDeclNode(IDENTIFIER, NULL);
			setParent(parent, c);
			prev = c;
			checkEOF();
			tok = getNextToken();	
			count_id++;
		}


		if (tok == '(')
		{
			isOpenBracket = True;
			if (count_id > 0)
			{
				c2 = newSubDeclNode(FUNC_DEF, NULL);
				c2->Complx_child[0] = c;
				c = c2;
				checkEOF();
				tok = getNextToken();
			}
			else if (count_id == 0)
			{
				checkEOF();
				tok = getNextToken();
				if (count_id == 0 && (typ_tok = check_type_specifier(&isTypeSpecf, &count_spef)) != -1)
				{
					while ((typ_tok = check_type_specifier(&isTypeSpecf, &count_spef)) != -1)
					{

					}
					c = newSubDeclNode(PARAMTYPE, NULL);
					setParent(parent, c);
					c2 = direct_declarator_dash(&count_id);
					c->Complx_child[1] = c2;
					return c;
				}
				else if (tok == '*')
				{
					while (tok == '*')
					{
						c = newSubDeclNode(POINTER_OF, NULL);
						setParent(parent, c);
						if (prev != NULL)
							prev->Complx_child[0] = c;
						prev = c;
						checkEOF();
						tok = getNextToken();
						while ((qual_tok=check_type_qualifier(&isTypeQual, &count_qual)) != -1)
						{
						    if (qual_tok == CONST)
						    {
							   c = newSubDeclNode(CONST_TYP_QUAL, NULL);
							   if (prev != NULL)
								   prev->Complx_child[0] = c;
							   prev = c;
							   tok = qual_tok;
						    }
						    else if (qual_tok == VOLATILE)
							{
							   c = newSubDeclNode(VOLAT_TYP_QUAL, NULL);
							   if (prev != NULL)
								   prev->Complx_child[0] = c;
							   prev = c;
							   tok = qual_tok;
						    }
							//type_qualifier_list();
							 //check for more type qualifiers
						}
					}
					if(tok != ID)
					{
						c2 = direct_declarator();
						prev->Complx_child[0] = c2;
					}
					else
					{
						c = newSubDeclNode(IDENTIFIER, NULL);
						setParent(parent, c);
						if (prev != NULL)
							prev->Complx_child[0] = c;
						prev = c;

					}
					checkEOF();
					tok=getNextToken();
					if (tok != ')')
					{
						printf("error: expected ')' !");
						_exit(0);
					}
					else
					{
						checkEOF();
						tok = getNextToken();
						c3 = direct_declarator_dash(&count_id);
						prev->Complx_child[1] = c3;
						return parent;

					}
				}


			}

			if (tok == ')')
			{
				checkEOF();
				tok = getNextToken();
				c2=direct_declarator_dash(&count_id);
				if(c != NULL)
					c->Complx_child[1] = c2;
				return c;
			}
			else if (tok == '*')
			{
				while (tok == '*')
				{
					c = newSubDeclNode(POINTER_OF, NULL);
					setParent(parent, c);
					if (prev != NULL)
						prev->Complx_child[1] = c;
					prev = c;
					checkEOF();
					tok = getNextToken();
				}
                if (tok == ID)
				{
					count_id++;
					c = newSubDeclNode(IDENTIFIER, NULL);
					setParent(parent, c);
					if (prev != NULL)
						prev->Complx_child[1] = c;
					checkEOF();
					tok = getNextToken();
				}
				else if (check_type_qualifier(&isTypeQual, &count_qual) != -1)
				{
					c = newSubDeclNode(PARAMTYPE, NULL);
					setParent(parent, c);
					if (prev != NULL)
						prev->Complx_child[1] = c;
					checkEOF();
					tok = getNextToken();
					//type_qualifier_list();
					 //check for more type qualifiers
				}



			}
			else
			{
					if (count_id == 1)
					{
						if (lookahead() == ',' || lookahead() == ')')
						{
							type_specifier_list(&isTypeSpecf);
							tok = getCurrentToken();
							if (isTypeSpecf == True && tok == ')')
							{
								c2 = newSubDeclNode(FUNC_DCL, NULL);
								checkEOF();
								getNextToken();
								c3=direct_declarator_dash(&count_id);
								c->Complx_child[0] = c2;
								c->Complx_child[1] = c3;
								return c;
							}
						}
						else
						{
							c2 = newSubDeclNode(FUNC_DCL, NULL);
							func_declare_parameter_list(d,&params);
						}
					}
					/*else
					{
						c2=declaration(&IsDcl);
					} */
			}
			tok = getCurrentToken();
			if (tok == ')' && isOpenBracket == True)
			{
				isOpenBracket = False;
				checkEOF();
				getNextToken();

			}
			else if (isOpenBracket == True)
			{
				printf("error: expected ')' !");
				_exit(0);
			}

			c3=direct_declarator_dash(&count_id);
			if (c != NULL)
			{
				c->Complx_child[1] = c3;
				c->Complx_child[0] = c2;
			}
			return c;
			
		}
		else if (tok == '[')
		{
			checkEOF();
			getNextToken();
			if (tok == ']')
			{
				c2 = newSubDeclNode(ARRAY_OF, NULL);
				c2->Complx_child[0] = c;
				checkEOF();
				getNextToken();
				return c2;
			}
			else
			{
				c2 = newSubDeclNode(ARRAY_OF, NULL);
				c2->Complx_child[0] = c;
				tNode = constant_expression();
				tok = getCurrentToken();
				if (tok != ']')
				{
					printf("error: expected ']' !\n");
					_exit(0);
				}
				else
				{
					checkEOF();
					getNextToken();
				}
			}
			c3=direct_declarator_dash(&count_id);
			c2->Complx_child[1] = c3;
			return c2;
		}
		else if (count_id == 1)
		{
		  return c;
        }
		else if (count_id < 1)
		{
			printf("error: expected identifier !\n");
			_exit(0);
		}
	} 
	else if (tok == ')' || tok == ']')
	{
		printf("error: declaration syntax is not correct !\n");
		_exit(0);
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

ComplxNode* declarator()
{
	ComplxNode *c= NULL, * c2=NULL, * prev = NULL, *parent = NULL;
	int tok = getCurrentToken();
	int qual_tok = NA;
	bool_t isTypeQual = False;
	int count_qual = 0;
	if (tok == '*')
	{
		while (((qual_tok=check_type_qualifier(&isTypeQual, &count_qual)) != -1) || tok == '*')
		{
			if (tok == '*')
			{
				c = newSubDeclNode(POINTER_OF, NULL);
				setParent(parent,c);
				getNextToken();
				prev = c;
				tok = getCurrentToken();
			}
			else if (qual_tok == CONST)
			{
				c = newSubDeclNode(CONST_TYP_QUAL, NULL);
				setParent(parent, c);
				if (prev != NULL)
				{
					prev->Complx_child[0] = c;
				}
				prev = c;
			}
			else if (qual_tok == VOLATILE)
			{
				c = newSubDeclNode(VOLAT_TYP_QUAL, NULL);
				setParent(parent, c);
				if (prev != NULL)
				{
					prev->Complx_child[0] = c;
				}
				prev = c;

			}
			//type_qualifier_list();
			 //check for more type qualifiers
		}
		
	}
	c2 = direct_declarator();
	if (prev != NULL)
		prev->Complx_child[1] = c2;
	else
	{
		c = c2;
		setParent(parent, c2);
	}
	return parent;
}

ComplxNode* initializer(TreeNode* tNode)
{
	ComplxNode* c= NULL;
	tNode = NULL;
	int tok = getCurrentToken();
	if (tok == '{')
	{
		checkEOF();
		getNextToken();
		c = initializer_list();
		if (tok != '}')
		{
			printf("error: expected '}' !\n");
			_exit(0);
		}
	}
	else
	{
		
		tNode = assignment_expression();
		c = newSubDeclNode(EXPR_ASSIGN, tNode);
	}
	return c;
}

ComplxNode* initializer_list()
{
	ComplxNode* c= NULL;
	ComplxNode* c2 = NULL;
	TreeNode* tNode = NULL;

	int tok = getCurrentToken();
	c = initializer(tNode);
	c->array_size = tNode;
	if (tok == ',')
	{
		checkEOF();
		getNextToken();
		c2 = initializer_list();
		c->Complx_child[0] = c2;
		return c;
	}
	return c;
}

ComplxNode* init_declarator()
{
	ComplxNode* t = NULL,* t2 = NULL, *t3 = NULL;
	TreeNode* tNode = NULL;
	t = declarator();
	if (t == NULL)
		return NULL;
	int tok = getCurrentToken();
	if (tok == '=')
	{	
		checkEOF();
		getNextToken();
		t2 = initializer(tNode);
		t2->Complx_child[0] = t;
	//	printf("\nt3:%p t2:%p t:%p \n", t3, t2, t);
	}
	if (t3 != NULL)
		return t3;
	else
		return t;
}

ComplxNode* init_declarator_list()
{ 
	ComplxNode* t = NULL,* t2= NULL;
	t=init_declarator();
	int tok = getCurrentToken();
	if (tok == ',')
	{
		checkEOF();
		getNextToken();
		t2= init_declarator_list();
	}
	//printf("t: %p \n", t);
	return t;
}
/*
  declaration
	: declaration_specifiers ';'
	| declaration_specifiers init_declarator_list ';'
*/

ComplxNode* declaration(bool_t *IsDecl)
{
	int tok = getCurrentToken();
	Symbol s;
	ComplxNode* c = NULL;
	bool_t IsDclSpecf = declaration_specifiers(&s);
	if (IsDclSpecf == False)
	{
		*IsDecl = False;
		return NULL;
	}
	tok = getCurrentToken();
	if (tok != ';')
	{
		c= init_declarator_list();	
		tok = getCurrentToken();
	//	printf("c is:%p \n");
		if (tok != ';')
		{
			printf("error: expected ';' at end !\n");
			_exit(0);
		}
		else
		{
			*IsDecl = True;
			if(lookahead() != EOF)
			   getNextToken();
			return c;
		}
	}
	else 
	{  
		c = newSubDeclNode(IDENTIFIER, NULL);
		*IsDecl = True;
		if (lookahead() != EOF)
		 getNextToken();
		//printf("c is:%p c->child[0]:%p c->child[1]:%p\n", c, c->Complx_child[0], c->Complx_child[1]);
		return c;
	}
	return NULL;
}



ComplNodetype convert_token_to_nodeTyp(int tok)
{
	switch (tok)
	{
	case CONST:
		return CONST_TYP_QUAL;
	case VOLATILE:
		return VOLAT_TYP_QUAL;
		
	}

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
		_exit(0);
	}
	else if (isTypeQual == True && isTypeSpecf == False)
	{
		printf("error: Type qualifier defined without type specifier");
		_exit(0);
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
	direct_declarator();
}