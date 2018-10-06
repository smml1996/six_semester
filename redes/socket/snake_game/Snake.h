#include <iostream>
#include <vector>
#include <ncurses.h>
#include "Client.h"


#define SNAKE_SIZE 20
class Snake{
  int snakeDirection;

  bool canMove(int direction);
  int limitX, limitY;
  bool isInsideLimits(const int &x,const int &y);
public:
  Snake(int limitX,int limitY);
  std::string message;
  std::vector< std::pair<int, int> > body;
  std::vector< Client > clientes;
  bool isGameOver();
  void move();
  void doesSnakeEat();
  void drawSnake();
  bool isSnakeEaten(std::pair<int, int> pos);
};
