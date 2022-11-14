#include "halal/led.h"
#include "tim.h"
#include <ws2812b.h>

extern bool is_dirty;
extern uint8_t *led_value;
uint8_t led_value_l[LED_Strip::NUM_LEDS * 3] = {0,};
uint8_t led_value_r[LED_Strip::NUM_LEDS * 3] = {0,};
uint8_t led_value_off[LED_Strip::NUM_LEDS * 3] = {0,};
LED_Strip *LED_Strip::_instance;

static const uint8_t LED_VALUE_STEP = 30;
static const uint8_t LED_VALUE_MAX = 127;

void LED_Strip::_set_strip_values(void) {
	for (unsigned int led = 0; led < NUM_LEDS/2; led++) {
		led_value_r[3 * led + RL] = _red_value;
		led_value_r[3 * led + GL] = _green_value;
	}
	for (unsigned int led = NUM_LEDS/2 + 1; led < NUM_LEDS; led++) {
		led_value_l[3 * led + RL] = _red_value;
		led_value_l[3 * led + GL] = _green_value;
	}
	is_dirty = true;
}

void LED_Strip::reset_color() {
	_green_value = LED_VALUE_MAX;
	_red_value = 0;
	_set_strip_values();
}

void LED_Strip::more_red() {
	if (_green_value == 0) {
		return;
	}
	if (_green_value <= LED_VALUE_STEP) {
		_green_value = 0;
	} else {
		_green_value -= LED_VALUE_STEP;
	}
	_red_value = LED_VALUE_MAX - _green_value;
	_set_strip_values();
}

void LED_Strip::more_green() {
	if (_green_value == LED_VALUE_MAX) {
		return;
	}
	if (_green_value >= LED_VALUE_MAX - LED_VALUE_STEP) {
		_green_value = LED_VALUE_MAX;
	} else {
		_green_value += LED_VALUE_STEP;
	}
	_red_value = LED_VALUE_MAX - _green_value;
	_set_strip_values();
}

LED_Strip::LED_Strip(void) {
	reset_color();
	for (unsigned int led = 0; led < NUM_LEDS; led++) {
		led_value_r[3 * led + BL] = 0;
	}
	ws2812b_init(&htim4, TIM_CHANNEL_1, NUM_LEDS);
	HAL_TIM_Base_Start(&htim4);
	HAL_TIM_PWM_Start_IT(&htim4, TIM_CHANNEL_1);
}

void LED_Strip::_write_to_strip() {
	if (!_left_state && !_right_state) {
		led_value = led_value_off;
	} else if (_left_state) {
		led_value = led_value_l;
	} else if (_right_state) {
		led_value = led_value_r;
	}
	is_dirty = true;
}
