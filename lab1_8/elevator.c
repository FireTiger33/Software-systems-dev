#include "elevator.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#define ESC "\033"
#define gotoxy(x, y)        printf(ESC "[%d;%dH", x, y)


Elevator* this_elevator;
int* fds;
int OUT_STREAM;
char* set_ursor_pos_in_esc(int x, int y) {
    static unsigned char x_str[3] = "\0";
    static unsigned char y_str[3] = "\0";
    char* esc = malloc(15 * sizeof(char));
    esc[0] = 27;
    esc[1] = '[';
    int i = 2;
    int j = 0;

    sprintf((char *) x_str, "%d", x);  // пишем позицию в строку
    sprintf((char *) y_str, "%d", y);
    while (y_str[j])
        esc[i++] = y_str[j++];
    esc[i++] = ';';
    j = 0;
    while (x_str[j])
        esc[i++] = x_str[j++];
    esc[i++] = 'H';
    esc[i] = '\0';
    write(fds[1], esc, strlen(esc));    
    
    return esc;
}




int get_x_for_term(int label_len, int pos) {
    return label_len + 2 * pos;
}


void elevator_start(Elevator* elevator, int fds_vis[2]) {
    this_elevator = elevator;
    fds = fds_vis;
    OUT_STREAM = fds[1];
    int num_elevator = elevator->num;
    int len_label = strlen(elevator->label);
    char* interfase = (char*)malloc(len_label + 21);
    
    strncat(interfase, elevator->label, len_label);
    strncat(interfase, " 0 - - - - - - - - -\n", 21);
    set_val_in_pos(interfase, 0, elevator->num);
    
    int key = -1;
    while (1) {
        if ((elevator->pick_floors & (1 << elevator->pos)) > 0) {
            elevator->num_persons[0]++;
            elevator->pick_floors = elevator->pick_floors - (1 << elevator->pos);
            if (!elevator->pick_floors) {
                elevator->direction = WAIT;
            }
        }
        if ((elevator->land_floors & (1 << elevator->pos)) > 0) {
            elevator->num_persons[0]--;
            elevator->land_floors = elevator->land_floors - (1 << elevator->pos);
            if (!elevator->land_floors) {
                elevator->num_persons[0] = '0';
                elevator->direction = PAUSE;
            }
        }
        
        if (elevator->pick_floors || elevator->land_floors) {
            if (elevator->direction == DOWN) {
                elevator->pos--;
                key = 1;
            } else if (elevator->direction == UP) {
                elevator->pos++;
                key = -1;
            }
        }
        char state[3] = {};
        int pos = elevator->pos;
        state[0] = '0' + (pos == 9? 0: pos + 1);
        switch (elevator->direction) {
            case UP: state[1] = 'U';
                    break;
            case DOWN: state[1] = 'D';
                    break;
            case PAUSE: state[1] = 'P';
                    break;
            case WAIT: state[1] = 'W'; 
                    break;
            default: state[1] = '0';
        }
        write(1, state, 2);
        if (pos == 0) {
            key = 1;
        } else if (pos == elevator->num_floors){
            key = -1;
        }
        set_val_in_pos(elevator->num_persons, get_x_for_term(len_label, pos), num_elevator);
        set_val_in_pos("-", get_x_for_term(len_label, pos + key), num_elevator);
        
        usleep(elevator->num * 500000);
    }
}


Elevator* create_elevator(char* label, int floor_num) {
    static int num_elevators = 1;
    Elevator* elevator = (Elevator*)malloc(sizeof(Elevator));
    if (!elevator) {
        return NULL;
    }
    elevator->direction = PAUSE;
    elevator->num_floors = floor_num;
    elevator->num_persons = malloc(1);
    elevator->num_persons[0] = '0';
    elevator->num = num_elevators;
    num_elevators++;
    elevator->label = label;

    return elevator;
}

void listener(int sig) {
    char buf[3] = {};
    int permission = 0;
    if (sig == SIGUSR1) {
        read(0, buf, 2);
        if (buf[0] == 'l') {
            if (this_elevator->num_persons[0] - '0' > 0) {
                land_per(this_elevator, buf[1]);
                permission = 1;
            }
        } else {
            pick_per(this_elevator, buf[1]);
            permission = 1;
        }
        if (permission) {
            if (this_elevator->pos < buf[1]) {
                this_elevator->direction = UP;
            } else if (this_elevator->pos > buf[1]) {
                this_elevator->direction = DOWN;
            } else {
                this_elevator->direction = PAUSE;
            }
        }
    }
}

void pick_per(Elevator* elevator, int floor) {
    floor-=1;
    elevator->pick_floors |= 1 << floor;
}

void land_per(Elevator* elevator, int floor) {
    elevator->land_floors |= 1 << floor;
}

int get_pos(Elevator* elevator) {
    return elevator->pos;
}

int get_direction(Elevator* elevator) {
    return elevator->direction;
}

int get_elevator_label(Elevator* elevator) {
    return elevator->pick_floors;
}














