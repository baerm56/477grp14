#include "speaker.h"

void SpeakerGPIOInit(){
	GPIO_InitTypeDef GPIO_InitStruct;

	// Enable Analog Output
	SPEARER_GPIO_BUS_ENABLE_FUNCTION;

	GPIO_InitStruct.Pin = SPEAKER_GPIO_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(SPEAKER_GPIO_BUS, &GPIO_InitStruct);
}

void DACInit(DAC_HandleTypeDef * hdac){
	DAC_ChannelConfTypeDef sConfig = {0};

	// Enable DAC clock
	__HAL_RCC_DAC_CLK_ENABLE();
	hdac->Instance = DAC;
	HAL_DAC_Init(hdac);

	// DAC Setup Settings of Timer 2 Trigger
	sConfig.DAC_Trigger = DAC_DMA_TRIGGER;
	sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
	HAL_DAC_ConfigChannel(hdac, &sConfig, DAC_CHANNEL);
}

void DMAInit(){
	// DMA controller clock enable
	DMA_CLOCK_ENABLE_FUNCTION;

	// DMA interrupt init
	// DMA1_Channel2_IRQn interrupt configuration
	HAL_NVIC_SetPriority(DMA_IRQ, 0, 0);
	HAL_NVIC_EnableIRQ(DMA_IRQ);
}

void TimerInit(TIM_HandleTypeDef * htim){
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	htim->Instance = TIMER_NAME;
	htim->Init.Prescaler = TIMER_PSC;
	htim->Init.CounterMode = TIM_COUNTERMODE_UP;
	htim->Init.Period = TIMER_ARR;
	htim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(htim);

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(htim, &sClockSourceConfig);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(htim, &sMasterConfig);
}

void SpeakerInit(DAC_HandleTypeDef * hdac, TIM_HandleTypeDef * htim){
	SpeakerGPIOInit();
	DMAInit();
	DACInit(hdac);
	TimerInit(htim);
}

