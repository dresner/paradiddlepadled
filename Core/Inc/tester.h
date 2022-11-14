#ifndef INC_TESTER_H_
#define INC_TESTER_H_

#include <string>
#include "halal/serial_console.h"

class Tester {
public:
	static void greet(void);

	static bool run_test(const std::string &command);
	static void main_loop(void);

private:
	static void help(void);
	static void right_on(void);
	static void right_off(void);
	static void left_on(void);
	static void left_off(void);
	static void metronome_on(void);
	static void metronome_off(void);
	static void metronome_up(void);
	static void metronome_down(void);
	static void metronome_up_one(void);
	static void metronome_down_one(void);
	static void adc_dump_start(void);
	static void play_wav_sample(void);
	static void more_green(void);
	static void more_red(void);

	struct Test {
		const char *command;
		const char *help;
		void (*callback)(void);
	};

	const static Test _test_table[];

	static Serial_Console _console;
};

#endif /* INC_TESTER_H_ */
