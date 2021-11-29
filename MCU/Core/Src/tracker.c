#include "tracker.h"

volatile static struct Piece Chessboard[NUM_ROWS][NUM_COLS];
volatile static enum PieceOwner CurrentTurn;
volatile static enum TransitionType LastTransitionType;
volatile static struct PieceCoordinate LastPickedUpPiece;
static struct PieceCoordinate ExpectedKingCastleCoordinate;
static struct PieceCoordinate ExpectedRookCastleCoordinate;

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
	// Initialize globals
	LastPickedUpPiece = EMPTY_PIECE_COORDINATE;
	ExpectedKingCastleCoordinate = EMPTY_PIECE_COORDINATE;
	ExpectedRookCastleCoordinate = EMPTY_PIECE_COORDINATE;
	LastTransitionType = PLACE;
	CurrentTurn = WHITE;

	// Initialize output column bits IO and the chessboard data structure
	for(uint8_t columnBit = 0; columnBit < NUM_COL_BITS; columnBit++)
	{
		InitGPIO_Pin(COLUMN_BIT_TO_PIN_TABLE[columnBit], GPIO_MODE_OUTPUT_PP, GPIO_NOPULL);
	}

	// Initialize input rows
	for(uint8_t row = 0; row < NUM_ROWS; row++)
	{
		InitGPIO_Pin(ROW_NUMBER_TO_PIN_TABLE[row], GPIO_MODE_INPUT, GPIO_NOPULL);
	}

	// Initialize the board data structure to the initial chessboard
	for(uint8_t column = 0; column < NUM_COLS; column++)
	{
		for(uint8_t row = 0; row < NUM_ROWS; row++)
		{
			Chessboard[row][column] = INITIAL_CHESSBOARD[row][column];
		}
	}
}

static void WriteColumn(uint8_t column)
{
	uint8_t columnBit0 = (column & 1) >> 0;
	uint8_t columnBit1 = (column & 2) >> 1;
	uint8_t columnBit2 = (column & 4) >> 2;

	HAL_GPIO_WritePin(COLUMN_BIT_TO_PIN_TABLE[0].bus, COLUMN_BIT_TO_PIN_TABLE[0].pin, columnBit0);
	HAL_GPIO_WritePin(COLUMN_BIT_TO_PIN_TABLE[1].bus, COLUMN_BIT_TO_PIN_TABLE[1].pin, columnBit1);
	HAL_GPIO_WritePin(COLUMN_BIT_TO_PIN_TABLE[2].bus, COLUMN_BIT_TO_PIN_TABLE[2].pin, columnBit2);
}

static GPIO_PinState ReadRow(uint8_t rowNumber)
{
	struct GPIO_Pin rowPin = ROW_NUMBER_TO_PIN_TABLE[rowNumber];
	GPIO_PinState value = HAL_GPIO_ReadPin(rowPin.bus, rowPin.pin);

	return value;
}

void HandleLowToHigh(struct PieceCoordinate pieceCoordinate)
{
	// Don't do anything except update board if piece did not move
	if(IsPieceCoordinateSamePosition(pieceCoordinate, LastPickedUpPiece))
	{
		SetPiece(pieceCoordinate.row, pieceCoordinate.column, LastPickedUpPiece.piece);
		LastTransitionType = PLACE;
		return;
	}

	// If player filled the expected position of the KING during castle
	if(IsPieceCoordinateSamePosition(ExpectedKingCastleCoordinate, pieceCoordinate))
	{
		SetPiece(pieceCoordinate.row, pieceCoordinate.column, ExpectedKingCastleCoordinate.piece);
		ExpectedKingCastleCoordinate = EMPTY_PIECE_COORDINATE;

		// If ROOK finished castle as well, switch turns
		if(IsPieceCoordinateEqual(ExpectedRookCastleCoordinate, EMPTY_PIECE_COORDINATE))
		{
			SwitchTeam();
		}
	}
	// If player filled the expected position of the ROOK during castle
	else if(IsPieceCoordinateSamePosition(ExpectedRookCastleCoordinate, pieceCoordinate))
	{
		SetPiece(pieceCoordinate.row, pieceCoordinate.column, ExpectedRookCastleCoordinate.piece);
		ExpectedRookCastleCoordinate = EMPTY_PIECE_COORDINATE;

		// If KING finished castle as well, switch turns
		if(IsPieceCoordinateEqual(ExpectedKingCastleCoordinate, EMPTY_PIECE_COORDINATE))
		{
			SwitchTeam();
		}
	}
	// Any other move, the last picked up piece is set to this position
	else
	{
		SetPiece(pieceCoordinate.row, pieceCoordinate.column, LastPickedUpPiece.piece);
		SwitchTeam();
	}

	// Calculate/Handle Checkmate //
	/// @todo

	LastTransitionType = PLACE;
}

