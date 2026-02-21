
//#include "stdafx.h"
#include "CLexer.h"
//#include "string_t.h"
#include <iostream>

struct Token* token_struct_ptr = NULL;
int token_struct_len = 0;

int keyword_check(string_t str, bool* is_keyword)
{
	const char* keyword[32] =
	{ "auto",
	"break",
	"case",
	"char",
	"const",
	"continue",
	"default",
	"do",
	"double",
	"else",
	"enum",
	"extern",
	"float",
	"for",
	"goto",
	"if",
	"int",
	"long",
	"register",
	"return",
	"short",
	"signed",
	"sizeof",
	"static",
	"struct",
	"switch",
	"typedef",
	"union",
	"unsigned",
	"void",
	"volatile",
	"while" };

	for (int i = 0; i < 32; i++)
	{
		string_t key = string_const(keyword[i]);
		if (compare(str, key) == 0)
		{
			*is_keyword = true;
			return i;
		}

	}
	return 0;
}
int skip_spaces(string_t str, int* move_cursor)
{
	int pos = *move_cursor;
	if (IS_SPACE(str, pos))
	{
		pos++;
	}
	*move_cursor = pos;
	return pos;
}


int getTokenByPos(string_t str, int* k)
{
	static int line_no = 0;
	int pos = *k;
	static bool is_enclosed = false;
	static bool is_comment = false;
	static bool is_block = false;
	int start_pos = 0, last_pos = 0;
	int start_str_pos = 0;

START: // starting point of the code

#ifdef NO_PREPROCESSOR
	skip_spaces(str, &pos);
	is_prep(pos);
#endif

	//  if (NULL_CHECK(pos) || CHAR_AT(pos, line_no) == '\n')

	if (at(str, pos) == '\n')
	{
		//   printf("%d %d %d\n", pos, line_no,str_line[line_no].at(pos));
		printf("New Line\n");
		//  cout << str_line[line_no] << " " << CHAR_AT(pos, line_no) << endl;
		line_no++;
		pos++;
	}


	/*  if (at(str, pos) == '"' && is_enclosed == false)
	  {
		  is_enclosed = true;
		  pos++;
		  while (at(str, pos) != '"')
		  {
			  if (at(str, pos) == EOF)
			  {
				  printf("error: unexpected \" ");
				  CRASH_NOW();
			  }
			  pos++;
		  }
		  if (at(str, pos) == '"')
			  pos++;
		  metadata = partOfstring(str, start_str_pos, pos - start_str_pos - 1);
		  *k = pos;
		  return STROBJ;
	  }
	  */
	while (at(str, pos) == ' ' || at(str, pos) == '\t' || at(str, pos) == '\v') // Ignore all whitespaces
		pos++;

	if (is_comment == true)
	{
		while (at(str, pos) != '*' && at(str, pos + 1) != '/')
			pos++;
		if (at(str, pos) == '*' && at(str, pos + 1) == '/')
		{
			pos += 2;
			is_comment = false;
			goto START;
		}
	}
	//printf("Char before switch is:%d %d %d %d\n", at(str, pos), pos, line_no, is_comment);
	//cout << at(str,pos) << endl;
	switch (at(str, pos))
	{
	case '\n':
	{
		pos++;
		goto START;
	}
	case ' ':
	{
		pos++;
		*k = pos;
	}
	case EOF:
		return EOF;
	case '#':
		goto START;
	case '0':

		/*   if (NULL_CHECK(pos+1) || SPACE_CHECK(pos+1))
		{
		pos++;
		return
		} */
		if (at(str, pos + 1) == '0' && at(str, pos + 1) == '9')
		{
			pos++;
			while ('0' <= at(str, pos) && at(str, pos) <= '9')
				pos++;
			*k = pos;
			return OCTAL_CONST;

		}
		else if (at(str, pos + 1) == 'x' || at(str, pos + 1) == 'X')
		{
			pos += 2;
			while ('0' <= at(str, pos) && at(str, pos) <= '9')
				pos++;
			*k = pos;
			return HEXADECI_CONST;
		}
		else if (at(str, pos + 1) >= '0' && at(str, pos + 1) <= '9')
		{
			pos += 2;
			while ('0' <= at(str, pos) && at(str, pos) <= '9')
				pos++;
			*k = pos;
			return INT_CONST;
		}
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	{
		int dec_pos = 0;
		start_pos = pos;
		while ('0' <= at(str, pos) && at(str, pos) <= '9')
			pos++;

		if (at(str, pos) == '.')
		{
			dec_pos = pos;
			pos++;
		}
		else if (Is_EndOfFile(str, pos) || IS_SPACE(str, pos))
		{
			*k = pos;
			return INT_CONST;
		}

		while ('0' <= at(str, pos) && at(str, pos) <= '9')
			pos++;
		if (dec_pos == 0 && (at(str, pos) == 'u' || at(str, pos) == 'U'))
		{
			pos++;
			*k = pos;
			return USIGN_INT_CONST;
		}
		else  if (dec_pos == 0 && (at(str, pos) == 'l' || at(str, pos) == 'L'))
		{
			pos++;
			*k = pos;
			return LONG_INT_CONST;
		}
		else if (dec_pos > 0 && (at(str, pos) == 'f' || at(str, pos) == 'F'))
		{
			pos++;
			*k = pos;
			return F_CONST;
		}
		else  if (dec_pos > 0 && (at(str, pos) == 'l' || at(str, pos) == 'L'))
		{
			pos++;
			*k = pos;
			return LONG_DOUBLE_CONST;
		}
		else  if (dec_pos > 0 && (at(str, pos) == 'e' || at(str, pos) == 'E'))
		{
			pos++;
			if (at(str, pos) == '+' || at(str, pos) == '-' || ('0' <= at(str, pos) && at(str, pos) <= '9'))
			{
				while ('0' <= at(str, pos) && at(str, pos) <= '9')
					pos++;
			}
			*k = pos;
			return E_F_CONST;
		}
		*k = pos;
		if (dec_pos > 0)
			return F_CONST;
		else
			return INT_CONST;

		break;
	}
	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j':
	case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't':
	case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
	{
		// printf("Alphabet case !\n");
		goto KEYWORD;
		break;
	}
	case '"':
	{
		if (is_enclosed == false)
		{
			is_enclosed = true;
			pos++;
			while (at(str, pos) != '"')
			{
				if (at(str, pos) == EOF)
				{
					printf("error: unexpected \" ");
					CRASH_NOW();
				}
				pos++;
			}
			if (at(str, pos) == '"')
			{
				is_enclosed = false;
				pos++;
			}
			//metadata = partOfstring(str, start_str_pos, pos - start_str_pos - 1);
			*k = pos;
			return STROBJ;
		}


	}
	case '/':
	{
		pos++;
		//printf("is comment! %c\n", at(str, pos));
		if (at(str, pos) == '/') // Ignore single line comment
		{
			pos++;
			while (at(str, pos) != '\n')
			{
				if (at(str, pos) == EOF)
				{
					printf("error: unexpected \" ");
					CRASH_NOW();
				}
				pos++;

			}
			*k = pos;
			if (at(str, pos) == '\n')
				line_no++;
		}
		else if (at(str, pos) == '=')
		{
			pos++;
			*k = pos;
			return DIV_ASSIGN;
		}
		else if (at(str, pos) == '*')
		{
			pos++;
			is_comment = true;
			//printf("File changes %s", str->str);
			//skip_spaces(str, &pos);
		COMMENTLOOP:
			do
			{
				//printf("is comment!");

				if (at(str, pos) == '\n')
				{

					line_no++;
				}
				else if (at(str, pos) == '*')
					break;
				pos++;
			} while (at(str, pos) != '*');

			if (at(str, pos) == '*')
			{
				pos++;
				//printf("comment new line %d\n", at(str, pos));
				if (at(str, pos) == '\n')
				{
					line_no++;
					pos++;
					goto COMMENTLOOP;
				}
				else if (at(str, pos) == '/')
				{
					//printf("comments ended! %c\n", at(str, pos));
					is_comment = false;
					pos++; // Comments have been completed, just ignore the comments(do nothing) and go to beginning of the code
					//printf("comments ended! %d\n", at(str, pos));
					goto START;
				}
				else
				{
					pos++;
					goto COMMENTLOOP;
				}
			}
			else if (at(str, pos) != '/')
			{
				goto COMMENTLOOP;
			}

		}
		else
		{
			*k = pos;
			return '/';
		}


		break;

	}
	case '\'':
	{
		int start_char_pos = pos;
		if (is_enclosed == false)
		{
			is_enclosed = true;
			pos++;
			while (at(str, pos) != '\'')
			{
				if (at(str, pos) == EOF)
				{
					printf("error: unexpected \' ");
					CRASH_NOW();
				}
				pos++;
			}
			if (at(str, pos) == '\'')
			{
				is_enclosed = false;
				pos++;
			}
			//metadata = partOfstring(str, start_char_pos, pos - start_char_pos - 1);
			*k = pos;
			return CHAR_CONST;
		}

		break;
	}
	case '.':
	{
		pos++;
		if (at(str, pos) == '.' && at(str, pos + 1) == '.')
		{
			pos += 2;
			*k = pos;
			return ELIPPSIS;
		}
		else {
			*k = pos;
			return '.';
		}
		break;
	}
	case '+':
	{
		pos++;
		if (at(str, pos) == '=')
		{
			pos++;
			*k = pos;
			return ADD_ASSIGN;
		}
		else if (at(str, pos) == '+')
		{
			pos++;
			*k = pos;
			return INC_OP;
		}
		else {
			*k = pos;
			return '+';
		}
		break;
	}
	case '-':
	{
		pos++;
		if (at(str, pos) == '=')
		{
			pos++;
			*k = pos;
			return SUB_ASSIGN;
		}
		else if (at(str, pos) == '-')
		{
			pos++;
			*k = pos;
			return DEC_OP;
		}
		else {
			*k = pos;
			return '-';
		}
		break;
	}

	case '*':
	{
		pos++;
		if (is_comment == true)
			goto COMMENTLOOP;

		if (at(str, pos) == '=')
		{
			pos++;
			*k = pos;
			return MUL_ASSIGN;
		}
		else {
			*k = pos;
			return '*';
		}
		break;

	}
	case '%':
	{
		pos++;
		if (at(str, pos) == '=')
		{
			pos++;
			*k = pos;
			return MOD_ASSIGN;
		}
		else {
			*k = pos;
			return '%';
		}
		break;

	}
	case '&':
	{
		pos++;
		if (at(str, pos) == '=')
		{
			pos++;
			*k = pos;
			return AND_ASSIGN;
		}
		else if (at(str, pos) == '&')
		{
			pos++;
			*k = pos;
			return AND_AND_OP;
		}
		else {
			*k = pos;
			return '&';
		}
		break;

	}
	case '^':
	{
		pos++;
		if (at(str, pos) == '=')
		{
			pos++;
			*k = pos;
			return XOR_ASSIGN;
		}
		else {
			*k = pos;
			return '^';
		}
		break;

	}
	case '|':
	{
		pos++;
		if (at(str, pos) == '=')
		{
			pos++;
			*k = pos;
			return OR_ASSIGN;
		}
		else if (at(str, pos) == '|')
		{
			pos++;
			*k = pos;
			return OR_OR_OP;
		}
		else {
			*k = pos;
			return '|';
		}
		break;

	}
	case '!':
	{
		pos++;
		if (at(str, pos) == '=')
		{
			pos++;
			*k = pos;
			return NOT_ASSIGN;
		}
		else {
			*k = pos;
			return '!';
		}
		break;

	}


	/* for other operators and characters*/
	case '>':
	{
		pos++;
		if (at(str, pos) == '>')
		{
			pos++;
			if (at(str, pos) == '=')
			{
				pos++;
				*k = pos;
				return RIGHT_ASSIGN_OP;
			}
			*k = pos;
			return RIGHT_OP;
		}
		else if (at(str, pos) == '=')
		{
			pos++;
			*k = pos;
			return GRT_EQ_OP;
		}
		else {
			*k = pos;
			return '>';
		}
		break;

	}
	case '<':
	{
		pos++;
		if (at(str, pos) == '<')
		{
			pos++;
			if (at(str, pos) == '=')
			{
				pos++;
				*k = pos;
				return LEFT_ASSIGN_OP;
			}
			*k = pos;
			return LEFT_OP;
		}
		else if (at(str, pos) == '=')
		{
			pos++;
			*k = pos;
			return LESR_EQ_OP;
		}
		else {
			*k = pos;
			return '<';
		}
		break;
	}
	case '=':
	{
		pos++;
		if (at(str, pos) == '=')
		{
			pos++;
			*k = pos;
			return EQ_EQ_COND;
		}
		else {
			*k = pos;
			return '=';
		}
		break;

	}
	case ';':
	{
		pos++;
		*k = pos;
		return ';';
		break;
	}
	case '{':
	{
		pos++;
		*k = pos;
		return '{';

		break;
	}
	case '}':
	{
		pos++;
		*k = pos;
		return '}';

		break;
	}
	case ',':
	{
		pos++;
		*k = pos;
		return ',';

		break;
	}
	case ':':
	{
		pos++;
		*k = pos;
		return ':';
		break;
	}
	case '(':
	{
		pos++;
		*k = pos;
		return '(';
		break;
	}
	case ')':
	{
		pos++;
		*k = pos;
		return ')';
		break;
	}
	case '[':
	{
		pos++;
		*k = pos;
		return '[';
		break;
	}
	case ']':
	{
		pos++;
		*k = pos;
		return ']';
		break;
	}
	case '?':
	{
		pos++;
		*k = pos;
		return '?';
		break;
	}
	case '~':
	{
		pos++;
		*k = pos;
		return '~';
		break;
	}


	}


	if (at(str, pos) == '\n')
	{
		line_no++;
		pos++;
	}


KEYWORD:	 // start with keyword portion

	start_pos = pos;
	//last_pos = pos;
	while ((('a' <= at(str, pos) && at(str, pos) <= 'z')) || ('_' == at(str, pos)))
	{

		// printf("keyword loop %c!\n", at(str, pos));
		last_pos = pos;
		pos++;
		*k = pos;
	}
	if (((('a' <= at(str, start_pos) && at(str, start_pos) <= 'z')) || ('_' <= at(str, start_pos))) && (last_pos - start_pos) == 0) // single character identifier
	{
		return ID;
	}
	else if (last_pos == pos - 1 && (last_pos - start_pos))
	{
		bool is_keyword = false;
		//  printf("keyword hit! next char is %d\n", at(str, pos));
		string_t string_id = partOfstring(str, start_pos, last_pos - start_pos+1);
		printf("KEYWORD: String fetched is:%s\n", c_string(string_id));
		int token = keyword_check(string_id, &is_keyword);
		switch (at(str, pos))
		{

		case '_':
		{
			pos -= last_pos - start_pos;
			goto IDENTIFIER;
		}
		case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
		{
			pos -= last_pos - start_pos;
			goto IDENTIFIER;
		}
		case '\0':
		case '\n':
			pos++;
			*k = pos;
			line_no++;
			if (is_keyword == true)
			{
				return token;
			}
			else
			{
				return ID;
			}
			break;
		case ' ':
			//     printf("space encountered!");
		case '\v':
		case '\t':
		{	//pos++;
			*k = pos;
			if (is_keyword == true)
			{
				return token;
			}
			else
			{
				return ID;
			}
			break;
		}
		case '\\':

		case '(':
		{
			// pos++;
			*k = pos;
			char* strid = c_string(string_id);
			if (strncmp(strid, "if", 2) || strncmp(strid, "switch", 6) || strncmp(strid, "return", 6) || strncmp(strid, "sizeof", 6) || strncmp(strid, "while", 5) || strncmp(strid, "for", 3))
				return token;
			else if (is_keyword == true)
			{
				printf("error: Wrong keyword usage !");
				exit(0);
			}
			else
				return ID;
			break;
		}

		case '[':
			// don't need to increase pos
			*k = pos;
			return ID;
		case '+':
		case '-':
		case '*':
		case '/':
		case '%':
		case '&':
		case '^':
		case ')':
		case '~':
		case '<':
		case '>':
		case '.':
		case '?':
		case '|':
		{        // don't need to increase pos
			*k = pos;
			if (is_keyword == true)
				return token;
			else
				return ID;
		}
		case ',':
		{      // don't need to increase pos
			*k = pos;
			if (is_keyword == true)
				return token;
			else
				return ID;
		}
		case '!':
		{     //pos++;
			*k = pos;
			if (is_keyword == true)
			{
				printf("error: Wrong keyword usage !");
				exit(0);
			}
			else
			{
				return ID;
			}
		}
		case '$':
		case '@':
		case '#':
		{   //pos++;
			*k = pos;
			if (is_keyword == true)
			{
				printf("error: Wrong keyword usage !");
				exit(0);
			}
		}
		case '"':
		{
			int start_str_pos = pos;
			is_enclosed = true;
			pos++;
			while (at(str, pos) != '"')
			{
				if (at(str, pos) == EOF)
				{
					printf("error: unexpected \" ");
					CRASH_NOW();
				}
				pos++;
			}
			if (at(str, pos) == '"')
				is_enclosed = false;
			//metadata = partOfstring(str, start_str_pos, pos - start_str_pos - 1);
			*k = pos;
			return STROBJ;
			break;
		}
		case ':':
		case '=':
		case ';':
		{   //pos++;
			*k = pos;
			if (is_keyword == true)
			{
				return token;
			}
			else
				return ID;

			break;
		}
		}
	}

IDENTIFIER: // identifier section
	start_pos = pos;
	//        last_pos = pos;
	int count = 0;
	while (('a' <= at(str, pos) && at(str, pos) <= 'z') || ('A' <= at(str, pos) && at(str, pos) <= 'Z') || ('0' <= at(str, pos) && at(str, pos) <= '9') || at(str, pos) == '_')
	{
		//       if (count == 0)
		//       {
			//  printf("Identifier loop !\n");
		switch (at(str, pos)) // for first letter cannot be digit
		{

		case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
			if (pos == start_pos)
			{
				printf("error: not an identifier !");
				exit(0);
			}
			break;
		}
		//       count++;
	   //       }	
		last_pos = pos;
		pos++;
		*k = pos;
	}
	if (pos - start_pos > 511)
	{
		printf("error: an identifier has a limit of size 512 !");
		exit(0);
	}
	if (((last_pos - start_pos) > 0) && last_pos > 0 && last_pos == (pos - 1))
	{
		bool is_keyword = false;
		//printf("indentifier/keyword hit!\n");
		string_t string_id = partOfstring(str, start_pos, pos - start_pos - 1);
		printf("IDENTIFIER: String fetched is:%s\n", c_string(string_id));
		int token = keyword_check(string_id, &is_keyword);
		switch (at(str, pos))
		{
		case '+':
		case '-':
		case '*':
		case '/':
		case '%':
		case '&':
		case '=':
		case '^':
		case '(':
		case ')':
		case '{':
		case '}':
		case '[':
		case ']':
		case '~':
		case '<':
		case '>':
		case '.':
		case ',':
		case ':':
		case ';':
		case '?':
		case '|':
		case ' ':
		case '\v':
		case '\t':
		{
			if (is_keyword == false)
			{
				*k = pos;
				return ID;
			}

		}
		case '\0':
		case '\n':
			line_no++;
			*k = pos;
			return ID;
			break;
		}
	}

	//  printf("NOT_FOUND CHAR IS:%d , prev char %c prev prev char %d\n", at(str, pos), at(str, pos-1), at(str, pos-2));
	return NOT_FOUND;
}


