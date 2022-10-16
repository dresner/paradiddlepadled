#include "halal/timer.h"
#include "tim.h"

TIM_HandleTypeDef * const TIM = &htim2;

bool Timer::_running = false;

void Timer::start(void) {
	HAL_TIM_Base_Start_IT(TIM);
	_running = true;
}

void Timer::stop(void) {
	HAL_TIM_Base_Stop_IT(TIM);
	_running = false;
}

void Timer::bpm_up(unsigned int how_much) {
	bool r = _running;
	if (r) {
		stop();
	}
	TIM->Init.Period -= how_much * BPM_STEP;
	HAL_TIM_Base_Init(TIM);
	if (r) {
		start();
	}
}

void Timer::bpm_down(unsigned int how_much) {
	bool r = _running;
	if (r) {
		stop();
	}
	TIM->Init.Period += how_much * BPM_STEP;
	HAL_TIM_Base_Init(TIM);
	if (r) {
		start();
	}
}