void HandleHighToLow(struct PieceCoordinate pieceCoordinate)
{
	SetPiece(pieceCoordinate.row, pieceCoordinate.column, EMPTY_PIECE);

	// Handle Killing //

	// Other team picked up piece right before picking up this team's piece - either killing this piece or illegal
	if(DidOtherTeamPickupLast(pieceCoordinate.piece))
	{
		KillPiece(pieceCoordinate.piece);

		/// @todo Expect LastPickedUpPiece to be placed in pieceCoordinate's place
		return;
	}


	// Handle Castling //

	// Same team picked up piece twice in a row - either castling or illegal
	if(DidSameTeamPickupLast(pieceCoordinate.piece))
	{
		// If current piece picked up is ROOK and last piece was KING or vice versa, castling is occurring
		if(pieceCoordinate.piece.type == ROOK && LastPickedUpPiece.piece.type == KING)
		{
			CalculateCastlingPositions(pieceCoordinate, &ExpectedKingCastleCoordinate, &ExpectedRookCastleCoordinate);
		}
		else if(pieceCoordinate.piece.type == KING && LastPickedUpPiece.piece.type == ROOK)
		{
			CalculateCastlingPositions(LastPickedUpPiece, &ExpectedKingCastleCoordinate, &ExpectedRookCastleCoordinate);
		}
		else
		{
			/// @todo Illegal move, cannot have two picked up pieces at the same time that aren't ROOK/KING
		}
	}

	// Handle Promotion //
	/// @todo



	LastPickedUpPiece = pieceCoordinate;
	LastTransitionType = PICKUP;
}

void Track()
{
	for(uint8_t column = 0; column < NUM_COLS; column++)
	{
		WriteColumn(column);

		for(uint8_t row = 0; row < NUM_ROWS; row++)
		{
			GPIO_PinState cellValue = ReadRow(row);

			struct PieceCoordinate currentPieceCoordinate = {
				.piece = GetPiece(row, column),
				.row = row,
				.column = column
			};

			// If there was no piece here but the IO is HIGH, a previously picked-up piece was placed
			if((currentPieceCoordinate.piece.type == NONE) && (cellValue == GPIO_PIN_SET))
			{
				HandleLowToHigh(currentPieceCoordinate);
			}

			// If there was a piece here but the IO is LOW, a piece has been picked up
			else if ((currentPieceCoordinate.piece.type != NONE) && (cellValue == GPIO_PIN_RESET))
			{
				HandleHighToLow(currentPieceCoordinate);
			}
		}
	}
}

uint8_t ValidateStartPositions()
{
	for(uint8_t columnNumber = 0; columnNumber < NUM_COLS; columnNumber++)
	{
		WriteColumn(columnNumber);
		for(uint8_t rowNumber = 0; rowNumber < NUM_ROWS; rowNumber++)
		{
			GPIO_PinState cellValue = ReadRow(rowNumber);

			switch(rowNumber)
			{

			// Make sure rows 0, 1, 6, 7 are all filled with pieces
			case 0:
			case 1:
			case 6:
			case 7:
				if(cellValue == GPIO_PIN_RESET)
				{
					return 0;
				}
				break;

			// Make sure other rows do not have a piece
			default:
				if(cellValue == GPIO_PIN_SET)
				{
					return 0;
				}
			}
		}
	}

	return 1;
}

