/* Brutal Test Framework 
    for C compiler testing
*/

#include "tape_t.h"
#include "Brutal.h"
#include "CLexer.h"
#include "tree.h"
#include <iostream>
#include <direct.h>


bool_t Brutal_get_current_dir(char* cwd)
{
	size_t size = 1024;
	if (getcwd(cwd, size) != NULL) {
		printf("Current working directory: %s\n", cwd);
		return True;
	}
	else {
		//fprintf(stderr, "Error getting current directory: %s\n", strerror(errno));
		free(cwd);
		return False;
	}
	
}


int Brutal_expr_test_file(const char* loc)
{
	char* tape = {};
	TreeNode* expr;
	int t = 0;
	tape = (char*)calloc(1, sizeof(char));
	tape_read((char*)loc, &tape);
	int file_length;
	string_t str = string_file(tape, &file_length);
	//const char *tape2 = "i++ + ++i;";
	//string_t str = string((char*)tape2);
	//string_t meta = string("\0");
	//tokenizer(str);
	init_tokenizer(str);

	Brutal_register_file_name(string_const(loc));
	/*for (i = 0; t != EOF;)
	{
		printf("Returned token is:%d\n", t = getTokenByPos(str, &i));

	} */
	if ((expr = expression()) != NULL)
	{
		printf("It is an expression() !\n");
		printf("Printing the expression tree:\n");
		printTree(expr);
		free(expr);
	}
	else
	{
		printf("error: expression failed !\n");
		_exit(0);

	}
	free(tape);
	resetToken();
	//  printf("entire file length is :%d\n", file_length);


	return 0;
}


int Brutal_dcl_test_file(const char* loc)
{
	char* tape = {};
	TreeNode* expr;
	int t = 0;
	tape = (char*)calloc(1, sizeof(char));
	tape_read((char*)loc, &tape);
	int file_length;
	string_t str = string_file(tape, &file_length);
	//const char *tape2 = "i++ + ++i;";
	//string_t str = string((char*)tape2);
	//string_t meta = string("\0");
	//tokenizer(str);
	init_tokenizer(str);
	Brutal_register_file_name(string_const(loc));
	/*for (i = 0; t != EOF;)
	{
		printf("Returned token is:%d\n", t = getTokenByPos(str, &i));

	} */
	bool_t IsDcl = False;
	declaration(&IsDcl);
	if (IsDcl == True)
	{
		printf("Successful declaration !\n");
	}
	else
	{
		printf("error: declaration failed !\n");
		_exit(0);
	}
	free(tape);
	resetToken();
	//  printf("entire file length is :%d\n", file_length);


	return 0;
}

int Brutal_stmt_test_file(const char* loc)
{
	char* tape = {};
	TreeNode* t_stmt;
	int t = 0;
	tape = (char*)calloc(1, sizeof(char));
	tape_read((char*)loc, &tape);
	int file_length;
	string_t str = string_file(tape, &file_length);
	//const char *tape2 = "i++ + ++i;";
	//string_t str = string((char*)tape2);
	//string_t meta = string("\0");
	//tokenizer(str);
	init_tokenizer(str);
	Brutal_register_file_name(string_const(loc));
	/*for (i = 0; t != EOF;)
	{
		printf("Returned token is:%d\n", t = getTokenByPos(str, &i));

	} */

	t_stmt = statement();
	if (t_stmt == NULL)
	{
		printf("error: statement failed !\n");
		_exit(0);

	}
	else
	{
		printf("Successful statement !\n");
		printf("Printing the statement tree:\n");
		printTree(t_stmt);
		free(t_stmt);
	}
	free(tape);
	resetToken();
	//  printf("entire file length is :%d\n", file_length);


	return 0;
}

int Brutal_func_test_file(const char* loc)
{
	char* tape = {};
	TreeNode* expr;
	int t = 0;
	tape = (char*)calloc(1, sizeof(char));
	tape_read((char*)loc, &tape);
	int file_length;
	string_t str = string_file(tape, &file_length);
	//const char *tape2 = "i++ + ++i;";
	//string_t str = string((char*)tape2);
	//string_t meta = string("\0");
	//tokenizer(str);
	init_tokenizer(str);
	Brutal_register_file_name(string_const(loc));
	/*for (i = 0; t != EOF;)
	{
		printf("Returned token is:%d\n", t = getTokenByPos(str, &i));

	} */

	start_function();
	free(tape);
	resetToken();
	//  printf("entire file length is :%d\n", file_length);


	return 0;
}

