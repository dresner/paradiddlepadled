#include "halal/button.h"
#include "gpio.h"
#include <climits>
#include "state_machine.h"
#include "stm32f411e_discovery_accelerometer.h"

#define WRAP_DIFF_UINT(x,y) (((y) >= (x)) ? ((y) - (x)) : ((UINT_MAX) - (x) + (y)))

static const uint32_t BUTTON_QUICK_TIME = 500;
static const uint32_t BUTTON_HOLD_TIME = 1000;
static const uint32_t BUTTON_LONG_HOLD_TIME = 2000;

static volatile uint32_t button_stream_start = UINT_MAX;
volatile bool Button::accelerometer_clicked = false;
volatile uint8_t Button::button_presses = 0;

static bool button_is_pressed(void) {
	return HAL_GPIO_ReadPin(GPIO_USER_BUTTON_GPIO_Port, GPIO_USER_BUTTON_Pin);
}

void Button::handle_accelerometer_event(void) {
	State_Machine::transition_event_1();
	accelerometer_clicked = false;
}

void Button::handle_button_event(void) {
	bool hold_processed = false;
	while (true) {
		uint32_t now;
		while (button_is_pressed()) {
			now = HAL_GetTick();
			if (WRAP_DIFF_UINT(button_stream_start, now) >= BUTTON_HOLD_TIME) {
				State_Machine::transition_event_2();
				hold_processed = true;
				break;
			}
		}
		while (button_is_pressed()) {
			now = HAL_GetTick();
			if (WRAP_DIFF_UINT(button_stream_start, now) >= BUTTON_LONG_HOLD_TIME) {
				State_Machine::toggle_onoff();
				State_Machine::transition_event_2();
				hold_processed = true;
				break;
			}
		}
		if (hold_processed) {
			break;
		}

		now = HAL_GetTick();
		if (WRAP_DIFF_UINT(button_stream_start, now) >= BUTTON_QUICK_TIME) {
			if (button_presses == 1) {
				State_Machine::current_state->event_1();
				break;
			} else if (button_presses == 2) {
				State_Machine::current_state->event_2();
				break;
			}
		}
	}

	button_presses = 0;
}

static const uint32_t GPIO_DEBOUNCE_TIME = 10;
static uint8_t n_accelerometer_clicks = 0;

static void handle_accelerometer_interrupt(void) {
	static uint32_t last_tick = 0;

	uint32_t tick_now = HAL_GetTick();
	uint32_t time_diff = WRAP_DIFF_UINT(last_tick, tick_now);
	if (time_diff < GPIO_DEBOUNCE_TIME) {
		return;
	}
	last_tick = tick_now;

	if (time_diff >= BUTTON_HOLD_TIME) {
		n_accelerometer_clicks = 1;
	} else if (++n_accelerometer_clicks >= 2) {
		n_accelerometer_clicks = 0;
		Button::accelerometer_clicked = true;
	}
}

static void handle_button_interrupt(void) {
	static uint32_t last_tick = 0;
	n_accelerometer_clicks = 0;

	uint32_t tick_now = HAL_GetTick();
	uint32_t time_diff = WRAP_DIFF_UINT(last_tick, tick_now);
	if (time_diff < GPIO_DEBOUNCE_TIME) {
		return;
	}

	if (button_is_pressed()) {
		if (Button::button_presses == 0) {
			button_stream_start = tick_now;
		}
		++Button::button_presses;
	}

	last_tick = tick_now;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == GPIO_USER_BUTTON_Pin) {
		handle_button_interrupt();
	} else if(GPIO_Pin == ACCELERO_INT1_PIN) {
		handle_accelerometer_interrupt();
	}
}
