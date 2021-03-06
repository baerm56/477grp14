#include "waveplayer.h"
#include "leds.h"
#include <string.h>

static FIL wavFile;
static FATFS FatFs;

static uint32_t fileLength;
static __IO uint32_t audioRemainSize = 0;
static uint8_t audioBuffer[AUDIO_BUFFER_SIZE];

static UINT playerReadBytes = 0;
static uint8_t isFinished = 0;

static volatile WAVEPLAYER_E waveplayerStatus = WAVEPLAYER_IDLE;

static void ResetAudio(){
	audioRemainSize = 0;
	playerReadBytes = 0;
	memset(audioBuffer, 128, AUDIO_BUFFER_SIZE * sizeof(audioBuffer[0]));
}

void GetFile(const char* filePath){
  WAV_Header wavHeader;
  UINT readBytes = 0;

  f_mount(&FatFs, "", 1);
  f_open(&wavFile, filePath, FA_READ);

  f_read(&wavFile, &wavHeader, sizeof(wavHeader), &readBytes);

  fileLength = wavHeader.FileSize;

}

void PlayAudio(const char* filePath, DAC_HandleTypeDef * hdac){
	HAL_Delay(100);
	GetFile(filePath);
	isFinished = 0;

	f_read(&wavFile, &audioBuffer[0], AUDIO_BUFFER_SIZE, &playerReadBytes);
	audioRemainSize = fileLength - playerReadBytes - sizeof(WAV_Header);

	//HAL_DAC_Start_DMA(hdac, DAC_CHANNEL_2, (uint32_t *) audioBuffer, AUDIO_BUFFER_SIZE, DAC_ALIGN_8B_R);

	while(!GetAudioStatus()){
		ProcessAudio();
	}
	//HAL_DAC_Stop_DMA(hdac, DAC_CHANNEL_2);
	HAL_Delay(200);
}

void ProcessAudio(){
	switch(waveplayerStatus) {
		case WAVEPLAYER_IDLE:
			break;

		case WAVEPLAYER_HALFBUFFER:
			playerReadBytes = 0;
			waveplayerStatus = WAVEPLAYER_IDLE;
			f_read(&wavFile, &audioBuffer[0], AUDIO_BUFFER_SIZE / 2, &playerReadBytes);
			if (audioRemainSize > (AUDIO_BUFFER_SIZE / 2))
				audioRemainSize -= playerReadBytes;
			else {
				audioRemainSize = 0;
				waveplayerStatus = WAVEPLAYER_EOF;
			}
			break;

		case WAVEPLAYER_FULLBUFFER:
			playerReadBytes = 0;
			waveplayerStatus = WAVEPLAYER_IDLE;
			f_read(&wavFile, &audioBuffer[AUDIO_BUFFER_SIZE / 2], AUDIO_BUFFER_SIZE / 2, &playerReadBytes);
			if (audioRemainSize > (AUDIO_BUFFER_SIZE / 2))
				audioRemainSize -= playerReadBytes;
			else {
				audioRemainSize = 0;
				waveplayerStatus = WAVEPLAYER_EOF;
			}
			break;

		case WAVEPLAYER_EOF:
			f_close(&wavFile);
			f_mount(NULL, "", 0);
			ResetAudio();
			isFinished = 1;
			waveplayerStatus = WAVEPLAYER_IDLE;
	}
}

int GetAudioStatus(){
  return isFinished;
}

void HAL_DACEx_ConvCpltCallbackCh2(DAC_HandleTypeDef *hdac){
	waveplayerStatus = WAVEPLAYER_FULLBUFFER;
}

void HAL_DACEx_ConvHalfCpltCallbackCh2(DAC_HandleTypeDef *hdac){
	waveplayerStatus = WAVEPLAYER_HALFBUFFER;
}

void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef *hdac){
	waveplayerStatus = WAVEPLAYER_FULLBUFFER;
}

void HAL_DAC_ConvHalfCpltCallbackCh1(DAC_HandleTypeDef *hdac){
	waveplayerStatus = WAVEPLAYER_HALFBUFFER;
}

void WaveplayerInit(SPI_HandleTypeDef * hspi, DAC_HandleTypeDef *hdac){
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	// SPI setup
	hspi->Instance = SPI3;
	hspi->Init.Mode = SPI_MODE_MASTER;
	hspi->Init.Direction = SPI_DIRECTION_2LINES;
	hspi->Init.DataSize = SPI_DATASIZE_8BIT;
	hspi->Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi->Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi->Init.NSS = SPI_NSS_SOFT;
	hspi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
	hspi->Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi->Init.TIMode = SPI_TIMODE_DISABLE;
	hspi->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi->Init.CRCPolynomial = 10;
	HAL_SPI_Init(hspi);

	// Setting up SD_CS
	__HAL_RCC_GPIOD_CLK_ENABLE();

	HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_RESET);

	GPIO_InitStruct.Pin = SD_CS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(SD_CS_GPIO_Port, &GPIO_InitStruct);

	MX_FATFS_Init();

	memset(audioBuffer, 128, AUDIO_BUFFER_SIZE * sizeof(audioBuffer[0]));
}

void prepAudio(SPI_HandleTypeDef * hspi1, SPI_HandleTypeDef * hspi2, DAC_HandleTypeDef *hdac){
	disableOutput(hspi1);
	disableOutput(hspi2);
	HAL_DAC_Start_DMA(hdac, DAC_CHANNEL_2, (uint32_t *) audioBuffer, AUDIO_BUFFER_SIZE, DAC_ALIGN_8B_R);
	HAL_Delay(1000);
}

void resetAudio(SPI_HandleTypeDef * hspi1, SPI_HandleTypeDef * hspi2, DAC_HandleTypeDef *hdac){
	HAL_DAC_Stop_DMA(hdac, DAC_CHANNEL_2);
	enableOutput(hspi1);
	enableOutput(hspi2);
}













