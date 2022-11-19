#include "halal/timer.h"
#include "paradiddle.h"
#include "ws2812b.h"

TIM_HandleTypeDef* const TIM_LED_STRIP = &htim4;
TIM_HandleTypeDef* const TIM_PARADIDDLE = &htim2;
TIM_HandleTypeDef* const TIM_METRONOME = &htim3;
uint32_t const TIM_CHANNEL = TIM_CHANNEL_1;
uint32_t const TIM_CHANNEL_LED = TIM_CHANNEL_2;

bool Timer::_running = false;

void Timer::start(void) {
	HAL_TIM_Base_Start_IT(TIM_METRONOME);
	HAL_TIM_PWM_Start_IT(TIM_METRONOME, TIM_CHANNEL);
	HAL_TIM_Base_Start_IT(TIM_PARADIDDLE);
	HAL_TIM_PWM_Start_IT(TIM_PARADIDDLE, TIM_CHANNEL);
	_running = true;
}

void Timer::stop(void) {
	HAL_TIM_PWM_Stop_IT(TIM_PARADIDDLE, TIM_CHANNEL);
	HAL_TIM_Base_Stop_IT(TIM_PARADIDDLE);
	HAL_TIM_PWM_Stop_IT(TIM_METRONOME, TIM_CHANNEL);
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
	} else if (htim == TIM_PARADIDDLE) {
		Paradiddle::step_rise();
	}
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
	if (htim == TIM_METRONOME) {
		Paradiddle::metronome_fall();
	} else if (htim == TIM_PARADIDDLE) {
		Paradiddle::step_fall();
	} else if (htim == TIM_LED_STRIP) {
		ws2812b_HAL_TIM_PWM_PulseFinishedCallback();
	}
}
