#include <halal/serial_console.h>

#ifdef __cplusplus
extern "C" {
#endif
void paradiddlepadled_main(void);
#ifdef __cplusplus
}
#endif

void paradiddlepadled_main(void) {
	Serial_Console c;

	c << "Hello!" << c.endl;

	while (true) {
		std::string in;
		c >> in;
		c << in << c.endl;
	}
}
