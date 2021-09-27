#include "tracker.h"

volatile GPIO_PinState Board[NUM_COLS][NUM_ROWS] = {0};

void TrackerInit()
{
	/* Configure Column-Selecting GPIO */
    GPIO_InitTypeDef colGPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /* Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIO_BUS_COL, GPIO_COL_SEL0 | GPIO_COL_SEL1 | GPIO_COL_SEL2, GPIO_PIN_RESET);

    /* Configure GPIO pins */
    colGPIO_InitStruct.Pin = GPIO_COL_SEL0 | GPIO_COL_SEL1 | GPIO_COL_SEL2;
    colGPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    colGPIO_InitStruct.Pull = GPIO_NOPULL;
    colGPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIO_BUS_COL, &colGPIO_InitStruct);



    /* Enable Row-Reading GPIO */
    GPIO_InitTypeDef rowGPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOD_CLK_ENABLE();

	/* Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIO_BUS_ROW, GPIO_ROW0 | GPIO_ROW1 | GPIO_ROW2 | GPIO_ROW3, GPIO_PIN_RESET);

	/* Configure GPIO pins */
	rowGPIO_InitStruct.Pin = GPIO_ROW0 | GPIO_ROW1 | GPIO_ROW2 | GPIO_ROW3;
	rowGPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	rowGPIO_InitStruct.Pull = GPIO_PULLUP;
	rowGPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIO_BUS_ROW, &rowGPIO_InitStruct);
}

void ReadRows(GPIO_PinState* rows)
{
	if(sizeof(rows) / sizeof(*rows) < NUM_ROWS)
	{
		/// @todo: assert
	}

	rows[0] = HAL_GPIO_ReadPin(GPIO_BUS_ROW, GPIO_ROW0);
	rows[1] = HAL_GPIO_ReadPin(GPIO_BUS_ROW, GPIO_ROW1);
	rows[2] = HAL_GPIO_ReadPin(GPIO_BUS_ROW, GPIO_ROW2);
	rows[3] = HAL_GPIO_ReadPin(GPIO_BUS_ROW, GPIO_ROW3);
}

void SelectColumn(uint8_t column)
{
	GPIO_PinState colSel0State = (column & 1) >> 0; // 0th bit
	GPIO_PinState colSel1State = (column & 2) >> 1; // 1st bit
	GPIO_PinState colSel2State = (column & 4) >> 2; // 2nd bit

	HAL_GPIO_WritePin(GPIO_BUS_COL, GPIO_COL_SEL0, colSel0State);
	HAL_GPIO_WritePin(GPIO_BUS_COL, GPIO_COL_SEL1, colSel1State);
	HAL_GPIO_WritePin(GPIO_BUS_COL, GPIO_COL_SEL2, colSel2State);
}

void Track()
{
	GPIO_PinState rows[NUM_ROWS] = {0};

	for(uint8_t colNum = 0; colNum < NUM_COLS; colNum++)
	{
		SelectColumn(colNum);
		ReadRows(Board[colNum]);
	}
}
