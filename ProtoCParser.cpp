#include "Brutal.h"

int main(int argc, char* argv[])
{
#if BRUTAL_FRAMEWORK_ENABLE == 1
	Brutal_test_all_expr_TC_file();
	Brutal_test_all_dcl_TC_file();
	Brutal_test_all_func_TC_file();
	Brutal_test_all_stmt_TC_file();
#endif
	return 0;
}
