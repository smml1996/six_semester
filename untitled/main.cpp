#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;


map<int, int>problems;
map<int,int>::iterator it;

vector<int> countProblems;
int main(){

    int n;

    cin >> n;

    int temp;

    for(int i =0; i < n; i++){
        cin >> temp;
        it = problems.find(temp);
        if(it == problems.end() ){
            problems[temp] = 1;
        }else{
            it->second++;
        }
    }

    for (it=problems.begin(); it!=problems.end(); ++it){
        countProblems.push_back(it->second);
    }

    if(countProblems.size() == 1){
        cout << countProblems[0] << endl;
        return 0;
    }

    sort(countProblems.begin(), countProblems.end());
    int answer =-1;
    int temporal;
    temp =0;

    for(int i =countProblems.size()-1; i>=0; ){

        if(temp ==0){
            temporal = (countProblems[i]/2) * 2;
            if(i > 0 && countProblems[i-1] >= temporal/2){
                temp+= temporal;
            }else{
                answer = max(countProblems[i], answer);
                if(i > 0){
                    temporal = (countProblems[i-1]*2);
                    temp = temporal;
                }
            }
            i--;
        }else if(countProblems[i] >= temporal/2 && temporal%2 == 0){
            temporal = temporal/2;
            temp+=temporal;
            i--;

        }else{
            answer = max(temp, answer);
            temp = 0;
        }

    }
    answer= max(temp, answer);

    cout << answer << endl;



    return 0;
}