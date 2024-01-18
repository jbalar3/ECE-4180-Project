#ifndef _GAME_
#define _GAME_

// ------ Includes -----

#include "Board.h"
#include "Pieces.h"
#include "uLCD_4DGL.h"

// ------ Defines -----

#define WAIT_TIME 700           // Number of milliseconds that the piece remains before going 1 block down */ 


// -----------------------------------------------------
// Game
// -----------------------------------------------------

class Game
{
public:

    Game (Board *pBoard, Pieces *pPieces, int pScreenHeight, uLCD_4DGL *uLCD,int,int,int,int);

    void DrawScene ();
    void CreateNewPiece (int,int);
    
    void ErasePiece(int x,int y,int piece, int rotation);
    void DrawBoard ();
    void AddPoints(int);
    void AddClearedLines(int);
    int GetPoints();
    int GetClearedLines();
    
    int mPosX, mPosY;               // Position of the piece that is falling down
    int mPiece, mRotation;          // Kind and rotation the piece that is falling down
    
    int points;
    int clearedLineCount;
    
    void InitGame(int,int,int,int);
    

private:

    int mScreenHeight;              // Screen height in pixels
    int mNextPosX, mNextPosY;       // Position of the next piece
    int mNextPiece, mNextRotation;  // Kind and rotation of the next piece
    
    Board *mBoard;
    Pieces *mPieces;
    uLCD_4DGL *uLCD;

    
    //void InitGame(int,int,int,int);
    void DrawPiece (int pX, int pY, int pPiece, int pRotation, int colorIndex);
    
};

#endif // _GAME_
