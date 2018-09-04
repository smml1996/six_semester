#include <iostream>

using namespace std;


int main(){

    long long n;

    cin >> n;

    long long x;

    long long doubleN = 2*n;

    long long count = 1;
    for(long long i =2; i< n; i++){

        x = doubleN - i*(i-1);

        if(x %(2*(i)) == 0 && x>0) {
            count++;
        }
    }

    cout << count << endl;

    return 0;
}
