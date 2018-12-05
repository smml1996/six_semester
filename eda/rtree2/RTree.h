#include "Node.h"


class RTree{

  int m, M;
  void pickSeeds(Node *n, Rectangle &group1, Rectangle &group2, int &indexG1, int&indexG2);
  void adjustTree(Node *n);
public:
  RTree(int &m, int &M);
  void insertar(Point &p);
  Node * chooseLeaf(Node * actual, Point &p);
  pair<Node*, Node*> splitNode(Node *node,Rectangle &group1,Rectangle &group2);
  Node *nodoMadre;

}
