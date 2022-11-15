#ifndef INC_PARADIDDLE_H_
#define INC_PARADIDDLE_H_

#include "halal/led.h"

class Paradiddle {
public:
	typedef uint8_t Step;

	Paradiddle(const Step states[]);

	static const Step NONE = 0;
	static const Step L = 1 << 0;
	static const Step R = 1 << 1;
	static const Step LR = L | R;
	static const Step END = -1;

	Step value() const { return *_current_step; }
	bool left() const { return left(*_current_step); }
	bool right() const { return right(*_current_step); }
	void set_next() {
		auto next = _current_step + 1;
		_current_step = (*next == END) ? _head : next;
	}
	void reset() { _current_step = _head; }

	static bool left(Step value) { return value & L; }
	static bool right(Step value) { return value & R; }

	static Paradiddle * current(void) { return _current_pattern; }
	static void next(void) {
		_current_pattern = _current_pattern->_next_pattern;
		_current_pattern->reset();
	}
	static void previous(void) {
		_current_pattern = _current_pattern->_previous_pattern;
		_current_pattern->reset();
	}

	static void metronome_fall(void) { metronome_led.write(false); }
	static void metronome_rise(void) { metronome_led.write(true); }

	static void step_rise(void);
	static void step_fall(void);

private:
	const Step * _current_step;
	const Step * _head;

	Paradiddle * _previous_pattern;
	Paradiddle * _next_pattern;

	static Paradiddle * _current_pattern;
	static Paradiddle * _last_pattern;

	static LED left_led;
	static LED right_led;
	static LED metronome_led;
	static LED pattern_start_led;
};

#endif /* INC_PARADIDDLE_H_ */
