/* Server code in C */

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

char buffer[10000];
int n;
int ConnectFD;
bool chatActive = true;

std::string getFinalMessage(const std::string &s){
  if(s.size() > 9999){
    return "0";
  }
  std::string temp = std::to_string(s.size());

  std::string answer ="";
  while(answer.size() + temp.size() < 4){
    answer+="0";
  }
  answer+=temp;
  answer+=s;
  return answer;
}

void receiveMessage(){
  std::string endS = "END";
  std::string msg;
  while(chatActive){

    bzero(buffer,10000);
    n = read(ConnectFD, buffer, 4);
    int l = atoi(buffer);
    bzero(buffer,10000);
    read(ConnectFD, buffer, l);
    msg = buffer;
    if(msg == endS.c_str()) chatActive = false;
    if(n>=0)
      std::cout << "Client: " << buffer << std::endl;
  }
}

void sendMessage(){
  std::string txt;
  std::string endS = "END";
  std::string tempbuffer;
  while(chatActive){
    tempbuffer = buffer;
    if(chatActive)std::getline(std::cin, txt);
    else break;
    txt = getFinalMessage(txt);
    n = write(ConnectFD,txt.c_str(), txt.length());
  }
}

int main(void)
{
  struct sockaddr_in stSockAddr;
  int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);


  memset(&stSockAddr, 0, sizeof(struct sockaddr_in));

  stSockAddr.sin_family = AF_INET;
  stSockAddr.sin_port = htons(5003);
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



  ConnectFD = accept(SocketFD, NULL, NULL);
std::thread sendThread(sendMessage);
  std::thread receiveThread(receiveMessage);
  sendThread.join();
  std::cout << "sendThread joines" << std::endl;
  receiveThread.join();
  std::cout << "reveiberThread joines" << std::endl;



   /* perform read write operations ... */

  shutdown(ConnectFD, SHUT_RDWR);
  close(ConnectFD);




  close(SocketFD);
  return 0;
}
