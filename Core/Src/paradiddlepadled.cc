#include "tester.h"
#include "gpio.h"
#include "halal/timer.h"
#include "paradiddle.h"
#include <climits>

#ifdef __cplusplus
extern "C" {
#endif
void paradiddlepadled_main(void);
#ifdef __cplusplus
}
#endif

void paradiddlepadled_main(void) {
	Tester::run_test("mon");
	Tester::main_loop();
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	static const uint32_t GPIO_DEBOUNCE_TIME = 3;
	static const uint32_t GPIO_QUICK_TIME = 400;
	static const uint32_t GPIO_HOLD_TIME = 1000;
	static uint32_t last_tick = 0;
	static uint32_t last_up_tick = 0;

	if (GPIO_Pin != GPIO_USER_BUTTON_Pin) {
		return;
	}

	uint32_t tick_now = HAL_GetTick();
	uint32_t time_diff = (tick_now >= last_tick) ? (tick_now - last_tick) : (UINT_MAX - last_tick + tick_now);
	if (time_diff < GPIO_DEBOUNCE_TIME) {
		return;
	}

	last_tick = tick_now;
	GPIO_PinState state = HAL_GPIO_ReadPin(GPIO_USER_BUTTON_GPIO_Port, GPIO_USER_BUTTON_Pin);
	if (state == GPIO_PIN_SET) {
		uint32_t up_time_diff = (tick_now >= last_up_tick) ? (tick_now - last_up_tick) : (UINT_MAX - last_up_tick + tick_now);
		last_up_tick = tick_now;
		if (up_time_diff > GPIO_QUICK_TIME) {
			Timer::bpm_up(4);
			LED_Strip::get_instance()->reset_color();
		} else {
			Timer::bpm_down(4);
			LED_Strip::get_instance()->reset_color();
		}
	} else {
		if (time_diff >= GPIO_HOLD_TIME) {
			Paradiddle::next();
			LED_Strip::get_instance()->reset_color();
		}
	}
}
