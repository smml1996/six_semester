/* Client code in C */
/*clang++ -lncurses -std=c++11 snakeClient.cpp Client.cpp Snake.cpp -o cliente */

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
 #include <fstream>
 #include <ncurses.h>

 #include "Snake.h"

 using namespace std;

 #define TICKRATE 100
 #define WORLD_WIDTH 40
 #define WORLD_HEIGHT 40

 int n , listSize, SocketFD = 0;
 char buffer[10000];
 bool chatActive = true, hasNickname = false;
 string message;
 bool isGameOver = false;
 bool tempBool;
 char ch;
 int indexC;
 Snake s(WORLD_WIDTH, WORLD_HEIGHT);
 Client c("nickname", WORLD_WIDTH, WORLD_HEIGHT);


 string txt, nickname, to, mensaje;
string normalizeNumber(int number){
  if(number > 99) tempBool = false;
  string answer = to_string(number);
  answer = number < 10 ? "0" + answer : answer;
  tempBool = true;
  return answer;
}

void writeOnServer(const string &s){
  string sizeS = normalizeNumber(s.size());
  if(!tempBool){
    cout << "too big to be sent to server (" << sizeS <<"), rolling back" << endl;
    return ;
  }
  write(SocketFD, sizeS.c_str(),2);
  write(SocketFD, s.c_str(), s.size());
}

string readFromServer(){
  bzero(buffer,10000);

  n = read(SocketFD, buffer, 2);
  txt = buffer;
  int lengthMessage = stoi(txt);
  bzero(buffer,10000);
  // recibir mensaje
  n = read(SocketFD, buffer, lengthMessage);
  string ans = buffer;
  return ans;
}

void requestUpdate(){
  string temp;
  temp = "U";
  write(SocketFD, temp.c_str(),1);
  bzero(buffer,10000);

  int lengthSnake, numUsers;
  n = read(SocketFD, buffer, 2);
  temp = buffer;
  bzero(buffer, 10000);
  lengthSnake = stoi(temp);
  int x, y;
  s.body.clear();

  for(int i =0; i < lengthSnake; i++){
    n = read(SocketFD, buffer, 2);
    temp = buffer;
    bzero(buffer, 10000);
    x = stoi(temp);

    n = read(SocketFD, buffer, 2);
    temp = buffer;
    bzero(buffer, 10000);
    y= stoi(temp);


    s.body.push_back(make_pair(x,y));
  }

  n = read(SocketFD, buffer, 2);
  temp = buffer;

  bzero(buffer, 10000);

  numUsers = stoi(temp);
  s.clientes.clear();

  string tempNickname;
  int lengthNick;
  for(int i =0; i < numUsers; i++){
    n = read(SocketFD, buffer, 2);
    temp = buffer;
    bzero(buffer, 10000);
    lengthNick = stoi(temp);

    read(SocketFD, buffer, lengthNick);
    tempNickname = buffer;
    bzero(buffer, 10000);
    n = read(SocketFD, buffer, 2);
    temp = buffer;


    x = stoi(temp);
    bzero(buffer, 10000);

    n = read(SocketFD, buffer, 2);
    temp = buffer;
    bzero(buffer, 10000);
    y= stoi(temp);

    bzero(buffer, 10000);
    Client tempc (tempNickname, WORLD_WIDTH, WORLD_HEIGHT);
    s.clientes.push_back(tempc);
    s.clientes[i].posX = x;
    s.clientes[i].posY = y;
  }
  s.drawSnake();


}

void requestIndex(){
  string temp;
  temp = "I";
  write(SocketFD, temp.c_str(),1);
  read(SocketFD, buffer, 2);
  temp = buffer;
  indexC = stoi(temp);
}

void requestAliveness(){

  // 0: dead
  // 1: alive
  // 2: snake dead
  string temp;
  temp = "A";
  write(SocketFD, temp.c_str(), 1);
  read(SocketFD, buffer, 2);
  temp = buffer;
  int isAlive = stoi(temp);
  if(isAlive == 0){
    message = "you were eaten";
    isGameOver= true;
  }else if(isAlive == 2){
    message = "you and your team won";
    isGameOver = true;
  }
}

 int main(void){

   cout << "nickname: ";
   cin >> nickname;
   c.nickname = nickname;
   initscr();
   noecho();
   cbreak();
   timeout(TICKRATE);
   keypad(stdscr, TRUE);

   struct sockaddr_in stSockAddr;
   int Res;
   SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);


   if (-1 == SocketFD){
     perror("cannot create socket");
     exit(EXIT_FAILURE);
   }

   memset(&stSockAddr, 0, sizeof(struct sockaddr_in));

   stSockAddr.sin_family = AF_INET;
   stSockAddr.sin_port = htons(5001);
   Res = inet_pton(AF_INET, "192.168.165.166", &stSockAddr.sin_addr);

   if (0 > Res){
     perror("error: first parameter is not a valid address family");
     close(SocketFD);
     exit(EXIT_FAILURE);
   }
   else if (0 == Res){
     perror("char string (second parameter does not contain valid ipaddress");
     close(SocketFD);
     exit(EXIT_FAILURE);
   }

   if (-1 == connect(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in))){
     perror("connect failed");
     close(SocketFD);
     exit(EXIT_FAILURE);
   }
   //thread receiveThread(receiveMessage);
   //thread sendThread(sendMessage);

   /* perform read write operations ... */
   writeOnServer(nickname);
   requestIndex();
	/* We get F1, F2 etc..		*/

   //printw("Snake Game");
   clear();
   mvprintw(5,45,"%s","Snake Game");
   refresh();
   s.clientes.push_back(c);

   string mess;
   string temp;
   bool isMove = false;
   while(  (ch = getch()) != 'x'){
     if(isGameOver) break;
     isMove = true;
     if(int(ch) == 3){
       //up
        c.move(UP);

     }else if(int(ch) == 2){
       //down
      c.move(DOWN);
     }else if(int(ch) == 5){
       //right
       c.move(RIGHT);
     }else if(int(ch) == 4){
       //left
        c.move(LEFT);
     }else{
       isMove = false;
     }

     if(isMove){
       mess = "M";
       write(SocketFD, mess.c_str(),1);

       temp = (normalizeNumber(c.posX) + normalizeNumber(c.posY) );
       write(SocketFD, temp.c_str(),4);
     }

     requestUpdate();
     requestAliveness();

   }
   clear();
   cout << message << endl;
   mvprintw(0,0,"%s",message.c_str());
   cin >> ch;

   shutdown(SocketFD, SHUT_RDWR);

   close(SocketFD);
   return 0;
 }
