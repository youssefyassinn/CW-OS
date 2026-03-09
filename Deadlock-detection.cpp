#include <iostream>
#include <fstream>
using namespace std;

int main() {
    ifstream f("input.txt");

    int p, r, E[10], A[10], C[10][10], R[10][10];
    bool finish[10] = {0};

    f >> p >> r;

    for(int j=0;j<r;j++) f >> E[j];

    for(int i=0;i<p;i++)
        for(int j=0;j<r;j++)
            f >> C[i][j];

    for(int i=0;i<p;i++)
        for(int j=0;j<r;j++)
            f >> R[i][j];

    for(int j=0;j<r;j++){
        int sum=0;
        for(int i=0;i<p;i++) sum+=C[i][j];
        A[j]=E[j]-sum;
    }

    bool change=true;
    while(change){
        change=false;
        for(int i=0;i<p;i++){
            if(!finish[i]){
                bool ok=true;
                for(int j=0;j<r;j++)
                    if(R[i][j]>A[j]) ok=false;

                if(ok){
                    for(int j=0;j<r;j++) A[j]+=C[i][j];
                    finish[i]=true;
                    change=true;
                }
            }
        }
    }

    bool dead=false;
    for(int i=0;i<p;i++)
        if(!finish[i]) dead=true;

    if(!dead) cout<<"No deadlock";
    else{
        cout<<"Deadlocked processes: ";
        for(int i=0;i<p;i++)
            if(!finish[i]) cout<<"P"<<i<<" ";
    }
}
