/* Client code in C */
/*clang++ -std=c++11 emisor.cpp -o emisor */

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
 #include <vector>
 #define TYPE_TEXT "0"
 #define TYPE_IMG "1"

 using namespace std;
 int SocketFD;
 int n;
 char buffer[10000];
 string txt;
 vector<string> chunks;
 int CHUNK_SIZE = 100;


string normalizeNumber(const int&number){
   string temp = to_string(number);
   string answer = "";

   while(temp.size() + answer.size() < 4){
     answer+="0";
   }
   answer+=temp;
   return answer;
}

string getHash(int checksum){

  string temp = "";

  while(checksum > 0){
      if(checksum & 1){
        temp+="1";
      }else{
        temp+="0";
      }
  }
  string answer;

  while(answer.size() + temp.size() < 16){
    answer+="0";
  }

  for(int i = temp.size()-1; i>=0; i--){
    answer+=temp[i];
  }
  return answer;

}


void sendShortText(const string &name){

  int checksum = 0;
  chunks.clear();
  string temp;
  ifstream myfile (name.c_str());
  char c;
  while(myfile.get(c)){
      checksum+=int(c);
      temp+=c;
      if(temp.size() == CHUNK_SIZE){
        chunks.push_back(temp);
        temp= "";
      }
  }

  if(temp.size()!=0){
    chunks.push_back(temp);
  }

  checksum = -1 * checksum;


  txt= normalizeNumber(chunks.size());
  write(SocketFD,txt.c_str(), txt.length());
  txt= normalizeNumber(chunks[chunks.size()-1].size());
  write(SocketFD,txt.c_str(), txt.length());
  txt = getHash(checksum);
  write(SocketFD,txt.c_str(), txt.length());
}

void sendLongText(){
  sendShortText("long_text.txt");
}

void sendImage(){

  system("python matrix.py");
  sendShortText("img.csv");

}
 void sendMessage(){

   int choice;

   while(true){
      choice = rand()%2;
      switch (choice) {
        choice = 0;
        case 0:
          cout << "sending short text" << endl;
          // send short text
          txt= TYPE_TEXT;
          write(SocketFD,txt.c_str(), txt.length());
          sendShortText("short_text.txt");
          break;
        case 1:
          cout << "sending long text" << endl;
          //send long text
          txt= TYPE_TEXT;
          write(SocketFD,txt.c_str(), txt.length());
          sendLongText();
          break;
        default:
          cout << "sending img" << endl;
          txt = TYPE_IMG;
          write(SocketFD,txt.c_str(), txt.length());
          //send img
          sendImage();

          break;
      }
      sleep(2000); // sleep 2 seconds
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

   sendMessage();

   /* perform read write operations ... */
   shutdown(SocketFD, SHUT_RDWR);

   close(SocketFD);
   return 0;
 }
