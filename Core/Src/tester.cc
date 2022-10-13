#include <tester.h>
#include <cstring>
#include "halal/led.h"

Serial_Console Tester::_console;

const Tester::Test Tester::_test_table[] = {
		{"hi",   "Show greeting", Tester::greet},
		{"help", "Show this help", Tester::help},
		{"ron",  "Turn on LED indicator of Right hit", Tester::right_on},
		{"roff", "Turn off LED indicator of Right hit", Tester::right_off},
		{"lon",  "Turn on LED indicator of Left hit", Tester::left_on},
		{"loff", "Turn off LED indicator of Left hit", Tester::left_off},
};

void Tester::greet(void) {
	_console << "Hi! Welcome to Paradiddlepadled Tester v0.1" << _console.endl;
}

void Tester::help(void) {
	_console << "Available commands:" << _console.endl;
	for (const auto &t : _test_table) {
		_console << t.command << ": " << t.help << _console.endl;
	}
}

bool Tester::run_test(const std::string &command) {
	for (const auto &t : _test_table) {
		if (command == t.command) {
			t.callback();
			return true;
		}
	}
	_console << "Unrecognized command: " << command << _console.endl;
	return false;
}

void Tester::right_on(void) { LED{LED::Right{}}.on(); }
void Tester::right_off(void) { LED{LED::Right{}}.off(); }
void Tester::left_on(void) { LED{LED::Left{}}.on(); }
void Tester::left_off(void) { LED{LED::Left{}}.off(); }
