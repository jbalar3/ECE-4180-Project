#include "hockey.h"
#include "globals.h"

// Custom memory allocation methods
#define PHYSAC_NO_THREADS
#define PHYSAC_STANDALONE
#define PHYSAC_IMPLEMENTATION
#define _STDBOOL_H
#include "physac.h"

// Controls size of gamepieces in the hockey arena. This influences both
// rendering and collision, so be careful adjusting dimensions too small
#define HOCKEY_PUCK_RADIUS 4
#define HOCKEY_PADDLE_W 8
#define HOCKEY_PADDLE_H 24

// Physics sim properties
//  HOCKEY_PHYSICS_STEPS    number of physics ticks between frames
//  HOCKEY_PHYSICS_DELTA    how many ms the physics engine believes pass per
//                          update. This is arbitrary. Pick what behaves well.
//  HOCKEY_PUCK_VELOCITY    how fact the puck moves in one physics tick
#define HOCKEY_PHYSICS_STEPS 4
#define HOCKEY_PHYSICS_DELTA 1.66
#define HOCKEY_PUCK_VELOCITY (5.0f / HOCKEY_PHYSICS_STEPS)

// Screen-space constants:
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 128

// Controls the dimensions of the arena. You can adjust the screen margin above
// and below the court, and the width of the goal.
#define HOCKEY_ARENA_TOP 16
#define HOCKEY_ARENA_BOT 127
#define HOCKEY_GOAL_HEIGHT 32

// Properties derived from other macros
#define _HOCKEY_ARENA_H (HOCKEY_ARENA_BOT - HOCKEY_ARENA_TOP)
#define _HOCKEY_ARENA_MID ((HOCKEY_ARENA_BOT + HOCKEY_ARENA_TOP)/2)
#define _HOCKEY_GOAL_TOP ((_HOCKEY_ARENA_H - HOCKEY_GOAL_HEIGHT)/2 + HOCKEY_ARENA_TOP)
#define _HOCKEY_GOAL_BOT (_HOCKEY_GOAL_TOP + HOCKEY_GOAL_HEIGHT)


PhysicsBody puck;
PhysicsBody paddle_a;
PhysicsBody paddle_b;

int blue_score;
int red_score;
bool serve_direction;   // 0 means toward blue, 1 means red
float input_sensitivity = 4.00f;

// Helper functions
// @{
int maxOf(int a, int b)
{
    return (a > b) ? a : b;
}
int minOf(int a, int b)
{
    return (a < b) ? a : b;
}
int clamp(int a, int low, int upp)
{
    return minOf(upp, maxOf(a, low));
}
// @}

/**
 * Sets up physics elements within the Physac library
 */
void initPhysicsObjects()
{
    SetPhysicsTimeStep(HOCKEY_PHYSICS_DELTA);

    puck = CreatePhysicsBodyCircle((Vector2) {
        SCREEN_WIDTH/2, SCREEN_HEIGHT/2
    }, HOCKEY_PUCK_RADIUS, 1);
    puck->enabled = true;
    puck->useGravity = false;
    puck->restitution = 1.0;
    puck->dynamicFriction = 0;
    puck->velocity = (Vector2) {
        HOCKEY_PUCK_VELOCITY, 0
    };

    paddle_a = CreatePhysicsBodyRectangle((Vector2) {
        32, _HOCKEY_ARENA_MID
    }, HOCKEY_PADDLE_W, HOCKEY_PADDLE_H, 100);
    paddle_a->enabled = false; // Disable body state to convert it to static (no dynamics, but collisions)
    paddle_a->useGravity = false;
    paddle_a->restitution = 1.0;
    SetPhysicsBodyRotation(paddle_a, 3.14159 / 6);

    paddle_b = CreatePhysicsBodyRectangle((Vector2) {
        96, _HOCKEY_ARENA_MID
    }, HOCKEY_PADDLE_W, HOCKEY_PADDLE_H, 100);
    paddle_b->enabled = false; // Disable body state to convert it to static (no dynamics, but collisions)
    paddle_b->useGravity = false;
    paddle_b->restitution = 1.0;
    SetPhysicsBodyRotation(paddle_b, 3.14159 / 6);
}

