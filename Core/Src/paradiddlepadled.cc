#include "tester.h"
#include "adc.h"
#include "halal/timer.h"
#include "halal/button.h"
#include "halal/led.h"
#include "paradiddle.h"
#include "state_machine.h"
#include "stm32f411e_discovery_accelerometer.h"

#ifdef __cplusplus
extern "C" {
#endif
void paradiddlepadled_main(void);
#ifdef __cplusplus
}
#endif

void paradiddlepadled_main(void) {
	/* Initialize MEMS Accelerometer mounted on STM32F4-Discovery board */
	if(BSP_ACCELERO_Init() != ACCELERO_OK)
	{
		/* Initialization Error */
		Error_Handler();
	}

	/* Enable click config for pause/play */
	BSP_ACCELERO_Click_ITConfig();
	State_Machine::reset();
	State_Machine::toggle_onoff();

	Tester tester;
	while(true) {
		Serial_Console console;
		if (console.has_data()) {
			tester.read_console();
		}
		Button::check_events();
	}
}

void State_Machine::toggle_onoff(void) {
	static bool on = false;
	on = !on;
	if (on) {
		HAL_ADC_Start(&hadc1);
		Timer::start();
	} else {
		Timer::stop();
		HAL_ADC_Stop(&hadc1);
	}
}

static const uint8_t BPM_ADJUST = 10;

void State_Machine::event_bpm_up(void) {
	Timer::bpm_up(BPM_ADJUST);
	LED_Strip::get_instance()->reset_color();
}
void State_Machine::event_bpm_down(void) {
	Timer::bpm_down(BPM_ADJUST);
	LED_Strip::get_instance()->reset_color();
}
void State_Machine::event_pattern_next(void) {
    Paradiddle::next_pattern();
	LED_Strip::get_instance()->reset_color();
}
void State_Machine::event_pattern_previous(void) {
	Paradiddle::previous_pattern();
	LED_Strip::get_instance()->reset_color();
}
