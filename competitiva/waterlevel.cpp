#include <iostream>
#include <vector>

using namespace std;

int n, m;
int main(){

    cin >> n;
    vector<int> aboveMarks;
    vector<int> marksNeeded;

    int temp;

    for (int i =0; i < n; i++){
        cin>> temp;

        aboveMarks.push_back(temp);
        if(i ==0){
            marksNeeded.push_back(1);
        }else{
            marksNeeded.push_back(max(marksNeeded[i-1], temp+1));
        }
    }


    int answer = marksNeeded[marksNeeded.size()-1]-1 - aboveMarks[aboveMarks.size()-1];


    for(int i = marksNeeded.size()-2; i>=0; i--) {
        if (marksNeeded[i] + 1 < marksNeeded[i + 1]) {
            marksNeeded[i] = marksNeeded[i + 1] - 1;
        }
        answer += marksNeeded[i] - 1 - aboveMarks[i];
    }

    cout << answer << endl;


    return 0;
}
