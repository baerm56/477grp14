#include "chessclock.h"
#include "leds.h"

static SPI_HandleTypeDef* SpiHandle;
static TIM_HandleTypeDef* TimerHandle;
static uint32_t WhiteTime;
static uint32_t BlackTime;

void InitChessClock(SPI_HandleTypeDef* spiHandle, TIM_HandleTypeDef* timerHandle)
{
	SpiHandle = spiHandle;
	TimerHandle = timerHandle;
	WhiteTime = 5 * 60;
	BlackTime = 5 * 60;
	writeTime(SpiHandle, WhiteTime, 0);
	writeTime(SpiHandle, BlackTime, 1);
}

void StartChessClock()
{
	HAL_TIM_Base_Start_IT(TimerHandle);
}

void StopChessClock()
{
	HAL_TIM_Base_Stop_IT(TimerHandle);
}

void ChessClockTimerCallback(enum PieceOwner turn)
{
	if(turn == WHITE)
	{
		writeTime(SpiHandle, WhiteTime, 0);
		if(WhiteTime == 0)
		{
			StopChessClock();
		}

		WhiteTime--;
	}
	else
	{
		writeTime(SpiHandle, BlackTime, 1);
		if(BlackTime == 0)
		{
			StopChessClock();
		}

		BlackTime--;
	}
}
