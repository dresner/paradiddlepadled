#include "tester.h"
#include "adc.h"
#include "halal/timer.h"
#include "halal/button.h"
#include "halal/led.h"
#include "paradiddle.h"
#include "state_machine.h"

#ifdef __cplusplus
extern "C" {
#endif
void paradiddlepadled_main(void);
#ifdef __cplusplus
}
#endif

void paradiddlepadled_main(void) {
	State_Machine::reset();
	State_Machine::toggle_onoff();

	Tester tester;

	while(true) {
		Serial_Console console;
		if (console.has_data()) {
			tester.read_console();
		}
		if (Button::button_was_pressed()) {
			Button::handle_button_event();
		}
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
void State_Machine::event_bpm_up(void) {
	Timer::bpm_up(4);
	LED_Strip::get_instance()->reset_color();
}
void State_Machine::event_bpm_down(void) {
	Timer::bpm_down(4);
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