void SwitchTeam()
{
	/// @todo Invoke necessary peripherals to switch teams
}

void CalculateCastlingPositions(
		struct PieceCoordinate rookPieceCoordinate,
		struct PieceCoordinate* expectedKingPieceCoordinate, struct PieceCoordinate* expectedRookPieceCoordinate)
{
	// Fill in the piece attributes
	expectedKingPieceCoordinate->piece.owner = rookPieceCoordinate.piece.owner;
	expectedRookPieceCoordinate->piece.owner = rookPieceCoordinate.piece.owner;
	expectedKingPieceCoordinate->piece.type = KING;
	expectedRookPieceCoordinate->piece.type = ROOK;

	// Calculate expected ROOK and KING columns
	if(rookPieceCoordinate.column == 0)
	{
		expectedKingPieceCoordinate->column = 2;
		expectedRookPieceCoordinate->column = 3;
	}
	else if(rookPieceCoordinate.column == 7)
	{
		expectedKingPieceCoordinate->column = 6;
		expectedRookPieceCoordinate->column = 5;
	}

	// Calculate expected ROOK and KING rows
	if(rookPieceCoordinate.piece.owner == WHITE)
	{
		expectedKingPieceCoordinate->row = expectedRookPieceCoordinate->row = 0;
	}
	else if(rookPieceCoordinate.piece.owner == BLACK)
	{
		expectedKingPieceCoordinate->row = expectedRookPieceCoordinate->row = 7;
	}
}



uint8_t KingCanCastle(struct PieceCoordinate kingCoordinate)
{
	if(kingCoordinate.column == 4)
	{
		if(kingCoordinate.piece.owner == WHITE && kingCoordinate.row == 0)
		{
			return 1;
		}
		else if(kingCoordinate.piece.owner == BLACK && kingCoordinate.row == 7)
		{
			return 1;
		}
	}

	return 0;
}

uint8_t RookCanCastle(struct PieceCoordinate rookCoordinate)
{
	if(rookCoordinate.column == 0 || rookCoordinate.column == 7)
	{
		if(rookCoordinate.piece.owner == WHITE && rookCoordinate.row == 0)
		{
			return 1;
		}
		else if(rookCoordinate.piece.owner == BLACK && rookCoordinate.row == 7)
		{
			return 1;
		}
	}

	return 0;
}

void KillPiece(struct Piece piece)
{
	/// @todo handle killing a piece - maybe nothing needs to be done here...
}

inline void SetPiece(uint8_t row, uint8_t column, struct Piece piece)
{
	Chessboard[row][column] = piece;
}

inline struct Piece GetPiece(uint8_t row, uint8_t column)
{
	return Chessboard[row][column];
}

inline uint8_t DidOtherTeamPickupLast(struct Piece piece)
{
	return LastTransitionType == PICKUP && LastPickedUpPiece.piece.owner != piece.owner;
}

inline uint8_t DidSameTeamPickupLast(struct Piece piece)
{
	return LastTransitionType == PICKUP && LastPickedUpPiece.piece.owner == piece.owner;
}

inline uint8_t IsPieceEqual(struct Piece piece1, struct Piece piece2)
{
	return piece1.owner == piece2.owner
			&& piece1.type == piece2.type;
}

inline uint8_t IsPieceCoordinateEqual(struct PieceCoordinate pieceCoordinate1, struct PieceCoordinate pieceCoordinate2)
{
	return IsPieceEqual(pieceCoordinate1.piece, pieceCoordinate2.piece)
			&& pieceCoordinate1.row == pieceCoordinate2.row
			&& pieceCoordinate1.column == pieceCoordinate2.column;
}

inline uint8_t IsPieceCoordinateSamePosition(struct PieceCoordinate pieceCoordinate1, struct PieceCoordinate pieceCoordinate2)
{
	return pieceCoordinate1.row == pieceCoordinate2.row && pieceCoordinate1.column == pieceCoordinate2.column;
}
