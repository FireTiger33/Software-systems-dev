#include <unistd.h>
#include <stdio.h>
#include <sys/timeb.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <math.h>
#include <stropts.h>
#include <sys/poll.h>
#include "elevator.h"
#include "keypress.h"
#include "moded_outer.h"

int OUT_STREAM = 1;

int search_char(char* first, char* last, char val){
    char* iterator = first;
    for (; iterator != last+1; ++iterator) {
        if (*iterator == val) {
            return iterator - first;
        }
    }
    return -1;
}

int main() {
    char keyboard_1[10] = {'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p'};
    char keyboard_2[10] = {'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';'};
    Elevator* elevator_1 = create_elevator("ЛП", 10);
    Elevator* elevator_2 = create_elevator("ЛГ", 10);
    char state[2][3] = {{}, {}};
    int pid[2];
    int fds[5][2];
    for (int i = 0; i < 5; i++) {
        pipe(fds[i]);
    }
    struct pollfd input[] = {{0, POLLIN, 0}};
    struct pollfd input_el_1[] = {{fds[0][0], POLLIN, 0}};
    struct pollfd input_el_2[] = {{fds[1][0], POLLIN, 0}};
    dup2(1, fds[2][1]);
    clrscr();
    shadow_keypress();
    if ((pid[0] = fork()) == 0) {
        signal(SIGUSR1, listener);
        dup2(fds[0][1], 1);
        dup2(fds[3][0], 0);
        for (int i = 0; i < 5; i++) {
            if (i == 2) continue;
            close(fds[i][0]); close(fds[i][1]);
        }
        elevator_start(elevator_1, fds[2]);
    }
    if ((pid[1] = fork()) == 0) {
        signal(SIGUSR1, listener);
        dup2(fds[1][1], 1);
        dup2(fds[4][0], 0);
        for (int i = 0; i < 5; i++) {
            if (i == 2) continue;
            close(fds[i][0]); close(fds[i][1]);
        }
        elevator_start(elevator_2, fds[2]);
    }
    int land_floor;
    char buf_click_btn[2] = {};
    while(1) {
        if (poll(input, 1, 0)) {
            read(0, buf_click_btn, 1);
            if (buf_click_btn[0] >= '0' && buf_click_btn[0] <= '9') {
                buf_click_btn[1] = buf_click_btn[0] != '0'? buf_click_btn[0] - '0':10;
                buf_click_btn[0] = 'p';
                state[0][0] -= '0';
                if (((state[0][0] < buf_click_btn[1] && state[0][1] == 'U')
                        || (state[0][0] > buf_click_btn[1] && state[0][1] == 'D')
                    || state[0][1] == 'P'
                    || (state[0][1] == 'W' && state[0][0] == buf_click_btn[1]))
                    && state[0][0] - buf_click_btn[1] < state[1][0] - buf_click_btn[1] / 2) {
                    kill(pid[0], SIGUSR1);
                    write(fds[3][1], buf_click_btn, 2);
                } else if ((state[1][0] < buf_click_btn[1] && state[1][1] == 'U')
                        || (state[1][0] > buf_click_btn[1] && state[1][1] == 'D')
                        || state[1][1] == 'P'
                        || (state[1][1] == 'W' && buf_click_btn[1] == state[1][0])) {
                    kill(pid[1], SIGUSR1);
                    write(fds[4][1], buf_click_btn, 2);
                } else {
                    char to_in[1] = {buf_click_btn[1] == 10? '0': buf_click_btn[1] + '0'};
                    //ungetc(to_in, stdout);
                    fputs(to_in, stdin);
                }
            } else if ((land_floor = search_char(keyboard_1, &keyboard_1[9], buf_click_btn[0])) != -1) {
                buf_click_btn[1] = land_floor;
                buf_click_btn[0] = 'l';
                kill(pid[0], SIGUSR1);
                write(fds[3][1], buf_click_btn, 2);
            } else if ((land_floor = search_char(keyboard_2, &keyboard_2[9], buf_click_btn[0])) != -1) {
                buf_click_btn[1] = land_floor;
                buf_click_btn[0] = 'l';
                kill(pid[1], SIGUSR1);
                write(fds[4][1], buf_click_btn, 2);
            }
        }
        if (poll(input_el_1, 1, 0)) {
            read(fds[0][0], state[0], 2);
            set_val_in_pos(state[0], 0, 3); 
        }
        if (poll(input_el_2, 1, 0)) {
            read(fds[1][0], state[1], 2);
            set_val_in_pos(state[1], 0, 4);
        }
    }

    return 0;
}
