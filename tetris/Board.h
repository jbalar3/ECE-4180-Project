#ifndef _BOARD_
#define _BOARD_

// ------ Includes -----

#include "Pieces.h"

// ------ Defines -----

#define BOARD_LINE_WIDTH 6          // Width of each of the two lines that delimit the board
#define BLOCK_SIZE 6              // Width and Height of each block of a piece
#define BOARD_POSITION 50          // Center position of the board from the left of the screen
#define BOARD_WIDTH 10              // Board width in blocks 
#define BOARD_HEIGHT 20             // Board height in blocks
#define MIN_VERTICAL_MARGIN 8      // Minimum vertical margin for the board limit      
#define MIN_HORIZONTAL_MARGIN 20    // Minimum horizontal margin for the board limit
#define PIECE_BLOCKS 5              // Number of horizontal and vertical blocks of a matrix piece


// --------------------------------------------------------------------------------
//                                   Board
// --------------------------------------------------------------------------------

class Board
{
public:

    Board                       (Pieces *pPieces, int pScreenHeight);

    int GetXPosInPixels         (int pPos);
    int GetYPosInPixels         (int pPos);
    bool IsFreeBlock            (int pX, int pY);
    bool IsPossibleMovement     (int pX, int pY, int pPiece, int pRotation);
    void StorePiece             (int pX, int pY, int pPiece, int pRotation);
    int DeletePossibleLines    ();
    bool IsGameOver             ();
    void InitBoard();

private:

        // enum holds no value besides the described properties
    enum { POS_FREE, POS_FILLED };          // POS_FREE = free position of the board; POS_FILLED = filled position of the board
    int mBoard [BOARD_WIDTH][BOARD_HEIGHT]; // Board that contains the pieces
    Pieces *mPieces;
    int mScreenHeight;

    //void InitBoard();
    void DeleteLine (int pY);
};

#endif
