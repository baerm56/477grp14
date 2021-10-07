#ifndef SPEAKER_H
#define SPEAKER_H

#include "stm32l1xx_hal.h"

#define SPEARER_GPIO_BUS_ENABLE_FUNCTION __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPEAKER_GPIO_BUS GPIOA
#define SPEAKER_GPIO_PIN GPIO_PIN_4

#define DAC_DMA_TRIGGER DAC_TRIGGER_T2_TRGO
#define DAC_CHANNEL DAC_CHANNEL_1

#define DMA_CLOCK_ENABLE_FUNCTION __HAL_RCC_DMA1_CLK_ENABLE()
#define DMA_IRQ DMA1_Channel2_IRQn

#define TIMER_NAME TIM2
#define TIMER_PSC 0
#define TIMER_ARR 4000-1

void SpeakerInit(DAC_HandleTypeDef * hdac, TIM_HandleTypeDef * htim);

#endif /* SPEAKER_H */
