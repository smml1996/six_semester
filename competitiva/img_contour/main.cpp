#include <iostream>
#include <vector>
#include <queue>
#include <fstream>

using namespace std;

vector< vector<int> > image;
queue< pair<int, int> > q;
vector< vector<bool> >isVisited;
vector< pair<int, int> >delta;
string line;
vector<int> getValues(){
  string number;
  vector<int> ans;
  for(int i =0; i<line.size(); i++){
    if(line[i] != ' '){
      number+=line[i];
    }else{
      ans.push_back(stoi(number));
      number="";
    }
  }

  return ans;

}

void loadImage(){
  ifstream myfile("face.out");
  if(myfile.is_open()){
      while(getline(myfile, line)){
        image.push_back(getValues());
      }
      myfile.close();
  }else{
    cout << "no se pudo abrir archivo" << endl;
  }
}

void displayImage(){


  ofstream myfile ("example.txt");
 if (myfile.is_open())
 {

   for(int i =0; i < image.size(); ++i){

     for(int j =0; j < image[i].size(); ++j){
       if(image[i][j] == 300){
         myfile << '0';
       }else if(image[i][j] > 200)
         myfile << ' ';
       else
         myfile << '1';

     }
     myfile<< endl;
   }
   myfile.close();
 }
 else cout << "Unable to open file";

}

bool isNodeValid(const int &i, const int &j){
  if( i>= 0 and j>=0){
    if( i< image.size() && j < image[0].size()){
      if(image[i][j] > 200)return true;
    }
  }

  return false;
}

void bfs(int startI, int startJ){
  if(isNodeValid(startI, startJ) && !isVisited[startI][startJ]){
    image[startI][startJ] = 300;
    isVisited[startI][startJ] = true;
    int tempi, tempj;

    q.push(make_pair(startI, startJ));
    while(!q.empty()){
      tempi = q.front().first;
      tempj = q.front().second;
      q.pop();
      for(int i =0; i < delta.size(); i++){
          if(isNodeValid(tempi+delta[i].first, tempj + delta[i].second) && !isVisited[tempi+delta[i].first][tempj + delta[i].second]){
            isVisited[tempi + delta[i].first][tempj + delta[i].second] = true;
            q.push(make_pair(tempi + delta[i].first, tempj + delta[i].second));
            image[tempi + delta[i].first][tempj + delta[i].second] = 300;
          }
      }
    }


  }

  return ;
}

void colorAll(){
  isVisited = vector< vector<bool> >(image.size(),vector<bool>(image[0].size(), false) );

  for(int i =0; i < image.size(); i++){
    for(int j =0; j < image[0].size(); j++){
      bfs(i, j);
    }
  }
  displayImage();
}

void colorRegion(const int &i , const int &j){
  isVisited = vector< vector<bool> >(image.size(),vector<bool>(image[0].size(), false) );
  bfs(i, j);
  displayImage();
}


int main(){
  delta.push_back(make_pair(-1,0));
  delta.push_back(make_pair(0,1));
  delta.push_back(make_pair(0,-1));
  delta.push_back(make_pair(1,0));
  loadImage();
  colorRegion(7,205);
  return 0;
}
