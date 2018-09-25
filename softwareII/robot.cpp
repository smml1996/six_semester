#include <iostream>

using namespace std;
int h,w;
char moves[]= {'U','R','D','L'}; int cont=0;
int posX=0, posY =0;


char mainBoard[h][w];
int total=1;

char uBoard[h][w], rBoard[h][w], dBoard[h][w], lBoard[h][w];

void recibirInput(){
    for(int i =0; i<h;i++){

        for(int j=0; j<w; j++){
            cin>>mainBoard[i][j];

            if(mainBoard[i][j]!='.'&& mainBoard[i][j]!='*'){

                switch(mainBoard[i][j]){
                    case 'U':
                        cont=0;
                        uBoard[i][j]='O';
                        break;
                    case'R':
                        cont=1;
                        rBoard[i][j]='O';
                    case 'D':
                        cont=2;
                        dBoard[i][j]='O';
                    case 'L':
                        cont=3;
                        lBoard[i][j]='O';
                }
                posY=i; posX=j;
            }

        }
    }
}


int main(){

    cin>>h; cin>>w;


//input

    recibirInput();

// end input;

    int limitY= h-1, limitX= w-1;
    bool exit=false;

    while(true){

        switch(cont){

            case 0:

                if(posY-1>=0 & mainBoard[posY-1][posX]!='*'){
                    posY-=1;
                    if(uBoard[posY][posX]=='O'){
                        exit = true;
                    }else{
                        uBoard[posY][posX]=='O';
                        total++;
                    }
                }else{
                    cont++;
                }
                break;
            case 1:

                if(posX+1<= limitX& mainBoard[posY][posX+1]!='*'){
                    posX+=1;
                    if(rBoard[posY][posX]=='O'){
                        exit=true;
                    }else{
                        rBoard[posY][posX]=='O';
                        total++;
                    }
                }else{
                    cont++;
                }
                break;
            case 2:
                if(posY+1 <= limitY& mainBoard[posY+1][posX]!='*'){
                    posY+=1;
                    if(dBoard[posY][posX]=='O'){
                        exit=true;
                    }else{
                        dBoard[posY][posX]='O';
                        total++;
                    }
                }else{
                    cont++;
                }
                break;
            case 3:
                if(posX-1>=0& mainBoard[posY][posX-1]!='*'){
                    posX-=1;
                    if(lBoard[posY][posX]=='O'){
                        exit=true;
                    }else{
                        lBoard[posY][posX]='O';
                        total++;
                    }

                }else{
                    cont++;
                }
                break;
        }

        if(cont==4)cont=0;
        if(exit)break;
    }

    cout<<total;

    return 0;
}
