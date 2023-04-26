#include "bluefruit_controller.h"

enum State {
    STATE_IDLE,         // Waiting for a message
    STATE_MSG,          // '!' received, read in a tag
    STATE_PAYLOAD,      // tag received, read in payload bytes
};


BluefruitController::BluefruitController(PinName tx, PinName rx, int baud)
    : Serial(tx, rx, baud)
{
    buff_len = 0;
    buff_i = 0;
}

void BluefruitController::parseMessage()
{
    while (readable()) {
        switch (state) {
            // Read until the '!'
            case STATE_IDLE: {
                if (getc() == '!')
                    state = STATE_MSG;
                break;
            }

            // Read in the msg tag and begin reading that many bytes
            case STATE_MSG: {
                msg_tag = getc();
                buff_i = 0;
                state = STATE_PAYLOAD;

                switch (msg_tag) {
                    case 'B':
                        buff_len = 3;
                        break;
                    case 'Q':
                        buff_len = 4*4;
                        break;

                    // For unrecognized message types, go back to waiting for
                    // the next '!'
                    default:
                        state = STATE_IDLE;
                }
                break;
            }

            // Read in payload bytes
            case STATE_PAYLOAD: {
                // Read in bytes
                if (buff_i < buff_len) {
                    buff[buff_i] = getc();
                    buff_i++;
                }

                // When done, parse the messages
                if (buff_i == buff_len) {
                    state = STATE_IDLE;
                    switch(msg_tag) {
                        case 'B':
                            parseButton();
                            break;
                        case 'Q':
                            parseQuaternion();
                            break;
                    }
                }
                break;
            }
        }
    }
}


void BluefruitController::reset()
{
    state = STATE_IDLE;
    buff_len = 0;
    buff_i = 0;
}

void BluefruitController::parseButton()
{
    unsigned int id = buff[0] - '1';
    bool val = buff[1] - '0';

    if (id >= 8) return;

    button[id] = val;
}

void BluefruitController::parseQuaternion()
{
    for (int i = 0; i < 4; i++) {
        quaternion[i] = *(((float*)buff) + i);
    }
}
