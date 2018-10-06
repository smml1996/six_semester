//
// Created by Stefanie Muroya lei on 9/29/18.
//

#ifndef JUEGOSNAKE_SNAKE_H
#define JUEGOSNAKE_SNAKE_H

#include <iostream>
#include <vector>
#include <ncurses.h>

using namespace std;

#define UP 0;
#define DOWN 1;
#define LEFT 2;
#define RIGHT 3;
#define SNAKE_SIZE 15

class Snake {
public:
    vector< pair<int, int> > positions;
    void moveSnake(WINDOW * win, int direction);
    bool isSnakeEaten(pair<int,int> pos);
    bool doesSnakeEat(pair<int, int> pos);

};


#endif //JUEGOSNAKE_SNAKE_H
