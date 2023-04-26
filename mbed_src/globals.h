#include "mbed.h"
#include "rtos.h"

#include "wave_player.h"
#include "Speaker.h"
#include "SDFileSystem.h"
#include "uLCD_4DGL.h"
#include "bluefruit_controller.h"
#include "Nav_Switch.h"

extern Nav_Switch myNav;

// Global state flags
extern volatile bool game1;
extern volatile bool game2;
extern volatile int menu_flag;

// Debug systems
extern Serial pc;
extern Mutex pc_mutex;
#define PRINTF(...) \
    pc_mutex.lock(); \
    pc.printf(__VA_ARGS__); \
    pc_mutex.unlock();

// Display
extern uLCD_4DGL uLCD;
extern Mutex uLCD_mutex;

// Input controllers
extern BluefruitController blue;
//extern BusIn navSwitch;

// Sournd + Lighting systems
extern SDFileSystem sd;
extern DigitalOut myled;
extern PwmOut Rgb;
extern PwmOut rGb;
extern PwmOut rgB;

extern Speaker DACout;
//extern wave_player waver;
