#include "button.h"
#include "main.h"

static void OnButton1Press();
static void OnButton2Press();

TIM_HandleTypeDef* Button1DebounceTimer;
TIM_HandleTypeDef* Button2DebounceTimer;

void InitButtons(TIM_HandleTypeDef *button1DebounceTimer, TIM_HandleTypeDef *button2DebounceTimer)
{
	Button1DebounceTimer = button1DebounceTimer;
	Button2DebounceTimer = button2DebounceTimer;
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

}

static void OnButton2Press()
{

}