void Brutal_test_all_expr_TC_file()
{
	char* char_pwd = NULL;
	size_t size = 1024;
	char_pwd = (char*)malloc(size);
	Brutal_get_current_dir(char_pwd);
	string_t pwd = string(char_pwd);
	Brutal_expr_test_file(Brutal_concat(pwd, string_const("\\Testfiles\\expr1.c")));
	Brutal_expr_test_file(Brutal_concat(pwd, string_const("\\Testfiles\\expr2.c")));
	Brutal_expr_test_file(Brutal_concat(pwd, string_const("\\Testfiles\\expr3.c")));
	Brutal_expr_test_file(Brutal_concat(pwd, string_const("\\Testfiles\\expr4.c")));
	Brutal_expr_test_file(Brutal_concat(pwd, string_const("\\Testfiles\\expr5.c")));
	Brutal_expr_test_file(Brutal_concat(pwd, string_const("\\Testfiles\\expr6.c")));
	Brutal_expr_test_file(Brutal_concat(pwd, string_const("\\Testfiles\\expr7.c")));
	Brutal_expr_test_file(Brutal_concat(pwd, string_const("\\Testfiles\\expr8.c")));
	Brutal_expr_test_file(Brutal_concat(pwd, string_const("\\Testfiles\\expr9.c")));
	Brutal_expr_test_file(Brutal_concat(pwd, string_const("\\Testfiles\\expr10.c")));
	Brutal_expr_test_file(Brutal_concat(pwd, string_const("\\Testfiles\\expr11.c")));
	Brutal_expr_test_file(Brutal_concat(pwd, string_const("\\Testfiles\\expr12.c")));
	Brutal_expr_test_file(Brutal_concat(pwd, string_const("\\Testfiles\\expr13.c")));
	Brutal_expr_test_file(Brutal_concat(pwd, string_const("\\Testfiles\\expr14.c")));
}

void Brutal_test_all_dcl_TC_file()
{
	char* char_pwd = NULL;
	size_t size = 1024;
	char_pwd = (char*)malloc(size);
	Brutal_get_current_dir(char_pwd);
	string_t pwd = string(char_pwd);

	Brutal_dcl_test_file(Brutal_concat(pwd, string_const("\\Testfiles\\dcl1.c")));
	Brutal_dcl_test_file(Brutal_concat(pwd, string_const("\\Testfiles\\dcl2.c")));
	Brutal_dcl_test_file(Brutal_concat(pwd, string_const("\\Testfiles\\dcl3.c")));
	Brutal_dcl_test_file(Brutal_concat(pwd, string_const("\\Testfiles\\dcl4.c")));
	Brutal_dcl_test_file(Brutal_concat(pwd, string_const("\\Testfiles\\dcl5.c")));
	Brutal_dcl_test_file(Brutal_concat(pwd, string_const("\\Testfiles\\dcl6.c")));
}

void Brutal_test_all_func_TC_file()
{
	char* char_pwd = NULL;
	size_t size = 1024;
	char_pwd = (char*)malloc(size);
	Brutal_get_current_dir(char_pwd);
	string_t pwd = string(char_pwd);

	Brutal_func_test_file(Brutal_concat(pwd, string_const("\\Testfiles\\func1.c")));
}

void Brutal_test_all_stmt_TC_file()
{
	char* char_pwd = NULL;
	size_t size = 1024;
	char_pwd = (char*)malloc(size);
	Brutal_get_current_dir(char_pwd);
	string_t pwd = string(char_pwd);

	Brutal_func_test_file(Brutal_concat(pwd, string_const("\\Testfiles\\stmt1.c")));
	Brutal_func_test_file(Brutal_concat(pwd, string_const("\\Testfiles\\stmt2.c")));
	Brutal_func_test_file(Brutal_concat(pwd, string_const("\\Testfiles\\stmt3.c")));
	Brutal_func_test_file(Brutal_concat(pwd, string_const("\\Testfiles\\stmt4.c")));
	Brutal_func_test_file(Brutal_concat(pwd, string_const("\\Testfiles\\stmt5.c")));
	Brutal_func_test_file(Brutal_concat(pwd, string_const("\\Testfiles\\stmt6.c")));
}