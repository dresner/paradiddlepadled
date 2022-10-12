#include <halal/serial_console.h>
#include "tester.h"

#ifdef __cplusplus
extern "C" {
#endif
void paradiddlepadled_main(void);
#ifdef __cplusplus
}
#endif

void paradiddlepadled_main(void) {
	Serial_Console c;
	Tester t;

	c << "Hello!" << c.endl;
	t.greet();

	while (true) {
		std::string in;
		c >> in;
		t.run_test(in);
	}
}
