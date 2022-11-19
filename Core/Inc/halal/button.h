#ifndef INC_HALAL_BUTTON_H_
#define INC_HALAL_BUTTON_H_

#include <stdint.h>

class Button {
public:
	static void check_events(void) {
		if (button_presses) {
			handle_button_event();
		}
		if (accelerometer_clicked) {
			handle_accelerometer_event();
		}
	}

//private: // Used by interrupt. Consider as private.
	static volatile uint8_t button_presses;
	static volatile bool accelerometer_clicked;
private:
	static void handle_button_event(void);
	static void handle_accelerometer_event(void);

	static bool button_was_pressed(void) { return button_presses; }
};


#endif /* INC_HALAL_BUTTON_H_ */
