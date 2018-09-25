#include "node.h"
#include <fstream>
#include <string>
#include <sstream>
class QuadTree{

public:
  Node *head;

  QuadTree(const int &maxPoints, const vector< pair<double, double> > &l);
  int maxPointsAllowed;
  void addPoint(const vector<double> &pp);
  bool searchPoint(vector<double> pp, Node * &pointer);
  bool canPointBeAdded(const vector<double> &pp);
  void writeTree(Node *actual);
  void getPointsInCircle(const double &r, const vector<double> &center, Node *actual);
};
