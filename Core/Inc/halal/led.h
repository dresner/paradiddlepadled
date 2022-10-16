#ifndef INC_HALAL_LED_H_
#define INC_HALAL_LED_H_

#include <gpio.h>

class LED {
private:
	GPIO_TypeDef* _base;
	unsigned int _pin;
	bool _state;

public:
	enum Left {};
	enum Right {};
	enum Up {};
	enum Down {};

	LED(const Left & left_led) : _base(GPIOD), _pin(GPIO_PIN_12) {}
	LED(const Up & up_led) : _base(GPIOD), _pin(GPIO_PIN_13) {}
	LED(const Right & right_led) : _base(GPIOD), _pin(GPIO_PIN_14) {}
	LED(const Down & down_led) : _base(GPIOD), _pin(GPIO_PIN_15) {}

	void write(bool onoff) { _state = onoff; HAL_GPIO_WritePin(_base, _pin, onoff ? GPIO_PIN_SET : GPIO_PIN_RESET);	}
	void on() { write(true); }
	void off() { write(false); }
	bool toggle() { write(!_state); return _state; }
	bool state() const { return _state; }
};

#endif /* INC_HALAL_LED_H_ */
