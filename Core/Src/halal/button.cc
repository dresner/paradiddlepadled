#include "halal/button.h"
#include "gpio.h"
#include <climits>
#include "state_machine.h"

#define WRAP_DIFF_UINT(x,y) (((y) >= (x)) ? ((y) - (x)) : ((UINT_MAX) - (x) + (y)))

static const uint32_t BUTTON_QUICK_TIME = 500;
static const uint32_t BUTTON_HOLD_TIME = 1000;

static volatile uint32_t button_stream_start = UINT_MAX;
static volatile bool button_is_pressed = false;
volatile uint8_t Button::button_presses = 0;

void Button::handle_button_event(void) {
	while (true) {
		if (Button::button_presses >= 3) {
			State_Machine::transition_event_1();
			break;
		}
		uint32_t now = HAL_GetTick();
		if (button_is_pressed) {
			if (WRAP_DIFF_UINT(button_stream_start, now) >= BUTTON_HOLD_TIME) {
				State_Machine::transition_event_2();
				break;
			}
		} else {
			if (WRAP_DIFF_UINT(button_stream_start, now) >= BUTTON_QUICK_TIME) {
				if (Button::button_presses == 1) {
					State_Machine::current_state->event_1();
					break;
				} else if (button_presses == 2) {
					State_Machine::current_state->event_2();
					break;
				}
			}
		}
	}

	Button::button_presses = 0;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	static const uint32_t GPIO_DEBOUNCE_TIME = 5;
	static uint32_t last_tick = 0;

	if (GPIO_Pin != GPIO_USER_BUTTON_Pin) {
		return;
	}

	uint32_t tick_now = HAL_GetTick();
	uint32_t time_diff = WRAP_DIFF_UINT(tick_now, last_tick);
	if (time_diff < GPIO_DEBOUNCE_TIME) {
		return;
	}

	button_is_pressed = HAL_GPIO_ReadPin(GPIO_USER_BUTTON_GPIO_Port, GPIO_USER_BUTTON_Pin);
	if (button_is_pressed) {
		if (Button::button_presses == 0) {
			button_stream_start = tick_now;
		}
		++Button::button_presses;
	}

	last_tick = tick_now;
}
