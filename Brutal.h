#pragma once

#define BRUTAL_FRAMEWORK_ENABLE 1 // 0 for disabling the framework, 1 for enabling the framework

#include <stdio.h>
#include <stdlib.h>
#include "string_t.h"

#define Brutal_concat(a,b)  (const char*)(concat_copy(a,b)->str)
static string_t currentFilename;
static void Brutal_register_file_name(string_t filename)
{
	currentFilename = copy(filename);
}

// Macro to replace exit() calls
#if BRUTAL_FRAMEWORK_ENABLE == 1
#define _exit(status) ;
#else
#define _exit(status) exit(status)
#endif


void Brutal_test_all_expr_TC_file();
void Brutal_test_all_dcl_TC_file();
void Brutal_test_all_func_TC_file();
void Brutal_test_all_stmt_TC_file();
