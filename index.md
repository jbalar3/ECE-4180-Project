# ECE 4180 Final Project - MBED Game Console
## Team
- Diego Angulo (dangulo7@gatech.edu)
- Jay Balar (jbalar3@gatech.edu)
- Jordan Sanchez (jsanchez307@gatech.edu)
- Justin Stephens (jstephens78@gatech.edu)

Project completed in Fall 2022 semester.

## Project Summary
This project implements a miniature game console on the MBED. Users can select to play Tetris or a multiplayer Air Hockey game through a menu system.

[![Video Demo](https://github.gatech.edu/jstephens78/ece-4180-final-project/blob/main/docs/full-demo.png?raw=true)](https://youtu.be/7TsWzEyEEXQ)

Quick links:
- [GitHub Repo](https://github.gatech.edu/jstephens78/ece-4180-final-project)
- [MBED Repository](https://os.mbed.com/users/jstephens78/code/4180_Project/)
- [Project Presentation](https://gtvault.sharepoint.com/:p:/s/ECE4180Team/EXTeZv0QTBlPpx739TwLbfwBgCJTbnV_jyxLZLHjIR2ksg?e=Mnd4pm)

## Hardware
### Components
- [uLCD-144-G2](https://os.mbed.com/users/4180_1/notebook/ulcd-144-g2-128-by-128-color-lcd/)
- [NavSwitch](https://os.mbed.com/users/4180_1/notebook/using-a-navigation-switch-digital-joystick/)
- [Bluefruit](https://os.mbed.com/users/4180_1/notebook/adafruit-bluefruit-le-uart-friend---bluetooth-low-/)
- [Sparkfun Audio Amp + Speaker](https://os.mbed.com/users/4180_1/notebook/tpa2005d1-class-d-audio-amp/)
- External Power Supply

### Hookup Guide
The devices and the LPC 1768 should be connected as follows:

| LPC 1768 Pin | Device        | Device Pin |
|--------------|---------------|------------|
| p13 (TX)     | uLCD          | RX         |
| p14 (RX)     | uLCD          | TX         |
| p30          | uLCD          | Reset      |
| p18          | Speaker       | Amp +      |
| p15          | NavSwitch     | RIGHT      |
| p16          | NavSwitch     | DOWN       |
| p17          | NavSwitch     | LEFT       |
| p19          | NavSwitch     | CENTER     |
| p20          | NavSwitch     | UP         |
| p27 (RX)     | Bluefruit     | TX         |
| p28 (TX)     | Bluefruit     | RX         |

When hooking up to power, use the following:

| Device           | Power Bus     |
|------------------|---------------|
| External 5v Jack | LPC 1768 VIN  |
| Bluefruit (VIN)  | LPC 1768 VU   |



## Software
The full source for this project can be found in the [MBED Repository](https://os.mbed.com/users/jstephens78/code/4180_Project/).

For documentation on the libraries and game subroutines, **please see the following pages**:
 - [Air Hockey Game](game-air-hockey.md)
 - [Tetris Game](game-tetris.md)
 - [Physac-MBED Library](mbed-physac.md)

Each game runs in its own thread, with volatile variables used to coordinate which game is active at a given time. Shared devices such as the uLCD and UART to the pc are placed behind Mutex locks.

#### globals.h
`globals.h` holds definitions for all variables used across threads.
```c++ globals.h
extern volatile bool game1;
extern volatile bool game2;
extern volatile int menu_flag;

extern Serial pc;
extern Mutex pc_mutex;
#define PRINTF(...) pc_mutex.lock(); pc.printf(__VA_ARGS__); pc_mutex.unlock();

extern uLCD_4DGL uLCD;
extern Mutex uLCD_mutex;

extern BluefruitController blue;
extern Nav_Switch myNav;
extern BusIn navSwitch;
extern Speaker DACout;
```

Each game will sit idle until its respective `volatile bool game*` flag is set high, at which point the game will begin. If the flag is ever set low, the game will exit. The audio thread will also watch the `game*` flags, and will change the active song accordingly.

#### main.cpp
The program `main` method launches all of the worker threads, then itself runs the system menu. Game threads are launched with higher priority to yield more stable gameplay at the expense of the accuracy of secondary functions.
```c++
int main()
{
    // Launch four threads
    Thread thread1(tetrisGame, osPriorityHigh);
    Thread thread2(hockeyGame, osPriorityHigh);
    Thread thread3(audioThread, osPriorityLow);
    Thread thread4(ledThread, osPriorityLow);
    
    while (true) {
        // menu loop
        ...
    }
}
```

## De-scoped work
- Work was done toward applying the [Fusion AHRS algorithm](https://github.com/xioTechnologies/Fusion) to the sparkfun IMU, but this was never completed. Partial work can be found [at revision 8:005b0a85be70](https://os.mbed.com/users/jstephens78/code/4180_Project//file/005b0a85be70/) for any future parties who might want to attempt it.
