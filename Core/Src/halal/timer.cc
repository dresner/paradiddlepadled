#include "halal/timer.h"
#include "tim.h"
#include "paradiddle.h"

auto const TIM_PARADIDDLE = &htim2;
auto const TIM_METRONOME = &htim3;
auto const CHANNEL = TIM_CHANNEL_1;

bool Timer::_running = false;

void Timer::start(void) {
	HAL_TIM_Base_Start_IT(TIM_METRONOME);
	HAL_TIM_PWM_Start_IT(TIM_METRONOME, CHANNEL);
	HAL_TIM_Base_Start_IT(TIM_PARADIDDLE);
	HAL_TIM_PWM_Start_IT(TIM_PARADIDDLE, CHANNEL);
	_running = true;
}

void Timer::stop(void) {
	HAL_TIM_PWM_Stop_IT(TIM_PARADIDDLE, CHANNEL);
	HAL_TIM_Base_Stop_IT(TIM_PARADIDDLE);
	HAL_TIM_PWM_Stop_IT(TIM_METRONOME, CHANNEL);
	HAL_TIM_Base_Stop_IT(TIM_METRONOME);
	_running = false;
}

void Timer::bpm_up(unsigned int how_much) {
	bool r = _running;
	if (r) {
		stop();
	}
	TIM_PARADIDDLE->Init.Period -= how_much * BPM_STEP;
	HAL_TIM_Base_Init(TIM_PARADIDDLE);
	if (r) {
		start();
	}
}

void Timer::bpm_down(unsigned int how_much) {
	bool r = _running;
	if (r) {
		stop();
	}
	TIM_PARADIDDLE->Init.Period += how_much * BPM_STEP;
	HAL_TIM_Base_Init(TIM_PARADIDDLE);
	if (r) {
		start();
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim == TIM_METRONOME) {
		Paradiddle::metronome_rise();
	} else {
		Paradiddle::step_rise();
	}
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
	if (htim == TIM_METRONOME) {
		Paradiddle::metronome_fall();
	} else {
		Paradiddle::step_fall();
	}
}
