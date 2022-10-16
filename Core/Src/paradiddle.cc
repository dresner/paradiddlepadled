#include "tim.h"
#include "halal/led.h"
#include "paradiddle.h"

const auto L = Paradiddle::L;
const auto R = Paradiddle::R;
const auto LR = Paradiddle::LR;
const auto END = Paradiddle::END;

Paradiddle * Paradiddle::_current_pattern = NULL;
Paradiddle * Paradiddle::_last_pattern = NULL;

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

LED right_led{LED::Right{}};
LED left_led{LED::Left{}};

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	auto pattern = Paradiddle::current();
	const auto value = pattern->value();
	pattern->set_next();

	left_led.write(Paradiddle::left(value));
	right_led.write(Paradiddle::right(value));
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
	left_led.write(false);
	right_led.write(false);
}
