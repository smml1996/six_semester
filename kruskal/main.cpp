#include <iostream>
#include <queue>
#include <vector>
using namespace std;



typedef struct arista{
    int nodeFrom;
    int nodeTo;
    int weight;
};


class mycomparison
{
    bool reverse;
public:
    mycomparison();

    bool operator() (const arista& lhs, const arista&rhs) const
    {
       return (lhs.weight<rhs.weight);
    }
};

typedef priority_queue<arista, vector<arista>, mycomparison> mypq_type;

mypq_type aristas;

vector<int> ids;
vector<int>szs;


bool find(int p){
    if(ids[p] == p) return p;
    return find(ids[p]);
}

void u(int p , int q){
    p = find(p);
    q = find(q);

    if (szs[p] > szs[q]){
     swap(p,q);
    }

    ids[p] = q;
    szs[q] =szs[q]+1;
}


vector<arista> answer;


int main() {


    int n;

    cin >> n;

    for(int i =0; i < n; i++){
        ids.push_back(i);
        szs.push_back(0);
    }

    int nodeFrom, nodeTo, weight;

    arista ar;

    for(int i =0; i < n; i++){
        cin>>nodeFrom >> nodeTo >> weight;
        ar.nodeFrom = nodeFrom;
        ar.nodeTo= nodeTo;
        ar.weight = weight;
        aristas.push(ar);
    }




    /*

    answer.push_back(aristas.top());

    u(aristas.top().nodeFrom, aristas.top().nodeTo);
    int parentLast =aristas.top().nodeFrom;
    aristas.pop();

    while(!aristas.empty()){
        if(find(aristas.top().nodeFrom)!= parentLast && find(aristas.top().nodeTo)!=parentLast){
            answer.push_back(aristas.top());
            u(aristas.top().nodeFrom, parentLast);
            u(aristas.top().nodeTo, parentLast);
        }
        aristas.pop();
    }

*/




    return 0;
}