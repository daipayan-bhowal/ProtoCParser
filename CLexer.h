#pragma once
#pragma once
#include "string_t.h"
#include <signal.h>
#include "bool_t.h"


#define CRASH_NOW()  raise(SIGSEGV); 
#define EOT -9999
#define Is_EndOfFile(str,pos) 	at(str,pos) == EOF




/*#define NL_CHECK(pos)     if (NULL_CHECK(pos))    \
						  {                  \
						  pos = 0;         \
						  line_no++;       \
						  }
						  */

						  //#define ALPHA_NUM_CHECK(pos) ((CHAR_AT(pos, line_no) >= 'a' && CHAR_AT(pos, line_no) <= 'z') || (CHAR_AT(pos, line_no) >= 'A' && CHAR_AT(pos, line_no) <= 'Z') || (CHAR_AT(pos, line_no) >= '0' && CHAR_AT(pos, line_no) <= '9'))

						  /* Macro Values for Tokens */
#define ELIPPSIS 0x1000
#define ADD_ASSIGN 0x1001
#define INC_OP 0x1002
#define SUB_ASSIGN 0x1003
#define DEC_OP 0x1004
#define DIV_ASSIGN 0x1005
#define MUL_ASSIGN 0x1006
#define MOD_ASSIGN 0x1007
#define AND_ASSIGN 0x1008
#define XOR_ASSIGN 0x1009
#define OR_ASSIGN 0x1010
#define NOT_ASSIGN 0x1011
#define RIGHT_OP 0x1012
#define LEFT_OP 0x1013
#define EQ_EQ_COND 0x1014
#define GRT_EQ_OP 0x1015
#define LESR_EQ_OP 0x1016
#define AND_AND_OP 0x1017
#define OR_OR_OP 0x1018
#define RIGHT_ASSIGN_OP 0x1019
#define LEFT_ASSIGN_OP 0x1020

#define ID 0x2015
#define STROBJ 0x2016
#define INT_CONST 0x2017
#define CHAR_CONST 0x2018
#define F_CONST 0x2019
#define OCTAL_CONST 0x2020
#define USIGN_INT_CONST 0x2021
#define LONG_INT_CONST 0x2021
#define LONG_DOUBLE_CONST 0x2022
#define E_F_CONST 0x2023 // extended float const
#define HEXADECI_CONST 0x2024
#define ESCAPE_SEQ_CONST 0x2025
#define ERR 0xff00
#define NOT_FOUND 0xff01

/* Keyword token */
#define AUTO 0
#define BREAK 1
#define CASE 2
#define CHAR 3
#define CONST 4
#define CONTINUE 5
#define DEFAULT 6
#define DO 7
#define DOUBLE 8
#define ELSE 9
#define ENUM 10
#define EXTERN 11
#define FLOAT 12
#define FOR 13
#define GOTO 14
#define IF 15
#define INT 16
#define LONG 17
#define REGISTER 18
#define RETURN 19
#define SHORT 20
#define SIGNED 21
#define SIZEOF 22
#define STATIC 23
#define STRUCT 24
#define SWITCH 25
#define TYPEDEF 26
#define UNION 27
#define UNSIGNED 28
#define VOID 29
#define VOLATILE 30
#define WHILE 31


struct Token
{
	int start_pos;
	int len;
	string_t str_token;
	int token_integerform;
};

void tokenizer(string_t str);
void init_tokenizer(string_t str);
int keyword_check(string_t str, bool* is_keyword);
int getTokenByPos(string_t str, int* k);
void setStringBuffer(string_t string_buf);
int getCurrentToken();
int getNextToken();
string_t getTokenString();
int getTokenPos();
bool_t MarkEndOfFile(bool_t Mark);
int lookahead();


#define NO_PREPROCESSOR 1

#ifdef NO_PREPROCESSOR
//bool is_first(string_t str, char c);


#define is_prep(i)   if(is_first(str,i, '#')) \
{                                    \
   while (at(str, i) != '\n')         \
   {	  pos++;                        \
	line_no++; }                    \
}                                    \

#endif