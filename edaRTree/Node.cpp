//
// Created by Stefanie Muroya lei on 11/4/18.
//

#include "Node.h"
#include <iostream>
#include <fstream>

using namespace Util;
using namespace std;



Node::Node(const int &m, const int &M){
    isLeaf = true;
    this->m = m;
    this->M = M;
    isLeaf = true;
    mother = nullptr;
}

void Node::updateMinimumBoundingRectangle(){

    double minX , maxX, minY, maxY;
    if(isLeaf){
        minX = entriesLeaf[0].x;
        maxX = entriesLeaf[0].x;
        minY = entriesLeaf[0].y;
        maxY = entriesLeaf[0].y;
        for(int i =1; i<entriesLeaf.size(); i++){
            minX = min(minX, entriesLeaf[i].x);
            maxX = max(maxX, entriesLeaf[i].x);
            minY = min(minY, entriesLeaf[i].y);
            maxY = max(maxY, entriesLeaf[i].y);
        }
    }else{
        minX = entries[0].minX;
        maxX = entries[0].maxX;
        minY = entries[0].minY;
        maxY = entries[0].maxY;
        cout << "************" << endl;
        cout << minX << endl;
        cout << maxX << endl;
        cout << minY << endl;
        cout << maxY << endl;
        cout << "************" << endl;
        for(int i =1; i<entries.size(); i++){
            minX = min(minX, entries[i].minX);
            maxX = max(maxX, entries[i].maxX);
            minY = min(minY, entries[i].minY);
            maxY = max(maxY, entries[i].maxY);
        }
    }

    boundingRectangle.minX = minX;
    boundingRectangle.maxX = maxX;
    boundingRectangle.minY = minY;
    boundingRectangle.maxY = maxY;
}
double Node::getArea(const int &i ){
    double axis1Length = entries[i].maxX - entries[i].minX;
    double axis2Length = entries[i].maxY - entries[i].minY;
    return axis1Length * axis2Length;
}

double Node::getEnlargement(const int &i, const Point &p){

    if(p.x >= entries[i].minX && p.x <=entries[i].maxX){
        if(p.y >= entries[i].minY && p.y<=entries[i].maxY) return 0.00;
    }
    double maxX = max(entries[i].maxX, p.x);
    double minX = min(entries[i].minX, p.x);
    double maxY = max(entries[i].maxY, p.y);
    double minY = min(entries[i].minY, p.y);


    double area= (maxX - minX) * (maxY - minY);

    double actualArea =getArea(i);
    if(area <= actualArea)return 0.00;
    return area - actualArea;
}

bool Node::isInsideCircle(Point p, double centerX, double centerY, double radius){
    return (pow(p.x - centerX, 2.0) + pow(p.y - centerY, 2.0)) <= pow(radius, 2.0);
}


void Node::writePointsInsideCircle(double centerX, double centerY, double radius) {
    ofstream myfile("circle.txt", fstream::app);
    for(int i =0; i < entriesLeaf.size(); i++){
        if(isInsideCircle(entriesLeaf[i], centerX, centerY, radius)){

            myfile << entriesLeaf[i].x << " " << entriesLeaf[i].y<< " ";

        }
    }
    myfile.close();
}
void Node::writePoints() {

    ofstream myfile("points.txt", fstream::app);

    if(myfile.is_open()){

        for(int i =0; i < entriesLeaf.size(); i++){
            myfile << entriesLeaf[i].x << " " << entriesLeaf[i].y<< " ";
        }

    }else{
        cout << "no se pudo abrir archivo" << endl;
    }
    myfile.close();
}

void Node::writeLimits() {
    ofstream myfile("limits.txt", fstream::app);

    if(myfile.is_open()){
        myfile << boundingRectangle.minX << " " << boundingRectangle.minY << " "<<boundingRectangle.maxX  << " " << boundingRectangle.minY <<" ";
        myfile << boundingRectangle.minX << " " << boundingRectangle.minY << " "<<boundingRectangle.minX  << " " << boundingRectangle.maxY <<" ";
        myfile << boundingRectangle.minX << " " << boundingRectangle.maxY << " "<< boundingRectangle.maxX << " " << boundingRectangle.maxY <<" ";
        myfile << boundingRectangle.maxX << " " << boundingRectangle.minY << " "<<boundingRectangle.maxX  << " " << boundingRectangle.maxY <<" ";

    }else{
        cout << "no se pudo abrir archivo" << endl;
    }
};

bool Node::hasSpace(){
    if(isLeaf){
        return entriesLeaf.size()<M;
    }
    return entries.size() < M;
}
