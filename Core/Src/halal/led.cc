#include "halal/led.h"
#include "tim.h"
#include <ws2812b.h>

extern bool is_dirty;
extern uint8_t *led_value;
uint8_t led_value_l[LED_Strip::NUM_LEDS * 3] = {0,};
uint8_t led_value_r[LED_Strip::NUM_LEDS * 3] = {0,};
uint8_t led_value_lr[LED_Strip::NUM_LEDS * 3] = {0,};
uint8_t led_value_off[LED_Strip::NUM_LEDS * 3] = {0,};

LED_Strip::LED_Strip(void) {
	for (unsigned int led = NUM_LEDS/2 + 1; led < NUM_LEDS; led++) {
		led_value_l[3 * led + RL] = 0;
		led_value_l[3 * led + GL] = 10;
		led_value_l[3 * led + BL] = 0;
	}
	for (unsigned int led = 0; led < NUM_LEDS/2; led++) {
		led_value_r[3 * led + RL] = 0;
		led_value_r[3 * led + GL] = 10;
		led_value_r[3 * led + BL] = 0;
	}
	for (unsigned int led = 0; led < NUM_LEDS; led++) {
		led_value_lr[3 * led + RL] = 0;
		led_value_lr[3 * led + GL] = 10;
		led_value_lr[3 * led + BL] = 0;
	}
	ws2812b_init(&htim4, TIM_CHANNEL_1, NUM_LEDS);
	HAL_TIM_Base_Start(&htim4);
	HAL_TIM_PWM_Start_IT(&htim4, TIM_CHANNEL_1);
}

void LED_Strip::_write_to_strip() {
	if (!_left_state && !_right_state) {
		led_value = led_value_off;
	}
	else if (_left_state) {
		if (_right_state) {
			led_value = led_value_lr;
		} else {
			led_value = led_value_l;
		}
	} else if (_right_state) {
		led_value = led_value_r;
	}
	is_dirty = true;
}
