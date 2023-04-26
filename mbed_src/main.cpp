#include "mbed.h"
#include "rtos.h"

#include "globals.h"
#include "hockey.h"
#include "tetris.h"

//#include "Speaker.h"
#include "SDFileSystem.h"
#include "uLCD_4DGL.h"



///////////////////////////
// GLOBALS.H DEFINITIONS //
///////////////////////////
// The following variables are declared as extern in "globals.h", and we define
// them here:
volatile bool game1 = false;
volatile bool game2 = false;
volatile int menu_flag = 0;

Serial pc(USBTX, USBRX);
Mutex pc_mutex;

//uLCD_4DGL uLCD(p28, p27, p29);
uLCD_4DGL uLCD(p13,p14,p30);
Mutex uLCD_mutex;

BluefruitController blue(p28,p27);
//BusIn navSwitch(p15, p16, p17, p19, p20);
Nav_Switch myNav(p20, p16, p17, p15, p19); // pins(up, down, left, right, fire)

SDFileSystem sd(p5, p6, p7, p8, "sd");
PwmOut Rgb(p23);
PwmOut rGb(p24);
PwmOut rgB(p25);
Speaker DACout(p18);
//wave_player waver(&DACout);


////////////////////////////////
// MAIN.CPP LOCAL DEFINITIONS //
////////////////////////////////
// The following variables are *not* from "globals.h" and are not meant for
// reference outside of main.cpp
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);



//////////////////////
// MAIN.CPP THREADS //
//////////////////////

