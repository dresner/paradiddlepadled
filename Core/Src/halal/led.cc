#include "halal/led.h"
#include "halal/timer.h"
#include <ws2812b.h>

static const uint8_t NUM_ACCURACY_LEVELS = 10;
static const uint8_t LED_VALUE_STEP = 5;
static const uint8_t LED_VALUE_MAX = (NUM_ACCURACY_LEVELS-1) * LED_VALUE_STEP;

extern bool is_dirty;
extern volatile uint8_t *led_value;
static uint8_t led_value_l[NUM_ACCURACY_LEVELS][LED_Strip::NUM_LEDS * 3];
static uint8_t led_value_r[NUM_ACCURACY_LEVELS][LED_Strip::NUM_LEDS * 3];
static uint8_t led_value_off[LED_Strip::NUM_LEDS * 3];
static volatile uint8_t *led_value_l_now;
static volatile uint8_t *led_value_r_now;
LED_Strip *LED_Strip::_instance;

void LED_Strip::_set_strip_values(void) {
	led_value_l_now = led_value_l[_current_accuracy_level];
	led_value_r_now = led_value_r[_current_accuracy_level];
}

void LED_Strip::reset_color() {
	_current_accuracy_level = NUM_ACCURACY_LEVELS / 2 - 1;
	led_value_l_now = led_value_l[_current_accuracy_level];
	led_value_r_now = led_value_r[_current_accuracy_level];
	_set_strip_values();
}

void LED_Strip::more_red() {
	if (_current_accuracy_level < NUM_ACCURACY_LEVELS - 1) {
		++_current_accuracy_level;
		_set_strip_values();
	}
}

void LED_Strip::more_green() {
	if (_current_accuracy_level > 0) {
		--_current_accuracy_level;
		_set_strip_values();
	}
}

LED_Strip::LED_Strip(void) {
	reset_color();
	for (unsigned int level = 0; level < NUM_ACCURACY_LEVELS; level++) {
		for (unsigned int led = 0; led < NUM_LEDS; led++) {
			led_value_r[level][3 * led + GL] = 0;
			led_value_r[level][3 * led + RL] = 0;
			led_value_r[level][3 * led + BL] = 0;
			led_value_l[level][3 * led + GL] = 0;
			led_value_l[level][3 * led + RL] = 0;
			led_value_l[level][3 * led + BL] = 0;
		}
	}
	for (unsigned int level = 0; level < NUM_ACCURACY_LEVELS; level++) {
		uint8_t red_value = LED_VALUE_STEP * level;
		uint8_t green_value = LED_VALUE_MAX - red_value;
		for (unsigned int led = 0; led < NUM_LEDS/2; led++) {
			led_value_l[level][3 * led + GL] = green_value;
			led_value_l[level][3 * led + RL] = red_value;
		}
		for (unsigned int led = NUM_LEDS/2; led < NUM_LEDS; led++) {
			led_value_r[level][3 * led + GL] = green_value;
			led_value_r[level][3 * led + RL] = red_value;
		}
	}
	ws2812b_init(TIM_LED_STRIP, TIM_CHANNEL_LED, NUM_LEDS);
	_write_to_strip();
	HAL_TIM_Base_Start(TIM_LED_STRIP);
	HAL_TIM_PWM_Start_IT(TIM_LED_STRIP, TIM_CHANNEL_LED);
}

void LED_Strip::_write_to_strip() {
	if (!_left_state && !_right_state) {
		led_value = led_value_off;
	} else if (_left_state) {
		led_value = led_value_l_now;
	} else if (_right_state) {
		led_value = led_value_r_now;
	}
	is_dirty = true;
}
