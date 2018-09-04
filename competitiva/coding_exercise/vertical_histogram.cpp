#include <iostream>
#include <vector>

using namespace std;


vector<int> histogram('Z' - 'A' +1, 0);

int maxCount = 0;

void f(const char &l){

    if( l <= 'Z' && l >= 'A'){
        histogram[l-'A']++;

        if(maxCount < histogram[l-'A']) maxCount = histogram[l-'A'];
    }
}

string temp;

int main(){

    for(int i =0; i < 4; i++){
        getline(cin, temp);


        for(int j =0; j < temp.size(); j++){
            f(temp[j]);
        }
    }


    while(maxCount >0){
        for(int i =0; i < histogram.size(); i++){
            if(histogram[i] >= maxCount)cout << "*";
            else cout << " ";

            if(i+1 != histogram.size()) cout << " ";
        }
        maxCount--;

        cout << endl;
    }

    for(int i =0; i < histogram.size(); i++){

        cout << char('A' + i);
        if(i+1 != histogram.size()) cout <<" ";
    }

    return 0;
}