void tokenizer(string_t str)
{
	int start_position, end_position;
	int t = 0;
	token_struct_ptr = (struct Token*)malloc(sizeof(struct Token));
	token_struct_len = 0;
	int i = 0;
	for (i = 0; t != EOF;)
	{
		start_position = i;
		printf("Returned token is:%d\n", t = getTokenByPos(str, &i));
		end_position = i;
		token_struct_ptr->start_pos = start_position;
		token_struct_ptr->len = end_position - start_position;
		token_struct_ptr->token_integerform = t;
		token_struct_ptr->str_token = partOfstring(str, start_position, token_struct_ptr->len);
		printf("String token is:%s\n", token_struct_ptr->str_token->str);
		token_struct_len++;
		token_struct_ptr = (struct Token*)realloc(token_struct_ptr, sizeof(struct Token) * token_struct_len);
		//    printf("pos is:%d\n", i);
			//printf("meta data is:%s\n", meta);
		  //  printf("file line no  is :%d %d\n", i, file_length);
		//debug_lex(t);
	}


}

string_t FileStrBuffer = NULL;
void setStringBuffer(string_t string_buf) {
	FileStrBuffer = copy(string_buf);
}


static int current_pos = 0;
int getNextToken()
{
	
	int start_position = 0, end_position = 0;

	start_position = current_pos;
	int t = getTokenByPos(FileStrBuffer, &current_pos);
	if (t == EOF)
	{
		exit(0);
	}
	end_position = current_pos;
	token_struct_ptr->start_pos = start_position;
	token_struct_ptr->len = end_position - start_position;
	token_struct_ptr->token_integerform = t;
	token_struct_ptr->str_token = partOfstring(FileStrBuffer, start_position, token_struct_ptr->len);
	token_struct_len++;
	if (token_struct_ptr == NULL)
		return EOF;
	token_struct_ptr = (struct Token*)realloc(token_struct_ptr, sizeof(struct Token) * token_struct_len);
	if (token_struct_ptr == NULL)
		exit(0);

	return t;

}

string_t getTokenString()
{
	return token_struct_ptr->str_token;
}
void resetToken()
{
	current_pos = 0;
	token_struct_ptr = (struct Token*)realloc(token_struct_ptr, sizeof(struct Token));
	//token_struct_len = 1;
	//getNextToken();
}

int lookahead()
{
	int future_pos = token_struct_ptr->start_pos + token_struct_ptr->len;
	int futuretoken = getTokenByPos(FileStrBuffer, &future_pos);
	return futuretoken;
}

void backtrack()
{
	(struct Token*)realloc(token_struct_ptr, sizeof(struct Token) * (token_struct_len - 1));
	token_struct_len--;
}
int getTokenPos()
{
	return token_struct_ptr->start_pos;
}
int getCurrentToken()
{
	return token_struct_ptr->token_integerform;
}

void init_tokenizer(string_t string_file)
{
	token_struct_ptr = (struct Token*)malloc(sizeof(struct Token));
	token_struct_len = 1;
	setStringBuffer(string_file);
	getNextToken();
}
bool_t MarkEndOfFile(bool_t IsMark)
{
	static bool_t eofFlag = False;
	if (IsMark == True)
		eofFlag = True;

	return eofFlag;
}
