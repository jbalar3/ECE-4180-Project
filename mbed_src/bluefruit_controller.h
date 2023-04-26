
#ifndef __BLUEFRUIT_CONTROLLER_HPP
#define __BLUEFRUIT_CONTROLLER_HPP

#include "mbed.h"

enum ButtonId {
    BUTTON_A = 0,
    BUTTON_B = 1,
    BUTTON_X = 2,
    BUTTON_Y = 3,
    BUTTON_UP = 4,
    BUTTON_DOWN = 5,
    BUTTON_LEFT = 6,
    BUTTON_RIGHT = 7
};

/**
 * \brief Encapsulates the controller functionality of the Bluefruit module
 *      through the default app.
 */
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

#endif