/**
 * Draws the fixed graphics of the game. This is the border of the arena
 *
 * note: locks `uLCD_mutex`
 */
void drawStaticEnvironment()
{
    uLCD_mutex.lock();

    // Clear screen
    uLCD.cls();

    // Draw arena border
    uLCD.line(0, _HOCKEY_GOAL_TOP, 0, HOCKEY_ARENA_TOP, BLUE);
    uLCD.line(0, _HOCKEY_GOAL_BOT, 0, HOCKEY_ARENA_BOT, BLUE);
    uLCD.line(0, HOCKEY_ARENA_TOP, 63, HOCKEY_ARENA_TOP, BLUE);
    uLCD.line(0, HOCKEY_ARENA_BOT, 63, HOCKEY_ARENA_BOT, BLUE);
    uLCD.line(127, _HOCKEY_GOAL_TOP, 127, HOCKEY_ARENA_TOP, RED);
    uLCD.line(127, _HOCKEY_GOAL_BOT, 127, HOCKEY_ARENA_BOT, RED);
    uLCD.line(64, HOCKEY_ARENA_TOP, 127, HOCKEY_ARENA_TOP, RED);
    uLCD.line(64, HOCKEY_ARENA_BOT, 127, HOCKEY_ARENA_BOT, RED);

    // Draw score
    uLCD.color(BLUE);
    uLCD.locate(0, 0);
    uLCD.printf("%i", blue_score);
    uLCD.color(RED);
    uLCD.locate(17, 0);
    uLCD.printf("%i", red_score);

    // Draw serve indicator
    uLCD.filled_circle(
        serve_direction ? SCREEN_WIDTH - 16 : 16,
        HOCKEY_ARENA_TOP / 2,
        HOCKEY_ARENA_TOP / 6,
        serve_direction ? RED : BLUE);

    uLCD_mutex.unlock();
}

/**
 * Redraws the puck and paddles
 * Each elements is erased, updated, and redrawn in as short of a window as
 * possible.
 *
 * note: locks `uLCD_mutex`
 */
void drawDynamicObjects()
{
    static int puck_x = SCREEN_WIDTH / 2,
               puck_y = _HOCKEY_ARENA_MID;

    uLCD_mutex.lock();

    // Redraw puck
    uLCD.filled_circle(puck_x, puck_y, HOCKEY_PUCK_RADIUS, BLACK);
    puck_x = puck->position.x;
    puck_y = puck->position.y;
    uLCD.filled_circle(puck_x, puck_y, HOCKEY_PUCK_RADIUS, GREEN);

    // Redraw blue paddle
    static int pa_x1 = 64,
               pa_x2 = 64,
               pa_y1 = 64,
               pa_y2 = 64;
    float sinA = sin(-paddle_a->orient) / 2;
    float cosA = cos(-paddle_a->orient) / 2;
    uLCD.line(pa_x1, pa_y1, pa_x2, pa_y2, BLACK);
    pa_x1 = paddle_a->position.x + HOCKEY_PADDLE_H * sinA;
    pa_x2 = paddle_a->position.x - HOCKEY_PADDLE_H * sinA;
    pa_y1 = paddle_a->position.y + HOCKEY_PADDLE_H * cosA;
    pa_y2 = paddle_a->position.y - HOCKEY_PADDLE_H * cosA;
    uLCD.line(pa_x1, pa_y1, pa_x2, pa_y2, BLUE);

    // Redraw red paddle
    static int pb_x1 = 64,
               pb_x2 = 64,
               pb_y1 = 64,
               pb_y2 = 64;
    float sinB = sin(-paddle_b->orient) / 2;
    float cosB = cos(-paddle_b->orient) / 2;
    uLCD.line(pb_x1, pb_y1, pb_x2, pb_y2, BLACK);
    pb_x1 = paddle_b->position.x + HOCKEY_PADDLE_H * sinB;
    pb_x2 = paddle_b->position.x - HOCKEY_PADDLE_H * sinB;
    pb_y1 = paddle_b->position.y + HOCKEY_PADDLE_H * cosB;
    pb_y2 = paddle_b->position.y - HOCKEY_PADDLE_H * cosB;
    uLCD.line(pb_x1, pb_y1, pb_x2, pb_y2, RED);

    uLCD_mutex.unlock();
}

