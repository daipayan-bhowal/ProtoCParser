#include "Brutal.h"

int main(int argc, char* argv[])
{
	Brutal_init();
	Brutal_test_all_expr_TC_file();
	Brutal_all_test_case_stats();
	//Brutal_test_all_dcl_TC_file();
	Brutal_test_all_func_TC_file();
	Brutal_all_test_case_stats();
	Brutal_test_all_stmt_TC_file();
	Brutal_all_test_case_stats();
	
	return 0;
}
