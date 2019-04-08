#ifndef INCLUDE_ELEVATOR_
#define INCLUDE_ELEVATOR_

#include <stdlib.h>
#include "moded_outer.h"

enum {
    PAUSE,
    UP,
    DOWN,
    WAIT
};


typedef struct Elevator {
    int num;
    int num_floors;
    char* num_persons;
    char* label;
    unsigned int pick_floors, land_floors;
    int pos;
    int direction;
    char* keyboard;
} Elevator;

void elevator_start();
Elevator* create_elevator(char* label, int floor_num);
void pick_per(Elevator* elevator, int floor);
void land_per(Elevator* elevator, int floor);
int get_pos(Elevator* elevator);
int get_direction(Elevator* elevator);
int get_elevator_label(Elevator* elevator);
void listener(int sig);

#endif  // INCLUDE_ELEVATOR_
