/* Server code in C */
/*
clang++ -std=c++11 server2.cpp -o server
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

int n;
int ConnectFD;
bool chatActive = true;
std::map< std::string, int > clientes;

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

void sendAck(int type, int socket){
  std::string temp = "A";
  write(socket, temp.c_str(), 1);
  sendType(type,socket);
}

void sendError(int type, int socket){
  std::string temp = "E";
  write(socket, temp.c_str(), 1);
  sendType(type,socket);
}

void login(int socket){
  char buffer[10000];
  cleanBuffer(buffer);
  // read nickname length
  read(socket, buffer,2);
  buffer[2]= '\0';
  int size_nickname = std::stoi(buffer);
  read(socket, buffer, size_nickname);
  buffer[size_nickname] = '\0';
  if(clientes.find(buffer) == clientes.end()){
    clientes[buffer] = socket;
    sendAck(1, socket);
    return ;
  }
  sendError(1, socket);

}

void sendList(int socket){
  char buffer[10000];
  cleanBuffer(buffer);
  std::map<std::string, int>::iterator it;
  std::string type = "i";
  write(socket, type.c_str(), 1);
  std::string sizeMap = normalizeNumber(clientes.size());
  write(socket, sizeMap.c_str(), 2);
  for(it = clientes.begin(); it!=clientes.end(); it++){
    sizeMap = normalizeNumber((*it).first.size());
    write(socket, sizeMap.c_str(), 2);
    std::string temp = ((*it).first);
    write(socket, temp.c_str(),(*it).first.size());
  }
}

std::map<std::string, int>::iterator getClienteBySocket(int s){
  std::map<std::string, int>::iterator it;
  for (it=clientes.begin(); it!=clientes.end(); ++it)
   if(it->second == s){
      return it;
   }
   return it;

}

void logout(int socket){
  char buffer[10000];
  cleanBuffer(buffer);
  std::map<std::string, int>::iterator it = getClienteBySocket(socket);
   if(it != clientes.end()){
      clientes.erase(it);
      sendAck(3, socket);
      return;
   }
   sendError(3, socket);
}


void transmitirMensaje(int fromSocket){

  // recibir tamaño mensaje
  char buffer[10000];
  cleanBuffer(buffer);
  read(fromSocket, buffer, 2);
  int sizeMessage = std::stoi(buffer);

  cleanBuffer(buffer);
  read(fromSocket, buffer, sizeMessage);
  std::string message = buffer;
  cleanBuffer(buffer);
  read(fromSocket, buffer, 2);
  int sizeTo = std::stoi(buffer);
  cleanBuffer(buffer);
  read(fromSocket, buffer, sizeTo);

  std::string to = buffer;
  int toSocket = clientes[buffer];

  // writing message
  std::string type = "W";
  write(toSocket, type.c_str(), 1);
  write(toSocket, normalizeNumber(sizeMessage).c_str(), 2);
  write(toSocket, message.c_str(), message.size());
  std::map< std::string, int>::iterator it = getClienteBySocket(fromSocket);

  if(it == clientes.end()){
    sendError(2, fromSocket);
    return;
  }
  write(toSocket, normalizeNumber((it->first).size()).c_str(), 2);
  write(toSocket, (it->first).c_str(), (it->first).size());
  sendAck(2,fromSocket);
}



void broadcast(int socket){
  char buffer[10000];
  cleanBuffer(buffer);
  read(socket, buffer, 2);

  int sizeMessage = std::stoi(buffer);

  std::string message;
  cleanBuffer(buffer);
  read(socket, buffer, sizeMessage);
  message = buffer;
  std::map<std::string, int>::iterator it;
  std::string type = "W";
  std::string lengthMS = normalizeNumber(sizeMessage);
  std::string from = getClienteBySocket(socket)->first;
  std::string sizefrom= normalizeNumber(from.size());
  for (it=clientes.begin(); it!=clientes.end(); ++it){
    if(it->second ==  socket)continue;
    write(it->second, type.c_str(), 1);
    write(it->second, lengthMS.c_str(), 2);
    write(it->second, message.c_str(), message.size());
    write(it->second, sizefrom.c_str(), 2);
    write(it->second, from.c_str(), from.size());

  }
}

void sendFile(int socket){
  char buffer[10000];
  cleanBuffer(buffer);

  n = read(socket, buffer, 2);

  int len = std::stoi(buffer);
  cleanBuffer(buffer);

  n = read(socket, buffer, len);
  std::string to = buffer;
  cleanBuffer(buffer);

  n = read(socket, buffer, 2);
  len = std::stoi(buffer);
  cleanBuffer(buffer);

  n = read(socket, buffer, len);
  std::string fileName = buffer;
  cleanBuffer(buffer);


  n = read(socket, buffer, 2);
  len = std::stoi(buffer);
  cleanBuffer(buffer);

  n = read(socket, buffer, len);
  std::string file = buffer;


  int toSocket = clientes[to];
  std::string from = getClienteBySocket(socket)->first;

  // sending Message
  std::string type = "R";
  write(toSocket, type.c_str(), 1);

  write(toSocket, normalizeNumber(from.size()).c_str(), 2);
  write(toSocket, from.c_str(), from.size());

  write(toSocket, normalizeNumber(fileName.size()).c_str(), 2);
  write(toSocket, fileName.c_str(), fileName.size());

  write(toSocket, normalizeNumber(file.size()).c_str(), 2);
  write(toSocket, file.c_str(), file.size());

  sendAck(socket, 5);

}

void processChatClient(int socket){

  std::cout << "new chat stablished" << std::endl;
  char buffer[10000];
  cleanBuffer(buffer);
  for(;;){
    n = read(socket, buffer, 1);

    if(n == 0)continue;
    if(buffer[0]== 'L'){
        login(socket);
    }else if(buffer[0] == 'I'){
        sendList(socket);
    }else if(buffer[0] == 'O'){
        logout(socket);
    }else if(buffer[0] == 'M'){
        transmitirMensaje(socket);
    }else if(buffer[0] == 'B'){
      broadcast(socket);
    }else if(buffer[0] == 'F'){
      sendFile(socket);
    }
    cleanBuffer(buffer);
  }

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


  for(;;){
    ConnectFD = accept(SocketFD, NULL, NULL);
    std::thread (processChatClient, ConnectFD).detach();
  }

  //std::thread sendThread(sendMessage);
  //std::thread receiveThread(receiveMessage);


  //sendThread.join();
  //std::cout << "sendThread joines" << std::endl;
  //receiveThread.join();
  //std::cout << "reveiberThread joines" << std::endl;



   /* perform read write operations ... */

  shutdown(ConnectFD, SHUT_RDWR);
  close(ConnectFD);




  close(SocketFD);
  return 0;
}
