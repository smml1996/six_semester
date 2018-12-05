#include <iostream>
#include <fstream>
#include <cmath>
#include "RTree.h"
#include "Node.h"


using namespace std;

string line;

vector< vector<double> > coordinates;
vector< double > promedios;

double normalConst = 50;

void normalizePoints(){
    promedios[0] = 0;
    promedios[1] = 1;

    for(int i =0; i <  coordinates.size() ; i++){
        promedios[0] += coordinates[i][0];
        promedios[1]+=coordinates[i][1];
    }
    promedios[0]/=coordinates.size();

    promedios[1]/= coordinates.size();

    for(int i =0; i < coordinates.size(); i++){
        coordinates[i][0] = (coordinates[i][0] - promedios[0]+15)*normalConst;
        coordinates[i][1] = (coordinates[i][1] - promedios[1]+15)*normalConst;

    }
}


void loadCrimesFile(){
    // columnas 20 y 21
    ifstream file("latitudes.txt");
    double tempDouble;
    if(file.is_open()){
        getline(file, line);
        while(getline(file, line)){
            coordinates.push_back(vector<double>());
            tempDouble = stod(line);
            coordinates[coordinates.size()-1].push_back(tempDouble);

        }
        file.close();

    }else{

        cout << "couldn't open file" << endl;
    }


    ifstream file2("longitudes.txt");
    int i =0;
    if(file2.is_open()){
        getline(file2, line);
        while(getline(file2, line)){
            tempDouble = stod(line);
            coordinates[i].push_back(tempDouble);
            i++;
        }

        file2.close();

    }else{

        cout << "couldn't open file" << endl;
    }
}

int main(){

    promedios.push_back(0.0);
    promedios.push_back(0.0);

    loadCrimesFile();
    normalizePoints();
    normalizePoints();

    Point p;
    p.x = 5;
    p.y=5;
    int m = 2, M =4;
     Node *n =  new Node(m,M,0,p);
    Rectangle r;
    r.minX=0;
    r.maxX = 500;
    r.minY = 0;
    r.maxY = 500;
    RTree mytree(m,M,r);
    mytree.insert(n);

    cout << "******************" <<endl;
    p.x = 1;
    p.y=1;
    Node *n1 =  new Node(m,M,0,p);
    mytree.insert(n1);
    cout << "******************" <<endl;

    p.x = 3;
    p.y=3;
    Node *n3 =  new Node(m,M,0,p);
    mytree.insert(n3);
    cout << "******************" <<endl;

    p.x = 10;
    p.y=10;
    Node *n4 =  new Node(m,M,0,p);
    mytree.insert(n4);
    cout << "******************" <<endl;

    p.x = 11;
    p.y=11;
    Node *n5 =  new Node(m,M,0,p);
    mytree.insert(n5);
    cout << "******************" <<endl;

    mytree.writeTree(mytree.nodoMadre);





    return 0;
}
