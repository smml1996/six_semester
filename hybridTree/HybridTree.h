//
// Created by Stefanie Muroya lei on 11/19/18.
//

#ifndef HYBRIDTREE_HYBRIDTREE_H
#define HYBRIDTREE_HYBRIDTREE_H

#include "Node.h"
#include <vector>

using namespace std;

class HybridTree {

public:
    HybridTree(const int &maxEntries, const int &numDimensions);
    int maxEntries;
    int numDimensions;
    Node *head;
    void insertar(const vector<double> &registro);
    Node *getLeaf(Node *actual, const vector<double> &registro);
    bool search(Node *actual,vector<double> registro);


};


#endif //HYBRIDTREE_HYBRIDTREE_H
