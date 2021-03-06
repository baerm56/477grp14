#ifndef LEDS_H
#define LEDS_H

#include "stm32l1xx_hal.h"
#include "types.h"

#define LED_NO_OP 0x00
#define LED_DECODE_MODE 0x01
#define LED_INTENSITY 0x02
#define LED_SCAN_LIMIT 0x03
#define LED_CONFIGURATION 0x04
#define LED_DISPLAY_TEST 0x07
#define LED_1 0x60
#define LED_2 0x61
#define LED_3 0x62
#define LED_4 0x63
#define LED_5 0x64
#define LED_6 0x65
#define LED_7 0x66
#define LED_8 0x67

void LEDSInit(SPI_HandleTypeDef * hspi, uint8_t spi_num);
void writeHexVal(SPI_HandleTypeDef * hspi1, uint8_t reg, uint8_t val);
void ChessTimerLEDInit(SPI_HandleTypeDef * hspi);
void BoardLEDInit(SPI_HandleTypeDef * hspi);
void writeBoardValue(SPI_HandleTypeDef * hspi, uint8_t board[8][8]);
void writeCoordinates(SPI_HandleTypeDef * hspi, struct Coordinate* coordinates, uint8_t numCoordinates);
void writeTime(SPI_HandleTypeDef * hspi, int time, int player);
void disableOutput(SPI_HandleTypeDef * hspi);
void enableOutput(SPI_HandleTypeDef * hspi);
void writeAIMove(SPI_HandleTypeDef * hspi, uint8_t board[8][8], char move[4]);

#endif
