#include "button.h"
#include "main.h"

static void OnButton1Press();
static void OnButton2Press();

static TIM_HandleTypeDef* Button1DebounceTimer;
static TIM_HandleTypeDef* Button2DebounceTimer;
static enum ButtonType LastButtonPressed;

void InitButtons(TIM_HandleTypeDef *button1DebounceTimer, TIM_HandleTypeDef *button2DebounceTimer)
{
	Button1DebounceTimer = button1DebounceTimer;
	Button2DebounceTimer = button2DebounceTimer;
	LastButtonPressed = INVALID_BUTTON;
}

enum GameMode GetGameModeSwitchState()
{
	if(HAL_GPIO_ReadPin(SWITCH1_1_GPIO_Port, SWITCH1_1_Pin) == GPIO_PIN_SET)
	{
		return EDUCATION;
	}
	else if(HAL_GPIO_ReadPin(SWITCH1_2_GPIO_Port, SWITCH1_2_Pin) == GPIO_PIN_SET)
	{
		return PRACTICE;
	}
	else if(HAL_GPIO_ReadPin(SWITCH1_3_GPIO_Port, SWITCH1_3_Pin) == GPIO_PIN_SET)
	{
		return COMPETITIVE;
	}
	else
	{
		return INVALID_GAMEMODE;
	}
}

enum AiDifficulty GetDifficultySwitchState()
{
	if(HAL_GPIO_ReadPin(SWITCH2_1_GPIO_Port, SWITCH2_1_Pin) == GPIO_PIN_SET)
	{
		return EASY;
	}
	else if(HAL_GPIO_ReadPin(SWITCH2_2_GPIO_Port, SWITCH2_2_Pin) == GPIO_PIN_SET)
	{
		return MEDIUM;
	}
	else if(HAL_GPIO_ReadPin(SWITCH2_3_GPIO_Port, SWITCH2_3_Pin) == GPIO_PIN_SET)
	{
		return HARD;
	}
	else
	{
		return INVALID_AIDIFFICULTY;
	}
}

void Button1DebounceTimerCallback()
{
	HAL_TIM_Base_Stop_IT(Button1DebounceTimer);
	if(HAL_GPIO_ReadPin(BUTTON1_GPIO_Port, BUTTON1_Pin) == GPIO_PIN_RESET)
	{
		OnButton1Press();
	}
}

void Button2DebounceTimerCallback()
{
	HAL_TIM_Base_Stop_IT(Button2DebounceTimer);
	if(HAL_GPIO_ReadPin(BUTTON2_GPIO_Port, BUTTON2_Pin) == GPIO_PIN_RESET)
	{
		OnButton2Press();
	}
}

void Button1Callback()
{
	HAL_TIM_Base_Start_IT(Button1DebounceTimer);
}

void Button2Callback()
{
	HAL_TIM_Base_Start_IT(Button2DebounceTimer);
}

static void OnButton1Press()
{
	LastButtonPressed = BUTTON1;
}

static void OnButton2Press()
{
	LastButtonPressed = BUTTON2;
}

enum ButtonType GetLastButtonPressed()
{
	return LastButtonPressed;
}
