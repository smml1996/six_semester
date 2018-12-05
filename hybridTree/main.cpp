#include <iostream>
#include <vector>
#include "Node.h"
#include "HybridTree.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */


int main() {

    vector< vector<double> > puntos;
    HybridTree mytree(3,2);
    vector<double> registro;
    srand (time(NULL));

    for(int i =0; i < 10; i++){
        registro.clear();
        registro.push_back(rand() % 1000);
        registro.push_back(rand() % 1000);
        puntos.push_back(registro);
        mytree.insertar(registro);
    }


    for(int i =0; i < puntos.size(); i++){
        cout << mytree.search(mytree.head, puntos[i]) << endl;
    }


    return 0;
}