void audioThread()
{
    while (true) {
        //FILE *wave_file;
        while (game1 == false && game2 == false) {
            DACout.PlayNote(369.99, 0.4, 1);
            DACout.PlayNote(0, 0.4, 1);
            DACout.PlayNote(466.16, 0.2, 1);
            DACout.PlayNote(554.37, 0.2, 1);
            if(game1 == true || game2 == true) break;
            DACout.PlayNote(0, 0.4, 1);
            DACout.PlayNote(466.16, 0.4, 1);
            DACout.PlayNote(0, 0.4, 1);
            DACout.PlayNote(369.99, 0.4, 1);
            if(game1 == true || game2 == true) break;
            DACout.PlayNote(293.66, 0.2, 1);
            DACout.PlayNote(293.66, 0.2, 1);
            DACout.PlayNote(293.66, 0.2, 1);
            DACout.PlayNote(0, 0.8, 1);
            if(game1 == true || game2 == true) break;
            DACout.PlayNote(277.18, 0.4, 1);
            DACout.PlayNote(293.66, 0.2, 1);
            DACout.PlayNote(369.99, 0.2, 1);
            DACout.PlayNote(466.16, 0.2, 1);
            if(game1 == true || game2 == true) break;
            DACout.PlayNote(554.37, 0.2, 1);
            DACout.PlayNote(0, 0.4, 1);
            DACout.PlayNote(466.16, 0.4, 1);
            DACout.PlayNote(0, 0.4, 1);
            if(game1 == true || game2 == true) break;
            DACout.PlayNote(369.99, 0.4, 1);
            DACout.PlayNote(659.25, 0.4, 1);
            DACout.PlayNote(622.25, 0.4, 1);
            DACout.PlayNote(587.33, 0.4, 1);
            if(game1 == true || game2 == true) break;
            DACout.PlayNote(0, 0.4, 1);
            
            
            
            DACout.PlayNote(392.00, 0.4, 1);
            DACout.PlayNote(0, 0.4, 1);
            DACout.PlayNote(554.37, 0.2, 1);
            if(game1 == true || game2 == true) break;
            DACout.PlayNote(369.99, 0.2, 1);
            DACout.PlayNote(0, 0.4, 1);
            DACout.PlayNote(554.37, 0.4, 1);
            DACout.PlayNote(0, 0.4, 1);
            if(game1 == true || game2 == true) break;
            DACout.PlayNote(329.00, 0.4, 1);
            DACout.PlayNote(0, 0.4, 1);
            DACout.PlayNote(523.25, 0.4, 1);
            DACout.PlayNote(0, 0.4, 1);
            if(game1 == true || game2 == true) break;
            DACout.PlayNote(329.00, 0.4, 1);
            DACout.PlayNote(369.99, 0.4, 1);
            DACout.PlayNote(0, 0.4, 1);
            DACout.PlayNote(329.63, 0.4, 1);
            if(game1 == true || game2 == true) break;
            DACout.PlayNote(0, 0.4, 1);
            DACout.PlayNote(277.18, 0.2, 1);
            DACout.PlayNote(277.18, 0.2, 1);
            DACout.PlayNote(277.18, 0.2, 1);
            if(game1 == true || game2 == true) break;
            DACout.PlayNote(0, 0.4, 1);
            DACout.PlayNote(0, 0.4, 1);
            DACout.PlayNote(277.18, 0.2, 1);
            DACout.PlayNote(277.18, 0.2, 1);
            if(game1 == true || game2 == true) break;
            DACout.PlayNote(277.18, 0.4, 1);
            DACout.PlayNote(0, 0.4, 1);
            DACout.PlayNote(0, 0.4, 1);
            DACout.PlayNote(311.13, 0.4, 1);
            if(game1 == true || game2 == true) break;
            DACout.PlayNote(293.66, 0.4, 1);
            //DACout.PlayNote(440.00, 0.3, 1);
            
            /*led2 = true;
            wave_file=fopen("/sd/MiiMenu.wav","r");
            PRINTF("Opening");
            waver.play(wave_file);
            fclose(wave_file);
            led2 = false;*/
        }
        while (game1 == true && game2 == false) {
            DACout.PlayNote(659.25, 0.3, 1);
            DACout.PlayNote(493.88, 0.3, 1);
            DACout.PlayNote(523.25, 0.3, 1);
           if(game1 != true && game2 == false) break;
            DACout.PlayNote(587.33, 0.3, 1);
            DACout.PlayNote(523.25, 0.3, 1);
            DACout.PlayNote(493.88, 0.3, 1);
            DACout.PlayNote(440.00, 0.3, 1);
            if(game1 != true && game2 == false) break;
            DACout.PlayNote(440.00, 0.3, 1);
            DACout.PlayNote(523.25, 0.3, 1);
            DACout.PlayNote(659.25, 0.3, 1);
            DACout.PlayNote(587.33, 0.3, 1);
            if(game1 != true && game2 == false) break;
            DACout.PlayNote(523.25, 0.3, 1);
            DACout.PlayNote(493.88, 0.3, 1);
            DACout.PlayNote(523.25, 0.3, 1);
            DACout.PlayNote(587.33, 0.3, 1);
            if(game1 != true && game2 == false) break;
            DACout.PlayNote(659.25, 0.3, 1);
            DACout.PlayNote(523.25, 0.3, 1);
            DACout.PlayNote(440.00, 0.3, 1);
            DACout.PlayNote(440.00, 0.3, 1);
            if(game1 != true && game2 == false) break;
            
            DACout.PlayNote(583.33, 0.3, 1);
            DACout.PlayNote(698.46, 0.3, 1);
            DACout.PlayNote(880.00, 0.3, 1);
            DACout.PlayNote(783.99, 0.3, 1);
            if(game1 != true && game2 == false) break;
            DACout.PlayNote(698.46, 0.3, 1);
            DACout.PlayNote(659.25, 0.3, 1);
            DACout.PlayNote(523.25, 0.3, 1);
            DACout.PlayNote(659.25, 0.3, 1);
            if(game1 != true && game2 == false) break;
            DACout.PlayNote(587.33, 0.3, 1);
            DACout.PlayNote(523.25, 0.3, 1);
            DACout.PlayNote(493.88, 0.3, 1);
            DACout.PlayNote(493.88, 0.3, 1);
            if(game1 != true && game2 == false) break;
            DACout.PlayNote(523.25, 0.3, 1);
            DACout.PlayNote(587.33, 0.3, 1);
            DACout.PlayNote(659.25, 0.3, 1);
            DACout.PlayNote(523.35, 0.3, 1);
            if(game1 != true && game2 == false) break;
            DACout.PlayNote(440.00, 0.3, 1);
            DACout.PlayNote(440.00, 0.3, 1);
           /* led2 = true;
            wave_file=fopen("/sd/tetris.wav","r");
            waver.play(wave_file);
            fclose(wave_file);
            led2 = false;*/
        }
        while (game2 == true && game1 == false) {
            DACout.PlayNote(329.63, 0.6, 1);
            DACout.PlayNote(349.23, 0.6, 1);
            DACout.PlayNote(392.00, 0.15, 1);
            DACout.PlayNote(523.25, 0.15, 1);
            if(game1 != true && game2 == false) break;
            DACout.PlayNote(493.88, 0.15, 1);
            DACout.PlayNote(523.25, 0.15, 1);
            DACout.PlayNote(392.00, 0.15, 1);
            DACout.PlayNote(329.63, 0.15, 1);
            if(game1 != true && game2 == false) break;
            DACout.PlayNote(261.63, 0.15, 1);
            DACout.PlayNote(293.66, 0.15, 1);
            DACout.PlayNote(329.33, 0.15, 1);
            DACout.PlayNote(293.66, 0.6, 1);
            if(game1 != true && game2 == false) break;
            DACout.PlayNote(293.66, 1.2, 1);
            DACout.PlayNote(329.63, 0.6, 1);
            DACout.PlayNote(349.23, 0.3, 1);
            DACout.PlayNote(329.63, 0.15, 1);
            if(game1 != true && game2 == false) break;
            DACout.PlayNote(349.23, 0.15, 1);
            
            
            DACout.PlayNote(392.00, 0.15, 1);
            DACout.PlayNote(523.25, 0.15, 1);           
            DACout.PlayNote(587.33, 0.15, 1);
            DACout.PlayNote(523.25, 0.3, 1);
            if(game1 != true && game2 == false) break;
            DACout.PlayNote(587.33, 0.1, 1);
            DACout.PlayNote(659.25, 0.1, 1);
            DACout.PlayNote(698.46, 0.15, 1);
            DACout.PlayNote(659.25, 0.15, 1);
            if(game1 != true && game2 == false) break;
            DACout.PlayNote(587.33, 0.15, 1);
            DACout.PlayNote(523.25, 0.3, 1);
            DACout.PlayNote(392.00, 0.15, 1);
            DACout.PlayNote(440.00, 0.15, 1);
            if(game1 != true && game2 == false) break;
            DACout.PlayNote(440.00, 0.15, 1);
            DACout.PlayNote(587.33, 0.15, 1);
            DACout.PlayNote(587.33, 0.1, 1);
            DACout.PlayNote(587.33, 0.15, 1);
            if(game1 != true && game2 == false) break;
            DACout.PlayNote(587.33, 0.1, 1);
            DACout.PlayNote(659.25, 0.1, 1);
            DACout.PlayNote(698.46, 0.15, 1);
            DACout.PlayNote(659.25, 0.15, 1);
            if(game1 != true && game2 == false) break;
            DACout.PlayNote(587.33, 0.15, 1);
            DACout.PlayNote(523.25, 0.3, 1);
            DACout.PlayNote(392.00, 0.3, 1);
            DACout.PlayNote(392.00, 0.6, 1);
            if(game1 != true && game2 == false) break;
            DACout.PlayNote(392.00, 0.15, 1);
            DACout.PlayNote(587.33, 0.1, 1);
            DACout.PlayNote(659.25, 0.1, 1);
            DACout.PlayNote(698.46, 0.15, 1);
            DACout.PlayNote(659.25, 0.15, 1);
            if(game1 != true && game2 == false) break;
            DACout.PlayNote(587.33, 0.15, 1);
            DACout.PlayNote(523.25, 0.3, 1);
            DACout.PlayNote(783.99, 0.3, 1);
            DACout.PlayNote(783.99, 0.6, 1);
            if(game1 != true && game2 == false) break;
            DACout.PlayNote(783.99, 0.15, 1);
            DACout.PlayNote(783.99, 0.1, 1);
            DACout.PlayNote(880.00, 0.1, 1);
            if(game1 != true && game2 == false) break;
            DACout.PlayNote(932.33, 0.15, 1);
            DACout.PlayNote(880.00, 0.15, 1);           
            DACout.PlayNote(783.99, 0.15, 1);
            DACout.PlayNote(698.46, 0.6, 1);
            if(game1 != true && game2 == false) break;
            DACout.PlayNote(830.61, 0.15, 1);
            DACout.PlayNote(783.99, 0.15, 1);
            DACout.PlayNote(698.46, 0.15, 1);
            DACout.PlayNote(622.25, 0.3, 1);
            if(game1 != true && game2 == false) break;
            DACout.PlayNote(739.99, 0.15, 1);
            DACout.PlayNote(698.46, 0.1, 1);
            DACout.PlayNote(698.46, 0.15, 1);
            DACout.PlayNote(622.25, 0.15, 1);
            if(game1 != true && game2 == false) break;
            DACout.PlayNote(554.37, 0.15, 1);
            DACout.PlayNote(415.30, 0.15, 1);
            DACout.PlayNote(349.23, 0.15, 1);
            DACout.PlayNote(277.18, 0.15, 1);
            DACout.PlayNote(311.13, 0.3, 1);
            if(game1 != true && game2 == false) break;
            DACout.PlayNote(554.37, 0.3, 1);
            DACout.PlayNote(587.33, 0.6, 1);
            DACout.PlayNote(587.33, 0.6, 1);
            DACout.PlayNote(587.33, 0.15, 1);
            DACout.PlayNote(392.00, 0.15, 1);
            if(game1 != true && game2 == false) break;
            DACout.PlayNote(587.33, 0.3, 1);
            DACout.PlayNote(783.99, 0.3, 1);
            DACout.PlayNote(783.99, 0.6, 1);
            DACout.PlayNote(783.99, 1.2, 1);
            if(game1 != true && game2 == false) break;
            /*led2 = true;
            wave_file=fopen("/sd/WiiPlayAirHockey.wav","r");
            waver.play(wave_file);
            fclose(wave_file);
            led2 = false;*/
        }
    }
}

