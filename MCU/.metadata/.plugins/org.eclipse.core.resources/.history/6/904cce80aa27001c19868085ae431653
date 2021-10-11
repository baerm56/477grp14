#include "leds.h"

void enableHex(SPI_HandleTypeDef * hspi){
	writeHexVal(hspi, LED_DECODE_MODE, 0xFF);
}

void setIntensity(SPI_HandleTypeDef * hspi, uint8_t val){
	writeHexVal(hspi, LED_INTENSITY, val);
}

void setScanLimit(SPI_HandleTypeDef * hspi, uint8_t val){
	writeHexVal(hspi, LED_SCAN_LIMIT, val);
}

void enableOutput(SPI_HandleTypeDef * hspi){
	writeHexVal(hspi, LED_CONFIGURATION, 0x01);
}

void LEDSInit(SPI_HandleTypeDef * hspi){
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	// SPI setup
	hspi->Instance = SPI1;
	hspi->Init.Mode = SPI_MODE_MASTER;
	hspi->Init.Direction = SPI_DIRECTION_2LINES;
	hspi->Init.DataSize = SPI_DATASIZE_8BIT;
	hspi->Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi->Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi->Init.NSS = SPI_NSS_SOFT;
	hspi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
	hspi->Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi->Init.TIMode = SPI_TIMODE_DISABLE;
	hspi->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi->Init.CRCPolynomial = 10;
	HAL_SPI_Init(hspi);

	// Setting up PA4 for CS
	__HAL_RCC_GPIOA_CLK_ENABLE();

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);

	GPIO_InitStruct.Pin = GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

void ChessTimerLEDInit(SPI_HandleTypeDef * hspi){
	enableOutput(hspi);
	setScanLimit(hspi, 0x07);
	setIntensity(hspi, 0x05);
	enableHex(hspi);
}

void writeHexVal(SPI_HandleTypeDef * hspi, uint8_t reg, uint8_t val){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
	HAL_SPI_Transmit(hspi, &reg, 1, 100);
	HAL_SPI_Transmit(hspi, &val, 1, 100);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	//HAL_Delay(100);
}
