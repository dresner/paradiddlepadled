#ifndef INC_HALAL_SERIAL_CONSOLE_H_
#define INC_HALAL_SERIAL_CONSOLE_H_

#include <string>

class Serial_Console {
public:
	enum Endl { endl };

	Serial_Console& operator<<(const std::string &s);
	Serial_Console& operator<<(const Endl e);

	// Reads a single word, discarding leading whitespace
	Serial_Console& operator>>(std::string &s);
};

#endif /* INC_HALAL_SERIAL_CONSOLE_H_ */
