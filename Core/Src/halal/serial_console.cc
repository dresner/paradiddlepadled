#include "halal/serial_console.h"
#include "usart.h"

UART_HandleTypeDef * const UART = &huart2;
const uint32_t PUT_TIMEOUT = 1000;
const uint32_t GET_TIMEOUT = HAL_MAX_DELAY;

Serial_Console& Serial_Console::operator<<(const std::string &s) {
	HAL_UART_Transmit(UART, (const uint8_t*)s.c_str(), s.length(), PUT_TIMEOUT);
	return *this;
};

Serial_Console& Serial_Console::operator<<(const Serial_Console::Endl e) {
	HAL_UART_Transmit(UART, (const uint8_t*)"\r\n", 2, PUT_TIMEOUT);
	return *this;
}

Serial_Console& Serial_Console::operator>>(std::string &s) {
	// Remove any leading whitespace first
	s.clear();
	uint8_t in = ' ';
	while(in == ' ' || in == '\r' || in == '\n') {
		auto ok = HAL_UART_Receive(UART, &in, 1, GET_TIMEOUT);
		if (ok != HAL_OK) {
			return *this;
		}
	}
	// Read until whitespace
	while(in != ' ' && in != '\r' && in != '\n') {
		s.push_back(in);
		auto ok = HAL_UART_Receive(UART, &in, 1, GET_TIMEOUT);
		if (ok != HAL_OK) {
			return *this;
		}
	}
	return *this;
}
