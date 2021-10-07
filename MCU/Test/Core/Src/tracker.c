#include "tracker.h"

volatile static struct GPIO_Pin RowNumberToPinTable[NUM_ROWS] = {
	{ GPIO_PIN_10, GPIOA }, // D2
	{ GPIO_PIN_3,  GPIOB }, // D3
	{ GPIO_PIN_5,  GPIOB }, // D4
	{ GPIO_PIN_4,  GPIOB }, // D5
	{ GPIO_PIN_10, GPIOB }, // D6
	{ GPIO_PIN_8,  GPIOA }, // D7
	{ GPIO_PIN_9,  GPIOA }, // D8
	{ GPIO_PIN_7,  GPIOC }, // D9

};

volatile static struct GPIO_Pin ColumnBitToPinTable[NUM_COL_BITS] = {
	{ GPIO_PIN_0,  GPIOA }, // A0
	{ GPIO_PIN_1,  GPIOA }, // A1
	{ GPIO_PIN_4,  GPIOA }, // A2

};

volatile static struct Piece Chessboard[NUM_ROWS][NUM_COLS];
volatile static struct Piece InitialChessboard[NUM_ROWS][NUM_COLS] = {
	{{CASTLE, WHITE}, {KNIGHT, WHITE}, {BISHOP, WHITE}, {QUEEN, WHITE},  {KING, WHITE},   {BISHOP, WHITE}, {KNIGHT, WHITE}, {CASTLE, WHITE}},
	{{PAWN, WHITE},   {PAWN, WHITE},   {PAWN, WHITE},   {PAWN, WHITE},   {PAWN, WHITE},   {PAWN, WHITE},   {PAWN, WHITE},   {PAWN, WHITE}},
	{{NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}},
	{{NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}},
	{{NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}},
	{{NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}},
	{{PAWN, BLACK},   {PAWN, BLACK},   {PAWN, BLACK},   {PAWN, BLACK},   {PAWN, BLACK},   {PAWN, BLACK},   {PAWN, BLACK},   {PAWN, BLACK}},
	{{CASTLE, BLACK}, {KNIGHT, BLACK}, {BISHOP, BLACK}, {QUEEN, BLACK},  {KING, BLACK},   {BISHOP, BLACK}, {KNIGHT, BLACK}, {CASTLE, BLACK}},
};


volatile struct Piece LastPickedUpPiece = {NONE, NEUTRAL};

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
	enum PieceType bottomRow[] = {};

	// Initialize output column bits IO and the chessboard data structure
	for(uint8_t columnNumber = 0; columnNumber < NUM_COL_BITS; columnNumber++)
	{
		InitGPIO_Pin(ColumnBitToPinTable[columnNumber], GPIO_MODE_OUTPUT_PP, GPIO_NOPULL);
	}

	// Initialize input rows
	for(uint8_t rowNumber = 0; rowNumber < NUM_ROWS; rowNumber++)
	{
		InitGPIO_Pin(RowNumberToPinTable[rowNumber], GPIO_MODE_INPUT, GPIO_NOPULL);
	}

	for(uint8_t columnNumber = 0; columnNumber < NUM_COL_BITS; columnNumber++)
	{
		for(uint8_t rowNumber = 0; rowNumber < NUM_ROWS; rowNumber++)
		{
			Chessboard[rowNumber][columnNumber] = InitialChessboard[rowNumber][columnNumber];
		}
	}


}

static void WriteColumn(uint8_t columnNumber)
{
	uint8_t columnBit0 = (columnNumber & 1) >> 0;
	uint8_t columnBit1 = (columnNumber & 2) >> 1;
	uint8_t columnBit2 = (columnNumber & 4) >> 2;

	HAL_GPIO_WritePin(ColumnBitToPinTable[0].bus, ColumnBitToPinTable[0].pin, columnBit0);
	HAL_GPIO_WritePin(ColumnBitToPinTable[1].bus, ColumnBitToPinTable[1].pin, columnBit1);
	HAL_GPIO_WritePin(ColumnBitToPinTable[2].bus, ColumnBitToPinTable[2].pin, columnBit2);
}

static GPIO_PinState ReadRow(uint8_t rowNumber)
{
	struct GPIO_Pin rowPin = RowNumberToPinTable[rowNumber];
	GPIO_PinState value = HAL_GPIO_ReadPin(rowPin.bus, rowPin.pin);

	return value;
}

void Track()
{
	for(uint8_t columnNumber = 0; columnNumber < NUM_COLS; columnNumber++)
	{
		WriteColumn(columnNumber);

		for(uint8_t rowNumber = 0; rowNumber < NUM_ROWS; rowNumber++)
		{
			GPIO_PinState cellValue = ReadRow(rowNumber);
			struct Piece currentPiece = GetPiece(rowNumber, columnNumber);

			// If there was no piece here but the IO is HIGH, a previously picked-up piece was placed
			if((currentPiece.type == NONE) && (cellValue == GPIO_PIN_SET))
			{
				SetPiece(rowNumber, columnNumber, LastPickedUpPiece);
			}

			// If there was a piece here but the IO is LOW, a piece has been picked up
			else if ((currentPiece.type != NONE) && (cellValue == GPIO_PIN_RESET))
			{
				struct Piece emptyPiece = {NONE, NEUTRAL};
				SetPiece(rowNumber, columnNumber, emptyPiece);

				// If the last picked-up piece owner is not the same as the current piece owner,
				// the current piece owner's piece is being killed by the last picked-up piece
				if(LastPickedUpPiece.owner != currentPiece.owner)
				{
					KillPiece(currentPiece);
				}

				// If the piece wasn't killed, store it as the last picked up piece
				else
				{
					LastPickedUpPiece = currentPiece;
				}
			}
		}
	}
}

uint8_t ValidateStartPositions()
{
	for(uint8_t columnNumber = 0; columnNumber < NUM_COLS; columnNumber++)
	{
		for(uint8_t rowNumber = 0; rowNumber < NUM_ROWS; rowNumber++)
		{
			struct Piece piece = GetPiece(rowNumber, columnNumber);

			switch(rowNumber)
			{

			// Make sure rows 0, 1, 6, 7 are all filled with pieces
			case 0:
			case 1:
			case 6:
			case 7:
				if(piece.type == NONE)
				{
					return 0;
				}
				break;

			// Make sure other rows do not have a piece
			default:
				if(piece.type != NONE)
				{
					return 0;
				}
			}
		}
	}

	return 1;
}

inline void KillPiece(struct Piece piece)
{

}

inline void SetPiece(uint8_t row, uint8_t column, struct Piece piece)
{
	Chessboard[row][column] = piece;
}

inline struct Piece GetPiece(uint8_t row, uint8_t column)
{
	return Chessboard[row][column];
}
