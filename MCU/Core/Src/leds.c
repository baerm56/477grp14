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

void disableOutput(SPI_HandleTypeDef * hspi){
	writeHexVal(hspi, LED_CONFIGURATION, 0x00);
}

void writeTime(SPI_HandleTypeDef * hspi, int time, int player){
	int sec = time % 60;
	int min = time / 60;
	if (!player){
		writeHexVal(hspi, LED_4, sec % 10);
		writeHexVal(hspi, LED_3, sec / 10);
		writeHexVal(hspi, LED_2, min % 10);
		writeHexVal(hspi, LED_1, min / 10);
	}
	else{
		writeHexVal(hspi, LED_8, sec % 10);
		writeHexVal(hspi, LED_7, sec / 10);
		writeHexVal(hspi, LED_6, min % 10);
		writeHexVal(hspi, LED_5, min / 10);
	}
}

void LEDSInit(SPI_HandleTypeDef * hspi, uint8_t spi_num){
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	// SPI setup
	if (spi_num == 1){
		hspi->Instance = SPI1;

		__HAL_RCC_GPIOE_CLK_ENABLE();
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_RESET);

		GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_12;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	}
	else if (spi_num == 2){
		hspi->Instance = SPI2;

		__HAL_RCC_GPIOD_CLK_ENABLE();
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET);

		GPIO_InitStruct.Pin = GPIO_PIN_8;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	}
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


}

void ChessTimerLEDInit(SPI_HandleTypeDef * hspi){
	enableOutput(hspi);
	setScanLimit(hspi, 0x07);
	setIntensity(hspi, 0x00);
	enableHex(hspi);
}

void BoardLEDInit(SPI_HandleTypeDef * hspi){
	enableOutput(hspi);
	setScanLimit(hspi, 0x07);
	setIntensity(hspi, 0x05);
}

void writeBoardValue(SPI_HandleTypeDef * hspi, uint8_t board[8][8]){
	for (int row = 0; row < 8; row++){
		uint8_t val = 0x0;
		for (int col = 7; col > 0; col--){
			val = (val << 1) | board[row][col];
		}
		val |= board[row][0] << 7;

		writeHexVal(hspi, LED_1 + row, val);
	}
}

void writeAIMove(SPI_HandleTypeDef * hspi, uint8_t board[8][8], char move[4]){
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			board[i][j] = 0;
		}
	}
	int row1, row2, col1, col2 = 0;

	row1 = move[1] - '1';
	col1 = (move[0] - 'a');
	row2 = move[3] - '1';
	col2 = (move[2] - 'a');

	board[row1][col1] = 1;
	board[row2][col2] = 1;

	writeBoardValue(hspi, board);
}

void writeHexVal(SPI_HandleTypeDef * hspi, uint8_t reg, uint8_t val){
	if (hspi->Instance == SPI2){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET);
	}
	else if (hspi->Instance == SPI1){
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_RESET);
	}

	HAL_SPI_Transmit(hspi, &reg, 1, 100);
	HAL_SPI_Transmit(hspi, &val, 1, 100);

	if (hspi->Instance == SPI2){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_SET);
	}
	else if (hspi->Instance == SPI1){
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_SET);
	}
}
