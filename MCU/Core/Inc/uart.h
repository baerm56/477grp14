#ifndef UART_H
#define UART_H

#include "stm32l1xx_hal.h"

void EnableUart(UART_HandleTypeDef * huart);
void sendData(UART_HandleTypeDef * huart, char p1, char p2, char p3, char p4);
void sendStart(UART_HandleTypeDef * huart, uint8_t color, uint8_t edum);
void sendDifficulty(UART_HandleTypeDef * huart, uint8_t df);
void receiveData(UART_HandleTypeDef * huart, char buffer[]);
void sendMove(UART_HandleTypeDef * huart, char buffer[]);

#endif /* INC_UART_H_ */
