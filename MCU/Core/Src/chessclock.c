#include "chessclock.h"
#include "leds.h"
#include "button.h"

static SPI_HandleTypeDef* SpiHandle;
static TIM_HandleTypeDef* TimerHandle;
static uint32_t WhiteTime;
static uint32_t BlackTime;

extern enum AiDifficulty difficulty;
extern enum GameMode gameMode;
uint8_t updateAiClock = 0;

void InitChessClock(SPI_HandleTypeDef* spiHandle, TIM_HandleTypeDef* timerHandle)
{
	SpiHandle = spiHandle;
	TimerHandle = timerHandle;
	WhiteTime = 10 * 60;
	BlackTime = 10 * 60;
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
		updateAiClock = 1;
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
		if(BlackTime <= 0)
		{
			StopChessClock();
		}

		if (gameMode == PRACTICE){
			if (updateAiClock){
				updateAiClock = 0;
				if (difficulty == EASY){
					BlackTime -= 60;
				}
				else if (difficulty == MEDIUM){
					BlackTime -= 30;
				}
				else if (difficulty == HARD){
					BlackTime -= 20;
				}
			}
		}
		else{
			BlackTime--;
		}
	}
}
