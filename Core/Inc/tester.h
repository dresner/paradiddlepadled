#ifndef INC_TESTER_H_
#define INC_TESTER_H_

#include <string>
#include "halal/serial_console.h"

class Tester {
public:
	static void greet(void);

	static bool run_test(const std::string &command);

private:
	static void help(void);

	struct Test {
		const char *command;
		const char *help;
		void (*callback)(void);
	};

	const static Test _test_table[];

	static Serial_Console _console;
};

#endif /* INC_TESTER_H_ */
