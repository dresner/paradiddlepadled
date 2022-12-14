#ifndef INC_HALAL_TIMER_H_
#define INC_HALAL_TIMER_H_

#include "tim.h"

class Timer {
public:
	static void start(void);
	static void stop(void);

	static void bpm_up(unsigned int how_much);
	static void bpm_down(unsigned int how_much);

private:
	static bool _running;
	static const unsigned int BPM_STEP = 271186;
};

extern TIM_HandleTypeDef* const TIM_LED_STRIP;
extern TIM_HandleTypeDef* const TIM_PARADIDDLE;
extern TIM_HandleTypeDef* const TIM_METRONOME;
extern uint32_t const TIM_CHANNEL;
extern uint32_t const TIM_CHANNEL_LED;

#endif /* INC_HALAL_TIMER_H_ */
