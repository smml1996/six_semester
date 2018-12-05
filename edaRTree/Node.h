//
// Created by Stefanie Muroya lei on 11/4/18.
//

#ifndef EDARTREE_NODE_H
#define EDARTREE_NODE_H

#include <vector>
#include "util.cpp"

using namespace std;
using namespace Util;

class Node{
    int m, M;
public:
    bool hasSpace();
    Node *mother;
    void writePoints();
    void writeLimits();
    double getArea(const int &i);
    void updateMinimumBoundingRectangle();
    double getEnlargement(const int &i, const Point &p);
    Node(const int &m ,const int &M);
    bool isLeaf = true;
    void writePointsInsideCircle(double centerX, double centerY, double radius);
    bool isInsideCircle(Point p, double centerX, double centerY, double radius);
    Rectangle boundingRectangle;
    vector<Point> entriesLeaf;
    vector<Rectangle> entries;
    vector<Node*> hijas;
};


#endif //EDARTREE_NODE_H
