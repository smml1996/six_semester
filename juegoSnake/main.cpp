#include <iostream>
#include <ncurses.h>

#define TICKRATE 100
#define WORLD_WIDTH 80
#define WORLD_HEIGHT 80
int main() {
    WINDOW *win;
    int offsetx, offsety, i, ch;
    initscr();
    noecho();
    cbreak();
    timeout(TICKRATE);
    keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/

    printw("Snake Game");
    refresh();

    //offsetx = (COLS - WORLD_WIDTH) / 2;
   // offsety = (LINES - WORLD_HEIGHT) / 2;

    win = newwin(WORLD_HEIGHT, WORLD_WIDTH,0,0);



    delwin(win);		/* Wait for user input */
    endwin();
    return 0;
}