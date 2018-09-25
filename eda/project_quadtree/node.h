#include <vector>

using namespace std;


class Node{

public:
  vector< pair< double, double > > limits;
  vector< vector<double> > points;
  vector< Node * > daughters;
  Node();
  void splitNode();
  bool isPointInRange(const vector<double> &point);
  void writeLimits();
  void writePoints();

  void writePoint(const vector<double> &point);
};
