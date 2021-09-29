#ifndef TRACKER_H
#define TRACKER_H

#include "stm32l1xx_hal.h"

struct GPIO_Pin {
	uint16_t pin;
	GPIO_TypeDef* bus;
};

/// @todo: switch to 8 with real circuit
#define NUM_COLS 4
#define NUM_ROWS 4
#define NUM_COL_BITS 2

void Track(void);

void InitTracker(void);

#endif // TRACKER_H
