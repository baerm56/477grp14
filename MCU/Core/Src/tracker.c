#include <math.h>
#include "tracker.h"
#include "pathfinder.h"
#include "types.h"
#include "leds.h"
#include "button.h"
#include "uart.h"
#include "waveplayer.h"

// Debouncing //
static void AppendHistory(uint8_t row, uint8_t column, uint8_t cellValue);
static uint8_t IsHistoryConsensus(uint8_t row, uint8_t column);

// Placement Handlers //
static void HandlePlace(struct PieceCoordinate placedPiece);
static void HandlePlaceIllegalState(struct PieceCoordinate placedPiece);
static void HandlePlaceKiller(struct PieceCoordinate placedPiece);
static void HandlePlaceCastling(struct PieceCoordinate placedPiece);
static void HandlePlaceMove(struct PieceCoordinate placedPiece);
static void HandlePlaceNoMove(struct PieceCoordinate placedPiece);
static void HandlePlacePreemptPromotion(struct PieceCoordinate placedPiece);
static void HandlePlacePromotion(struct PieceCoordinate placedPiece);

// Pickup Handlers //
static void HandlePickup(struct PieceCoordinate pickedUpPiece);
static void HandlePickupIllegalState(struct PieceCoordinate pickedUpPiece);
static void HandlePickupVictim(struct PieceCoordinate pickedUpPiece);
static void HandlePickupKiller(struct PieceCoordinate pickedUpPiece);
static void HandlePickupCastling(struct PieceCoordinate pickedUpPiece);
static void HandlePickupMove(struct PieceCoordinate pickedUpPiece);
static void HandlePickupPromotion(struct PieceCoordinate pickedUpPiece);

// Internal Updaters //
static void UpdateCastleFlags();
static void AddIllegalPiece(struct PieceCoordinate current, struct PieceCoordinate destination);
static void RemoveIllegalPiece(uint8_t index);
static void CheckChessboardValidity(uint8_t switchTurns);
static void EndTurn();
static void SetPiece(uint8_t row, uint8_t column, struct Piece piece);
static void SetPieceCoordinate(struct PieceCoordinate pieceCoordinate);
static void ClearPiece(struct PieceCoordinate* pieceCoordinate);
static void CheckForPromotion();

// Legal Move Detection //
static uint8_t ValidateMove(struct PieceCoordinate from, struct PieceCoordinate to);
static uint8_t ValidateKill(struct PieceCoordinate victim, struct PieceCoordinate killer);
static uint8_t ValidateCastling(struct PieceCoordinate rook, struct PieceCoordinate king);
static uint8_t DidOtherTeamPickupLast(struct Piece piece);
static uint8_t DidSameTeamPickupLast(struct Piece piece);

// Utilities //
static uint8_t PawnReachedEnd(struct PieceCoordinate pieceCoordinate);
static uint8_t PieceExists(struct PieceCoordinate placedPiece);
static void GetPiecesForTeam(enum PieceOwner team, enum PieceType type, struct PieceCoordinate* pieces, uint8_t* numPieces);

// LEDs
static void IlluminatePieceCoordinates(struct PieceCoordinate* pieceCoordinates, uint8_t numPieceCoordinates);
static void IlluminateCoordinates(struct Coordinate* coordinates, uint8_t numCoordinates);

// Debouncing //
uint8_t History[NUM_ROWS][NUM_COLS][NUM_HISTORY_ENTRIES];

// State Fields //
static struct Piece Chessboard[NUM_ROWS][NUM_COLS];
static enum PieceOwner CurrentTurn;
static enum TransitionType LastTransitionType;
static struct PieceCoordinate LastPickedUpPiece;

// Legal Piece Detection/Recovery Fields //
static struct PieceCoordinate Victim;
static struct PieceCoordinate Killer;
static struct IllegalMove IllegalPieces[MAX_ILLEGAL_PIECES];
static uint8_t NumIllegalPieces;
static uint8_t SwitchTurnsAfterLegalState;

// Castling //
static uint8_t CanA1Castle;
static uint8_t CanH1Castle;
static uint8_t CanA8Castle;
static uint8_t CanH8Castle;
static uint8_t CanWhiteKingCastle;
static uint8_t CanBlackKingCastle;
static struct PieceCoordinate ExpectedKingCastleCoordinate;
static struct PieceCoordinate ExpectedRookCastleCoordinate;

// Promotion //
static struct PieceCoordinate PawnToPromote;

