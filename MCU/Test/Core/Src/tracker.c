#include "tracker.h"

static const struct GPIO_Pin COL_BIT0_PIN = { .pin = GPIO_PIN_0,  .bus = GPIOA }; // A0
const struct GPIO_Pin COL_BIT1_PIN = { .pin = GPIO_PIN_1,  .bus = GPIOA }; // A1
const struct GPIO_Pin COL_BIT2_PIN = { .pin = GPIO_PIN_4,  .bus = GPIOA }; // A2

const struct GPIO_Pin ROW0_PIN = { .pin = GPIO_PIN_10, .bus = GPIOA }; // D2
const struct GPIO_Pin ROW1_PIN = { .pin = GPIO_PIN_3,  .bus = GPIOB }; // D3
const struct GPIO_Pin ROW2_PIN = { .pin = GPIO_PIN_5,  .bus = GPIOB }; // D4
const struct GPIO_Pin ROW3_PIN = { .pin = GPIO_PIN_4,  .bus = GPIOB }; // D5
const struct GPIO_Pin ROW4_PIN = { .pin = GPIO_PIN_10, .bus = GPIOB }; // D6
const struct GPIO_Pin ROW5_PIN = { .pin = GPIO_PIN_8,  .bus = GPIOA }; // D7
const struct GPIO_Pin ROW6_PIN = { .pin = GPIO_PIN_9,  .bus = GPIOA }; // D8
const struct GPIO_Pin ROW7_PIN = { .pin = GPIO_PIN_7,  .bus = GPIOC }; // D9

volatile static struct GPIO_Pin RowNumberToPinTable[NUM_ROWS] = {
	ROW0_PIN, ROW1_PIN, ROW2_PIN, ROW3_PIN, // ROW4_PIN, ROW5_PIN, ROW6_PIN, ROW7_PIN
};

volatile static struct GPIO_Pin ColumnBitToPinTable[NUM_COL_BITS] = {
	COL_BIT0_PIN, COL_BIT1_PIN, // COL_BIT2_PIN
};

volatile static GPIO_PinState Board[NUM_COLS][NUM_ROWS] = {0};

static void InitGPIO_Pin(struct GPIO_Pin pin, uint32_t mode, uint32_t pull)
{
	// Enable GPIO Bus
	if(pin.bus == GPIOA)
	{
	    __HAL_RCC_GPIOA_CLK_ENABLE();
	}
	else if(pin.bus == GPIOB)
	{
	    __HAL_RCC_GPIOB_CLK_ENABLE();
	}
	else if(pin.bus == GPIOC)
	{
	    __HAL_RCC_GPIOC_CLK_ENABLE();
	}
	else
	{
	    __HAL_RCC_GPIOD_CLK_ENABLE();
	}

    if(mode == GPIO_MODE_OUTPUT_PP)
    {
        HAL_GPIO_WritePin(pin.bus, pin.pin, GPIO_PIN_RESET);
    }

    // Configure GPIO
    GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = pin.pin;
	GPIO_InitStruct.Mode = mode;
	GPIO_InitStruct.Pull = pull;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(pin.bus, &GPIO_InitStruct);
}

void InitTracker()
{
	// Initialize output column bits
	for(uint8_t columnNumber = 0; columnNumber < NUM_COLS; columnNumber++)
	{
		InitGPIO_Pin(ColumnBitToPinTable[columnNumber], GPIO_MODE_OUTPUT_PP, GPIO_NOPULL);
	}

	// Initialize input rows
	for(uint8_t rowNumber = 0; rowNumber < NUM_ROWS; rowNumber++)
	{
		InitGPIO_Pin(RowNumberToPinTable[rowNumber], GPIO_MODE_INPUT, GPIO_NOPULL);
	}
}

static void WriteColumn(uint8_t columnNumber)
{
	uint8_t columnBit0 = (columnNumber & 1) >> 0;
	uint8_t columnBit1 = (columnNumber & 2) >> 1;
	uint8_t columnBit2 = (columnNumber & 4) >> 2;

	HAL_GPIO_WritePin(COL_BIT0_PIN.bus, COL_BIT0_PIN.pin, columnBit0);
	HAL_GPIO_WritePin(COL_BIT1_PIN.bus, COL_BIT1_PIN.pin, columnBit1);
	HAL_GPIO_WritePin(COL_BIT2_PIN.bus, COL_BIT2_PIN.pin, columnBit2);
}

static GPIO_PinState ReadRow(uint8_t rowNumber)
{
	struct GPIO_Pin rowPin = RowNumberToPinTable[rowNumber];
	GPIO_PinState value = HAL_GPIO_ReadPin(rowPin.bus, rowPin.pin);

	return value;
}

/**
 * @brief Write column bits to MUXs and read the value of each row. Store into Board data structure
 */
void Track()
{
	for(uint8_t columnNumber = 0; columnNumber < NUM_COLS; columnNumber++)
	{
		WriteColumn(columnNumber);

		for(uint8_t rowNumber = 0; rowNumber < NUM_ROWS; rowNumber++)
		{
			GPIO_PinState rowValue = ReadRow(rowNumber);
			Board[columnNumber][rowNumber] = rowValue;
		}
	}
}

uint8_t IsPiecePresent(uint8_t x, uint8_t y)
{
	return Board[x][y] == GPIO_PIN_SET;
}
