#ifndef INC_HALAL_LED_H_
#define INC_HALAL_LED_H_

#include <gpio.h>
#include <meta.h>

class Onboard_LED {
private:
	GPIO_TypeDef* _base;
	unsigned int _pin;
	bool _state;

public:
	enum Left {};
	enum Right {};
	enum Up {};
	enum Down {};

	Onboard_LED(const Left & left_led) : _base(GPIOD), _pin(GPIO_PIN_12) {}
	Onboard_LED(const Up & up_led) : _base(GPIOD), _pin(GPIO_PIN_13) {}
	Onboard_LED(const Right & right_led) : _base(GPIOD), _pin(GPIO_PIN_14) {}
	Onboard_LED(const Down & down_led) : _base(GPIOD), _pin(GPIO_PIN_15) {}

	void write(bool onoff) { _state = onoff; HAL_GPIO_WritePin(_base, _pin, onoff ? GPIO_PIN_SET : GPIO_PIN_RESET);	}
	void on() { write(true); }
	void off() { write(false); }
	bool toggle() { write(!_state); return _state; }
	bool state() const { return _state; }
};

class LED_Strip {
public:
	static const unsigned int NUM_LEDS = 50;

	struct Section {
		enum Left {};
		enum Right {};
	};

	LED_Strip(void);

	template<class S> void write(bool onoff);
	template<class S> void on() { write<S>(true); }
	template<class S> void off() { write<S>(false); }
	template<class S> void toggle() {
		bool& state = _state<S>();
		state = !state;
		write<S>(state);
	}
	template<class S> bool state() const { return _state<S>(); }

private:
	template <class S> bool& _state() {
		if (EQUAL<S, Section::Left>::Result) {
			return _left_state;
		} else if (EQUAL<S, Section::Right>::Result) {
			return _right_state;
		}
	}
	bool _left_state;
	bool _right_state;
};

typedef Onboard_LED LED;

#endif /* INC_HALAL_LED_H_ */
