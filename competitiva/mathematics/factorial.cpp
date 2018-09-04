#include <iostream>

using namespace std;

long get5s(long number){

    long power = 5;

    long answer =0;
    while(power <= number){

        answer+= number/power;
        power*=5;
    }

    return answer;

}
int main(){

    long tt;

    cin >> tt;
    long number;

    while(tt--){
        cin >> number;

        cout << get5s(number) << endl;
    }


    return 0;
}
