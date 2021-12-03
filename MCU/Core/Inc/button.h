#ifndef BUTTON_H
#define BUTTON_H

#include "stm32l1xx_hal.h"
#include "types.h"

enum ButtonType {
	INVALID_BUTTON,
	BUTTON1,
	BUTTON2,
};

enum GameMode {
	INVALID_GAMEMODE,
	EDUCATION,
	PRACTICE,
	COMPETITIVE,
};

enum AiDifficulty {
	INVALID_AIDIFFICULTY,
	EASY,
	MEDIUM,
	HARD
};

void InitButtons(TIM_HandleTypeDef *button1DebounceTimer, TIM_HandleTypeDef *button2DebounceTimer);

enum GameMode GetGameModeSwitchState();

enum AiDifficulty GetDifficultySwitchState();

void Button1DebounceTimerCallback();

void Button2DebounceTimerCallback();

void Button1Callback();

void Button2Callback();

enum ButtonType GetLastButtonPressed();

#endif // BUTTON_H
