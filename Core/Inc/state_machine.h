#ifndef INC_STATE_MACHINE_H_
#define INC_STATE_MACHINE_H_

#include "halal/led.h"

class State_Machine {
public:
	enum Mask {
		CONTROL_MASK = 1 << 0,
		LEDS_MASK    = 1 << 1,
	};

	enum State {
		BPM_GUIDED     = 0,
		BPM_LOOSE      = LEDS_MASK,
		PATTERN_GUIDED = CONTROL_MASK,
		PATTERN_LOOSE  = CONTROL_MASK | LEDS_MASK,
	};

	static bool is_loose(const State s) { return s & LEDS_MASK; }
	static bool is_guided(const State s) { return !is_loose(s); }
	static bool is_pattern(const State s) { return s & CONTROL_MASK; }
	static bool is_bpm(const State s) { return !is_pattern(s); }

	struct State_Action {
		const State state;
		LED led_1;
		LED led_2;
		void (*const event_1)(void);
		void (*const event_2)(void);
		State_Action * transition_event_1;
		State_Action * transition_event_2;
	};

	static State_Action state_machine[4];

	static State_Action * volatile current_state;

	static void reset() {
		for (auto &i : state_machine) {
			i.led_1.off();
			i.led_2.off();
		}
		current_state = &state_machine[0];
		current_state->led_1.on();
		current_state->led_2.on();
	}

	static void transition_event_1(void) {
		current_state->led_1.off();
		current_state->led_2.off();
		current_state = current_state->transition_event_1;
		current_state->led_1.on();
		current_state->led_2.on();
	}
	static void transition_event_2(void) {
		current_state->led_1.off();
		current_state->led_2.off();
		current_state = current_state->transition_event_2;
		current_state->led_1.on();
		current_state->led_2.on();
	}

	static void event_bpm_up(void);
	static void event_bpm_down(void);
	static void event_pattern_next(void);
	static void event_pattern_previous(void);
};

#endif /* INC_STATE_MACHINE_H_ */
