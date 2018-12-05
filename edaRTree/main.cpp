#include <iostream>
#include <fstream>
#include "RTree.h"

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
    ifstream file("../longitutes.txt");

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


    ifstream file2("../latitudes.txt");
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

int main() {
    int m, M;
    cin >> m;
    M = m*2;
    RTree mytree(m,M);
    promedios.push_back(0.0);
    promedios.push_back(0.0);

    loadCrimesFile();
    normalizePoints();
    normalizePoints();



    Point p;






    for(int i=0; i<coordinates.size(); i++){
        p.x = coordinates[i][0];
        p.y = coordinates[i][1];
        mytree.insertar(p);
    }



    mytree.writeTree(mytree.nodoMadre);
    mytree.writeCircle(mytree.nodoMadre, 500, 500, 500);

    return 0;
}
