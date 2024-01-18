#include "Game.h"

/* 
======================================                                  
Init
====================================== 
*/
Game::Game(Board *pBoard, Pieces *pPieces, int pScreenHeight, uLCD_4DGL *pLCD,int initPiece,int initPos,int initNextPiece,int initNextPos) 
{
    mScreenHeight = pScreenHeight;
    
    // Get the pointer to the Board and Pieces classes
    mBoard = pBoard;
    mPieces = pPieces;
    uLCD = pLCD;
    // Game initialization
    InitGame (initPiece,initPos,initNextPiece,initNextPos);
}

/* 
======================================                                  
Initial parameters of the game
====================================== 
*/
void Game::InitGame(int initPiece,int initPos,int initNextPiece,int initNextPos)
{
    //points
    points = 0;
    clearedLineCount=0;

    // First piece
    mPiece          = initPiece;
    mRotation       = initPos;
    mPosX           = (BOARD_WIDTH / 2) + mPieces->GetXInitialPosition (mPiece, mRotation);
    mPosY           = mPieces->GetYInitialPosition (mPiece, mRotation);

    //  Next piece
    mNextPiece      = initNextPiece;
    mNextRotation   = initNextPos;
    mNextPosX       = BOARD_WIDTH + 2;
    mNextPosY       = 2;    
}

/* 
======================================                                  
Create a random piece
====================================== 
*/
void Game::CreateNewPiece(int piece,int pos)
{
    // The new piece
    mPiece          = mNextPiece;
    mRotation       = mNextRotation;
    mPosX           = (BOARD_WIDTH / 2) + mPieces->GetXInitialPosition (mPiece, mRotation);
    mPosY           = mPieces->GetYInitialPosition (mPiece, mRotation);

    // Random next piece
    mNextPiece      = piece;
    mNextRotation   = pos;
}

/* 
======================================                                  
Draw piece

Parameters:

>> pX:      Horizontal position in blocks
>> pY:      Vertical position in blocks
>> pPiece:  Piece to draw
>> pRotation:   1 of the 4 possible rotations
====================================== 
*/
void Game::DrawPiece (int pX, int pY, int pPiece, int pRotation, int colorIndex)
{
    // Obtain the position in pixel in the screen of the block we want to draw
    int mPixelsX = mBoard->GetXPosInPixels (pX);
    int mPixelsY = mBoard->GetYPosInPixels (pY);
    int mColors [7]={16711680, 15955765, 16761893, 45401, 44763, 12462572, 29669};
    // Travel the matrix of blocks of the piece and draw the blocks that are filled
    for (int i = 0; i < PIECE_BLOCKS; i++)
    {
        for (int j = 0; j < PIECE_BLOCKS; j++)
        {
            if (mPieces->GetBlockType (pPiece, pRotation, j, i) != 0)
                uLCD->filled_rectangle  (mPixelsX + i * BLOCK_SIZE, 
                                    mPixelsY + j * BLOCK_SIZE, 
                                    (mPixelsX + i * BLOCK_SIZE) + BLOCK_SIZE - 1, 
                                    (mPixelsY + j * BLOCK_SIZE) + BLOCK_SIZE - 1, 
                                    mColors[colorIndex]);
        }
    }
}

/* 
======================================                                  
Draw board

Draw the two lines that delimit the board
====================================== 
*/
void Game::DrawBoard ()
{
    // Calculate the limits of the board in pixels  
    int mX1 = BOARD_POSITION - (BLOCK_SIZE * (BOARD_WIDTH / 2)) - 1;
    int mX2 = BOARD_POSITION + (BLOCK_SIZE * (BOARD_WIDTH / 2));
    int mY = mScreenHeight - (BLOCK_SIZE * BOARD_HEIGHT);
    
    // Check that the vertical margin is not to small
    //assert (mY > MIN_VERTICAL_MARGIN);

    // Rectangles that delimits the board
    uLCD->filled_rectangle (mX1 - BOARD_LINE_WIDTH, mY, mX1, mScreenHeight - 1, BLUE);
    uLCD->filled_rectangle (mX2, mY, mX2 + BOARD_LINE_WIDTH, mScreenHeight - 1, BLUE);
    
    // Check that the horizontal margin is not to small
    //assert (mX1 > MIN_HORIZONTAL_MARGIN);

    // Drawing the blocks that are already stored in the board
    mX1 += 1;
    for (int i = 0; i < BOARD_WIDTH; i++)
    {
        for (int j = 0; j < BOARD_HEIGHT; j++)
        {   
            // Check if the block is filled, if so, draw it
            if (!mBoard->IsFreeBlock(i, j)) 
                uLCD->filled_rectangle (    mX1 + i * BLOCK_SIZE, 
                                        mY + j * BLOCK_SIZE, 
                                        (mX1 + i * BLOCK_SIZE) + BLOCK_SIZE - 1, 
                                        (mY + j * BLOCK_SIZE) + BLOCK_SIZE - 1, 
                                        10066329);
        }
    }   
}

/* 
======================================                                  
Draw scene

Draw all the objects of the scene
====================================== 
*/
void Game::DrawScene ()
{
    DrawBoard();
    DrawPiece (mPosX, mPosY, mPiece, mRotation, mPiece);                    // Draw the playing piece
    DrawPiece (mNextPosX, mNextPosY, mNextPiece, mNextRotation, mNextPiece);    // Draw the next piece
}

void Game::ErasePiece (int pX, int pY, int pPiece, int pRotation)
{
    if(pPiece==-1) return;
    // Obtain the position in pixel in the screen of the block we want to draw
    int mPixelsX = mBoard->GetXPosInPixels (pX);
    int mPixelsY = mBoard->GetYPosInPixels (pY);

    // Travel the matrix of blocks of the piece and draw the blocks that are filled
    for (int i = 0; i < PIECE_BLOCKS; i++)
    {
        for (int j = 0; j < PIECE_BLOCKS; j++)
        {
            if (mPieces->GetBlockType (pPiece, pRotation, j, i) != 0)
                uLCD->filled_rectangle  (mPixelsX + i * BLOCK_SIZE, 
                                    mPixelsY + j * BLOCK_SIZE, 
                                    (mPixelsX + i * BLOCK_SIZE) + BLOCK_SIZE - 1, 
                                    (mPixelsY + j * BLOCK_SIZE) + BLOCK_SIZE - 1, 
                                    0);
        }
    }
}

void Game::AddPoints(int newGain)
{
    points+=newGain;
}

void Game::AddClearedLines(int newGain)
{
    clearedLineCount+=newGain;
}

int Game::GetPoints()
{
    return points;
}

int Game::GetClearedLines()
{
    return clearedLineCount;
}