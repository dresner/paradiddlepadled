#ifndef INC_HALAL_BUTTON_H_
#define INC_HALAL_BUTTON_H_

#include <stdint.h>

class Button {
public:
	static void handle_button_event(void);

	static bool button_was_pressed(void) { return button_presses; }

//private:
	static volatile uint8_t button_presses; // Used by interrupt. Consider as private.
};



#endif /* INC_HALAL_BUTTON_H_ */
