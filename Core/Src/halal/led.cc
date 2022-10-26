#include "halal/led.h"
#include "tim.h"
#include <ws2812b.h>

LED_Strip::LED_Strip(void) {
	ws2812b_init(&htim4, TIM_CHANNEL_1, NUM_LEDS);
	HAL_TIM_Base_Start(&htim4);
	HAL_TIM_PWM_Start_IT(&htim4, TIM_CHANNEL_1);
}

template<> void LED_Strip::write<LED_Strip::Section::Left>(bool onoff) {
	_left_state = onoff;
	for (unsigned int i = NUM_LEDS/2 + 1; i < NUM_LEDS; i++) {
		setLedValues(i, 0, onoff ? 10 : 0, 0);
	}
}

template<> void LED_Strip::write<LED_Strip::Section::Right>(bool onoff) {
	_right_state = onoff;
	for (unsigned int i = 0; i < NUM_LEDS/2; i++) {
		setLedValues(i, 0, onoff ? 10 : 0, 0);
	}
}
