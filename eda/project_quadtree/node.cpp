#include "node.h"
#include <fstream>
#include <vector>
#include <iostream>

using namespace std;


Node::Node(){

}

bool Node::isPointInRange(const vector<double> &point){

  for(int i =0; i < limits.size(); i++){
      if(point[i] < limits[i].first || point[i]>=limits[i].second)return false;
  }

  return true;
}

void Node::splitNode(){

  vector< vector< pair<double,double> > > splits;

  double middle;
  for(int i =0; i < limits.size(); i++){
      splits.push_back(vector< pair<double, double> >());
      middle = (limits[i].first + limits[i].second)/2;
      splits[i].push_back(make_pair(limits[i].first, middle));
      splits[i].push_back(make_pair(middle, limits[i].second));
  }

  vector<vector< pair<double, double> > > last;

  last.push_back(vector< pair<double, double> >());
  last[0].push_back(splits[0][0]);
  last.push_back(vector< pair<double, double> >());
  last[1].push_back(splits[0][1]);


  for(int i =1; i < splits.size(); i++){
    vector<vector< pair<double, double> > > temp;
    for(int j=0; j < last.size(); j++){
      temp.push_back(last[j]);
      temp[temp.size()-1].push_back(splits[i][0]);
      temp.push_back(last[j]);
      temp[temp.size()-1].push_back(splits[i][1]);
    }
    last = temp;
  }

  for(int i =0; i < last.size(); i++){
    daughters.push_back(new Node);
    daughters[daughters.size()-1]->limits = last[i];
  }

  for(int i =0; i < points.size(); i++){

    for(int j =0; j < daughters.size(); j++){
      if(daughters[j]->isPointInRange(points[i])){
        daughters[j]->points.push_back(points[i]);
        break;
      }
    }
  }
  points=vector< vector<double> >();
}

void Node::writeLimits(){
  ofstream myfile("limits.txt", fstream::app);

  if(myfile.is_open()){
     myfile << limits[0].first << " " << limits[1].first << " "<<limits[0].second  << " " << limits[1].first <<" ";
     myfile << limits[0].first << " " << limits[1].first << " "<<limits[0].first  << " " << limits[1].second <<" ";
     myfile << limits[0].first << " " << limits[1].second << " "<<limits[0].second << " " << limits[1].second <<" ";
     myfile << limits[0].second << " " << limits[1].first << " "<<limits[0].second  << " " << limits[1].second <<" ";

   }else{
    cout << "no se pudo abrir archivo" << endl;
  }
}

void Node::writePoints(){
  ofstream myfile("points.txt", fstream::app);

  if(myfile.is_open()){
    for(int i =0; i < points.size(); i++){
      myfile << points[i][0] << " " << points[i][1] << " ";
    }
   }else{
    cout << "no se pudo abrir archivo" << endl;
  }
  myfile.close();

}
