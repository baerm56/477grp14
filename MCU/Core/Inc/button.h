#ifndef BUTTON_H
#define BUTTON_H

#include "stm32l1xx_hal.h"

enum ButtonType {
	NO_BUTTON = 0,
	BUTTON1 = 1,
	BUTTON2 = 2
};

void InitButtons(TIM_HandleTypeDef *button1DebounceTimer, TIM_HandleTypeDef *button2DebounceTimer);

void Button1DebounceTimerCallback();

void Button2DebounceTimerCallback();

void Button1Callback();

void Button2Callback();

enum ButtonType GetLastButtonPressed();

#endif // BUTTON_H
