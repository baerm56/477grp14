#ifndef CHESSCLOCK_H
#define CHESSCLOCK_H

#include "stm32l1xx_hal.h"
#include "types.h"

void InitChessClock(SPI_HandleTypeDef* spiHandle, TIM_HandleTypeDef* timerHandle);

void StartChessClock();

void StopChessClock();

void ChessClockTimerCallback(enum PieceOwner turn);

#endif // CHESSCLOCK_H
