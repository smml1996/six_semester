/* Client code in C */
/*clang++ -std=c++11 cliente.cpp -o cliente */

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
 #include <stdlib.h>
 #include <time.h>
 #include <fstream>
 #include <chrono>
 #define TYPE_TEXT "0"
 #define TYPE_IMG "1"

 using namespace std;
 int SocketFD;
 int n;
 char buffer[10000];
 string txt;
 vector<string> chunks;
 int CHUNK_SIZE = 100;
 string MY_IP = "192.168.1.7";
 string nickname = "angie";



 void receiveMessage(){


   while(true){

      bzero(buffer,10000);
      n = read(SocketFD, buffer, 1);
      if(n>=0){
        //cout << "server: " << buffer << endl;
        int iCode = stoi(buffer);
        if(iCode == 0 or iCode == 1){
          txt= buffer;
          write(SocketFD,txt.c_str(), txt.length());
          n = read(SocketFD, buffer, 4); // reading number of chunks
          txt= buffer;
          write(SocketFD,txt.c_str(), txt.length());


        }

      }

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
   //std::cout << "sendThread joines" << std::endl;
   receiveThread.join();
   //std::cout << "reveiberThread joines" << std::endl;

    /* perform read write operations ... */

   shutdown(ConnectFD, SHUT_RDWR);
   close(ConnectFD);




   close(SocketFD);
   return 0;
 }
