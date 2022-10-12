#include <tester.h>
#include <cstring>

Serial_Console Tester::_console;

const Tester::Test Tester::_test_table[] = {
		{"hi",   "Show greeting", Tester::greet},
		{"help", "Show this help", Tester::help},
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
