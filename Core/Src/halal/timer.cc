#include "halal/timer.h"
#include "tim.h"

auto const TIM = &htim2;
auto const CHANNEL = TIM_CHANNEL_1;

bool Timer::_running = false;

void Timer::start(void) {
	HAL_TIM_Base_Start_IT(TIM);
	HAL_TIM_PWM_Start_IT(TIM, CHANNEL);
	_running = true;
}

void Timer::stop(void) {
	HAL_TIM_PWM_Stop_IT(TIM, CHANNEL);
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