void ledThread()
{
    float x = 0.0;
    while(1) {
        //get a new random number for PWM
        x = rand() / float(RAND_MAX);
        //add some exponential brightness scaling
        //for more of a fast flash effect
        x = x*x*x;
        Rgb = x;
        rGb = x;
        rgB = x;
        //fast update rate for welding flashes
        Thread::wait(20);
        //add a random pause between welds
        if (rand() / float(RAND_MAX) > 0.95) {
            Rgb = 0;
            rGb = 0;
            rgB = 0;
            Thread::wait(4000.0 * rand() / float(RAND_MAX));
        }
    }
}

int main()
{
    // Launch four threads
    Thread thread1(tetrisGame, osPriorityHigh);
    Thread thread2(hockeyGame, osPriorityHigh);
    Thread thread3(audioThread, osPriorityLow);
    Thread thread4(ledThread, osPriorityLow);
    
    // The main thread goes on to blink LEDs
    //Speaker mySpeaker(p18);
    // loops forever playing two notes on speaker using analog samples
    //PRINTF("test");
//    while(1) {
//        //mySpeaker.PlayNote(969.0, 0.5, 1.0);
////        mySpeaker.PlayNote(800.0, 0.5, 1.0);
//        //PRINTF("loop");
//    }
    while (true) {
        led1 = 1;
        Thread::wait(500);
        led1 = 0;
        Thread::wait(500);
        if(!game1 && !game2 && menu_flag==0){
            uLCD.cls();
            uLCD.color(WHITE);
            uLCD.text_height(2);
            uLCD.text_width(2);
            uLCD.locate(2,0);
            uLCD.printf("MENU:\n");
            uLCD.printf("UP:\n");
            uLCD.printf("Tetris \n");
            uLCD.printf("\n");
            uLCD.printf("DOWN:\n");
            uLCD.printf("Air\n");
            uLCD.printf("Hockey");
            menu_flag=1;
        }
        
        //DACout.PlayNote(50, 1.0/2.5, 0.08);
        
        PRINTF("[MAIN] Thread stacks: %lu %lu %lu %lu\r\n",
            thread1.used_stack(),
            thread2.used_stack(),
            thread3.used_stack(),
            thread4.used_stack());
        
        // Launch into the air hockey game
        if (game1 != true && game2 != true && myNav.up()){
            uLCD.cls();
            game1 = true;
            }
        if (game2 != true && game1 != true && myNav.down()){
            uLCD.cls();
            game2 = true;
            }
    }
}
