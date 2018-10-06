#include <iostream>
#include <fstream>
#include <cmath>
#include "quadtree.h"

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


  vector<  pair<double, double> > l;

  l.push_back(make_pair(0,1500));
  l.push_back(make_pair(0,1500));
  QuadTree qt(50,l);

  for(int i =0; i < coordinates.size(); i++){
    cout << coordinates[i][0] << " " << coordinates[i][1] << endl;

    qt.addPoint(coordinates[i]);
  }
  vector<double>center;
  center.push_back(700.0);
  center.push_back(700.0);
  qt.getPointsInCircle(50.0,center,qt.head);

  //qt.writeTree(qt.head);

/*

 vector<double> point;



  point.push_back(5);
  point.push_back(5);

  qt.addPoint(point);

  point[0]=1;
  point[1]=1;
  qt.addPoint(point);
  point[0]=7;
  point[1]=8;
  qt.addPoint(point);
  point[0]=88;
  point[1]=90;
  qt.addPoint(point);

  cout << qt.head->points.size() << endl;

  qt.getPointsInCircle(10,center, qt.head);

  qt.writeTree(qt.head);
  */


  return 0;
}
