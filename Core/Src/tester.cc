#include <tester.h>
#include <cstring>
#include "halal/led.h"
#include "halal/timer.h"
#include "tim.h"

Serial_Console Tester::_console;

const Tester::Test Tester::_test_table[] = {
		{"hi",   "Show greeting", Tester::greet},
		{"help", "Show this help", Tester::help},
		{"ron",  "Turn on LED indicator of Right hit", Tester::right_on},
		{"roff", "Turn off LED indicator of Right hit", Tester::right_off},
		{"lon",  "Turn on LED indicator of Left hit", Tester::left_on},
		{"loff", "Turn off LED indicator of Left hit", Tester::left_off},
		{"mon",  "Start metronome", Tester::metronome_on},
		{"moff", "Stop metronome", Tester::metronome_off},
		{"m+",  "Increase metronome BPM by 10", Tester::metronome_up},
		{"m-", "Decrease metronome BPM by 10", Tester::metronome_down},
		{"m+1",  "Increase metronome BPM by 1", Tester::metronome_up_one},
		{"m-1", "Decrease metronome BPM by 1", Tester::metronome_down_one},
};


LED r{LED::Right{}};
LED l{LED::Left{}};
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	l.toggle();
	r.toggle();
}

void Tester::main_loop(void) {
	greet();
	l.on();
	r.off();

	while (true) {
		std::string command;
		_console >> command;
		if (!run_test(command)) {
			_console << "Unrecognized command: " << command << _console.endl;
		}
	}
}

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
	return false;
}

void Tester::right_on(void) { LED{LED::Right{}}.on(); }
void Tester::right_off(void) { LED{LED::Right{}}.off(); }
void Tester::left_on(void) { LED{LED::Left{}}.on(); }
void Tester::left_off(void) { LED{LED::Left{}}.off(); }

void Tester::metronome_on(void) { Timer::start(); }
void Tester::metronome_off(void) { Timer::stop(); }
void Tester::metronome_up(void) { Timer::bpm_up(10); }
void Tester::metronome_down(void) { Timer::bpm_down(10); }
void Tester::metronome_up_one(void) { Timer::bpm_up(1); }
void Tester::metronome_down_one(void) { Timer::bpm_down(1); }
