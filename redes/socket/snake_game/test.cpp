#include <iostream>
#include <stdio.h>
#include <ncurses.h>
#include "Snake.h"

using namespace std;

#define TICKRATE 100
#define WORLD_WIDTH 40
#define WORLD_HEIGHT 40

int main(int argc, char *argv[]){
  WINDOW *win;
  int offsetx, offsety, i, ch;
  initscr();
  noecho();
  cbreak();
  timeout(TICKRATE);
  keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/

  //printw("Snake Game");
  mvprintw(5,45,"%s","Snake Game");
  refresh();

  /*
  Client c("Stefanie", WORLD_WIDTH, WORLD_HEIGHT);

  while((ch = getch()) != 'x'){
    if(ch != ERR) {
        switch(ch) {
            case KEY_UP:
                c.move(UP);
                break;
            case KEY_DOWN:
                c.move(DOWN);
                break;
            case KEY_RIGHT:
                c.move(RIGHT);
                break;
            case KEY_LEFT:
                c.move(LEFT);
                break;
            default:
                break;
        }

    }
  }*/

  Snake s(WORLD_WIDTH, WORLD_HEIGHT);
  //s.move(WORLD_WIDTH, WORLD_HEIGHT);
  while((ch = getch()) != 'x'){

    s.move();

  }

		/* Wait for user input */
	endwin();			/* End curses mode		  */


  return 0;
}
