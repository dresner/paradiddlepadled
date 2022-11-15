#include "tester.h"
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
	Tester tester;
	tester.run_test("mon");
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

void State_Machine::event_bpm_up(void) {
	Timer::bpm_up(4);
	LED_Strip::get_instance()->reset_color();
}
void State_Machine::event_bpm_down(void) {
	Timer::bpm_down(4);
	LED_Strip::get_instance()->reset_color();
}
void State_Machine::event_pattern_next(void) {
    Paradiddle::next();
	LED_Strip::get_instance()->reset_color();
}
void State_Machine::event_pattern_previous(void) {
	Paradiddle::previous();
	LED_Strip::get_instance()->reset_color();
}
