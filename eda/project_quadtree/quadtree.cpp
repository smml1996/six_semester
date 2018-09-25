#include "quadTree.h"
#include <iostream>
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
    if(searchPoint(pp,tempPointer)) return;
    cout << "******" << endl;
    cout << tempPointer->limits[0].first << " " << tempPointer->limits[0].second << endl;
    cout << tempPointer->limits[1].first << " " << tempPointer->limits[1].second << endl;
    cout << "******" << endl;
    tempPointer->points.push_back(pp);

    if(tempPointer->points.size() > maxPointsAllowed){
      tempPointer->splitNode();
    }
  }
}

void QuadTree::getPointsInCircle(const double &r, const vector<double> &center, Node *actual){

  if(actual->daughters.size() != 0){

      for(int i =0; i <4; i++){
          if(actual->daughters[i]->limits[0].first <= center[0] - r  && actual->daughters[i]->limits[1].first <= center[1] - r){
              getPointsInCircle(r, center, actual->daughters[i]);
          }

          if(actual->daughters[i]->limits[0].second >= center[0] + r  && actual->daughters[i]->limits[1].second >= center[1] + r){
              getPointsInCircle(r, center, actual->daughters[i]);
          }


      }
  }else{
    for(int i =0; i < actual->points.size(); i++){
      if(actual->points[i][0] <= center[0] +r && actual->points[i][0] >= center[0] -r){
        if(actual->points[i][1] <= center[1] + r && actual->points[i][1] >= center[0]-r ){

          actual->writePoint(actual->points[i]);
          //cout << actual->points[i][0] << " " << actual->points[i][1] << endl;
        }
      }
    }
  }
}
