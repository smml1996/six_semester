
/* Server code in C */
/*
clang++ -std=c++11 master.cpp -o master
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
#include <mutex>

std::string normalizeNumberString(const std::string &number){

  if(number.size() == 4)
    return number;

  if(number.size() > 4)
    return "-1";

  std::string answer = "";

  while(answer.size() + number.size() < 4){
    answer+="0";
  }

  answer+= number;

  return number;
}

std::string normalizeNumber(const int &number){
    return normalizeNumberString(std::to_string(number));
}

void cleanBuffer(char buffer[]){
  bzero(buffer,10000);
}
int n;
int ConnectFD;

unsigned long long skipFrom = 1500;
std::mutex mtx;
int id = 14;


void sendLinks(const int &socket){
  std::vector<std::string> links;
  mtx.lock();
  std::string commando = "curl -H \'Content-Type: application/json\' -XGET \'http://localhost:9200/crawler/links/_search\' -d\'{\"from\": "+ std::to_string(skipFrom) +", \"size\" : 10}}\'> results.txt";
  system(commando.c_str());
  skipFrom+=10;
  mtx.unlock();

  commando = "python getJson.py " + std::to_string(socket);
  system(commando.c_str());

  std::ifstream file("links"+std::to_string(socket)+".txt");
  std::string str;

  while (getline(file, str)){
    links.push_back(str);
  }
  write(socket, normalizeNumber(links.size()).c_str(), 4);

  for(int i =0; i < links.size(); i++){


    write(socket, normalizeNumber(links[i].size()).c_str(), 4);

    write(socket, links[i].c_str(), links[i].size());
  }
}

void receiveLinks(const int &socket){
  char buffer[10000];
  cleanBuffer(buffer);
  read(socket, buffer,4);
  int numLinks;
  try{
    numLinks = std::stoi(buffer);
  } catch(int e){
    return;
  }

  int linkLength;
  std::string type;
  std::string link;
  std::string command;
  for(int i =0; i < numLinks; i++){
    cleanBuffer(buffer);
    read(socket, buffer,1);
    type = buffer;
    cleanBuffer(buffer);
    read(socket, buffer,4);
    try{
      linkLength = std::stoi(buffer);
    }catch(int e){
      continue;
    }
    cleanBuffer(buffer);
    read(socket, buffer, linkLength);
    link = buffer;
    mtx.lock();

    command = "curl -H \'Content-Type:application/json\' -XPUT \'http://localhost:9200/crawler/links/"+std::to_string(id)+"\' -d \' {\"link\" : \""+link +"\", \"type\": \""+type+"\"}\'";
    std::cout << command << std::endl;
    system(command.c_str());
    id++;
    mtx.unlock();
  }



}

void processChatClient(int socket){


  char buffer[10000];

  for(;;){
    cleanBuffer(buffer);
    n = read(socket, buffer, 1);

    if(n == 0)continue;
    if(buffer[0]== 'G'){
        // ask master for links
        sendLinks(socket);
    }else if(buffer[0] == 'U'){
        //update links in database
        receiveLinks(socket);
    }
    //cleanBuffer(buffer);
  }

}


int main(void)
{

  //system("curl -XGET \'http://localhost:9200/megacorp/employee/_search?q=message:elasticsearch&size=0&terminate_after=1\' > file.txt");


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

  shutdown(ConnectFD, SHUT_RDWR);
  close(ConnectFD);




  close(SocketFD);

  return 0;
}
