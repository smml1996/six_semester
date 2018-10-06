#include <iostream>
#include <vector>
using namespace std;


vector<int> input;
int nElements;

void countingSort(const int &limit){
    vector<int>sums(limit+1, 0);

    for(int i =0; i < input.size(); i++){
        sums[input[i]]++; // contar cada elemento
    }
    for(int i =1; i < sums.size(); i++){
        sums[i] = sums[i] + sums[i-1]; // sumas acumuladas
    }

    vector<int> answer(input.size());

    int newIndex;
    for(int i =0; i < input.size(); i++){

        newIndex = (--sums[input[i]]);

        answer[newIndex] = input[i];
    }

    for(int i =0; i < input.size(); i++){
        cout << answer[i] << " ";
    }
    cout << endl;


}

int main(){

    int limit;
    cout << "limit: ";
    cin >> limit;
    cout << "num elements: ";
    cin >> nElements;

    int temp;

    for(int i =0; i < nElements; i++){
        cin >> temp;
        input.push_back(temp);
    }

    countingSort(limit);


    return 0;
}
