#include "uart.h"

void EnableUart(UART_HandleTypeDef * huart){
  huart->Instance = USART1;
  huart->Init.BaudRate = 9600;
  //huart->Init.WordLength = UART_WORDLENGTH_9B;
  huart->Init.WordLength = UART_WORDLENGTH_8B;
  huart->Init.StopBits = UART_STOPBITS_1;
  //huart->Init.Parity = UART_PARITY_EVEN;
  huart->Init.Parity = UART_PARITY_NONE;
  huart->Init.Mode = UART_MODE_TX_RX;
  huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart->Init.OverSampling = UART_OVERSAMPLING_16;

  HAL_UART_Init(huart);
}

void sendData(UART_HandleTypeDef * huart, char p1, char p2, char p3, char p4){
	uint8_t move[4] = {p1, p2, p3, p4};
	HAL_UART_Transmit(huart, move, 4, 10);
}

void sendData2(UART_HandleTypeDef * huart, char p1, char p2){
	uint8_t move[2] = {p1, p2};
	HAL_UART_Transmit(huart, move, 2, 10);
}

void sendMove(UART_HandleTypeDef * huart, char buffer[]){
	HAL_UART_Transmit(huart, (uint8_t *)buffer, 4, 10);
}

void sendStart(UART_HandleTypeDef * huart, uint8_t color, uint8_t edum){
	sendData(huart, 'S', 'T', 'R', 'T');
	//HAL_Delay(3000);
	/*if (color){
		sendData(huart, 'B', 'L', 'C', 'K');
	}
	else{
		sendData(huart, 'W', 'H', 'T', 'E');
	}

	if (edum){
		sendData(huart, 'E', 'D', 'U', 'M');
	}
	else{
		sendData(huart, '-', '-', '-', '-');
	}*/
}

void sendDifficulty(UART_HandleTypeDef * huart, uint8_t df){
	if (df == 1){
		sendData(huart, 'D', 'F', '0', '1');
	}
	else if (df == 2){
		sendData(huart, 'D', 'F', '0', '8');
	}
	else if (df == 3){
		sendData(huart, 'D', 'F', '1', '5');
	}
}

void receiveData(UART_HandleTypeDef * huart, char buffer[]){
	HAL_UART_Receive(huart, (uint8_t *)buffer, 5, 3000);
}
