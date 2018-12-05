//
// Created by Stefanie Muroya lei on 11/2/18.
//

#include "RTree.h"
#include <queue>
#include <vector>
#include <iostream>

using namespace Util;
using namespace std;


RTree::RTree(int &m,int &M, Rectangle r){
    this->m = m;
    this->M = M;
    this->nodoMadre->isRectangle = false;
}


class mycomparison{

public:
    mycomparison(){

    };
    bool operator() (const pair<int, double> & lhs, const pair<int, double> &rhs) const{
        return (lhs.second>rhs.second);
    };
};

typedef priority_queue< pair<int, double> ,vector< pair< int, double > >, mycomparison > mypq_type;

double RTree::getDifference(Node *group1, Node *group2, Node * element){

    double d1, d2;
    if(element->isRectangle){
        d1 = group1->getEnlargement(element->boundingRectangle);
        d2 = group2->getEnlargement(element->boundingRectangle);
    }else{
        d1 = group1->getEnlargement(element->p);
        d2 = group2->getEnlargement(element->p);
    }



    double answer = d1 -d2;

    if(answer < 0)
        answer*=-1;

    return answer;
}


pair<Node*, Node*> RTree::splitNode(Node *n){

    pair<int, int> groups = pickSeeds(n);


    Node * group1 =new Node(m, M,0 ,n->hijas[groups.first]->boundingRectangle); //n->hijas[groups.first];
    Node * group2 =new Node(m, M,0 ,n->hijas[groups.second]->boundingRectangle); //n->hijas[groups.second];
    group1->mother = n;
    group2->mother=n;
    group1->id = 0;
    group2->id= 1;

    mypq_type  elements;

    for(int i =0; i < n->hijas.size(); i++){

        if(i != groups.first && i != groups.second || !n->hijas[groups.first]->isRectangle){
            elements.push(make_pair(i, getDifference(group1, group2, n->hijas[i])));
        }
    }


    double enlargeGroup1, enlargeGroup2;


    while(!elements.empty()){
        cout << elements.top().first <<endl;
        if(group1->hijas.size() + elements.size() == m){

            group1->hijas.push_back(n->hijas[elements.top().first]);
            group1->hijas[group1->hijas.size()-1]->id = group1->hijas.size()-1;
            group1->hijas[group1->hijas.size()-1]->mother = group1;

        }else if (group2->hijas.size() + elements.size() == m){
            group2->hijas.push_back(n->hijas[elements.top().first]);
            group2->hijas[group2->hijas.size()-1]->id = group2->hijas.size()-1;
            group2->hijas[group2->hijas.size()-1]->mother = group2;


        }else{
            enlargeGroup1 = group1->getEnlargement(n->hijas[elements.top().first]->boundingRectangle);
            enlargeGroup2 = group2->getEnlargement(n->hijas[elements.top().first]->boundingRectangle);
            if(enlargeGroup1 == enlargeGroup2){
                if(group1->getArea() < group2->getArea()){
                    group1->hijas.push_back(n->hijas[elements.top().first]);
                    group1->hijas[group1->hijas.size()-1]->id = group1->hijas.size()-1;
                    group1->hijas[group1->hijas.size()-1]->mother = group1;

                }else{
                    group2->hijas.push_back(n->hijas[elements.top().first]);
                    group2->hijas[group2->hijas.size()-1]->id = group2->hijas.size()-1;
                    group2->hijas[group2->hijas.size()-1]->mother = group2;


                }
            }else if(enlargeGroup1 < enlargeGroup2){
                group1->hijas.push_back(n->hijas[elements.top().first]);
                group1->hijas[group1->hijas.size()-1]->id = group1->hijas.size()-1;
                group1->hijas[group1->hijas.size()-1]->mother = group1;
            }else{
                group2->hijas.push_back(n->hijas[elements.top().first]);
                group2->hijas[group2->hijas.size()-1]->id = group2->hijas.size()-1;
                group2->hijas[group2->hijas.size()-1]->mother = group2;


            }
        }
        elements.pop();
    }


    n->hijas.erase(n->hijas.begin(), n->hijas.end());

    return make_pair(group1, group2);
}

double RTree::getD(Point p1, Point p2){
    double tempMinX, tempMaxX, tempMinY, tempMaxY;

    tempMinX = min(p1.x, p2.x);
    tempMaxX = max(p1.x, p2.x);

    tempMinY = min(p1.y, p2.y);
    tempMaxY = max(p1.y, p2.y);

    double sideAxis1 = tempMaxX - tempMinX;
    double sideAxis2 = tempMaxY - tempMinY;
    return sideAxis1 * sideAxis2;
}

double RTree::getD(Rectangle r1, Rectangle r2){
    double tempMinX, tempMaxX, tempMinY, tempMaxY;

    tempMinX = min(r1.minX, r2.minX);
    tempMaxX = max(r1.maxX, r2.maxX);

    tempMinY = min(r1.minY, r2.minY);
    tempMaxY = max(r1.maxY, r2.maxY);

    double sideAxis1 = tempMaxX - tempMinX;
    double sideAxis2 = tempMaxY - tempMinY;
    return sideAxis1 * sideAxis2;
}

