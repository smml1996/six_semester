#include <iostream>

using namespace std;


int main(){

    string s = "HTTP/1.1 200 OK \r\n Date: Sun, 26 Sep 2010 20:09:20 GMT \r\n Server: Apache/2.0.52(CentOS) \r\n Last-Modified: Tue, 30 Oct 2007 17:00:02 GMT \r\n ETag: “17dc6-a5c-bf716880\" \r\n Accept-Ranges: bytes\r\n Content-Length: 43\r\n Keep-Alive: timeout=10, max=100\r\n Connection: Keep-Alive\r\n Content-Type: text/html; charset=ISO-8859-1\r\n\r\n <html><body><h2>UCSP!!!</h2></body></html>";

    cout << s;

    return 0;
}
