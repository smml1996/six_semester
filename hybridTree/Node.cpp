//
// Created by Stefanie Muroya lei on 11/19/18.
//

#include "Node.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <algorithm>

int Node::tempDimension = -1;

Node::Node(const int &maxEntries, const int &numberDimensions){
    this->totalDimensions = numberDimensions;
    this->maxEntries = maxEntries;
    isLeaf = true;
    leftDaughter = nullptr;
    rightDauhter = nullptr;
    srand (time(NULL));
}


bool Node::comparison(const vector<double> &a, const vector<double> &b) {
    return a[tempDimension] < b[tempDimension];
}

void Node::getDimensionToSplit() {
    this->tempDimension =  rand() % totalDimensions;

    dimension = tempDimension;

}

void Node::splitNode() {

    getDimensionToSplit();
    sort(entries.begin(), entries.end(), this->comparison);

    Node *left = new Node(this->maxEntries, this->totalDimensions);
    Node *right = new Node(this->maxEntries, this->totalDimensions);

    int middle = entries.size()/2;

    median = entries[middle][tempDimension];
    for(int i =0; i < entries.size(); i++){

        if(i <= middle){
            left->entries.push_back(entries[i]);
        }else{
            right->entries.push_back(entries[i]);
        }

    }

    this->leftDaughter = left;
    this->rightDauhter = right;
    this->isLeaf = false;
    this->entries.erase(this->entries.begin(), this->entries.end());

}




