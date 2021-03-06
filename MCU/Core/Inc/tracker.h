#ifndef TRACKER_H
#define TRACKER_H

#include "main.h"
#include "types.h"
#include "button.h"

/* Constants */

//#define TEST 1

#define NUM_HISTORY_ENTRIES 8
#define MAX_ILLEGAL_PIECES 16 // To reduce memory - the real max should be 32
#define NUM_COL_BITS 3
#define A1_COORDINATE 0, 0
#define A8_COORDINATE 7, 0
#define H1_COORDINATE 0, 7
#define H8_COORDINATE 7, 7
#define E1_COORDINATE 0, 3
#define E8_COORDINATE 7, 3

static const struct GPIO_Pin ROW_NUMBER_TO_PIN_TABLE[NUM_ROWS] = {
	{ HALLOUT0_Pin, HALLOUT0_GPIO_Port },
	{ HALLOUT1_Pin, HALLOUT1_GPIO_Port },
	{ HALLOUT2_Pin, HALLOUT2_GPIO_Port },
	{ HALLOUT3_Pin, HALLOUT3_GPIO_Port },
	{ HALLOUT4_Pin, HALLOUT4_GPIO_Port },
	{ HALLOUT5_Pin, HALLOUT5_GPIO_Port },
	{ HALLOUT6_Pin, HALLOUT6_GPIO_Port },
	{ HALLOUT7_Pin, HALLOUT7_GPIO_Port },
};

static const struct GPIO_Pin COLUMN_BIT_TO_PIN_TABLE[NUM_COL_BITS] = {
	{ HALLSEL0_Pin,  HALLSEL0_GPIO_Port },
	{ HALLSEL1_Pin,  HALLSEL1_GPIO_Port },
	{ HALLSEL2_Pin,  HALLSEL2_GPIO_Port },
};

#define QUEEN_BUTTON ((enum ButtonType) BUTTON1)
#define KNIGHT_BUTTON ((enum ButtonType) BUTTON2)

#ifdef TEST
volatile static const struct Piece INITIAL_CHESSBOARD[NUM_ROWS][NUM_COLS] = {
	{{ROOK, WHITE},  {KNIGHT, WHITE}, {NONE, NEUTRAL}, {KING, WHITE}, {QUEEN, WHITE}, {BISHOP, WHITE}, {KNIGHT, WHITE}, {ROOK, WHITE}},
	{{PAWN, WHITE}, {PAWN, WHITE}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {PAWN, WHITE}},
	{{NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}},
	{{NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}},
	{{NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}},
	{{NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}},
	{{NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}},
	{{NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}},
};
#else
volatile static const struct Piece INITIAL_CHESSBOARD[NUM_ROWS][NUM_COLS] = {
	{{ROOK, WHITE},   {KNIGHT, WHITE}, {BISHOP, WHITE}, {KING, WHITE},   {QUEEN, WHITE},  {NONE, NEUTRAL}, {KNIGHT, WHITE}, {ROOK, WHITE}},
	{{PAWN, WHITE},   {PAWN, WHITE},   {PAWN, WHITE},   {PAWN, WHITE},   {PAWN, WHITE},   {PAWN, WHITE},   {PAWN, WHITE},   {PAWN, WHITE}},
	{{NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}},
	{{NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}},
	{{NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}},
	{{NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}},
	{{PAWN, BLACK},   {PAWN, BLACK},   {PAWN, BLACK},   {PAWN, BLACK},   {PAWN, BLACK},   {PAWN, BLACK},   {PAWN, BLACK},   {PAWN, BLACK}},
	{{ROOK, BLACK},   {KNIGHT, BLACK}, {BISHOP, BLACK}, {KING, BLACK},   {QUEEN, BLACK},  {BISHOP, BLACK}, {KNIGHT, BLACK}, {ROOK, BLACK}},
};
#endif // TEST

/* Functions */

/**
 * @brief Write column bits to MUXs and read the value of each row. Keep track of piece moves.
 */
uint8_t Track(void);


void InitTracker(enum GameMode gameMode);


/**
 * @brief Make sure top and bottom 2 rows sensors are HIGH at the beginning of the match
 */
uint8_t ValidateStartPositions(void);


/**
 * @brief Gets the current turn in the chess game.
 */
enum PieceOwner GetCurrentTurn();


/**
 * @brief Returns the piece at the specified row and column.
 */
struct Piece GetPiece(uint8_t row, uint8_t column);
struct PieceCoordinate GetPieceCoordinate(uint8_t row, uint8_t column);

// Comparison //
uint8_t IsPieceEqual(struct Piece piece1, struct Piece piece2);
uint8_t IsPiecePresent(uint8_t row, uint8_t column);
uint8_t IsPieceCoordinateEqual(struct PieceCoordinate pieceCoordinate1, struct PieceCoordinate pieceCoordinate2);
uint8_t IsPieceCoordinateSamePosition(struct PieceCoordinate pieceCoordinate1, struct PieceCoordinate pieceCoordinate2);


#endif // TRACKER_H
