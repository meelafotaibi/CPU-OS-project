// STUDENT: MEELO
#include <iostream>
#include <vector>
using namespace std;

void FCFS(vector<int>&,vector<int>&);
void SJF(vector<int>&,vector<int>&);
void PNP(vector<int>&,vector<int>&,vector<int>&);
void PP(vector<int>&,vector<int>&,vector<int>&);
void RR(vector<int>&,vector<int>&,int);
void Banker();

int main() {
    int n, ch;
    cout<<"Enter number of processes: ";
    cin>>n;

    vector<int> AT(n), BT(n), PR(n);
    for(int i=0;i<n;i++){
        cout<<"AT BT PR for P"<<i+1<<": ";
        cin>>AT[i]>>BT[i]>>PR[i];
    }

    cout<<"\n1.FCFS  2.SJF  3.PriorityNP  4.PriorityP  5.RR  6.Banker\nChoice: ";
    cin>>ch;

    if(ch==1) FCFS(AT,BT);
    else if(ch==2) SJF(AT,BT);
    else if(ch==3) PNP(AT,BT,PR);
    else if(ch==4) PP(AT,BT,PR);
    else if(ch==5){ int q; cout<<"Quantum: "; cin>>q; RR(AT,BT,q); }
    else if(ch==6) Banker();
}
