#include "Node.h"

using namespace Util;

class RTree{
  int m, M;

  Node * chooseLeaf(Node *actual, Point &p, Node * &mother);
  pair<Node*, Node*>splitNode(Node * n);
  void adjustTree(Node * n);
  pair<int, int> pickSeeds(Node *n);
  double getD(Rectangle r1, Rectangle r2);
  double getD(Point p1, Point p2);
  double getDifference(Node * group1, Node * group2, Node*element);
  void condenseTree(Node *l);
public:
  Node* nodoMadre;
  RTree(Node * madreDeLasMadres, int &m,int &M);
  bool search(Node * actualNode, Point p);
  void insert(Node *n);
  void borrar(Point &p);
};
