#include "tape_t.h"
#include "string_t.h"
#include "CLexer.h"
#include "tree.h"
#include <iostream>


int expr_test(const char* loc)
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
		exit(0);

	}
	free(tape);
	resetToken();
	//  printf("entire file length is :%d\n", file_length);


	return 0;
}


int dcl_test(const char* loc)
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
		exit(0);
	}
	free(tape);
	resetToken();
	//  printf("entire file length is :%d\n", file_length);


	return 0;
}
void test_all_expr_TC_file()
{
	expr_test("C:\\MyWork\\expr1.c");
	expr_test("C:\\MyWork\\expr2.c");
	expr_test("C:\\MyWork\\expr3.c");
	expr_test("C:\\MyWork\\expr4.c");
	expr_test("C:\\MyWork\\expr5.c");
	expr_test("C:\\MyWork\\expr6.c");
	expr_test("C:\\MyWork\\expr7.c");
	expr_test("C:\\MyWork\\expr8.c");
	expr_test("C:\\MyWork\\expr9.c");
	expr_test("C:\\MyWork\\expr10.c");
	expr_test("C:\\MyWork\\expr11.c");
	expr_test("C:\\MyWork\\expr12.c");
	expr_test("C:\\MyWork\\expr13.c");
	expr_test("C:\\MyWork\\expr14.c");
}

void test_all_dcl_TC()
{
	dcl_test("C:\\MyWork\\dcl1.c");
	dcl_test("C:\\MyWork\\dcl2.c");
	dcl_test("C:\\MyWork\\dcl3.c");
	dcl_test("C:\\MyWork\\dcl4.c");
	dcl_test("C:\\MyWork\\dcl5.c");
    dcl_test("C:\\MyWork\\dcl6.c");
}

int main(int argc, char* argv[])
{
	test_all_expr_TC_file();
	test_all_dcl_TC();
}
