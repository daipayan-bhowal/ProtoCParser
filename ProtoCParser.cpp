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
	free(tape);
	resetToken();
	//  printf("entire file length is :%d\n", file_length);


	return 0;
}

int main(int argc, char* argv[])
{
	expr_test("C:\\MyWork\\prat.c");
	expr_test("C:\\MyWork\\prat2.c");
	expr_test("C:\\MyWork\\prat3.c");
	expr_test("C:\\MyWork\\prat4.c");

	dcl_test("C:\\MyWork\\prat5.c");

}
