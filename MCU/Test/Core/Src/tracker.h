#ifndef TRACKER_H
#define TRACKER_H

#include "stm32l1xx_hal.h"

struct GPIO_Pin {
	uint16_t pin;
	GPIO_TypeDef* bus;
};

enum PieceType {
	NONE,
	PAWN,
	KNIGHT,
	BISHOP,
	CASTLE,
	QUEEN,
	KING
};

enum PieceOwner {
	NEUTRAL,
	WHITE,
	BLACK
};

struct Piece {
	enum PieceType type;
	enum PieceOwner owner;
};

#define NUM_COLS 8
#define NUM_ROWS 8
#define NUM_COL_BITS 3

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
 * @brief Returns the piece at the specified row and column.
 */
struct Piece GetPiece(uint8_t row, uint8_t column);


/**
 * @brief Sets the piece at the specified row and column
 */
void SetPiece(uint8_t row, uint8_t column, struct Piece piece);


/**
 * @brief Kills the piece, marking it as off of the board
 */
void KillPiece(struct Piece piece);

#endif // TRACKER_H
