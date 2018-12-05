//
// Created by Stefanie Muroya lei on 11/19/18.
//

#ifndef HYBRIDTREE_NODE_H
#define HYBRIDTREE_NODE_H

#include <vector>

using namespace std;
class Node {
public:
    static int tempDimension;
    int dimension;
    int median;
    static bool comparison(const vector<double>&a , const vector<double> &b);
    int maxEntries;
    bool isLeaf;
    int totalDimensions;
    Node *leftDaughter;
    Node *rightDauhter;
    vector< vector<double> >entries; // each index is a register for a leaf node
    void getDimensionToSplit();
    void splitNode();
    Node(const int &maxEntries, const int &numberDimensions);
};


#endif //HYBRIDTREE_NODE_H
