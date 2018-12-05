#include <iostream>

using namespace std;

// lynx -dump -listonly http://aligajani.com > file.txt
// system("command");
/* Client code in C */
/*clang++ -std=c++11 slave.cpp util.cpp -o slave */

 #include <sys/types.h>
 #include <sys/socket.h>
 #include <netinet/in.h>
 #include <arpa/inet.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <string>
 #include <set>
 #include <vector>
 #include <unistd.h>
 #include <iostream>
 #include <thread>
 #include <fstream>
 using namespace std;

 std::string normalizeNumberString(const std::string &number){

   if(number.size() == 3)
     return number;

   if(number.size() > 3)
     return "-1";

   std::string answer = "";

   while(answer.size() + number.size() < 3){
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

int SocketFD;
string txt;
vector<string> links;
set<string> newLinks;
char buffer[10000];

void writeOnServer(){
  // set txt before calling this method
  if(normalizeNumber(txt.size()) == "-1")return;
  write(SocketFD, normalizeNumber(txt.size()).c_str(), 3); // sending message size
  write(SocketFD, txt.c_str(), txt.size()); // sending message
}

void getLinks(){
  txt = "G";
  write(SocketFD, txt.c_str(), 1); // asking master for links

  cleanBuffer(buffer);

  read(SocketFD, buffer, 3);

  int numberLinks = stoi(buffer);

  links.clear();

  int linkLength;
  string link;
  for(int i =0; i < numberLinks; i++){
    cleanBuffer(buffer);
    read(SocketFD, buffer, 3);
    linkLength = stoi(buffer);
    cleanBuffer(buffer);
    read(SocketFD, buffer, linkLength);
    link = buffer;
    cout << "new link: " << link << endl;
    links.push_back(link);
  }
}

void processFile(){
  cout << "process file" << endl;
  ifstream file("file.txt");
  string str;
  while (getline(file, str))
  {
    if(str.size() == 0)continue;
    if(str == "References") break;
    str = str.substr(str.find("h"));
    newLinks.insert(str);
  }
}

string writeLinkType(const string & l){
  if(l.find("img") > -1 )
    return "I";
  if(l.find("xml") > -1)
    return "X";
  if(l.find("js") > -1)
    return "J";
  if(l.find("css") > -1)
    return "C";
  return "H";
}

void updateLinks(){
  txt = "U";
  write(SocketFD, txt.c_str(), 1); // tell master to update links
  txt = normalizeNumber(newLinks.size());
  write(SocketFD, txt.c_str(), 3);

  for(set<string>::iterator it=newLinks.begin(); it!=newLinks.end(); it++){
    txt = writeLinkType(*it);

    write(SocketFD, txt.c_str(), 1);
    txt = *it;
    cout << "update link: " << txt << endl;
    writeOnServer();
  }

}


void beginCrawling(){
    getLinks();

    newLinks.clear();
    string command;
    for(int i =0; i < links.size(); i++){

      command = "lynx -dump -listonly " + links[i] +  " | sort | uniq > file.txt";
      system(command.c_str());
      processFile();
    }

    updateLinks();

}

 int main(void){

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
   Res = inet_pton(AF_INET, "192.168.1.4", &stSockAddr.sin_addr);

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

   while(true)
    beginCrawling();

   /* perform read write operations ... */
   //sendThread.join();
   //receiveThread.join();
   shutdown(SocketFD, SHUT_RDWR);

   close(SocketFD);
   return 0;
 }
