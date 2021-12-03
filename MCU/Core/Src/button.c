#include "button.h"
#include "main.h"

static void OnButton1Press();
static void OnButton2Press();

TIM_HandleTypeDef* Button1DebounceTimer;
TIM_HandleTypeDef* Button2DebounceTimer;
static enum ButtonType LastButtonPressed;

void InitButtons(TIM_HandleTypeDef *button1DebounceTimer, TIM_HandleTypeDef *button2DebounceTimer)
{
	Button1DebounceTimer = button1DebounceTimer;
	Button2DebounceTimer = button2DebounceTimer;
	LastButtonPressed = NO_BUTTON;
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
