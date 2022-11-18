#include "tim.h"
#include "halal/led.h"
#include "paradiddle.h"
#include <climits>
#include "state_machine.h"

static const uint32_t CORRECT_HIT_DELAY = 10;

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

static const auto L = Paradiddle::L;
static const auto R = Paradiddle::R;
static const auto END = Paradiddle::END;

static Paradiddle p1 {(const Paradiddle::Step[]){R, L, R, R, L, R, L, L, END}};
static Paradiddle p2 {(const Paradiddle::Step[]){R, L, R, R, L, L, END}};
static Paradiddle p3 {(const Paradiddle::Step[]){L, R, L, L, R, R, END}};

static volatile uint32_t last_hit_tick = UINT_MAX;
static volatile uint32_t last_rise_ticks[2] = {0,0};
static volatile uint8_t steps_count = 0;

// Not a generic circular buffer.
// It fulfills specific assumptions and requirements for usage in this file
template<typename T, size_t Size>
class Circular_Buffer {
public:
	Circular_Buffer() : _write_head{0}, _read_head{0} {}

	void operator <<(const T item) {
		_data[_write_head] = item;
		_write_head = (_write_head + 1) % Size;
	}
	void operator >>(T& item) {
		item = _data[_read_head];
		_read_head = (_read_head + 1) % Size;
	}
	T get_previous() {
		if (_read_head == 0) {
			return _data[Size-1];
		}
		return _data[_read_head - 1];
	}
	size_t write_head() { return _write_head; }
	size_t read_head() { return _read_head; }

private:
	T _data[Size];
	volatile size_t _write_head;
	volatile size_t _read_head;
};
static Circular_Buffer<uint32_t, 32> hit_timestamps {};
static Circular_Buffer<uint32_t, 2> rise_timestamps {};
static volatile size_t last_hit_index {0};

static uint32_t DEBOUNCE_ADC_DELAY = 10;

void HAL_ADC_LevelOutOfWindowCallback(ADC_HandleTypeDef* hadc) {
	static uint32_t last_tick = 0;
	uint32_t tick = HAL_GetTick();
	if (tick <= last_tick + DEBOUNCE_ADC_DELAY) return;
	last_tick = tick;
	hit_timestamps << HAL_GetTick();
}

void HAL_ADC_ErrorCallback(ADC_HandleTypeDef* hadc) {
	__HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_EOC);
}

void Paradiddle::reset_step() {
	_current_step = _head;
	last_hit_tick = UINT_MAX;
	last_rise_ticks[0] = 0;
	last_rise_ticks[1] = 0;
	steps_count = 0;
}

void Paradiddle::step_rise(void) {
	rise_timestamps << HAL_GetTick();
	last_hit_index = hit_timestamps.write_head();
	auto pattern = current_pattern();
	const auto value = pattern->value();
	pattern->next_step();

	bool light_up = true;

	if (State_Machine::is_loose(State_Machine::current_state->state)) {
		if (steps_count != 0) {
			light_up = false;
		}
		steps_count = (steps_count + 1) % 4;
	}

	if (light_up) {
		auto strip = LED_Strip::get_instance();
		strip->write<LED_Strip::Section::Left>(Paradiddle::left(value));
		strip->write<LED_Strip::Section::Right>(Paradiddle::right(value));
	}
}

void Paradiddle::step_fall(void) {
	uint32_t tick1, tick2;
	tick1 = rise_timestamps.get_previous();
	rise_timestamps >> tick2;

	auto strip = LED_Strip::get_instance();
	strip->off<LED_Strip::Section::Left>();
	strip->off<LED_Strip::Section::Right>();

	volatile size_t t;
    while ((t = hit_timestamps.read_head()) != last_hit_index) {
    	uint32_t hit;
    	hit_timestamps >> hit;
    	// Not considering timestamp wrap. Should be rare and low-impact enough not to matter.
    	if (hit <= tick1 + CORRECT_HIT_DELAY) {
    		strip->more_green();
    	} else if (hit >= tick2 - CORRECT_HIT_DELAY) {
    		strip->more_green();
    	} else {
    		strip->more_red();
    	}
    }
}
