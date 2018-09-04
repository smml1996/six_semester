#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;


vector< pair<string, int> > cadenas;

int n, m;

int getLetterWeightAtString(const int &index, const int &letterIndex){
    int answer =0;

    for(int i = letterIndex+1; i < cadenas[index].first.size(); i++){
        if( cadenas[index].first[letterIndex] - cadenas[index].first[i] > 0) answer++;
    }

    return answer;
}

void getStringWeight(const int& index){

    for(int i =0; i < cadenas[index].first.size(); i++){
        cadenas[index].second+=getLetterWeightAtString(index, i);
    }
}


bool f(const pair<string, int> &a, const pair<string, int> &b){

    return a.second < b.second;
}

int main(){

    cin >> n >> m;

    string temp;

    while(m--){

        cin >> temp;

        cadenas.push_back(make_pair(temp, 0));
    }


    for(int i =0; i < cadenas.size(); i++){
        getStringWeight(i);
    }

    sort(cadenas.begin(), cadenas.end(), f);

    for(int i =0; i < cadenas.size(); i++){

        cout << cadenas[i].first << endl;
    }


    return 0;
}
