#pragma once
/* Brutal Test Framework
	for C compiler testing
*/
#define BRUTAL_FRAMEWORK_ENABLE 0 // 0 for disabling the framework, 1 for enabling the framework

#include <stdio.h>
#include <stdlib.h>
#include "string_t.h"
#include "bool_t.h"

#define Brutal_concat(a,b)  (const char*)(concat_copy(a,b)->str)
static string_t currentFilename;
static bool_t TCFail;

void UpdateTcFail();
void ResetTcFail();

static void Brutal_register_file_name(string_t filename)
{
	currentFilename = copy(filename);
}

// Macro to replace exit() calls
#if BRUTAL_FRAMEWORK_ENABLE == 1
#define _exit(status) UpdateTcFail(); return t;
#else
#define _exit(status) exit(status)
#endif

void Brutal_init();
void Brutal_all_test_case_stats();
void Brutal_test_all_expr_TC_file();
void Brutal_test_all_dcl_TC_file();
void Brutal_test_all_func_TC_file();
void Brutal_test_all_stmt_TC_file();
