#include "Node.h"


Node::Node(const int &m, const int &M){
  this->data = p;
  isLeaf = true;
  this->m = m;
  this->M = M;
  isLeaf = True;
  mother = nullptr;
}

void Node::updateMinimumBoundingRectangle(){

  double minX , maxX, minY, maxY;
  if(isLeaf){
    minX = entriesLeaf[0]->p.x;
    maxX = entriesLeaf[0]->p.x;
    minY = entriesLeaf[0]->p.y;
    maxY = entriesLeaf[0]->p.y;
    for(int i =1; i<entriesLeaf.size(); i++){
      minX = min(minX, entriesLeaf->p.x);
      maxX = max(maxX, entriesLeaf->p.x);
      minY = min(minY, entriesLeaf->p.y);
      maxY = max(maxY, entriesLeaf->p.y);
    }
  }else{
    minX = entries[0]->boundingRectangle.minX;
    maxX = entries[0]->boundingRectangle.maxX;
    minY = entries[0]->boundingRectangle.minY;
    maxY = entries[0]->boundingRectangle.maxY;
    for(int i =1; i<entries.size(); i++){
      minX = min(minX, entries->boundingRectangle.minX);
      maxX = max(maxX, entries->boundingRectangle.maxX);
      minY = min(minY, entries->boundingRectangle.minY);
      maxY = max(maxY, entries->boundingRectangle.maxY);
    }
  }

  boundingRectangle.minX = minX;
  boundingRectangle.maxX = maxX;
  boundingRectangle.minY = minY;
  boundingRectangle.maxY = maxY;
}
double Node::getArea(const int &i ){
  double axis1Length = entries[i].maxX - entries[i].minX;
  double axis2Length = entries[i].maxY - entries[i].minY;
  return axis1Length * axis2Length;
}

double Node::getEnlargement(const int &i, const Point &p){

  maxX = max(entries[i].maxX, p.x);
  minX = min(entries[i].minX, p.x);
  maxY = max(entries[i].maxY, p.y);
  minY = min(entries[i].minY, p.y);

  area= (maxX - minX) * (maxY - minY);
  actualArea =getArea(i);
  if(area <= actualArea)return 0.00;
  return area - actualArea;
}

bool Node::hasSpace(){
  if(isLeaf){
    return entriesLeaf<M;
  }
  return entries < M;
}