pair<int, int> RTree::pickSeeds(Node *n){

    double dtemp, d = -1;
    double area1, area2;

    vector<Node *> entries = n->hijas;
    pair< int, int> answer;

    for(int i =0; i < entries.size(); i++){
        if(n->isRectangle)
            area1 = entries[i]->getArea();
        else
            area1 = 0;
        for (int j =i+1;j< entries.size(); j++){
            if(n->isRectangle)
                area2 = entries[i]->getArea();
            else
                area2 = 0;

            if(n->isRectangle)
                dtemp = getD(entries[i]->boundingRectangle, entries[j]->boundingRectangle);
            else
                dtemp = getD(entries[i]->p, entries[j]->p);

            dtemp -=area1 + area2;

            if(dtemp > d){
                d = dtemp;
                answer = make_pair(i,j);
            }

        }
    }
    return answer;
}

bool RTree::search(Node *actualNode, Point p){
    if(!actualNode->isRectangle){
        if(p.x == actualNode->p.x && p.y == actualNode->p.y) return true;
        else return false;
    }else{
        bool temp;
        for(int i = 0; i < actualNode->hijas.size(); i++){
            if(actualNode->hijas[i]->isInside(p)){
                temp = search(actualNode->hijas[i],p);
                if(temp) return temp;
            }
        }
        return false;
    }
}


void RTree::condenseTree(Node *l){

    Node*actual = l;

    queue<Node *> eliminated;

    Node *temp;

    while(actual!=nodoMadre){
        if(actual->hijas.size()<m){
            actual->mother->hijas.erase(actual->mother->hijas.begin(),actual->mother->hijas.begin() + actual->id);
            eliminated.push(actual);
            actual= actual->mother;
        }else{
            actual->updateMinimumBoundingRectagle();
        }
    }

    while(!eliminated.empty()){
        this->insert(eliminated.front());
        eliminated.pop();
    }
}

Node * RTree::chooseLeaf(Node *actual, Point &p, Node * &mother){

    if(!actual->isRectangle){
        return actual;
    }
    int indexBest = 0;
    double temp;
    double actualBestEnlargement = actual->hijas[0]->getEnlargement(p);
    for(int i =1; i < actual->hijas.size(); i++){
        temp = actual->hijas[i]->getEnlargement(p);
        if(actualBestEnlargement ==  temp){
            if(actual->hijas[i]->getArea() < actual->hijas[indexBest]->getArea()){
                indexBest = i; actualBestEnlargement = temp;
            }
        }else if(temp < actualBestEnlargement){
            actualBestEnlargement = temp;
            indexBest = i;
        }
    }
    return chooseLeaf(actual->hijas[indexBest], p, actual);
}

void RTree::insert(Node *n){
    if(nodoMadre == nullptr ){
        nodoMadre = n;
        return;
    }
    Node *actual = nodoMadre;
    Node *mother = nullptr;
    Node * leaf = chooseLeaf(actual, n->p, mother);

    n->mother = mother;
    n->id = leaf->hijas.size();
    leaf->hijas.push_back(n);
    pair<Node*, Node*> ll;
    if(!leaf->hasSpace()){

        ll = splitNode(leaf);


        //leaf->hijas.erase(leaf->hijas.begin(),leaf->hijas.end());

        leaf->hijas.push_back(ll.first);

        leaf->hijas.push_back(ll.second);

        cout << "start first adjust" <<endl;

        adjustTree(ll.first);

        cout << "finish first adjust " <<endl;
        adjustTree(ll.second);

    }else{
        adjustTree(leaf);

    }

}

void RTree::writeTree(Node *actual){
    if(actual == nullptr) return;
    actual->writeRectangle();
    if(!actual->isRectangle){
        actual->writePoints();
        return;
    }else{
        for(int i =0; i < actual->hijas.size(); i++){
            writeTree(actual->hijas[i]);
        }
    }
}

void RTree::borrar(Point &p){
    Node * nodePointer = nodoMadre;
    bool isFound = search(nodePointer, p);

    if(!isFound)return;
    nodePointer->mother->hijas.erase(nodePointer->mother->hijas.begin(),nodePointer->mother->hijas.begin() + nodePointer->id);

    Node * temp = nodePointer;
    nodePointer = nodePointer->mother;
    delete temp;

    condenseTree(nodePointer);

    if(nodoMadre->hijas.size() == 1){
        temp = nodoMadre;
        nodoMadre = nodoMadre->hijas[0];
        delete temp;
    }

}

void RTree::adjustTree(Node *n){



    if (n == nodoMadre){

        if(n->hasSpace())return;

        Rectangle r;
        Node * newRoot = new Node(m,M, 0,r);
        pair<Node *, Node*>newChild = splitNode(nodoMadre);
        newRoot->hijas.push_back(newChild.first);
        newRoot->hijas.push_back(newChild.second);
        newRoot->updateMinimumBoundingRectagle();
        delete nodoMadre;
        nodoMadre = newRoot;

        return;
    }


    n->updateMinimumBoundingRectagle();


    if(!n->hasSpace()){

        pair<Node*, Node*>ll = splitNode(n);
        n->mother->hijas.erase(n->mother->hijas.begin(),n->mother->hijas.begin() + n->id);
        n->mother->hijas.push_back(ll.first);
        n->mother->hijas.push_back(ll.second);
    }
    adjustTree(n->mother);
}