/**
 * Reset the game after a goal.
 * If the game will continue, re-draw and reset the pieces.
 * If the game is over, display end scene and then suspend the game loop until
 *      re-launched.
 */
void resetGame()
{
    // If game over, draw an end-game screen
    if (red_score >= 5 || blue_score >= 5) {
        uLCD_mutex.lock();
        uLCD.cls();
        uLCD.locate(5, 3);
        uLCD.color(GREEN);
        uLCD.printf("Game Over");

        if (red_score > blue_score) {
            uLCD.locate(5, 5);
            uLCD.color(RED);
            uLCD.printf("Red Wins!");
        } else {
            uLCD.locate(5, 5);
            uLCD.color(BLUE);
            uLCD.printf("Blue Wins!");
        }

        Thread::wait(2000);

        uLCD.cls();
        menu_flag = 0;

        game2 = false;
        uLCD_mutex.unlock();
    }

    // Otherwise re-draw the game board and reset the pieces
    else {
        drawStaticEnvironment();

        puck->position.x = SCREEN_WIDTH/2;
        puck->position.y = _HOCKEY_ARENA_MID;
        puck->velocity.x = HOCKEY_PUCK_VELOCITY * (serve_direction ? 1 : -1);
        puck->velocity.y = (float)(rand() % 50) / 100;

        paddle_a->position.x = 32;
        paddle_a->position.y = _HOCKEY_ARENA_MID;
        SetPhysicsBodyRotation(paddle_a, 0);

        paddle_b->position.x = 96;
        paddle_b->position.y = _HOCKEY_ARENA_MID;
        SetPhysicsBodyRotation(paddle_b, 0);
    }
}

/**
 * Handles puck bouncing off walls, goal scoring, player input, and end-game
 * conditions
 */
void runGameLogic()
{
    // If puck hits the ceiling or floor, reflect across the y component of
    // the velocity
    if (puck->position.y < HOCKEY_ARENA_TOP + HOCKEY_PUCK_RADIUS + 1) {
        puck->velocity.y *= -1;
        puck->position.y = maxOf(
                               puck->position.y,
                               HOCKEY_ARENA_TOP + HOCKEY_PUCK_RADIUS + 1);
    } else if (puck->position.y > HOCKEY_ARENA_BOT - HOCKEY_PUCK_RADIUS - 1) {
        puck->velocity.y *= -1;
        puck->position.y = minOf(
                               puck->position.y,
                               HOCKEY_ARENA_BOT - HOCKEY_PUCK_RADIUS - 1);
    }

    // true if the puck is in the y range corresponding to the goal
    bool puckInGoalRange =
        puck->position.y > _HOCKEY_GOAL_TOP &&
        puck->position.y < _HOCKEY_GOAL_BOT;

    // Puck collides with left or right walls
    if (puckInGoalRange == false) {
        if (puck->position.x < HOCKEY_PUCK_RADIUS + 1 && puck->position.x > 0) {
            puck->velocity.x *= -1;
            puck->position.x = maxOf(
                                   puck->position.x,
                                   HOCKEY_PUCK_RADIUS + 1);
        } else if (puck->position.x > SCREEN_WIDTH - HOCKEY_PUCK_RADIUS - 1 && puck->position.x < SCREEN_WIDTH) {
            puck->velocity.x *= -1;
            puck->position.x = minOf(
                                   puck->position.x,
                                   SCREEN_WIDTH - HOCKEY_PUCK_RADIUS - 2);
        }
    }

    // Puck in goals
    else {
        // SCORE RED
        if (puck->position.x <= -HOCKEY_PUCK_RADIUS * 2) {
            red_score += 1;
            serve_direction = 0;
            uLCD.locate(4, 1);
            uLCD.color(RED);
            uLCD.printf("Red Scores!");
            Thread::wait(2000);
            resetGame();
        }

        // SCORE BLUE
        else if (puck->position.x >= SCREEN_WIDTH + HOCKEY_PUCK_RADIUS * 2) {
            blue_score += 1;
            serve_direction = 1;
            uLCD.locate(3, 1);
            uLCD.color(BLUE);
            uLCD.printf("Blue Scores!");
            Thread::wait(2000);
            resetGame();
        }
    }
}

