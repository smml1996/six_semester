#include "Node.h"

using namespace Util;
Node::Node(const int &m, const int &M, const int &id, const Rectangle &r){
  this->boundingRectangle = r;
  isRectangle = true;
  mother = nullptr;
  this->m = m;
  this->M = M;
  this->id = id;
}

Node::Node(const int &m, const int &M,const int&id, const Point &p){
  isRectangle = false;
  this->p = p;
  mother = nullptr;
  this->m = m;
  this->M = M;

  this->id = id;
}

double Node::getArea(){
  double sideAxis1 = boundingRectangle.maxX - boundingRectangle.minX;
  double sideAxis2 = boundingRectangle.maxY - boundingRectangle.minY;
  return sideAxis1 * sideAxis2;
}


double Node::getEnlargement(Rectangle&r){
  double tempXmin = boundingRectangle.minX, tempXmax = boundingRectangle.maxX;
  double tempYmin = boundingRectangle.minY, tempYmax = boundingRectangle.maxY;


  if(tempXmin <= r.minX && tempXmax>=r.maxX && tempYmin<= r.minY && tempYmax>=r.maxY){
    return 0;
  }

  tempXmin = min(tempXmin, r.minX);
  tempXmax = max(tempXmax, r.maxX);

  tempYmin = min(tempYmin, r.minY);
  tempYmax = max(tempYmax, r.maxY);

  double newArea =  (tempXmax - tempXmin) * (tempYmax - tempYmin);

  return newArea - getArea();
}

double Node::getEnlargement(Point &p){
  double tempXmin = boundingRectangle.minX, tempXmax = boundingRectangle.maxX;
  double tempYmin = boundingRectangle.minY, tempYmax = boundingRectangle.maxY;

  if(p.x >= tempXmin && p.x <= tempXmax && p.y <= tempYmax && p.y >= tempYmin){
    return 0.00;
  }
  tempXmin = min(p.x, tempXmin);
  tempXmax = max(p.x, tempXmax);

  tempYmin = min(p.y, tempYmin);
  tempYmax = max(p.y, tempYmax);

  double newArea = (tempXmax - tempXmin) * (tempYmax - tempYmin);

  if(newArea == getArea()) return 0.00;

  return newArea - getArea();

}

bool Node::hasSpace(){
  if(hijas.size() >= M )return false;
  return true;
}


void Node::updateMinimumBoundingRectagle(){
  for(int i =0; i < hijas.size(); i++){
    boundingRectangle.minX = min(boundingRectangle.minX,
                                              hijas[i]->boundingRectangle.minX);
    boundingRectangle.minY = min(boundingRectangle.minY,
                                              hijas[i]->boundingRectangle.minY);
    boundingRectangle.maxX = max(boundingRectangle.maxX,
                                            hijas[i]->boundingRectangle.maxX);
    boundingRectangle.maxY = max(boundingRectangle.maxY,
                                            hijas[i]->boundingRectangle.maxY);

  }
}

bool Node::isInside(const Point &p){
  if(boundingRectangle.minX <= p.x && boundingRectangle.maxX >= p.x){
    if(boundingRectangle.maxY >= p.y && boundingRectangle.minY<= p.y)
      return true;
  }
  return false;
}


bool Node::isInside(const Rectangle &r){
  if(boundingRectangle.minX <= r.minX && boundingRectangle.maxX >= r.maxX){
    if(boundingRectangle.minY <= r.minY && boundingRectangle.maxY >= r.maxY){
      return true;
    }
  }
  return false;
}
