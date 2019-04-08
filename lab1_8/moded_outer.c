#include "moded_outer.h"

void clrscr() {
    unsigned char esc[11];

    esc[0] = 27;  // ESC
    esc[1] = '[';  // делаешь [H и Саске возвращается в коноху
    esc[2] = 'H';
    write(1, esc, 3);

    esc[2] = '2';  // полное уничтожение всего живого и возвращение домой
    esc[3] = 'J';
    write(1, esc, 4);

    return;

}

int set_val_in_pos(char* val, int x, int y) {
    static unsigned char ystr[3] = "\0";
    static unsigned char xstr[3] = "\0";
    char* esc = malloc(16);
    int i, j, k;
    sprintf((char *) xstr, "%d", x);
    sprintf((char *) ystr, "%d", y);
    esc[0] = 27;
    esc[1] = '[';
    i = 2;
    j = 0;
    k = 0;
    while (ystr[j])
        esc[i++] = ystr[j++];
    esc[i++] = ';';
    j = 0;
    while (xstr[j])
        esc[i++] = xstr[j++];
    esc[i++] = 'H';
    while(val[k])
        esc[i++] = val[k++];
    esc[i] = '\0';
    write (OUT_STREAM, esc, strlen(esc));
    return 0;
}
