#include "tim.h"
#include "halal/led.h"
#include "paradiddle.h"

const auto L = Paradiddle::L;
const auto R = Paradiddle::R;
const auto LR = Paradiddle::LR;
const auto END = Paradiddle::END;

Paradiddle * Paradiddle::_current_pattern = NULL;
Paradiddle * Paradiddle::_last_pattern = NULL;

LED Paradiddle::right_led{LED::Right{}};
LED Paradiddle::left_led{LED::Left{}};
LED Paradiddle::metronome_led{LED::Up{}};
LED Paradiddle::pattern_start_led{LED::Down{}};
LED_Strip Paradiddle::led_strip{};

Paradiddle::Paradiddle(const Step states[]): _head{states} {
	_current_step = _head;

	if (_current_pattern == NULL) {
		_next_pattern = this;
		_previous_pattern = this;
		_current_pattern = this;
		_last_pattern = this;
	} else {
		_next_pattern = _last_pattern->_next_pattern;
		_previous_pattern = _last_pattern;
		_previous_pattern->_next_pattern = this;
		_next_pattern->_previous_pattern = this;
		_last_pattern = this;
	}
}

const Paradiddle::Step s1[] {R, L, R, R, L, R, L, L, END};
const Paradiddle::Step s2[] {R, L, R, R, L, L, END};
const Paradiddle::Step s3[] {L, R, L, L, R, R, END};
Paradiddle p1 {s1};
Paradiddle p2 {s2};
Paradiddle p3 {s3};

