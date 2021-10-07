#include "waveplayer.h"

static FIL wavFile;
static FATFS FatFs;

static uint32_t fileLength;
static __IO uint32_t audioRemainSize = 0;

static uint32_t samplingFreq;

int GetFile(const char* filePath)
{
  WAV_Header wavHeader;
  UINT readBytes = 0;

  f_mount(&FatFs, "", 1);
  //Open WAV file
  if(f_open(&wavFile, filePath, FA_READ) != FR_OK)
  {
    return 0;
  }
  //Read WAV file Header
  f_read(&wavFile, &wavHeader, sizeof(wavHeader), &readBytes);
  //Get audio data size
  fileLength = wavHeader.FileSize;
  //Play the WAV file with frequency specified in header
  samplingFreq = wavHeader.SampleRate;

  return fileLength;
}

void readFile(uint8_t * buffer){
	UINT readBytes = 0;

	f_read(&wavFile, buffer, 512, &readBytes);
}

void closeFile(){
	f_close(&wavFile);
	f_mount(NULL, "", 0);
}

void WaveplayerInit(SPI_HandleTypeDef * hspi){
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	// SPI setup
	hspi->Instance = SPI2;
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
	__HAL_RCC_GPIOB_CLK_ENABLE();

	HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_RESET);

	GPIO_InitStruct.Pin = SD_CS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(SD_CS_GPIO_Port, &GPIO_InitStruct);

	MX_FATFS_Init();

}