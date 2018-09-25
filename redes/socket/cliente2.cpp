/* Client code in C */
/*clang++ -std=c++11 cliente2.cpp -o cliente */

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
 using namespace std;

 int n , listSize, SocketFD = 0;
 char buffer[10000];
 bool chatActive = true, hasNickname = false;
 bool tempBool;

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
    cout << "too big to be sent to server (" << sizeS<<"), rolling back" << endl;
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


void login(){
   cout << "Nickname: ";
   cin >> nickname;

   hasNickname = true;
   txt = 'L';
   write(SocketFD, txt.c_str(), 1);
   writeOnServer(nickname);
 }

 void sendMessageTo(){
  if(!hasNickname){
    cout << "login first" << endl;
    return ;
  }
  cout << "to (nickname): ";
  cin >> to;
  cout << "Message: ";
  mensaje = "";
  while(mensaje == "")
    getline (std::cin,mensaje);
  //cin >> mensaje;
  cout << endl;
  string type = "M";
  write(SocketFD, type.c_str(), 1);
  writeOnServer(mensaje);
  writeOnServer(to);
 }

 void getList(){
   txt = "I";
   write(SocketFD, txt.c_str(), 1);
 }

 void logout(){
   txt= "O";
   write(SocketFD, txt.c_str(), 1);
   hasNickname = false;
 }

 void broadcast(){
   if(!hasNickname){
     cout << "login first" << endl;
     return ;
   }
    string message;
    cout << "message: ";
    cin >> message;
    txt= "B";
    write(SocketFD, txt.c_str(), 1);
    writeOnServer(message);
 }

 void sendFile(){
   if(!hasNickname){
     cout << "login first" << endl;
     return ;
   }
   string path;
   cout << "path: ";
   cin >> path;
   cout << "file name: ";
   string fileName;
   cin >> fileName;
   cout << "to: ";
   cin >> to;

   ifstream file(path);
   string sendMessage;
   string line;
   if(file.is_open()){
     while(getline(file, line)){
       sendMessage+=line;
       sendMessage+='\n';

     }
     txt= "F";
     write(SocketFD, txt.c_str(), 1);
     writeOnServer(to);
     writeOnServer(fileName);
     writeOnServer(sendMessage);

   }else{
     cout << "couldn't open file" << endl;
   }

 }

 void printCommandList(){
   cout <<"******************" << endl;
   cout << "login: identify to server" << endl;
   cout << "msg: send message to some user" << endl;
   cout << "list: see all active users" << endl;
   cout << "logout: close session" << endl;
   cout << "broadcast: send message to all active users" << endl;
   cout << "file: send a file" << endl;
   cout <<"******************" << endl << endl;
 }

 void sendMessage(){
   while(chatActive){
     cin >> txt;
       if(txt == "login" )
          login();
       else if(txt == "msg")
          sendMessageTo();
       else if(txt == "list")
          getList();
       else if(txt == "logout")
          logout();
       else if(txt == "broadcast")
          broadcast();
       else if(txt == "file")
          sendFile();
       else if(txt== "help")
        printCommandList();
       else if(txt == "whoami")
        cout << nickname << endl << endl;
     }

 }

 void getCommandResponse(){
   bzero(buffer,10000);
   n = read(SocketFD, buffer, 2);
   switch (buffer[0]) {
     case '1':
       cout << "login" << endl;
       break;
     case '2':
        cout << "msg" <<endl;
        break;
     case '3':
        cout << "logout" << endl;
        break;
     case '4':
        cout << "broadcast" << endl;
        break;
     case '5':
        cout << "file" << endl;
        break;
     default:
        cout << endl;
   }
 }

 void receiveServerMessage(){
   mensaje = readFromServer();
   string from = readFromServer();
   cout << from << " says: " << mensaje << endl << endl;
 }

 void receiveUsers(){
   int numUsers;
   bzero(buffer,10000);
   n = read(SocketFD, buffer, 2);
   numUsers = stoi(buffer);
   int lengthNickname;
   string user;
   while(numUsers--){
     user = readFromServer();
     cout << user << endl;
   }
 }

void receiveFile(){
  string from = readFromServer();

  cout << from << " sent a file." <<endl;
  from = readFromServer();

  ofstream myfile (from);
  if (myfile.is_open())
  {
    from = readFromServer();
    bzero(buffer,10000);
    myfile << from;
    myfile.close();
  }
  else cout << "Unable to open file";

}

 void receiveMessage(){
   while(chatActive){
      bzero(buffer,10000);

      n = read(SocketFD, buffer, 1);

      switch (buffer[0]) {
        case 'A':
          //acknowledge
          cout << "Ok: ";
          getCommandResponse();
          break;
        case 'W':
          //mensaje recibido
          receiveServerMessage();
          break;
        case 'E':
          cout << "Error: ";
          getCommandResponse();
          //error
          break;
        case 'i':
          receiveUsers();
          //receive list
          break;
        case 'R':
          receiveFile();
          break;
        default:
          cout << "unknown server response" << endl;
      }

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
   stSockAddr.sin_port = htons(5001);
   Res = inet_pton(AF_INET, "192.168.1.3", &stSockAddr.sin_addr);

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
