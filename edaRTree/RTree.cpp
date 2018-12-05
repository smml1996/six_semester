//
// Created by Stefanie Muroya lei on 11/4/18.
//

#include "RTree.h"
#include <queue>
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;



RTree::RTree(int &m, int &M){
    this->m = m;
    this->M = M;
    nodoMadre = nullptr;
}

double RTree::getDistance(Rectangle group1, Rectangle group2, Point p){
    double d1 = costRectangle(group1, p);
    double d2 = costRectangle(group2, p);

    double answer = d1-d2;
    if(answer < 0) answer*=-1;
    return answer;
}

Rectangle RTree::getBoudingRectangle(Rectangle r1, Rectangle r2){
    Rectangle answer;
    answer.minX = min(r1.minX, r2.minX);
    answer.maxX = max(r1.maxX, r2.maxX);
    answer.minY = min(r1.minY, r2.minY);
    answer.maxY = max(r1.maxY, r2.maxY);
    return answer;
}


double RTree::costRectangle(Rectangle r, Rectangle rr){
    double area1 = areaRectangle(r);
    r.minX = min(r.minX, rr.minX);
    r.maxX = max(r.maxX, rr.maxX);
    r.minY = min(r.minY, rr.minY);
    r.maxY = max(r.maxY, rr.maxY);

    double area2 = areaRectangle(r);

    if(area2 <=area1)
        return 0.00;
    return area2 -area1;

}

double RTree::costRectangle(Rectangle r, Point p){
    double area1 = areaRectangle(r);
    r.minX = min(r.minX, p.x);
    r.maxX = max(r.maxX, p.x);
    r.minY = min(r.minY, p.y);
    r.maxY = max(r.maxY, p.y);

    double area2 = areaRectangle(r);

    if(area2 <=area1)
        return 0.00;
    return area2 -area1;

}

double RTree::areaRectangle(const Rectangle &rectangle){
    return (rectangle.maxX - rectangle.minX) * (rectangle.maxY - rectangle.minY);
}

double RTree::getDistance(Rectangle group1, Rectangle group2, Rectangle r){
    double d1 = costRectangle(group1, r);
    double d2 = costRectangle(group2, r);

    double answer = d1-d2;
    if(answer < 0) answer*=-1;
    return answer;
}


Node * RTree::chooseLeaf(Node *actual, Point &p){

    if(actual->isLeaf)
        return actual;
    int actualIndexBest = 0;

    double bestEnlargement = actual->getEnlargement(0, p);

    double temp;

    for(int i  = 1; i <  actual->entries.size(); i++){
        temp = actual->getEnlargement(i, p);
        if(temp < bestEnlargement){
            actualIndexBest = i;
            bestEnlargement = temp;
        }

    }
    return chooseLeaf(actual->hijas[actualIndexBest],p);
}

