# Air Hockey
## Overview
Air Hockey is a two player game in which each player controls a paddle and attempts to deflect a puck into the opponent's goal. Our game was inspired by Air Hockey game on the Wii, which we attempted to emulate in style and function.

At the start of the game, players are placed in the center of their half of the court, and the puck is served toward a random player. After each point, the players are reset, and the puck is served to the player most recently scored against. The game continues until one of the players scores 5 points.



## Project Demo
[![Video Demo](https://github.gatech.edu/jstephens78/ece-4180-final-project/blob/main/docs/full-demo.png?raw=true)](https://github.gatech.edu/raw/jstephens78/ece-4180-final-project/main/docs/game-air-hockey-demo.mp4?token=GHSAT0AAAAAAAACOJFRZJYJEKUPMZ35N43WY44WO4Q)



## Player Input
One player provides input via the [NavSwitch](https://os.mbed.com/users/4180_1/notebook/using-a-navigation-switch-digital-joystick/), where directional input translates the paddle, and pressing the center button causes a 45 degree rotation.

The other player provides input from their smartphone via the [Bluefruit Module](https://os.mbed.com/users/4180_1/notebook/adafruit-bluefruit-le-uart-friend---bluetooth-low-/). The directional pad is used to translate, and buttons `1` and `2` are used to adjust the paddle in 30 degree increments.

A `BluefruitController` class was written to handle parsing bluetooth data into controller inputs (see [bluefruit_controller.cpp](https://os.mbed.com/users/jstephens78/code/4180_Project//file/a09981ebd532/bluefruit_controller.cpp/))
```c++
enum ButtonId {
    BUTTON_A = 0,  BUTTON_B = 1,    BUTTON_X = 2,    BUTTON_Y = 3,
    BUTTON_UP = 4, BUTTON_DOWN = 5, BUTTON_LEFT = 6, BUTTON_RIGHT = 7
};

class BluefruitController : public Serial
{
public:
    BluefruitController(PinName tx, PinName rx, int baud = 9600);

    void parseMessage();

    void reset();

    float quaternion[4];
    bool button[8];

private:
    static const int BUFFER_SIZE = 20;

    void parseButton();
    void parseQuaternion();

    /// Holds the parse state
    int state;
    char msg_tag;
    
    // Buffer in which messages are read
    char buff[BUFFER_SIZE];
    int buff_i;
    int buff_len;
};
```

In addition to button inputs, the class has support for Quaternion data which can be used to extract an orientation of the phone. This in practice proves to run at too slow of a rate for fluid use, and the Adafruit app appears to provide incorrect quaternion fields (at least on Android, untested on iOS).




## Collisions
In order to support the collision mechanics of the game, we ported a lightweight 2D phyics library to the MBED and produced the [Physacs-MBED Library](https://github.gatech.edu/jstephens78/ece-4180-final-project/blob/main/docs/mbed-physac.md). The original library was modified to reduce the static memory footprint, and the memory footprint of dynamically-allocated physics object. This introduced some limitations which are described in the library wiki page. However, the library is sufficient for use in Air Hockey.

The paddles and puck are modeled with Physac as rectangles and circles, and are thus able to interact with one another:
```c++
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
```

Then, inside our core game loop, simply calling `PhysicsStep()` is sufficient for Physacs to handle the movement, collision, and reflection of the puck against the paddles. The objects `paddle_a` and `paddle_b` have dynamics disabled; Physacs will not attempt to move them due to velocity or in response to collisions. This allows us to manually control set their position according to player inputs.

While performance of the library is fairly good for the paddles and puck (taking around .1 ms to compute), we chose not to use Physac to model the walls of the arena. Doing so would introduce overhead to the simulation, when relatively simple axis-aligned boundary checking and reflection can achieve the same result:

```c++
void runGameLogic()
{
    // If puck hits the ceiling or floor, reflect across the y component of
    // the velocity
    if (puck->position.y < HOCKEY_ARENA_TOP + HOCKEY_PUCK_RADIUS + 1) {
        puck->velocity.y *= -1;
        puck->position.y = maxOf(puck->position.y, HOCKEY_ARENA_TOP + HOCKEY_PUCK_RADIUS + 1);
    } else if (puck->position.y > HOCKEY_ARENA_BOT - HOCKEY_PUCK_RADIUS - 1) {
        puck->velocity.y *= -1;
        puck->position.y = minOf(puck->position.y, HOCKEY_ARENA_BOT - HOCKEY_PUCK_RADIUS - 1);
    }

    // walls; reflect across the x component of velocity
    if (puck->position.x < HOCKEY_PUCK_RADIUS + 1 && puck->position.x > 0) {
        puck->velocity.x *= -1;
        puck->position.x = maxOf(puck->position.x, HOCKEY_PUCK_RADIUS + 1);
    } else if (puck->position.x > SCREEN_WIDTH - HOCKEY_PUCK_RADIUS - 1 && puck->position.x < SCREEN_WIDTH) {
        puck->velocity.x *= -1;
        puck->position.x = minOf(puck->position.x, SCREEN_WIDTH - HOCKEY_PUCK_RADIUS - 2);
    }

    // (scoring logic omitted from this example)
}
```

This blend of "full" physics simulation with "simple" wall collisions performs well and gives robustly decent output.


## Rendering
At each game reset (that is, at game start and after each point) the system will
- Clear the screen
- Draw the arena
- Print score
- Draw a serve indicator next to the score of the player the puck will be served toward
- Draw the players' paddles
- Draw the puck

This ensures that the game display starts in a perfectly valid state.

At each frame in the game, the renderer will erase the paddles and puck, then redraw them. The renderer never re-draws the arena or scores during a match. Occasionally, due to rounding errors in the underlying position, the puck or paddles may overlap the walls, causing a few stray pixels to be erased. These will remain until the next point reset.



## Resources
- Original [Physac](https://github.com/victorfisac/Physac) library
