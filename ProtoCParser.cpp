#include "tape_t.h"
#include "string_t.h"
#include "CLexer.h"
#include "tree.h"
#include <iostream>


int main(int argc, char* argv[])
{
	char* tape = {};
	TreeNode* expr;
	int t = 0;
	tape = (char*)calloc(1, sizeof(char));
	const char* loc = "C:\\MyWork\\prat.c";
	tape_read((char*)loc, &tape);
	int file_length;
	string_t str = string_file(tape, &file_length);
	//const char *tape2 = "i++ + ++i;";
	//string_t str = string((char*)tape2);
	//string_t meta = string("\0");
	tokenizer(str);
	init_tokenizer(str);
	/*for (i = 0; t != EOF;)
	{
		printf("Returned token is:%d\n", t = getTokenByPos(str, &i));

	} */
	if ((expr = expression()) != NULL)
	{
		printf("It is an expression() !");
		free(expr);
	}
	free(tape);
	//  printf("entire file length is :%d\n", file_length);


	return 0;
}
