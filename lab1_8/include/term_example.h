#ifndef TERM_EXAMPLE_
#define TERM_EXAMPLE_

#define ESC "\033"

#define home()          write(1, "27[H", 3);
#define clrscr()        write(1, "27[2J", 4);
#define gotoxy(x, y, val)     printf("27[%d;%dH%c", y, x, val);
#define visible_cursor() printf(ESC "[?251");

#endif // TERM_EXAMPLE_
