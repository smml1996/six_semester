#include "Snake.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <ncurses.h>

using namespace std;

#define WORLD_WIDTH 40
#define WORLD_HEIGHT 40

Snake::Snake(int limitX, int limitY){
  this->limitX = limitX;
  this->limitY = limitY;

  srand (time(NULL));
  snakeDirection = 3;

  int offsetx = (COLS - WORLD_WIDTH) / 2;
  int offsety = (LINES - WORLD_HEIGHT) / 2;

  //win = newwin(WORLD_HEIGHT, WORLD_WIDTH,offsety,offsetx);
  int sbegx = (limitX - SNAKE_SIZE) / 2;
  int sbegy = (limitY - 1) / 2;
  for(int i =0; i < SNAKE_SIZE; i++){

    body.push_back(make_pair(sbegx + i,sbegy));
  }


  refresh();

  for(int i =0; i < body.size(); i++){
    //mvwaddch(win, body[i].second, body[i].first, '#');

    //move(body[i].second,body[i].first);
    mvaddch(body[i].second,body[i].first,'*');
    //mvprintw(body[i].second,body[i].first,"%s","#");
  }


}


void Snake::doesSnakeEat(){
  int tempx = body[body.size()-1].first;
  int tempy = body[body.size()-1].second;
  for(int i =0; i <  clientes.size(); ){
      if(tempx == clientes[i].posX && clientes[i].posY == tempy){
        clientes[i].notifyLost();
        //clientes.erase(clientes.begin()+i);
      }else{
        i++;
      }
  }

}

bool Snake::isGameOver(){

  int countC = 0;

  for(int i =0 ; i < clientes.size(); i++){
    if(clientes[i].lost) countC++;
  }

  if(countC == clientes.size()){
    message = "computer wins";
    return true;
  }
  if(body.size() < 3){
    message = "computer lose";
    return true;
  }
  return false;
}

bool Snake::isSnakeEaten(pair<int, int> pos){
  for(int i =0; i < body.size(); i++){
    if(body[i].first == pos.first && body[i].second == pos.second){
      body.erase(body.begin() + i);
      return true;
    }
  }
  return false;
}

bool Snake::isInsideLimits(const int &x,const int &y){
  if(x >=0 &&x <= limitX){
    if(y>=0 && y <= limitY)return true;
  }
  return false;
}

bool Snake::canMove(int direction){
  int tempx = body[body.size()-1].first;
  int tempy = body[body.size()-1].second;

  if(direction == 3 ){
    if(snakeDirection == 2) return false;
    if(isInsideLimits(tempx+ 1, tempy))
      return true;
    return false;
  }

  if(direction == 2){
    if(snakeDirection == 3)return false;
    if(isInsideLimits(tempx-1, tempy))
      return true;
    return false;
  }

  if(direction == 0){
    if(snakeDirection == 1) return false;
    if(isInsideLimits(tempx, tempy + 1))return true;
    return false;
  }

  if(direction == 1){
    if(snakeDirection == 0) return false;
    if(isInsideLimits(tempx, tempy-1)) return true;
    return false;
  }
  return false;

}

void Snake::drawSnake(){
  erase();
  mvprintw(5,45,"%s","Snake Game");

  for(int i =0; i < clientes.size(); i++){
      mvprintw(6+1+i,45,"%s", clientes[i].nickname.c_str());
      clientes[i].drawSymbol();
  }

  for(int i =0; i < body.size(); i++){

    //cout << body[i].second << " "<<body[i].first << endl;
      mvaddch(body[i].second,body[i].first,'*');
  //  mvprintw(body[i].second,body[i].first,"%s","#");

    //mvwaddch(win, body[i].second, body[i].first, '#');
  }
  refresh();
}
void Snake::move(){

  int direction = rand()%2;

  if(snakeDirection ==3 || snakeDirection == 2){
     if(direction){
       snakeDirection = 0;
     }else{
       snakeDirection = 1;
     }
  }else if(snakeDirection==0 || snakeDirection == 1){
    if(direction){
      snakeDirection = 3;
    }else{
      snakeDirection = 2;
    }
  }
  //snakeDirection = 3;

  int plusX = 0, plusY =0;

  if(snakeDirection == 3) plusX++;
  else if(snakeDirection == 2) plusX--;
  else if(snakeDirection == 1) plusY++;
  else if(snakeDirection == 0)plusY--;

  for(int i =0; i <body.size()-1; i++){
      body[i].first = body[i+1].first;
      body[i].second = body[i+1].second;
  }
  body[body.size()-1].first+=plusX;
  body[body.size()-1].second+=plusY;

    if(snakeDirection == 2){
      if(body[body.size()-1].first < 0){
        body[body.size()-1].first = limitX;
      }
    }

    if(snakeDirection == 3){
      if(body[body.size()-1].first > limitX){
        body[body.size()-1].first = 0;
      }
    }

    if(snakeDirection == 0){
      if(body[body.size()-1].second <=0){
        body[body.size()-1].second = limitY;
      }

    }

    if(snakeDirection == 1){
      if(body[body.size()-1].second >= limitY){
        body[body.size()-1].second = 0;
      }
    }

    erase();
    //refresh();
    mvprintw(5,45,"%s","Snake Game");

    for(int i =0; i < clientes.size(); i++){
      if(clientes[i].lost)continue;
        mvprintw(6+1+i,45,"%s", clientes[i].nickname.c_str());
        clientes[i].drawSymbol();
    }

    for(int i =0; i < body.size(); i++){

      //cout << body[i].second << " "<<body[i].first << endl;
        mvaddch(body[i].second,body[i].first,'*');
    //  mvprintw(body[i].second,body[i].first,"%s","#");

      //mvwaddch(win, body[i].second, body[i].first, '#');
    }
    refresh();





}
