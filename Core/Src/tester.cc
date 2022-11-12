#include <tester.h>
#include <cstring>
#include "halal/led.h"
#include "halal/timer.h"
#include "tim.h"
#include "paradiddle.h"
#include "adc.h"

Serial_Console Tester::_console;

const Tester::Test Tester::_test_table[] = {
		{"hi",   "Show greeting", Tester::greet},
		{"help", "Show this help", Tester::help},
		{"ron",  "Turn on LED indicator of Right hit", Tester::right_on},
		{"roff", "Turn off LED indicator of Right hit", Tester::right_off},
		{"lon",  "Turn on LED indicator of Left hit", Tester::left_on},
		{"loff", "Turn off LED indicator of Left hit", Tester::left_off},
		{"mon",  "Start metronome", Timer::start},
		{"moff", "Stop metronome", Timer::stop},
		{"m+",   "Increase metronome BPM by 10", Tester::metronome_up},
		{"m-",   "Decrease metronome BPM by 10", Tester::metronome_down},
		{"m+1",  "Increase metronome BPM by 1", Tester::metronome_up_one},
		{"m-1",  "Decrease metronome BPM by 1", Tester::metronome_down_one},
		{"pn",   "Go to next paradiddle pattern", Paradiddle::next},
		{"pp",   "Go to previous paradiddle pattern", Paradiddle::previous},
		{"ads",  "ADC dump start", Tester::adc_dump_start},
};

LED_Strip *led_strip;

void Tester::main_loop(void) {
	greet();
	led_strip = new LED_Strip();
	led_strip->off<LED_Strip::Section::Left>();
	led_strip->off<LED_Strip::Section::Right>();

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

void Tester::right_on(void) {
	led_strip->on<LED_Strip::Section::Right>();
	LED{LED::Right{}}.on();
}
void Tester::right_off(void) {
	led_strip->off<LED_Strip::Section::Right>();
	LED{LED::Right{}}.off();
}
void Tester::left_on(void) {
	led_strip->on<LED_Strip::Section::Left>();
	LED{LED::Left{}}.on();
}
void Tester::left_off(void) {
	led_strip->off<LED_Strip::Section::Left>();
	LED{LED::Left{}}.off();
}

void Tester::metronome_up(void) { Timer::bpm_up(10); }
void Tester::metronome_down(void) { Timer::bpm_down(10); }
void Tester::metronome_up_one(void) { Timer::bpm_up(1); }
void Tester::metronome_down_one(void) { Timer::bpm_down(1); }

void Tester::adc_dump_start(void) {
	while(true) {
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
		uint32_t value = HAL_ADC_GetValue(&hadc1);
		HAL_ADC_Stop(&hadc1);
		_console << std::to_string(value) << _console.endl;
	}
}
