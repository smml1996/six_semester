/* Client code in C */

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

 using namespace std;
 int SocketFD;
 int n;
 char buffer[10000];
 string txt;
 bool chatActive = true;

 string getFinalMessage(const string &s){
   if(s.size() > 9999){
     return "0";
   }
   string temp = std::to_string(s.size());

   string answer ="";
   while(answer.size() + temp.size() < 4){
     answer+="0";
   }
   answer+=temp;
   answer+=s;
   return answer;
 }

 void sendMessage(){
   while(chatActive){
     getline(std::cin, txt);
     if(txt == "END") chatActive=false;
     txt = getFinalMessage(txt);
     write(SocketFD,txt.c_str(), txt.length());
    }
 }

 void receiveMessage(){
   while(chatActive){
     bzero(buffer,10000);
      n = read(SocketFD, buffer, 4);
      int l = atoi(buffer);
      bzero(buffer,10000);
      read(SocketFD, buffer, l);
      if(n>=0)
        cout << "server: " << buffer << endl;
      }
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
   stSockAddr.sin_port = htons(5003 );
   Res = inet_pton(AF_INET, "192.168.1.7", &stSockAddr.sin_addr);

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
   thread receiveThread(receiveMessage);
   thread sendThread(sendMessage);

   /* perform read write operations ... */
   sendThread.join();
   receiveThread.join();
   shutdown(SocketFD, SHUT_RDWR);

   close(SocketFD);
   return 0;
 }
