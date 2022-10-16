#include "tester.h"

#ifdef __cplusplus
extern "C" {
#endif
void paradiddlepadled_main(void);
#ifdef __cplusplus
}
#endif

void paradiddlepadled_main(void) {
	Tester::run_test("mon");
	Tester::main_loop();
}
