#pragma once
//#include "CLexer.h"
#include "string_t.h"


int is_constant(string_t s, int* type);
int is_id(string_t s);

long ConvertHextoDec(string_t hexval);

long ConvertOcttoDec(string_t octval);

long ConvertStrtoDec(string_t str);

double ConvertStrtoDouble(string_t str);

//ConvertExpoStrtoDouble(int current_ptr,int offset);

char ConvertStrtoASCII(string_t str);

//char* StrCopy(string_t str);


