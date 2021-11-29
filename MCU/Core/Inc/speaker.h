#ifndef SPEAKER_H
#define SPEAKER_H

#include "stm32l1xx_hal.h"

#define SPEARER_GPIO_BUS_ENABLE_FUNCTION __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPEAKER_GPIO_BUS GPIOA
#define SPEAKER_GPIO_PIN GPIO_PIN_5

#define DAC_DMA_TRIGGER DAC_TRIGGER_T4_TRGO
#define DAC_CHANNEL DAC_CHANNEL_2

#define DMA_CLOCK_ENABLE_FUNCTION __HAL_RCC_DMA1_CLK_ENABLE()
#define DMA_IRQ DMA1_Channel3_IRQn

#define TIMER_NAME TIM4
#define TIMER_PSC 0

#define TIMER_8000 4000-1
#define TIMER_11025 2902-1
#define TIMER_22050 1451-1
#define TIMER_44100 726-1

#define TIMER_ARR TIMER_44100

void SpeakerInit(DAC_HandleTypeDef * hdac, TIM_HandleTypeDef * htim);
void DMAInit();
void DACInit(DAC_HandleTypeDef * hdac);
void SpeakerGPIOInit();

#endif /* SPEAKER_H */
