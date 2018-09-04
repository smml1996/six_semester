#include <iostream>

using namespace std;

long long reverseNumber(long long number){

    long long answer =0;

    while(number>0){
        answer*=10;
        answer+= number%10;

        number/=10;
    }

    return answer;
}
int main(){

    long long tt;
    cin >> tt;

    long long a, b;
    while(tt--){
        cin >> a >> b;
        cout << reverseNumber(reverseNumber(a) + reverseNumber(b)) << endl;

    }


    return 0;
}
