#include "quadTree.h"
#include <string>

using namespace std;


QuadTree::QuadTree(const int &maxPoints, const vector< pair<double, double> > &l){
  maxPointsAllowed = maxPoints;
  head = new Node;
  head->limits = l;
}

bool QuadTree::canPointBeAdded(const vector<double> & pp){
  if(head->isPointInRange(pp)) return true;
  return false;
}


bool QuadTree::searchPoint(vector<double> pp, Node * &pointer){

  pointer = head;

  if(!head->isPointInRange(pp))return false;

  while(pointer->daughters.size()!= 0){

    for(int i =0; i < pointer->daughters.size(); i++){
      if(pointer->daughters[i]->isPointInRange(pp)){
        pointer= pointer->daughters[i];
        break;
      }
    }
  }

  for(int i =0; i < pointer->points.size(); i++){
    if(pointer->points[i] == pp)return true;
  }

  return false;
}

void QuadTree::writeTree(Node *actual){
  actual->writeLimits();
  actual->writePoints();

  if(actual->daughters.size() != 0){
    writeTree(actual->daughters[0]);
    writeTree(actual->daughters[1]);
    writeTree(actual->daughters[2]);
    writeTree(actual->daughters[3]);

  }
}

void QuadTree::addPoint(const vector<double> &pp){

  if(canPointBeAdded(pp)){
    Node *tempPointer;
    searchPoint(pp,tempPointer);
    tempPointer->points.push_back(pp);

    if(tempPointer->points.size() > maxPointsAllowed){
      tempPointer->splitNode();
    }
  }
}
