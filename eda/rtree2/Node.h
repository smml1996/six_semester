#include "util.cpp"
#include <iostream>

using namespace Util;
using namespace std;

class Node{

public:
  bool hasSpace();
  Node *mother;
  double getArea(const int &i);
  void updateMinimumBoundingRectangle();
  double getEnlargement(const int &i, const Point &p);
  Node(const int &m ,const int &M);
  bool isLeaf = true;
  Rectangle boundingRectangle;
  vector<Point> entriesLeaf;
  vector<Rectangle> entries;
  vector<Node*> hijas;
 };
