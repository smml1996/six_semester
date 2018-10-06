#include <iostream>
#include <string>
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <ncurses.h>
#include "Client.h"

using namespace std;

Client::Client(const string &nickname, const int &limitX, const int &limitY){
  this->nickname = nickname;
  this->limitX = limitX;
  this->limitY = limitY;
  lost = false;
  srand (time(NULL));
  posX = rand() % 30+5;

  posY = rand() % 30+5;
  refresh();
  mvaddch(posY, posX, '#');
}

void Client::notifyLost(){
  mvaddch(posY, posX, ' ');
  lost = true;
}
void Client::drawSymbol(){
  mvaddch(posY, posX, '#');
}

void Client::move(const int &direction){
  mvaddch(posY, posX, ' ');


  if(direction == 0){
    //up
    posY--;
    if(posY <=0){
      posY = limitY;
    }
  }else if(direction == 1){
    //DOWN
    posY++;
    if(posY >= limitY){
      posY= 0;
    }
  }else if(direction == 2){
    //LEFT
    posX--;
    if(posX <=0){
      posX = limitX;
    }

  }else if(direction == 3){
    //RIGHT
    posX++;
    if(posX >= limitX){
      posX = 0;
    }
  }

  mvaddch(posY, posX, '#');

}
