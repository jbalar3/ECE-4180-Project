#include "mbed.h"
#include "Game.h"
#include "globals.h"


int SinglePiecePoints = 50;
int LineClearPoints = 200;
int key_input = 0;
bool gameStarted = false;
void input_left()
{
    if(!gameStarted)
    {   gameStarted=true;
        return;
    }
    key_input = 1;
}

void input_right()
{
    if(!gameStarted)
    {   gameStarted=true;
        return;
    }
    key_input = 2;
}

void input_rotate()
{
    if(!gameStarted)
    {   gameStarted=true;
        return;
    }
    key_input = 3;
}

void input_down()
{
    if(!gameStarted)
    {   gameStarted=true;
        return;
    }
    key_input=4;
}

void input_menu()
{
    if(!gameStarted)
    {   gameStarted=true;
        return;
    }
    key_input = 5;
}

void clear_board()
{
    uLCD.filled_rectangle(20,0,79,128,0);
}

void clear_next_piece()
{
    uLCD.filled_rectangle(92,20,122,50,0);
}

void UpdateGameStatus(int points)   // used to include int lines
{
    uLCD.locate(13,12);
    uLCD.printf("Score");
    uLCD.locate(13,13);
    uLCD.printf("%d",points);
}



void tetrisGame()
{
    PRINTF("POINT 1");
    while (game1 == false) {
           
        PRINTF("[TETRIS] Idle\r\n");
        Thread::wait(500);
    }
    
    
    uLCD.text_width(2);
    uLCD.text_height(2);
    uLCD.color(WHITE);
    uLCD.locate(1,0);
    uLCD.printf("TETRIS");
    Thread::wait(2000);
    
    uLCD.baudrate(3000000);
    
    
    bool isGameOver = false;
    int mScreenHeight = 128;
    Pieces mPieces;
    Board mBoard (&mPieces, mScreenHeight);
    int a = 1;   // RandomGen('a')
    int b = 2;
    int c = 3;
    int d = 4;
    Game mGame (&mBoard, &mPieces, mScreenHeight, &uLCD,a,b,c,d);


    // ----- Main Loop -----
    int prevX=0;
    int prevY=0;
    int prevPiece=-1;
    int prevRot=0;
    Timer timer;
    timer.start();
    key_input=0;
    bool needErase = false;
    uLCD.cls();
    
    
    int piece = 0;
    int rotate = 0;
    
    //float x = 0.0;
    
    Rgb = 1;
    rGb = 0;
    rgB = 0;
    int count = 0;
    
    UpdateGameStatus(mGame.GetPoints());
    
    while (1)
    {
        PRINTF("POINT 2");
        // Wait until the game starts
        while (game1 == false) {
           
            PRINTF("[TETRIS] Idle\r\n");
            Thread::wait(500);
        }
        
        if (myNav.up()) input_rotate();
        if (myNav.down()) input_down();
        if (myNav.left()) input_left();
        if (myNav.right()) input_right();
        //if (myNav.fire()) input_menu();
        
        if(isGameOver) 
        {
            Thread::wait(1000);
            uLCD.cls();
            uLCD.text_width(2);
            uLCD.text_height(2);
            uLCD.printf("GAME OVER");
            Thread::wait(2000);
            uLCD.cls();
            
            //isGameOver = false;
            
            Thread::wait(1000);
            
            mBoard.InitBoard();
            uLCD.cls();
            
            mGame.points = 0;
            count = 0;
            
            //mGame.InitGame();
            isGameOver = false;
            
            menu_flag = 0;
            
            game1 = false;
            
            
            //mBoard.InitBoard();
            
            //free(mPieces);
            //free(mBoard);
            //free(mGame);
            
        }
        // ----- Draw ----
        if(needErase)
        {
            mGame.ErasePiece(prevX,prevY,prevPiece,prevRot);
            needErase=false;
        }
        mGame.DrawScene();
        
        prevX=mGame.mPosX;
        prevY=mGame.mPosY;
        prevPiece=mGame.mPiece;
        prevRot=mGame.mRotation;
    
        // ----- Input -----
        switch (key_input)
        {
            case (2): //right
            {
                if (mBoard.IsPossibleMovement (mGame.mPosX + 1, mGame.mPosY, mGame.mPiece, mGame.mRotation))
                    {mGame.mPosX++;needErase=true;}
                break;
            }

            case (1): //left
            {
                if (mBoard.IsPossibleMovement (mGame.mPosX - 1, mGame.mPosY, mGame.mPiece, mGame.mRotation))
                    {mGame.mPosX--;needErase=true;}  
                break;
            }

            case (4)://down
            {
                // Check collision from up to down
                while (mBoard.IsPossibleMovement(mGame.mPosX, mGame.mPosY, mGame.mPiece, mGame.mRotation)) { mGame.mPosY++; }
                needErase=true;
                mBoard.StorePiece (mGame.mPosX, mGame.mPosY - 1, mGame.mPiece, mGame.mRotation);
                mGame.AddPoints(SinglePiecePoints);
                int linesDeleted = mBoard.DeletePossibleLines ();
                if(linesDeleted>0)
                {
                    mGame.AddClearedLines(linesDeleted);
                    mGame.AddPoints(LineClearPoints*linesDeleted);
                    clear_board();
                }
                UpdateGameStatus(mGame.GetPoints()); // FIX

                if (mBoard.IsGameOver())
                {
                    isGameOver=true;
                    uLCD.cls();
                    
                }
                
                if(!isGameOver)
                {
                    mGame.CreateNewPiece(piece,rotate);
                    clear_next_piece();
                
                    piece++;
                    rotate++;
                    
                    if (piece==7) piece = 0;
                    if (rotate==4) rotate = 0;
                }
                break;
            }

            case (3)://rotate
            {
                if (mBoard.IsPossibleMovement (mGame.mPosX, mGame.mPosY, mGame.mPiece, (mGame.mRotation + 1) % 4))
                    {mGame.mRotation = (mGame.mRotation + 1) % 4;needErase=true;}
                break;
            }
            
            case (5)://menu
            {
                Thread::wait(1000);
                uLCD.cls();
                uLCD.text_width(2);
                uLCD.text_height(2);
                uLCD.printf("MENU");
                Thread::wait(2000);
                uLCD.cls();
                
                Thread::wait(1000); // FIX?
                
                menu_flag = 0;
                
                game1 = false;  // FIX? put after Thread::wait()
            
                
            }
            
            case (0):{break;}
        }
        key_input = 0;
        
        // ----- Vertical movement -----
    
        if(timer.read_ms()>WAIT_TIME)
        {
            needErase=true;
            if(!isGameOver)
            {
                if (mBoard.IsPossibleMovement (mGame.mPosX, mGame.mPosY + 1, mGame.mPiece, mGame.mRotation))
                {
                    mGame.mPosY++;
                }
                else
                {
                    mBoard.StorePiece (mGame.mPosX, mGame.mPosY, mGame.mPiece, mGame.mRotation);
                    mGame.AddPoints(SinglePiecePoints);
                    int linesDeleted = mBoard.DeletePossibleLines ();
                    if(linesDeleted>0)
                    {
                        mGame.AddClearedLines(linesDeleted);
                        mGame.AddPoints(LineClearPoints*linesDeleted);
                        clear_board();
                    }
                    UpdateGameStatus(mGame.GetPoints());
                        
                    if (mBoard.IsGameOver())
                    {
                        isGameOver=true;
                        uLCD.cls();
                    }
                    
                    if(!isGameOver)
                    {
                    mGame.CreateNewPiece(piece,rotate);
                    clear_next_piece(); // 0-6 Pieces, 0-3 rotations^^
                    piece++;
                    rotate++;
                    
                    if (piece==7) piece = 0;
                    if (rotate==4) rotate = 0;
                    }
                }
            }
            timer.reset();
        }
            Thread::wait(100);
            
            //get a new random number for PWM
        //x = rand() / float(RAND_MAX);
        //add some exponential brightness scaling
        //for more of a fast flash effect
        /*x = x*x*x;
        Rgb = x;
        rGb = x;
        rgB = x;
        //fast update rate for welding flashes
        //Thread::wait(20);
        //add a random pause between welds
        if (rand() / float(RAND_MAX) > 0.95) {
            Rgb = 0;
            rGb = 0;
            rgB = 0;
            //Thread::wait(4000.0 * rand() / float(RAND_MAX));
        }*/
        if (count % 10 == 0) {
            if (Rgb == 1) {
                Rgb = 0;
                rGb = 1;
                rgB = 0;
            }
            
            else if (rGb == 1) {
                Rgb = 0;
                rGb = 0;
                rgB = 1;
            }
             else if (rgB == 1) {
                Rgb = 1;
                rGb = 0;
                rgB = 0;
            } else if (Rgb == 0) {
                Rgb = 0;
                rGb = 1;
                rgB = 0;
            }
        }
        
        count++;
    }
}
