#include <vector>
#include "util.cpp"

using namespace std;
using namespace Util;

class Node{

public:
  vector< Node*> hijas;
  Rectangle boundingRectangle;
  bool isRectangle;
  Point p;
  Node * mother;
  int m, M;
  int id;
  Node(const int &m, const int &M, const int &id,const Rectangle &r);
  Node(const int &m, const int &M, const int&id, const Point &p);

  void updateMinimumBoundingRectagle();
  double getArea();
  double getEnlargement(Point &p);
  double getEnlargement(Rectangle &r);
  bool isInside(const Rectangle &r);
  bool isInside(const Point &p);
  bool hasSpace();
};
