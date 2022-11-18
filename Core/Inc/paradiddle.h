#ifndef INC_PARADIDDLE_H_
#define INC_PARADIDDLE_H_

#include "halal/led.h"

class Paradiddle {
public:
	typedef uint8_t Step;

	static const Step NONE = 0;
	static const Step L = 1 << 0;
	static const Step R = 1 << 1;
	static const Step LR = L | R;
	static const Step END = -1;

	static bool left(Step value) { return value & L; }
	static bool right(Step value) { return value & R; }

	static Paradiddle * current_pattern(void) { return _current_pattern; }
	static void next_pattern(void) {
		_current_pattern = _current_pattern->_next_pattern;
		_current_pattern->reset_step();
	}
	static void previous_pattern(void) {
		_current_pattern = _current_pattern->_previous_pattern;
		_current_pattern->reset_step();
	}

	static void metronome_fall(void) { }
	static void metronome_rise(void) { }

	static void step_rise(void);
	static void step_fall(void);

public:
	Paradiddle(const Step states[]);

	Step value() const { return *_current_step; }
	bool left() const { return left(*_current_step); }
	bool right() const { return right(*_current_step); }
	void next_step() {
		auto next = _current_step + 1;
		_current_step = (*next == END) ? _head : next;
	}
	void reset_step();

private:
	const Step * _current_step;
	const Step * _head;

	Paradiddle * _previous_pattern;
	Paradiddle * _next_pattern;

private:
	static Paradiddle * _current_pattern;
	static Paradiddle * _last_pattern;
};

#endif /* INC_PARADIDDLE_H_ */
