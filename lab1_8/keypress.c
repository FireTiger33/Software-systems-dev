#include "keypress.h"

#include <termios.h>
#include <sys/types.h>
#include <unistd.h>


static struct termios stored_set;

void shadow_keypress() {
    static struct termios shadow_set;
    static int key = 1;
    if (key) {
        tcgetattr(0, &stored_set);
        shadow_set = stored_set;
        shadow_set.c_lflag &= (~ICANON & ~ECHO);
        shadow_set.c_cc[VTIME] = 0;
        shadow_set.c_cc[VMIN] = 1;
        key = !key;
    }

    tcsetattr(0, TCSANOW, &shadow_set);
}

void reset_keypress() {
    tcsetattr(0, TCSANOW, &stored_set);
}
