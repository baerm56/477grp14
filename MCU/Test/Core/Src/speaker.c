#include "speaker.h"

void configure_DAC(DAC_HandleTypeDef *DAC_Config)
{
	DAC_ChannelConfTypeDef Channel_Config;
	__HAL_RCC_DAC_CLK_ENABLE();
	DAC_Config->Instance = DAC;
	HAL_DAC_Init(DAC_Config);

	Channel_Config.DAC_Trigger = DAC_TRIGGER_NONE;
	Channel_Config.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
	HAL_DAC_ConfigChannel(DAC_Config, &Channel_Config, DAC_CHANNEL_2);
}

void configure_GPIO(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitStruct.Pin = GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void SpeakerInit(DAC_HandleTypeDef *DAC_Config){
	GPIO_InitTypeDef GPIO_InitStruct;
	DAC_ChannelConfTypeDef Channel_Config;

	// Enable GPIO A pin 5
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitStruct.Pin = GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	// Enable DAC clock
	__HAL_RCC_DAC_CLK_ENABLE();
	DAC_Config->Instance = DAC;
	HAL_DAC_Init(DAC_Config);

	// DAC Setup Settings
	Channel_Config.DAC_Trigger = DAC_TRIGGER_NONE;
	Channel_Config.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
	HAL_DAC_ConfigChannel(DAC_Config, &Channel_Config, DAC_CHANNEL_2);
}

void Delay_ms(volatile int time_ms)
{
	for(int j = 0; j < time_ms*4000; j++);
}