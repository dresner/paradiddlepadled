#include "tim.h"
#include "halal/led.h"

class Paradiddle {
public:
	typedef uint8_t Step;

	Paradiddle(const Step states[]) : _head{states} {
		_current = _head;
	}

	static const Step NONE = 0;
	static const Step L = 1 << 0;
	static const Step R = 1 << 1;
	static const Step LR = L | R;
	static const Step END = -1;

	Step value() const { return *_current; }
	bool left() const { return left(*_current); }
	bool right() const { return right(*_current); }
	void set_next() {
		++_current;
		if (*_current == END) {
			_current = _head;
		}
	}

	static bool left(Step value) { return value & L; }
	static bool right(Step value) { return value & R; }

private:
	const Step * _current;
	const Step * _head;
};

const auto L = Paradiddle::L;
const auto R = Paradiddle::R;
const auto END = Paradiddle::END;

const Paradiddle::Step s1[] {R, L, R, R, L, R, L, L, END};
Paradiddle paradiddle {s1};

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	static LED right_led{LED::Right{}};
	static LED left_led{LED::Left{}};

	const auto value = paradiddle.value();
	paradiddle.set_next();

	left_led.write(Paradiddle::left(value));
	right_led.write(Paradiddle::right(value));
}

