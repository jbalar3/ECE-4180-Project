# Tetris
## Overview
We created a Tetris Game using the uLCD screen as a display and the navigation switch as the input control to move and rotate the Tetris blocks. 
Players have the option to move each block left and right, rotate the piece or place it directly down onto the Tetris stack using the navigation switch. The rules are the same as standard Tetris where, when each line of blocks is filled, they disappear and the game ends when the blocks reach the top of the screen. 



## Process
A tutorial was followed, which can be found in the resources section, which outlines the logic necessary to create the Tetris game. Changes were made to make the code compatible to projecting to the uLCD screen and the inputs were also modified to take in values from the [Navigation Switch](https://os.mbed.com/users/4180_1/notebook/using-a-navigation-switch-digital-joystick/). 
The order in which the blocks appear in this program follows a loop in which the blocks switch in order between seven different options and the rotation in which they appear switch between four options. Additional features include the score and a LED flash when a block is placed onto the stack. 



## Project Demo
[![Video Demo](https://github.gatech.edu/jstephens78/ece-4180-final-project/blob/main/docs/full-demo.png?raw=true)](https://github.gatech.edu/raw/jstephens78/ece-4180-final-project/main/docs/game-tetris-demo.mp4?token=GHSAT0AAAAAAAACOJFRELW67AGOPX3XN4TGY44W6PQ)



## Resources
A Tetris coding tutorial that was followed can be found here: https://javilop.com/gamedev/tetris-tutorial-in-c-platform-independent-focused-in-game-logic-for-beginners/
