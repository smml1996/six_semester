//
// Created by Stefanie Muroya lei on 11/19/18.
//

#include "HybridTree.h"

HybridTree::HybridTree(const int &maxEntries, const int &numDimensions) {
    this->maxEntries = maxEntries;
    this->numDimensions = numDimensions;
    this->head = nullptr;
}



Node * HybridTree::getLeaf(Node *actual, const vector<double> &registro) {

    if(actual->isLeaf)return actual;

    if(registro[actual->dimension] <= actual->median )
        return getLeaf(actual->leftDaughter, registro);
    return getLeaf(actual->rightDauhter, registro);
}

bool HybridTree::search(Node * actual, vector<double> registro){
    if(actual->isLeaf){

        for(int i =0; i < actual->entries.size(); i++){
            if(actual->entries[i] == registro) return true;
        }

        return false;
    }

    if(registro[actual->dimension] <= actual->median )
        return search(actual->leftDaughter, registro);
    return search(actual->rightDauhter, registro);

}

void HybridTree::insertar(const vector<double> &registro) {
    if(head == nullptr){
        head= new Node(this->maxEntries,this->numDimensions);
        head->entries.push_back(registro);
        return;
    }
    Node * actual = this->head;
    Node *temp = getLeaf(actual, registro);

    temp->entries.push_back(registro); // agregar registro al nodo hoja

    if(temp->entries.size() > maxEntries){
        temp->splitNode();
    }
}