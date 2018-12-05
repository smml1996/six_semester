//
// Created by Stefanie Muroya lei on 11/4/18.
//

#ifndef EDARTREE_RTREE_H
#define EDARTREE_RTREE_H

#include "Node.h"

using namespace Util;


class RTree{

    int m, M;
    void pickSeeds(Node *n, Rectangle &group1, Rectangle &group2, int &indexG1, int&indexG2, bool isHorizontal);
    void adjustTree(Node *n);
    double getDistance(Rectangle group1, Rectangle group2, Point p);
    double getDistance(Rectangle group1, Rectangle group2, Rectangle r);
    bool isRectangleInsideCircle(Rectangle r, double centerX, double centerY, double radius);
    bool isInsideCircle(Point p, double centerX, double centerY, double radius);

public:
    RTree(int &m, int &M);
    void insertar(Point &p);
    void writeTree(Node *actual);
    double areaRectangle(const Rectangle &rectangle);
    void writeCircle(Node *actual, double centerX, double centerY, double radius);
    double costRectangle(Rectangle r, Point p);
    double costRectangle(Rectangle r, Rectangle rr);
    Rectangle getBoudingRectangle(Rectangle r1, Rectangle r2);
    Node * chooseLeaf(Node * actual, Point &p);
    pair<Node*, Node*> splitNode(Node *node,Rectangle &group1,Rectangle &group2, bool isHorizontal);
    Node *nodoMadre;

};


#endif //EDARTREE_RTREE_H
