#include <iostream>
#include <string>


#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3


class Client{

public:
  int posX, posY;
  bool lost;
  int limitX, limitY;
  std::string nickname;
  Client(const std::string &nickname, const int &limitX, const int &limitY);
  void move(const int &direction);
  void drawSymbol();
  void notifyLost();
};