// Integration //
enum GameMode CurrentGameMode;
extern SPI_HandleTypeDef hspi1; // LED matrix controller
extern SPI_HandleTypeDef hspi2; // Timer
extern UART_HandleTypeDef huart1;
extern DAC_HandleTypeDef hdac;
char * audio[] = {"a.wav", "b.wav", "c.wav", "d.wav", "e.wav", "f.wav", "g.wav", "h.wav",
		  	  	  "1.wav", "2.wav", "3.wav", "4.wav", "5.wav", "6.wav", "7.wav", "8.wav",
				  "jazz.wav"};

// Uart Move Buffer //
char MoveBuffer[4] = {0};
char ReceiveBuffer[5] = {0};

void InitTracker(enum GameMode gameMode)
{
	// Initialize globals
	CurrentGameMode = gameMode;
	LastTransitionType = PLACE;
	CurrentTurn = WHITE;
	CanA1Castle = 1;
	CanH1Castle = 1;
	CanA8Castle = 1;
	CanH8Castle = 1;
	CanWhiteKingCastle = 1;
	CanBlackKingCastle = 1;
	SwitchTurnsAfterLegalState = 0;

	ClearPiece(&LastPickedUpPiece);
	ClearPiece(&Victim);
	ClearPiece(&Killer);
	ClearPiece(&ExpectedKingCastleCoordinate);
	ClearPiece(&ExpectedRookCastleCoordinate);
	ClearPiece(&PawnToPromote);

	// Initialize the Chessboard to the initial chessboard and zero the debouncing Histroy array
	for (uint8_t column = 0; column < NUM_COLS; column++)
	{
		for (uint8_t row = 0; row < NUM_ROWS; row++)
		{
			Chessboard[row][column] = INITIAL_CHESSBOARD[row][column];

			// Initialize debouncing history array
			for(uint8_t i = 0; i < NUM_HISTORY_ENTRIES; i++)
			{
				History[row][column][i] = 0;
			}
		}
	}

	// Initialize illegal piece destinations to empty pieces
	NumIllegalPieces = 0;
	for (uint8_t i = 0; i < MAX_ILLEGAL_PIECES; i++)
	{
		IllegalPieces[i].destination = EMPTY_PIECE_COORDINATE;
		IllegalPieces[i].current = EMPTY_PIECE_COORDINATE;
	}

	// Initialize PathFinder
	CalculateTeamsLegalMoves(CurrentTurn);
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

static uint8_t ReadRow(uint8_t rowNumber)
{
	struct GPIO_Pin rowPin = ROW_NUMBER_TO_PIN_TABLE[rowNumber];
	return HAL_GPIO_ReadPin(rowPin.bus, rowPin.pin);
}

uint8_t Track()
{
	uint8_t transitionOccured = 0;

	for (uint8_t column = 0; column < NUM_COLS; column++)
	{
		WriteColumn(column);

		for (uint8_t row = 0; row < NUM_ROWS; row++)
		{
			uint8_t cellValue = ReadRow(row);

			// Add cellValue to front of debouncing History array for this cell
			AppendHistory(row, column, cellValue);

			// If the debouncing History of this cell is all the same value, then we can consider it for a transition
			if(IsHistoryConsensus(row, column))
			{
				struct PieceCoordinate currentPieceCoordinate = GetPieceCoordinate(row, column);

				// If there was no piece here but the IO is HIGH, a piece was placed
				if ((currentPieceCoordinate.piece.type == NONE) && (cellValue == 1))
				{
					HandlePlace(currentPieceCoordinate);
					transitionOccured = 1;
				}

				// If there was a piece here but the IO is LOW, a piece has been picked up
				else if ((currentPieceCoordinate.piece.type != NONE) && (cellValue == 0))
				{
					HandlePickup(currentPieceCoordinate);
					transitionOccured = 1;
				}
			}
		}
	}

	return transitionOccured;
}

static void AppendHistory(uint8_t row, uint8_t column, uint8_t cellValue)
{
	// Append cellValue to front of History array
	for(int8_t i = NUM_HISTORY_ENTRIES - 1; i > 0; i--)
	{
		History[row][column][i] = History[row][column][i - 1];
	}
	History[row][column][0] = cellValue;
}

static uint8_t IsHistoryConsensus(uint8_t row, uint8_t column)
{
	// If any two History values are the same, there is not a consensus
	for(uint8_t i = 0; i < NUM_HISTORY_ENTRIES - 1; i++)
	{
		if(History[row][column][i] != History[row][column][i + 1])
		{
			return 0;
		}
	}

	return 1;
}

static void HandlePlace(struct PieceCoordinate placedPiece)
{
	// If board is in illegal state
	if (NumIllegalPieces > 0)
	{
		HandlePlaceIllegalState(placedPiece);
	}

	// If promotion is occurring, this placed piece must be a knight or queen placed into PawnToPromote's place
	else if (PieceExists(PawnToPromote))
	{
		HandlePlacePromotion(placedPiece);
	}

	// If there's a piece being killed, this placement should be in its stead
	else if (PieceExists(Victim))
	{
		HandlePlaceKiller(placedPiece);
	}

	// If the piece lifted did not move, don't do anything except update Chessboard
	else if (IsPieceCoordinateSamePosition(placedPiece, LastPickedUpPiece))
	{
		HandlePlaceNoMove(placedPiece);
	}

	// If player is castling, this placement should be the king or rook being placed in the right spots
	else if (PieceExists(ExpectedKingCastleCoordinate) || PieceExists(ExpectedRookCastleCoordinate))
	{
		HandlePlaceCastling(placedPiece);
	}

	// Any other move, the last picked up piece is set to this position
	else
	{
		HandlePlaceMove(placedPiece);
	}

	LastTransitionType = PLACE;
}

static void HandlePlaceIllegalState(struct PieceCoordinate placedPiece)
{
	for (uint8_t i = 0; i < NumIllegalPieces; i++)
	{
		// If placing an illegal piece in it's proper destination, remove it from the illegal pieces array
		if (IsPieceCoordinateSamePosition(IllegalPieces[i].destination, placedPiece))
		{
			SetPiece(placedPiece.row, placedPiece.column, IllegalPieces[i].destination.piece);

			// Remove from illegal pieces array
			RemoveIllegalPiece(i);

			// If chessboard is valid, switch turns if flagged to do so
			CheckChessboardValidity(SwitchTurnsAfterLegalState);

			return;
		}
	}

	// A piece was placed in an unexpected destination, add it as an illegal piece that must be removed from the board
	SetPiece(placedPiece.row, placedPiece.column, LastPickedUpPiece.piece);
	AddIllegalPiece(placedPiece, OFFBOARD_PIECE_COORDINATE);
}

static void HandlePlaceNoMove(struct PieceCoordinate placedPiece)
{
	SetPiece(placedPiece.row, placedPiece.column, LastPickedUpPiece.piece);

	// Turn off all squares
	IlluminateCoordinates(NULL, 0);
}

static void HandlePlaceKiller(struct PieceCoordinate placedPiece)
{
	// If there's no killer, then a kill isn't occurring so put victim back
	if(!PieceExists(Killer))
	{
		AddIllegalPiece(placedPiece, Victim);
		ClearPiece(&Victim);
		return;
	}
	SetPiece(placedPiece.row, placedPiece.column, LastPickedUpPiece.piece);

	// If player put killer in victim's place, clear Victim and Killer
	if (IsPieceCoordinateSamePosition(Victim, placedPiece))
	{
		// Store move for AI
		if(CurrentTurn == WHITE)
		{
			MoveBuffer[2] = 'h' - placedPiece.column;
			MoveBuffer[3] = '1' + placedPiece.row;
		}

		ClearPiece(&Killer);
		ClearPiece(&Victim);
		EndTurn();
	}
	// If player didn't put killer in the victim's spot, must put the killer in the victim spot
	else
	{
		// Put killer in victim spot
		struct PieceCoordinate killerDestination = Victim;
		killerDestination.piece = LastPickedUpPiece.piece;
		AddIllegalPiece(placedPiece, killerDestination);
		SwitchTurnsAfterLegalState = 1;
	}
}

static void HandlePlaceCastling(struct PieceCoordinate placedPiece)
{
	// If placing a piece in the King's expected location, assume it's a king and place it
	if (IsPieceCoordinateSamePosition(ExpectedKingCastleCoordinate, placedPiece))
	{
		SetPiece(placedPiece.row, placedPiece.column, ExpectedKingCastleCoordinate.piece);
		ClearPiece(&ExpectedKingCastleCoordinate);
	}
	// If placing a piece in the Rook's expected location, assume it's a rook and place it
	else if (IsPieceCoordinateSamePosition(ExpectedRookCastleCoordinate, placedPiece))
	{
		SetPiece(placedPiece.row, placedPiece.column, ExpectedRookCastleCoordinate.piece);
		ClearPiece(&ExpectedRookCastleCoordinate);
	}
	// If placing piece in wrong location
	else
	{
		// If King wasn't already placed in correct spot, put it in the correct spot
		if (PieceExists(ExpectedKingCastleCoordinate))
		{
			SetPiece(placedPiece.row, placedPiece.column, ExpectedKingCastleCoordinate.piece); // Assume the king was placed here (doesn't matter)
			AddIllegalPiece(placedPiece, ExpectedKingCastleCoordinate);
			SwitchTurnsAfterLegalState = 1;
		}

		// If Rook wasn't already placed in correct spot, put it in correct spot
		if (PieceExists(ExpectedRookCastleCoordinate))
		{
			SetPiece(placedPiece.row, placedPiece.column, ExpectedRookCastleCoordinate.piece); // Assume the rook was placed here (doesn't matter)
			AddIllegalPiece(placedPiece, ExpectedRookCastleCoordinate);
			SwitchTurnsAfterLegalState = 1;
		}
	}

	// If castling has been fulfilled
	if (!PieceExists(ExpectedKingCastleCoordinate) && !PieceExists(ExpectedRookCastleCoordinate))
	{
		MoveBuffer[0] = 'e';
		MoveBuffer[1] = '1';
		MoveBuffer[2] = 'h' - ExpectedKingCastleCoordinate.column;
		MoveBuffer[3] = '1' + ExpectedKingCastleCoordinate.row;
		EndTurn();
	}
}

static void HandlePlaceMove(struct PieceCoordinate placedPiece)
{
	uint8_t isMoveValid = ValidateMove(LastPickedUpPiece, placedPiece);
	SetPiece(placedPiece.row, placedPiece.column, LastPickedUpPiece.piece);

	if (isMoveValid)
	{
		// Store move to send to AI
		if(CurrentTurn == WHITE)
		{
			MoveBuffer[2] = 'h' - placedPiece.column;
			MoveBuffer[3] = '1' + placedPiece.row;
		}

		EndTurn();
	}
	// If move was invalid, put piece back
	else
	{
		AddIllegalPiece(placedPiece, LastPickedUpPiece);
	}
}

static void HandlePlacePreemptPromotion(struct PieceCoordinate placedPiece)
{
	PawnToPromote = placedPiece;

	// Illuminate the PawnToPromote
	IlluminatePieceCoordinates(&PawnToPromote, 1);
}

static void HandlePlacePromotion(struct PieceCoordinate placedPiece)
{
	// If placed the promoted piece back into the pawn's old spot, get the PieceType (knight or queen) from the last button pressed and set the piece as that type
	if (IsPieceCoordinateSamePosition(placedPiece, PawnToPromote))
	{
		// Get last button pressed, choose the piece type based on which button was pressed
		enum PieceType selectedPieceType;
		switch(GetLastButtonPressed())
		{
			case QUEEN_BUTTON:
				selectedPieceType = QUEEN;
				break;
			case KNIGHT_BUTTON:
				selectedPieceType = KNIGHT;
				break;
			default:
				/// @todo: play audio cue to select the piece type to promote to
				return;
		}

		// Set piece on chessboard and end the turn
		struct Piece promotedPiece = {selectedPieceType, CurrentTurn};
		SetPiece(placedPiece.row, placedPiece.column, promotedPiece);
		ClearPiece(&PawnToPromote);
		EndTurn();
	}

	// If player doesn't place the promotion into the pawn's old spot, it must be placed in the right spot
	else
	{
		AddIllegalPiece(placedPiece, PawnToPromote);
	}
}



static void HandlePickup(struct PieceCoordinate pickedUpPiece)
{
	SetPiece(pickedUpPiece.row, pickedUpPiece.column, EMPTY_PIECE);

	// If a piece is picked up during an illegal state, if it's not an illegal piece it is NOW illegal
	if (NumIllegalPieces > 0)
	{
		HandlePickupIllegalState(pickedUpPiece);
	}

	// If player picked up piece from other team, they will kill it
	else if (pickedUpPiece.piece.owner != CurrentTurn)
	{
		HandlePickupVictim(pickedUpPiece);
	}

	// If there's a piece to kill, this picked up piece must be able to kill it
	else if (PieceExists(Victim))
	{
		HandlePickupKiller(pickedUpPiece);
	}

	// If there's a pawn to promote, the picked up piece must be this pawn
	else if (PieceExists(PawnToPromote))
	{
		HandlePickupPromotion(pickedUpPiece);
	}

	// Same team picked up piece twice in a row, so castling is occurring
	else if (DidSameTeamPickupLast(pickedUpPiece.piece))
	{
		HandlePickupCastling(pickedUpPiece);
	}

	// If simple pickup
	else
	{
		HandlePickupMove(pickedUpPiece);
	}

	LastPickedUpPiece = pickedUpPiece;
	LastTransitionType = PICKUP;
}

static void HandlePickupIllegalState(struct PieceCoordinate pickedUpPiece)
{
	for (uint8_t i = 0; i < NumIllegalPieces; i++)
	{
		// If pickup for illegal piece, let it slide
		if (IsPieceCoordinateSamePosition(IllegalPieces[i].current, pickedUpPiece))
		{
			// If pickup an illegal piece which is to be removed from the board is picked up, it is no longer illegal
			if (IsPieceCoordinateEqual(IllegalPieces[i].destination, OFFBOARD_PIECE_COORDINATE))
			{
				// Remove from illegal pieces array
				RemoveIllegalPiece(i);

				// If chessboard is valid, switch turns if flagged to do so
				CheckChessboardValidity(SwitchTurnsAfterLegalState);
			}
			return;
		}
	}

	// Player picked up a piece that wasn't illegal, so it must be added as an illegal piece which must be placed back
	AddIllegalPiece(OFFBOARD_PIECE_COORDINATE, pickedUpPiece);
}

static void HandlePickupVictim(struct PieceCoordinate pickedUpPiece)
{
	Victim = pickedUpPiece;
}

static void HandlePickupKiller(struct PieceCoordinate pickedUpPiece)
{
	// If pickedUpPiece can kill it, illuminate Victim's spot
	if(ValidateKill(Victim, pickedUpPiece))
	{
		Killer = pickedUpPiece;
		IlluminatePieceCoordinates(&Victim, 1);

		// Store move for AI
		if(CurrentTurn == WHITE)
		{
			MoveBuffer[0] = 'h' - pickedUpPiece.column;
			MoveBuffer[1] = '1' + pickedUpPiece.row;
		}
	}
	// If pickedUpPiece can't kill Victim, they need to be put back to their initial positions, and Victim is not a piece to kill anymore
	else
	{
		AddIllegalPiece(OFFBOARD_PIECE_COORDINATE, Victim);
		AddIllegalPiece(OFFBOARD_PIECE_COORDINATE, pickedUpPiece);
		ClearPiece(&Victim);
	}
}

static void HandlePickupCastling(struct PieceCoordinate pickedUpPiece)
{
	struct PieceCoordinate rook;
	struct PieceCoordinate king;

	if (pickedUpPiece.piece.type == ROOK && LastPickedUpPiece.piece.type == KING)
	{
		rook = pickedUpPiece;
		king = LastPickedUpPiece;
	}
	else if (pickedUpPiece.piece.type == KING && LastPickedUpPiece.piece.type == ROOK)
	{
		rook = LastPickedUpPiece;
		king = pickedUpPiece;
	}
	// If the past two picked up pieces aren't a king and rook, put them back
	else
	{
		AddIllegalPiece(OFFBOARD_PIECE_COORDINATE, pickedUpPiece);
		AddIllegalPiece(OFFBOARD_PIECE_COORDINATE, LastPickedUpPiece);
		return;
	}

	if (ValidateCastling(rook, king))
	{
		struct PieceCoordinate expectedKingPieceCoordinate;
		struct PieceCoordinate expectedRookPieceCoordinate;
		CalculateCastlingPositions(rook, &expectedKingPieceCoordinate, &expectedRookPieceCoordinate);

		// If castling won't result in a self-check then it's valid so copy to globals. Otherwise fall through to AddIllegalPiece.
		if (!WillResultInSelfCheck(rook, expectedRookPieceCoordinate) && !WillResultInSelfCheck(king, expectedKingPieceCoordinate))
		{
			ExpectedKingCastleCoordinate = expectedKingPieceCoordinate;
			ExpectedRookCastleCoordinate = expectedRookPieceCoordinate;

			// Illuminate LEDs to the target castling locations
			struct PieceCoordinate pieceCoordinatesToIlluminate[] = { expectedKingPieceCoordinate, expectedRookPieceCoordinate};
			IlluminatePieceCoordinates(pieceCoordinatesToIlluminate, sizeof(pieceCoordinatesToIlluminate) / sizeof(*pieceCoordinatesToIlluminate));

			return;
		}
	}

	AddIllegalPiece(OFFBOARD_PIECE_COORDINATE, pickedUpPiece);
	AddIllegalPiece(OFFBOARD_PIECE_COORDINATE, LastPickedUpPiece);
}

static void HandlePickupPromotion(struct PieceCoordinate pickedUpPiece)
{
	// All picked up pieces during a promotion must be the PawnToPromote, otherwise they must be placed back
	if (!IsPieceCoordinateEqual(pickedUpPiece, PawnToPromote))
	{
		AddIllegalPiece(OFFBOARD_PIECE_COORDINATE, pickedUpPiece);
	}
}

static void HandlePickupMove(struct PieceCoordinate pickedUpPiece)
{
	// If this piece isn't owned by the current team, then they must put it back down
	if (pickedUpPiece.piece.owner != CurrentTurn)
	{
		AddIllegalPiece(EMPTY_PIECE_COORDINATE, pickedUpPiece);
	}
	else
	{
		// If in practice mode, don't illuminate the legal paths of the AI (BLACK)
		// In other mode, illuminate the legal paths of BLACK
		if (!(CurrentGameMode == PRACTICE && CurrentTurn == BLACK))
		{
			// Calculate all legal paths
			struct Coordinate allLegalPaths[MAX_LEGAL_MOVES];
			uint8_t numLegalPaths;
			CalculateAllLegalPathsAndChecks(pickedUpPiece, allLegalPaths, &numLegalPaths);

			// Illuminate legal paths on LEDs
			IlluminateCoordinates(allLegalPaths, numLegalPaths);
		}

		// Store move for AI
		if(CurrentTurn == WHITE)
		{
			MoveBuffer[0] = 'h' - pickedUpPiece.column;
			MoveBuffer[1] = '1' + pickedUpPiece.row;
		}
	}
}

static void IlluminatePieceCoordinates(struct PieceCoordinate* pieceCoordinates, uint8_t numPieceCoordinates)
{
	uint8_t board[NUM_ROWS][NUM_COLS] = {0};
	for(uint8_t i = 0; i < numPieceCoordinates; i++)
	{
		board[pieceCoordinates[i].row][pieceCoordinates[i].column] = 1;
	}
	writeBoardValue(&hspi1, board);
}

static void IlluminateCoordinates(struct Coordinate* coordinates, uint8_t numCoordinates)
{
	uint8_t board[NUM_ROWS][NUM_COLS] = {0};
	for(uint8_t i = 0; i < numCoordinates; i++)
	{
		board[coordinates[i].row][coordinates[i].column] = 1;
	}
	writeBoardValue(&hspi1, board);
}

/**
 * @brief Put an illegal piece in the IllegalPieceDestinations array. Destination is the correct destination of the piece and Current is the current position of the piece.
 */
static void AddIllegalPiece(struct PieceCoordinate current, struct PieceCoordinate destination)
{
	current.piece = destination.piece;

	// Do not add duplicate pieces
	for(uint8_t i = 0; i < NumIllegalPieces; i++)
	{
		if(IsPieceCoordinateSamePosition(IllegalPieces[i].current, current))
		{
			return;
		}
	}

	// Add illegal pieces to array
	IllegalPieces[NumIllegalPieces].current = current;
	IllegalPieces[NumIllegalPieces].destination = destination;
	NumIllegalPieces++;

	// Illuminate all illegal pieces
	uint8_t j = 0;
	struct PieceCoordinate pieceCoordinates[2 * MAX_ILLEGAL_PIECES] = {0};
	for(uint8_t i = 0; i < NumIllegalPieces; i++)
	{
		pieceCoordinates[j++] = IllegalPieces[i].current;
		pieceCoordinates[j++] = IllegalPieces[i].destination;
	}
	IlluminatePieceCoordinates(pieceCoordinates, sizeof(pieceCoordinates) / sizeof(*pieceCoordinates));
}

/**
 * @brief Remove illegal piece from IllegalPieces array given its index
 */
static void RemoveIllegalPiece(uint8_t index)
{
	// Remove illegal piece from array
	NumIllegalPieces--;
	for (uint8_t i = index; i < NumIllegalPieces; i++)
	{
		IllegalPieces[i] = IllegalPieces[i + 1];
	}

	// Unilluminate the piece that was removed
	uint8_t j = 0;
	struct PieceCoordinate pieceCoordinates[2 * MAX_ILLEGAL_PIECES] = {0};
	for(uint8_t i = 0; i < NumIllegalPieces; i++)
	{
		pieceCoordinates[j++] = IllegalPieces[i].current;
		pieceCoordinates[j++] = IllegalPieces[i].destination;
	}
	IlluminatePieceCoordinates(pieceCoordinates, sizeof(pieceCoordinates) / sizeof(*pieceCoordinates));
}

/**
 * @brief Check if chessboard is valid and switch turns if flagged to do so
 */
static void CheckChessboardValidity(uint8_t switchTurns)
{
	if (NumIllegalPieces == 0)
	{
		if (switchTurns)
		{
			EndTurn();
		}
	}
}

/**
 * @brief Return 1 if the given killer can take the victim, 0 otherwise. If the victim cannot be killed, then this is an illegal/impossible kill
 * so the victim and killer must return to their original spots, and a new move must be done.
 */
static uint8_t ValidateKill(struct PieceCoordinate victim, struct PieceCoordinate killer)
{
	// Temporarily add back victim and then check if it can be killed (need to be done for PAWN)
	SetPieceCoordinate(victim);

	uint8_t valid = ValidateMove(killer, victim);

	// Clear victim again
	victim.piece = EMPTY_PIECE;
	SetPieceCoordinate(victim);

	return valid;
}

/**
 * @brief Return 1 if the "to" is in the legal paths for "from", 0 otherwise. If the move is invalid, then the "from" must be placed back
 * in its original spot, and a new move must be done.
 */
static uint8_t ValidateMove(struct PieceCoordinate from, struct PieceCoordinate to)
{
	return IsLegalMove(from, to);
}

/**
 * @brief Return 1 if the given rook can castle with the given king. If not, they should return to their original positions.
 */
static uint8_t ValidateCastling(struct PieceCoordinate rook, struct PieceCoordinate king)
{
	// If white king can castle and the king and rook are in the starting row
	if (king.row == 0 && rook.row == 0 && CanWhiteKingCastle)
	{
		return (rook.column == 0 && CanA1Castle) || (rook.column == 7 && CanH1Castle);
	}
	// If black king can castle and the king and rook are in the starting row
	else if (king.row == 7 && rook.row == 7 && CanBlackKingCastle)
	{
		return (rook.column == 0 && CanA8Castle) || (rook.column == 7 && CanH8Castle);
	}
	return 0;
}

uint8_t ValidateStartPositions()
{
	struct Coordinate invalidCoordinates[NUM_COLS * NUM_ROWS] = {0};
	uint8_t numInvalidCoordinates = 0;

	for (uint8_t column = 0; column < NUM_COLS; column++)
	{
		WriteColumn(column);
		for (uint8_t row = 0; row < NUM_ROWS; row++)
		{
			GPIO_PinState cellValue = ReadRow(row);
			struct Coordinate coordinate = {row, column};

			if(cellValue == GPIO_PIN_SET && INITIAL_CHESSBOARD[row][column].type == NONE)
			{
				invalidCoordinates[numInvalidCoordinates++] = coordinate;
			}

			if(cellValue == GPIO_PIN_RESET && INITIAL_CHESSBOARD[row][column].type != NONE)
			{
				invalidCoordinates[numInvalidCoordinates++] = coordinate;
			}
		}
	}


	IlluminateCoordinates(invalidCoordinates, numInvalidCoordinates);
	return (numInvalidCoordinates == 0);
}

static void EndTurn()
{
	// Check for a promotion. If found, do not end turn, HandlePlacePreemptPromotion
	CheckForPromotion();
	if (PieceExists(PawnToPromote))
	{
		return;
	}

	// Turn LEDs off
	IlluminateCoordinates(NULL, 0);

	// Check if any rooks or kings moved so they can be flagged as not castleable
	UpdateCastleFlags();

	SwitchTurnsAfterLegalState = 0;

	// Switch teams
	CurrentTurn = CurrentTurn == WHITE ? BLACK : WHITE;

	// If in practice mode and the current turn is the AI, generate a move
	if(CurrentGameMode == PRACTICE && CurrentTurn == BLACK)
	{
		ReceiveBuffer[0] = '-';
		ReceiveBuffer[1] = '-';
		ReceiveBuffer[2] = '-';
		ReceiveBuffer[3] = '-';
		ReceiveBuffer[4] = '-';
		sendMove(&huart1, MoveBuffer);
		receiveData(&huart1, ReceiveBuffer);
		if (memcmp(ReceiveBuffer, "-----", 5) != 0){
			struct Coordinate from[2] = {0};
			from[0].row = (int8_t)(ReceiveBuffer[1] - '1');
			from[0].column = (int8_t)('h' - ReceiveBuffer[0]);
			from[1].row = (int8_t)(ReceiveBuffer[3] - '1');
			from[1].column = (int8_t)('h' - ReceiveBuffer[2]);
			IlluminateCoordinates(from, 2);
			/*
			prepAudio(&hspi1, &hspi2, &hdac);
			PlayAudio(audio[ReceiveBuffer[0] - 'a'], &hdac);
			PlayAudio(audio[ReceiveBuffer[1] - '1' + 8], &hdac);
			PlayAudio(audio[ReceiveBuffer[2] - 'a'], &hdac);
			PlayAudio(audio[ReceiveBuffer[3] - '1' + 8], &hdac);
			resetAudio(&hspi1, &hspi2, &hdac);
			*/
		}
	}

	// Invoke PathFinder to store all legal moves for this team
	CalculateTeamsLegalMoves(CurrentTurn);
}

static void CheckForPromotion()
{
	// Get all pawns
	struct PieceCoordinate pawns[NUM_PAWNS_PER_TEAM];
	uint8_t numPawns;
	GetPiecesForTeam(CurrentTurn, PAWN, pawns, &numPawns);

	// If the pawn has reached the end, it must be promoted
	for (uint8_t i = 0; i < numPawns; i++)
	{
		if (PawnReachedEnd(pawns[i]))
		{
			HandlePlacePreemptPromotion(pawns[i]);
		}
	}
}

static void UpdateCastleFlags()
{
	// If any rooks moved, flag them as not castle-able
	if (CanA1Castle && !IsPiecePresent(A1_COORDINATE))
	{
		CanA1Castle = 0;
	}
	else if (CanH1Castle && !IsPiecePresent(H1_COORDINATE))
	{
		CanH1Castle = 0;
	}
	else if (CanA8Castle && !IsPiecePresent(A8_COORDINATE))
	{
		CanA8Castle = 0;
	}
	else if (CanH8Castle && !IsPiecePresent(H8_COORDINATE))
	{
		CanH8Castle = 0;
	}
	// If any kings moved, flag them as not castle-able
	else if (CanWhiteKingCastle && !IsPiecePresent(E1_COORDINATE))
	{
		CanWhiteKingCastle = 0;
	}
	else if (CanBlackKingCastle && !IsPiecePresent(E8_COORDINATE))
	{
		CanBlackKingCastle = 0;
	}
}

static void GetPiecesForTeam(enum PieceOwner team, enum PieceType type, struct PieceCoordinate* pieces, uint8_t* numPieces)
{
	*numPieces = 0;
	for (uint8_t row = 0; row < NUM_ROWS; row++)
	{
		for (uint8_t col = 0; col < NUM_COLS; col++)
		{
			struct PieceCoordinate pieceCoordinate = { Chessboard[row][col], row, col };
			if (pieceCoordinate.piece.owner == team && pieceCoordinate.piece.type == type)
			{
				pieces[(*numPieces)++] = pieceCoordinate;
			}
		}
	}
}

uint8_t PawnReachedEnd(struct PieceCoordinate pieceCoordinate)
{
	uint8_t finalRow = CurrentTurn == WHITE ? 7 : 0;
	return (pieceCoordinate.piece.owner == CurrentTurn) && (pieceCoordinate.piece.type == PAWN) && (pieceCoordinate.row == finalRow);
}

inline uint8_t PieceExists(struct PieceCoordinate pieceCoordinate)
{
	return !IsPieceCoordinateEqual(pieceCoordinate, EMPTY_PIECE_COORDINATE);
}

inline void ClearPiece(struct PieceCoordinate* pieceCoordinate)
{
	*pieceCoordinate = EMPTY_PIECE_COORDINATE;
}

inline void SetPiece(uint8_t row, uint8_t column, struct Piece piece)
{
	Chessboard[row][column] = piece;
}

inline void SetPieceCoordinate(struct PieceCoordinate pieceCoordinate)
{
	Chessboard[pieceCoordinate.row][pieceCoordinate.column] = pieceCoordinate.piece;
}

inline struct Piece GetPiece(uint8_t row, uint8_t column)
{
	return Chessboard[row][column];
}

inline struct PieceCoordinate GetPieceCoordinate(uint8_t row, uint8_t column)
{
	struct PieceCoordinate pieceCoordinate = { GetPiece(row, column), row, column };
	return pieceCoordinate;
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

uint8_t IsPiecePresent(uint8_t row, uint8_t column)
{
	return Chessboard[row][column].type != NONE;
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

inline enum PieceOwner GetCurrentTurn()
{
	return CurrentTurn;
}