/**
 * Process user input and update game state
 */
void handleInput()
{
    // Process input from the game
    blue.parseMessage();
    if (blue.button[BUTTON_UP]) {
        paddle_a->position.y -= input_sensitivity;
    }
    if (blue.button[BUTTON_DOWN]) {
        paddle_a->position.y += input_sensitivity;
    }
    if (blue.button[BUTTON_LEFT]) {
        paddle_a->position.x -= input_sensitivity;
    }
    if (blue.button[BUTTON_RIGHT]) {
        paddle_a->position.x += input_sensitivity;
    }
    if (blue.button[BUTTON_A]) {
        SetPhysicsBodyRotation(paddle_a, paddle_a->orient - input_sensitivity * 0.05);
    }
    if (blue.button[BUTTON_B]) {
        SetPhysicsBodyRotation(paddle_a, paddle_a->orient + input_sensitivity * 0.05);
    }



    
    // Player 2 Input
    if (myNav.up()) { // Up
        paddle_b->position.y -= input_sensitivity;
    }
    if (myNav.down()) { // down
        paddle_b->position.y += input_sensitivity;
    }
    if (myNav.left()) { // left
        paddle_b->position.x -= input_sensitivity;
    }
    if (myNav.right()) { // right
        paddle_b->position.x += input_sensitivity;
    }
    if (myNav.fire()) { // rotate
        SetPhysicsBodyRotation(paddle_b, paddle_b->orient + 3.14159 / 4);
    }

    // Bounds checking. Don't let users steer paddles out of the arena
    paddle_a->position.y = clamp(paddle_a->position.y, HOCKEY_ARENA_TOP, HOCKEY_ARENA_BOT);
    paddle_a->position.x = clamp(paddle_a->position.x, 1, SCREEN_WIDTH - 1);

    paddle_b->position.y = clamp(paddle_b->position.y, HOCKEY_ARENA_TOP, HOCKEY_ARENA_BOT);
    paddle_b->position.x = clamp(paddle_b->position.x, 1, SCREEN_WIDTH - 1);
}

/**
 * The primary game loop for the air hockey game
 */
void hockeyGame(void)
{
    // Set up Physac objects & simulation
    initPhysicsObjects();

    while (true) { 
        // Wait until the game starts
        while (game2 == false) {
           
            PRINTF("[HOCKEY] Idle\r\n");
            Thread::wait(500);
        }
        
        // Reset game state
        red_score = 0;
        blue_score = 0;
        serve_direction = rand() % 2;
        resetGame();

        // Reset screen, draw arena
        drawStaticEnvironment();

        Timer timer;
        timer.start();

        while (game2) {
            float dt = timer.read() * 1000;
            timer.reset();

            // Update the physics of the game
            for (int i = 0; i < HOCKEY_PHYSICS_STEPS; i++) {
                PhysicsStep();
                runGameLogic();
                if (game2 == false) break;
            }

            // Exit loop early if game over after the logic step
            if (game2 == false) continue;

            handleInput();

            // Render the game
            drawDynamicObjects();

            PRINTF("[HOCKEY] Delta %f\r\n", dt);
            Thread::wait(100);
        }
    }

    ClosePhysics();
}