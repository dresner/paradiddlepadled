#include "tim.h"
#include "halal/led.h"
#include "paradiddle.h"
#include <climits>
#include "state_machine.h"

#define WRAP_DIFF_UINT(x,y) (((x) < (y)) ? ((x) - (y)) : ((UINT_MAX) - (y) + (x)))

static const uint32_t CORRECT_HIT_DELAY = 100;

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

void HAL_ADC_ErrorCallback(ADC_HandleTypeDef* hadc) {
	__HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_EOC);
}

static volatile uint32_t last_hit_tick = UINT_MAX;
static volatile uint32_t last_rise_ticks[2] = {0,0};

void HAL_ADC_LevelOutOfWindowCallback(ADC_HandleTypeDef* hadc) {
	last_hit_tick = HAL_GetTick();
}

void Paradiddle::step_rise(void) {
	static uint8_t steps_count = 0;

	last_rise_ticks[0] = last_rise_ticks[1];
	last_rise_ticks[1] = HAL_GetTick();
	auto pattern = current();
	bool start = pattern->_current_step == pattern->_head;
	const auto value = pattern->value();
	pattern->set_next();

	bool light_up = true;

	if (State_Machine::is_loose(State_Machine::current_state->state)) {
		if (start) {
			steps_count = 0;
		} else {
			++steps_count;
		}
		if (steps_count % 4) {
			light_up = false;
		}
	}

	if (light_up) {
		auto strip = LED_Strip::get_instance();
		strip->write<LED_Strip::Section::Left>(Paradiddle::left(value));
		strip->write<LED_Strip::Section::Right>(Paradiddle::right(value));
	}
}

void Paradiddle::step_fall(void) {
	auto strip = LED_Strip::get_instance();
	strip->off<LED_Strip::Section::Left>();
	strip->off<LED_Strip::Section::Right>();
	uint32_t time_diff0, time_diff1;

	time_diff0 = WRAP_DIFF_UINT(last_hit_tick, last_rise_ticks[0]);
	if (time_diff0 <= CORRECT_HIT_DELAY) {
		strip->more_green();
	} else {
		time_diff1 = WRAP_DIFF_UINT(last_hit_tick, last_rise_ticks[1]);
		if (time_diff1 <= CORRECT_HIT_DELAY) {
			strip->more_green();
		} else {
			strip->more_red();
		}
	}
}