void RTree::pickSeeds(Node* n, Rectangle &group1, Rectangle &group2, int &indexG1, int&indexG2, bool isHorizontal){
    if(n->isLeaf){

        double minX, maxX, maxY, minY,middleX;

        minX = n->entriesLeaf[0].x;
        maxX = n->entriesLeaf[0].x;
        maxY = n->entriesLeaf[0].y;
        minY = n->entriesLeaf[0].y;
        indexG1 = -1;
        indexG2 = -1;


        for(int i =0;i < n->entriesLeaf.size(); i++){
            minX = min(minX, n->entriesLeaf[i].x);
            maxX = max(n->entriesLeaf[i].x, maxX);
            maxY = max(n->entriesLeaf[i].y, maxY);
            minY = min(minY, n->entriesLeaf[i].y);
        }

        middleX = (maxX + minX)/2;

        if(isHorizontal) {
            group1.minX = minX;
            group1.maxX = middleX;
            group1.minY = minY;
            group1.maxY = maxY;

            group2.minX = middleX;
            group2.maxX = maxX;
            group2.minY = minY;
            group2.maxY = maxY;
        }else{
            group1.minX = minX;
            group1.maxX = maxX;
            group1.minY = minY;
            group1.maxY = (minY+maxY)/2;

            group2.minX = minX;
            group2.maxX = maxX;
            group2.minY = (minY+maxY)/2;
            group2.maxY = maxY;

        }

        return;
    }
    group1 = n->hijas[0]->boundingRectangle;

    indexG1 = 0;
    group2 = n->hijas[1]->boundingRectangle;
    indexG2 = 1;
    Rectangle boudingRectangle = getBoudingRectangle(group1, group2);
    double d = areaRectangle(boudingRectangle) - areaRectangle(group1) - areaRectangle(group2);

    Rectangle tempRect1, tempRect2;
    double tempd;
    Rectangle tempBounding;

    for(int i =0; n->hijas.size(); i++){
        for(int j =i+1; n->hijas.size(); j++){
            tempRect1 = n->hijas[i]->boundingRectangle;
            tempRect2 = n->hijas[j]->boundingRectangle;

            tempBounding = getBoudingRectangle(tempRect1, tempRect2);
            tempd = areaRectangle(tempBounding) - areaRectangle(tempRect1) - areaRectangle(tempRect2);
            if(tempd > d){
                d =tempd;
                group1 = tempRect1;
                group2 = tempRect2;
                indexG1 =i;
                indexG2 = j;
            }
        }
    }
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

pair<Node *, Node *> RTree::splitNode(Node *node,Rectangle &group1,Rectangle &group2, bool isHorizontal){
    int indexG1, indexG2;

    pickSeeds(node, group1, group2, indexG1, indexG2, isHorizontal);

    Node *n1 = new Node(m,M);
    n1->isLeaf= node->isLeaf;
    n1->mother = node->mother;
    Node *n2 = new Node(m, M);
    n2->isLeaf = node->isLeaf;
    n2->mother = node->mother;

    mypq_type elements;

    if(node->isLeaf){

        for(int i =0; i < node->entriesLeaf.size(); i++){
            elements.push(make_pair(i, getDistance(group1, group2, node->entriesLeaf[i])));
        }

        while(!elements.empty()){

            if(elements.size() + n1->entriesLeaf.size() <= m){
                n1->entriesLeaf.push_back(node->entriesLeaf[elements.top().first]);
            }else if(elements.size() + n2->entriesLeaf.size() <= m){
                n2->entriesLeaf.push_back(node->entriesLeaf[elements.top().first]);
            }else if(costRectangle(group1, node->entriesLeaf[elements.top().first]) ==  costRectangle(group2, node->entriesLeaf[elements.top().first])){
                if(areaRectangle(group1) < areaRectangle(group2)){
                    n1->entriesLeaf.push_back(node->entriesLeaf[elements.top().first]);
                }else{
                    n2->entriesLeaf.push_back(node->entriesLeaf[elements.top().first]);
                }
            }else if(costRectangle(group1, node->entriesLeaf[elements.top().first]) <  costRectangle(group2, node->entriesLeaf[elements.top().first])){
                n1->entriesLeaf.push_back(node->entriesLeaf[elements.top().first]);
            }else{
                n2->entriesLeaf.push_back(node->entriesLeaf[elements.top().first]);
            }
            elements.pop();
        }
    }else{

        for(int i =0; i < node->entries.size(); i++){
            if(i!= indexG1 && i != indexG2)
                elements.push(make_pair(i, getDistance(group1, group2, node->entries[i])));
        }

        while(!elements.empty()){

            if(elements.size() + n1->entries.size() <= m){
                n1->entries.push_back(node->entries[elements.top().first]);
            }else if(elements.size() + n2->entries.size() <= m){
                n2->entries.push_back(node->entries[elements.top().first]);
            }else if(costRectangle(group1, node->entries[elements.top().first]) ==  costRectangle(group2, node->entries[elements.top().first])){
                if(areaRectangle(group1) < areaRectangle(group2)){
                    n1->entries.push_back(node->entries[elements.top().first]);
                }else{
                    n2->entries.push_back(node->entries[elements.top().first]);
                }
            }else if(costRectangle(group1, node->entries[elements.top().first]) <  costRectangle(group2, node->entries[elements.top().first])){
                n1->entries.push_back(node->entries[elements.top().first]);
            }else{
                n2->entries.push_back(node->entries[elements.top().first]);
            }
            elements.pop();
        }

    }
    n1->updateMinimumBoundingRectangle();
    group1 = n1->boundingRectangle;
    n2->updateMinimumBoundingRectangle();
    group2 = n2->boundingRectangle;
    return make_pair(n1, n2);
}


void RTree::writeTree(Node *actual){
    if(actual->isLeaf){
        actual->writePoints();
        actual->writeLimits();
    }else{
        actual->writeLimits();
        for(int i =0; i < actual->hijas.size(); i++){
            writeTree(actual->hijas[i]);
        }
    }
}

void RTree::adjustTree(Node *n){
    if(n == nodoMadre)return ;

    if(n->entries.size() > M){
        Rectangle group1, group2;
        Rectangle group11, group22;
        double temp1, temp2;
        pair<Node *, Node *> split = splitNode(n, group1, group2, true);
        temp1 = areaRectangle(group1) + areaRectangle(group2);
        pair<Node *, Node *> split2 = splitNode(n, group11, group22, false);
        temp2 = areaRectangle(group11) + areaRectangle(group22);

        if(temp1 >temp2){
            split = split2;
            group1 = group11;
            group2 = group22;
        }
        n->mother->entries.push_back(group1);
        n->mother->entries.push_back(group2);
        n->mother->hijas.push_back(split.first);
        n->mother->hijas.push_back(split.second);
        adjustTree(split.first);
        adjustTree(split.second);
        delete n;

    }else{
        n->updateMinimumBoundingRectangle();
        adjustTree(n->mother);
    }

}

bool RTree::isInsideCircle(Point p, double centerX, double centerY, double radius) {

    return (pow(p.x - centerX, 2) + pow(p.y - centerY,2)) <= pow(radius, 2);

}

bool RTree::isRectangleInsideCircle(Rectangle r, double centerX, double centerY, double radius) {

    Point p;
    p.x = r.minX;
    p.y = r.minY;

    bool answer = isInsideCircle(p, centerX, centerY, radius);

    p.y = r.maxY;
    answer = answer || isInsideCircle(p, centerX, centerY, radius);
    p.x = r.maxX;
    answer = answer || isInsideCircle(p, centerX, centerY, radius);
    p.y = r.minY;
    answer = answer || isInsideCircle(p, centerX, centerY, radius);

    return answer;
}

void RTree::writeCircle(Node *actual, double centerX, double centerY, double radius) {
    if(actual->isLeaf){

        actual->writePointsInsideCircle(centerX, centerY, radius);
        return;
    }else{


        for(int i =0; i < actual->hijas.size(); i++ ){
            if(isRectangleInsideCircle(actual->hijas[i]->boundingRectangle, centerX, centerY, radius)){
                writeCircle(actual->hijas[i], centerX, centerY, radius);
            }
        }

    }
}
void RTree::insertar(Point &p){
    if(nodoMadre == nullptr){
        nodoMadre = new Node(m, M);
        nodoMadre->entriesLeaf.push_back(p);

        nodoMadre->boundingRectangle.minX = p.x;
        nodoMadre->boundingRectangle.maxX = p.x;
        nodoMadre->boundingRectangle.maxY = p.y;
        nodoMadre->boundingRectangle.minY = p.y;
        return;
    }

    Node *leaf = chooseLeaf(nodoMadre, p);


    if(leaf->hasSpace()){
        leaf->entriesLeaf.push_back(p);
        leaf->updateMinimumBoundingRectangle();
        adjustTree(leaf);

        return;
    }
    leaf->entriesLeaf.push_back(p);
    Rectangle group1, group2;
    double temp1, temp2;

    pair<Node *, Node *> split = splitNode(leaf, group1, group2, true);

    temp1 = areaRectangle(group1) + areaRectangle(group2);
    Rectangle group11, group22;
    pair<Node *, Node *> split2 = splitNode(leaf, group11, group22, false);
    temp2 = areaRectangle(group11) + areaRectangle(group22);
    if(temp2 < temp1){
        split = split2;
        group1 = group11;
        group2 = group22;
    }




    if(nodoMadre == leaf){

        nodoMadre->isLeaf = false;
        nodoMadre->entriesLeaf.clear();
        nodoMadre->entries.push_back(group1);
        nodoMadre->entries.push_back(group2);
        split.first->mother = nodoMadre;
        nodoMadre->hijas.push_back(split.first);

        split.second->mother = nodoMadre;
        nodoMadre->hijas.push_back(split.second);

        nodoMadre->updateMinimumBoundingRectangle();
        return;
    }
    for(int i =0; i < leaf->mother->entries.size(); i++){
        if(leaf->mother->hijas[i] == leaf){
            leaf->mother->hijas.erase( leaf->mother->hijas.begin() + i);
            leaf->mother->entries.erase( leaf->mother->entries.begin() + i);
        }
    }
    leaf->mother->entries.push_back(group1);
    leaf->mother->entries.push_back(group2);

    leaf->mother->hijas.push_back(split.first);
    leaf->mother->hijas.push_back(split.second);
    adjustTree(split.first);
    adjustTree(split.second);
    return ;

}


//void RTree::writeCircle(Node *actual, double )