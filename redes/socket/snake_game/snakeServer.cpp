/* Server code in C */
/*
clang++ -lncurses -std=c++11 snakeServer.cpp  Client.cpp Snake.cpp -o server
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <vector>
#include <map>
#include <fstream>
#include <ncurses.h>
#include "Snake.h"


#define TICKRATE 100
#define WORLD_WIDTH 40
#define WORLD_HEIGHT 40

int n;
char ch;
int ConnectFD;
bool chatActive = true;
std::map< std::string, int > clientes;
Snake s(WORLD_WIDTH, WORLD_HEIGHT);
bool gameOver = false;

std::string txt;

std::string normalizeNumber(int number){
  std::string answer = std::to_string(number);
  answer = number < 10 ? "0" + answer : answer;
  return answer;
}
void cleanBuffer(char buffer[]){
    bzero(buffer,10000);
}

void sendType(int type, int socket){
  std::string typeS = "00";
  switch (type) {
    case 1:
      typeS = "10";
      break;
    case 2:
      typeS = "20";
      break;
    case 3:
      typeS="30";
      break;
    case 4:
      typeS = "40";
      break;
    case 5:
      typeS = "50";
      break;

  }
  write(socket, typeS.c_str(), 2);
}

void processChatClient(int socket){

  //std::cout << "new chat stablished" << std::endl;
  char buffer[10000];
  cleanBuffer(buffer);
  read(socket, buffer,2);
  int size_nickname = std::stoi(buffer);
  cleanBuffer(buffer);
  read(socket, buffer, size_nickname);
  std::string nickname = buffer;
  Client * c = new Client(nickname, WORLD_WIDTH, WORLD_HEIGHT);
  s.clientes.push_back(*c);
  int index = s.clientes.size()-1;
  int x,y;
  while(!gameOver){
    n = read(socket, buffer, 1);

    if(n == 0)continue;
    if(buffer[0] == 'M'){
        read(socket, buffer,2);
        x = std::stoi(buffer);
        cleanBuffer(buffer);
        read(socket, buffer,2);
        y = std::stoi(buffer);
        s.clientes[index].posX = x;
        s.clientes[index].posY = y;
    }else if(buffer[0] == 'U'){
      write(socket, normalizeNumber(s.body.size()).c_str(), 2);

      for(int i =0; i < s.body.size(); i++){
        write(socket, normalizeNumber(s.body[i].first).c_str(), 2);
        write(socket, normalizeNumber(s.body[i].second).c_str(), 2);
      }

      write(socket, normalizeNumber(s.clientes.size()).c_str(), 2);
      for(int i =0; i < s.clientes.size(); i++){
        write(socket, normalizeNumber(s.clientes[i].nickname.size()).c_str(), 2);
        write(socket, s.clientes[i].nickname.c_str(), s.clientes[i].nickname.size());

        write(socket, normalizeNumber(s.clientes[i].posX).c_str(), 2);
        write(socket, normalizeNumber(s.clientes[i].posY).c_str(), 2);
      }

    }else if(buffer[0] == 'I'){
      write(socket, normalizeNumber(index).c_str(), 2);
    }else if(buffer[0] == 'A'){
      if(s.isGameOver()){
        if(s.message == "computer wins"){
          write(socket, normalizeNumber(0).c_str(), 2);

        }else{
          write(socket, normalizeNumber(2).c_str(), 2);

        }
      }
      else if(s.clientes[index].lost){
        write(socket, normalizeNumber(0).c_str(), 2);
      }else{
        write(socket, normalizeNumber(1).c_str(), 2);

      }

    }

    for(int i =0; i < s.clientes.size(); i++){
      if(!s.clientes[i].lost){
        s.isSnakeEaten(std::make_pair(s.clientes[i].posX, s.clientes[i].posY));
      }
    }
    if(s.isGameOver()){
      gameOver = true;
      break;
    }


    s.doesSnakeEat();

    cleanBuffer(buffer);
  }

}

void moveSnake(){
  while(!gameOver && (ch = getch()) != 'x')
    s.move();
}


int main(void)
{
  struct sockaddr_in stSockAddr;
  int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);


  memset(&stSockAddr, 0, sizeof(struct sockaddr_in));

  stSockAddr.sin_family = AF_INET;
  stSockAddr.sin_port = htons(5001);
  stSockAddr.sin_addr.s_addr = INADDR_ANY;
  if(-1 == bind(SocketFD,(const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
  {
    perror("error bind failed");
    close(SocketFD);
    exit(EXIT_FAILURE);
  }

  if(-1 == listen(SocketFD, 10))
  {
    perror("error listen failed");
    close(SocketFD);
    exit(EXIT_FAILURE);
  }

  initscr();
  noecho();
  cbreak();
  timeout(TICKRATE);
  keypad(stdscr, TRUE);
  mvprintw(5,45,"%s","Snake Game");
  refresh();


  std::thread (moveSnake).detach();
  for(;!gameOver;){

    ConnectFD = accept(SocketFD, NULL, NULL);
    std::thread (processChatClient, ConnectFD).detach();
  }

  erase();


  std:: cin >> ch;
  shutdown(ConnectFD, SHUT_RDWR);
  close(ConnectFD);


  close(SocketFD);
  return 0;
}
