#include <iostream>
#include "quadtree.h"

using namespace std;

int main(){

  vector<  pair<double, double> > l;

  l.push_back(make_pair(0,10));
  l.push_back(make_pair(0,10));
  QuadTree qt(2,l);

  vector<double> point;

  point.push_back(5);
  point.push_back(5);

  qt.addPoint(point);

  point[0]=1;
  point[1]=1;
  qt.addPoint(point);
  point[0]=7;
  point[1]=8;
  qt.addPoint(point);

  qt.writeTree(qt.head);

  return 0;
}
