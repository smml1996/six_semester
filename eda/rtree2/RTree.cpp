#include "RTree.h"
#include <queue>

using namespace std;



RTree::RTree(int &m, int &M){
  this->m = m;
  this->M = M;
  nodoMadre = nullptr;
}



Node * RTree::chooseLeaf(Node *actual, Point &p){
  if(actual->isLeaf)
    return actual;
  int actualIndexBest = 0;
  double bestEnlargement = getEnlargement(actual->[0]);
  double temp;
  for(int i  = 1; i <  actual->entries.size(); i++){
    temp = getEnlargement(i, p);
    if(temp < bestEnlargement){
      actualIndexBest = i;
      bestEnlargement = temp;
    }

  }
  return chooseLeaf(actual->entries[i]);
}

void RTree::pickSeeds(Node* n, Rectangle &group1, Rectangle &group2, int &indexG1, int&indexG2){

  group1 = hijas[0]->boundingRectangle;
  indexG1 = 0;
  group2 - hijas[1]->boundingRectangle;
  indexG2 = 1;
  boudingRectangle = getBoudingRectangle(group1, group2);
  double d = areaRectangle(boudingRectangle) - areaRectangle(group1) - areaRectangle(group2);

  Rectangle tempRect1, tempRect2;
  double tempd;
  Rectangle tempBounding;

  for(int i =0; n->hijas.size(); i++){
    for(int j =i+1; n->hijas.size(); j++){
      tempRect1 = hijas[i]->boudingRectangle;
      tempRect2 = hijas[j]->boudingRectangle;

      tempBounding = getBoudingRectangle(tempRect1, tempRect2);
      tempd = areaRectangle(tempBounding) - areaRectangle(tempRect1) - areaRectangle(tempRect2);
      if(tempd > d){
        d =tempd;
        group1 = tempRect1;
        group2 = tempRect2;
        indexG1 =i;
        indexG2 = j;
      }
    }
  }
}

class mycomparison{

  public:
    mycomparison(){

    };
    bool operator() (const pair<int, double> & lhs, const pair<int, double> &rhs) const{
      return (lhs.second>rhs.second);
    };
  };

typedef priority_queue< pair<int, double> ,vector< pair< int, double > >, mycomparison > mypq_type;

pair<Node *, Node *> RTree::splitNode(Node *node,Rectangle &group1,Rectangle &group2){
  int indexG1, indexG2;
  pickSeeds(node, group1, group2, indexG1, indexG2);
  Node *n1 = new Node(m,M);
  n1->isLeaf= node->isLeaf;
  n1->mother = node->mother;
  Node *n2 = new Node(m, M);
  n2->isLeaf = node->isLeaf;
  n2->mother = node->mother;

  mypq_type elements;

  if(node->isLeaf){

    for(int i =0; i < node->entriesLeaf.size(); i++){
      elements.push(make_pair(i, getDistance(group1, group2, node->entriesLeaf[i])));
    }

    while(!elements.empty()){

      if(elements.size() + n1->entriesLeaf.size() <= m){
        n1->entriesLeaf.push_back(elements.front());
      }else if(elements.size() + n2->entriesLeaf.size() <= m){
        n2->entriesLeaf.push_back(elements.front());
      }else if(costRectangle(group1, elements.front()) ==  costRectangle(group2, elements.front())){
        if(areaRectangle(group1) < areaRectangle(grou2)){
          n1->entriesLeaf.push_back(elements.front());
        }else{
          n2->entriesLeaf.push_back(elements.front());
        }
      }else if(costRectangle(group1, elements.front()) <  costRectangle(group2, elements.front())){
        n1->entriesLeaf.push_back(elements.front());
      }else{
        n2->entriesLeaf.push_back(elements.front());
      }
      elements.pop();
    }
  }else{

    for(int i =0; i < node->entries.size(); i++){
      elements.push(make_pair(i, getDistance(group1, group2, node->entries[i])));
    }

    while(!elements.empty()){

      if(elements.size() + n1->entries.size() <= m){
        n1->entries.push_back(elements.front());
      }else if(elements.size() + n2->entries.size() <= m){
        n2->entries.push_back(elements.front());
      }else if(costRectangle(group1, elements.front()) ==  costRectangle(group2, elements.front())){
        if(areaRectangle(group1) < areaRectangle(grou2)){
          n1->entries.push_back(elements.front());
        }else{
          n2->entries.push_back(elements.front());
        }
      }else if(costRectangle(group1, elements.front()) <  costRectangle(group2, elements.front())){
        n1->entries.push_back(elements.front());
      }else{
        n2->entries.push_back(elements.front());
      }
      elements.pop();
    }

  }
  n1->updateMinimumBoundingRectangle();
  group1 = n1->boudingRectangle;
  n2->updateMinimumBoundingRectangle();
  group2 = n2->boudingRectangle;
  return make_pair(n1, n2);
}


void RTree::adjustTree(Node *n){
  if(n == nodoMadre)return ;

  if(n->entries.size() > M){
    Rectangle group1, group2;
    pair<Node *, Node *> split = splitNode(leaf, group1, group2);
    n->mother->entries.push_back(group1);
    n->mother->entries.push_back(group2);
    n->mother->hijas.push_back(split.first);
    n->mother->hijas.push_back(split.second);
    adjustTree(split.first);
    adjustTree(split.second);
    delete n;

  }else{
    n->updateMinimumBoundingRectangle();
    adjustTree(n->mother);
  }

}
void RTree::insertar(Point &p){

  if(nodoMadre == nullptr){
    nodoMadre = new Node(m, M)
    nodoMadre->entriesLeaf.push_back(p);
    boundingRectangle.minX = p.x;
    boundingRectangle.maxX = p.x;
    boundingRectangle.maxY = p.y;
    boundingRectangle.minY = p.y;
    return;
  }
  Node *leaf = chooseLeaf(nodoMadre, p);
  if(leaf->hasSpace){
    leaf->entriesLeaf.push_back(p);
    nodoMadre->updateMinimumBoundingRectangle();
    adjustTree(leaf);
    return;
  }
  leaf->entriesLeaf.push_back(p);
  Rectangle group1, group2;
  pair<Node *, Node *> split = splitNode(leaf, group1, group2);

  if(nodoMadre == leaf){
    nodoMadre->isLeaf = false;
    nodoMadre->entriesLeaf.clear();
    nodoMadre->entries.push_back(group1);
    nodoMadre->entries.push_back(group2);
    nodoMadre->hijas.push_back(split.first);
    nodoMadre->hijas.push_back(split.second);
    nodoMadre->updateMinimumBoundingRectangle();
    return;
  }
  leaf->mother->entries.push_back(group1);
  leaf->mother->entries.push_back(group2);
  leaf->mother->hijas.push_back(split.first);
  leaf->mother->hijas.push_back(split.second);
  delete leaf;
  adjustTree(split.first);
  adjustTree(split.second);
  return ;

}
