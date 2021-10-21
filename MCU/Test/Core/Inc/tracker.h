#ifndef TRACKER_H
#define TRACKER_H

#include "stm32l1xx_hal.h"

#define TEST 1

#ifdef TEST
#define NUM_COLS 4
#define NUM_ROWS 4
#else
#define NUM_COLS 8
#define NUM_ROWS 8
#endif

#define NUM_COL_BITS 3

/* Types */

enum PieceType {
	NONE,
	PAWN,
	KNIGHT,
	BISHOP,
	ROOK,
	QUEEN,
	KING
};

enum PieceOwner {
	NEUTRAL,
	WHITE,
	BLACK
};

enum TransitionType {
	PICKUP,
	PLACE
};

struct GPIO_Pin {
	uint16_t pin;
	GPIO_TypeDef* bus;
};


struct Piece {
	enum PieceType type;
	enum PieceOwner owner;
};

struct PieceCoordinate {
	struct Piece piece;
	uint8_t row;
	uint8_t column;
};



/* Constants */

volatile static const struct GPIO_Pin ROW_NUMBER_TO_PIN_TABLE[NUM_ROWS] = {
	{ GPIO_PIN_10, GPIOA }, // D2
	{ GPIO_PIN_3,  GPIOB }, // D3
	{ GPIO_PIN_5,  GPIOB }, // D4
	{ GPIO_PIN_4,  GPIOB }, // D5
#ifndef TEST
	{ GPIO_PIN_10, GPIOB }, // D6
	{ GPIO_PIN_8,  GPIOA }, // D7
	{ GPIO_PIN_9,  GPIOA }, // D8
	{ GPIO_PIN_7,  GPIOC }, // D9
#endif

};

volatile static const struct GPIO_Pin COLUMN_BIT_TO_PIN_TABLE[NUM_COL_BITS] = {
	{ GPIO_PIN_0,  GPIOA }, // A0
	{ GPIO_PIN_1,  GPIOA }, // A1
	{ GPIO_PIN_0,  GPIOB }, // A2

};

#ifdef TEST
volatile static const struct Piece INITIAL_CHESSBOARD[NUM_ROWS][NUM_COLS] = {
	{{PAWN, WHITE},   {NONE, NEUTRAL}, {NONE, NEUTRAL}, {PAWN, WHITE}},
	{{NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}},
	{{NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}},
	{{PAWN, BLACK},   {NONE, NEUTRAL}, {NONE, NEUTRAL}, {PAWN, BLACK}},
};
#else
volatile static const struct Piece INITIAL_CHESSBOARD[NUM_ROWS][NUM_COLS] = {
	{{ROOK, WHITE},   {KNIGHT, WHITE}, {BISHOP, WHITE}, {QUEEN, WHITE},  {KING, WHITE},   {BISHOP, WHITE}, {KNIGHT, WHITE}, {ROOK, WHITE}},
	{{PAWN, WHITE},   {PAWN, WHITE},   {PAWN, WHITE},   {PAWN, WHITE},   {PAWN, WHITE},   {PAWN, WHITE},   {PAWN, WHITE},   {PAWN, WHITE}},
	{{NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}},
	{{NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}},
	{{NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}},
	{{NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}, {NONE, NEUTRAL}},
	{{PAWN, BLACK},   {PAWN, BLACK},   {PAWN, BLACK},   {PAWN, BLACK},   {PAWN, BLACK},   {PAWN, BLACK},   {PAWN, BLACK},   {PAWN, BLACK}},
	{{ROOK, BLACK},   {KNIGHT, BLACK}, {BISHOP, BLACK}, {QUEEN, BLACK},  {KING, BLACK},   {BISHOP, BLACK}, {KNIGHT, BLACK}, {ROOK, BLACK}},
};
#endif

volatile static const struct Piece EMPTY_PIECE = {NONE, NEUTRAL};
volatile static const struct PieceCoordinate EMPTY_PIECE_COORDINATE = {{NONE, NEUTRAL}, 0, 0};



/* Functions */

/**
 * @brief Write column bits to MUXs and read the value of each row. Keep track of piece moves.
 */
void Track(void);


/**
 * @brief Initialize IO ports to use for tracking via the Hall Effect sensors.
 */
void InitTracker(void);


/**
 * @brief Make sure that all the pieces are in the 0th, 1st, 7th and 6th rows, and no piece is anywhere else.
 */
uint8_t ValidateStartPositions(void);


/**
 * @brief Trigger peripherals to switch teams
 */
void SwitchTeam();


// Castling //
uint8_t KingCanCastle(struct PieceCoordinate kingCoordinate);
uint8_t RookCanCastle(struct PieceCoordinate rookCoordinate);
void CalculateCastlingPositions(struct PieceCoordinate rookPieceCoordinate, struct PieceCoordinate* expectedKingPieceCoordinate, struct PieceCoordinate* expectedRookPieceCoordinate);


/**
 * @brief Returns the piece at the specified row and column.
 */
struct Piece GetPiece(uint8_t row, uint8_t column);


/**
 * @brief Sets the piece at the specified row and column
 */
void SetPiece(uint8_t row, uint8_t column, struct Piece piece);


/**
 * @brief Triggers a piece to be killed in software
 */
void KillPiece(struct Piece piece);


/**
 * @brief Returns 1 if team opposing the given piece picked up a piece last
 * - Used to detect killing because the opposite team must pick up a piece right before picking up this piece to kill it
 */
uint8_t DidOtherTeamPickupLast(struct Piece piece);


/**
 * @brief Returns 1 if same team as the given piece picked up a piece last
 * - Used to detect castling because a team must pick up ROOK and KING in a row
 */
uint8_t DidSameTeamPickupLast(struct Piece piece);

// Comparison //
uint8_t IsPieceEqual(struct Piece piece1, struct Piece piece2);
uint8_t IsPieceCoordinateEqual(struct PieceCoordinate pieceCoordinate1, struct PieceCoordinate pieceCoordinate2);
uint8_t IsPieceCoordinateSamePosition(struct PieceCoordinate pieceCoordinate1, struct PieceCoordinate pieceCoordinate2);

#endif // TRACKER_H
