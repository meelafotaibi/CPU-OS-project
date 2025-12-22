#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct P{int id,at,bt,pr,st,ct,tat,wt,rem;};

void print(vector<P>&p,string name,bool showPr=false){
    cout<<"\n--- "<<name<<" ---\nPID AT BT";
    if(showPr) cout<<" PR";
    cout<<" ST CT TAT WT\n";

    double avg1=0,avg2=0;
    for(auto &x:p){
        cout<<"P"<<x.id<<"  "<<x.at<<"  "<<x.bt;
        if(showPr) cout<<"  "<<x.pr;
        cout<<"  "<<x.st<<"  "<<x.ct<<"  "<<x.tat<<"  "<<x.wt<<"\n";
        avg1+=x.wt; avg2+=x.tat;
    }
    cout<<"Avg WT = "<<avg1/p.size()<<", Avg TAT = "<<avg2/p.size()<<"\n";
}

void printGantt(vector<P>&p){
    cout<<"\nGantt Chart:\n|";
    vector<P> sorted = p;
    sort(sorted.begin(),sorted.end(),[](P&a,P&b){return a.st<b.st;});
    
    for(auto &x:sorted){
        cout<<"P"<<x.id;
        int len = x.ct - x.st;
        for(int i=1;i<len;i++) cout<<" ";
        cout<<"|";
    }
    cout<<"\n";
    
    for(auto &x:sorted){
        cout<<x.st;
        int len = x.ct - x.st;
        int digits = to_string(x.st).length();
        for(int i=digits;i<len+1;i++) cout<<" ";
    }
    cout<<sorted.back().ct<<"\n";
}

struct TimeSlot{int pid,start,end;};
void printGanttPreemptive(vector<TimeSlot>&timeline){
    cout<<"\nGantt Chart:\n|";
    for(auto &slot:timeline){
        cout<<"P"<<slot.pid;
        int len = slot.end - slot.start;
        for(int i=1;i<len;i++) cout<<" ";
        cout<<"|";
    }
    cout<<"\n";
    
    for(auto &slot:timeline){
        cout<<slot.start;
        int len = slot.end - slot.start;
        int digits = to_string(slot.start).length();
        for(int i=digits;i<len+1;i++) cout<<" ";
    }
    cout<<timeline.back().end<<"\n";
}

void FCFS(vector<int>&AT,vector<int>&BT){
    int n=AT.size(),t=0;
    vector<P>p(n);
    for(int i=0;i<n;i++){ p[i]={i+1,AT[i],BT[i],0,-1,0,0,0,BT[i]}; }
    sort(p.begin(),p.end(),[](P&a,P&b){return a.at<b.at;});
    for(int i=0;i<n;i++){
        if(t<p[i].at) t=p[i].at;
        p[i].st=t; p[i].ct=t+p[i].bt;
        p[i].tat=p[i].ct-p[i].at; p[i].wt=p[i].tat-p[i].bt;
        t=p[i].ct;
    }
    print(p,"FCFS");
    printGantt(p);
}

void SJF(vector<int>&AT,vector<int>&BT){
    int n=AT.size(),t=0,c=0;
    vector<P>p(n); vector<bool>done(n,false);
    for(int i=0;i<n;i++) p[i]={i+1,AT[i],BT[i],0,-1,0,0,0,BT[i]};
    while(c<n){
        int idx=-1,mn=1e9;
        for(int i=0;i<n;i++)
            if(!done[i] && p[i].at<=t && p[i].bt<mn) mn=p[i].bt,idx=i;
        if(idx==-1){t++;continue;}
        p[idx].st=t; p[idx].ct=t+p[idx].bt;
        p[idx].tat=p[idx].ct-p[idx].at; p[idx].wt=p[idx].tat-p[idx].bt;
        t=p[idx].ct; done[idx]=true; c++;
    }
    print(p,"SJF");
    printGantt(p);
}

void PNP(vector<int>&AT,vector<int>&BT,vector<int>&PR){
    int n=AT.size(),t=0,c=0;
    vector<P>p(n); vector<bool>done(n,false);
    for(int i=0;i<n;i++) p[i]={i+1,AT[i],BT[i],PR[i],-1,0,0,0,BT[i]};
    while(c<n){
        int idx=-1,mn=1e9;
        for(int i=0;i<n;i++)
            if(!done[i] && p[i].at<=t && p[i].pr<mn) mn=p[i].pr,idx=i;
        if(idx==-1){t++;continue;}
        p[idx].st=t; p[idx].ct=t+p[idx].bt;
        p[idx].tat=p[idx].ct-p[idx].at; p[idx].wt=p[idx].tat-p[idx].bt;
        t=p[idx].ct; done[idx]=true; c++;
    }
    print(p,"Priority NP",true);
    printGantt(p);
}

void PP(vector<int>&AT,vector<int>&BT,vector<int>&PR){
    int n=AT.size(),t=0,c=0;
    vector<P>p(n);
    vector<TimeSlot>timeline;
    for(int i=0;i<n;i++) p[i]={i+1,AT[i],BT[i],PR[i],-1,0,0,0,BT[i]};
    while(c<n){
        int idx=-1,mn=1e9;
        for(int i=0;i<n;i++)
            if(p[i].at<=t && p[i].rem>0 && p[i].pr<mn) mn=p[i].pr,idx=i;
        if(idx==-1){t++;continue;}
        if(p[idx].st==-1) p[idx].st=t;
        
        int start=t;
        p[idx].rem--; t++;
        
        if(!timeline.empty() && timeline.back().pid==p[idx].id)
            timeline.back().end = t;
        else
            timeline.push_back({p[idx].id,start,t});
            
        if(p[idx].rem==0){
            p[idx].ct=t; p[idx].tat=t-p[idx].at; p[idx].wt=p[idx].tat-p[idx].bt; c++;
        }
    }
    print(p,"Priority P",true);
    printGanttPreemptive(timeline);
}

void RR(vector<int>&AT,vector<int>&BT,int q){
    int n=AT.size(),t=0,c=0;
    vector<P>p(n); vector<bool>inQ(n,false);
    vector<int> Q;
    vector<TimeSlot>timeline;
    for(int i=0;i<n;i++) p[i]={i+1,AT[i],BT[i],0,-1,0,0,0,BT[i]};
    while(c<n){
        for(int i=0;i<n;i++) if(!inQ[i] && p[i].at<=t && p[i].rem>0) Q.push_back(i),inQ[i]=true;
        if(Q.empty()){t++;continue;}
        int idx=Q[0]; Q.erase(Q.begin());
        if(p[idx].st==-1) p[idx].st=t;
        
        int start=t;
        int run=min(q,p[idx].rem); p[idx].rem-=run; t+=run;
        timeline.push_back({p[idx].id,start,t});
        
        for(int i=0;i<n;i++) if(!inQ[i] && p[i].at<=t && p[i].rem>0) Q.push_back(i),inQ[i]=true;
        if(p[idx].rem>0) Q.push_back(idx);
        else{ p[idx].ct=t; p[idx].tat=t-p[idx].at; p[idx].wt=p[idx].tat-p[idx].bt; c++; }
    }
    print(p,"Round Robin");
    printGanttPreemptive(timeline);
}

void Banker(){
    int n,m;
    cout<<"\nProcesses: "; cin>>n;
    cout<<"Resources: "; cin>>m;

    vector<vector<int>> Max(n,vector<int>(m)),Alloc(n,vector<int>(m)),Need(n,vector<int>(m));
    vector<int>Avail(m);

    cout<<"MAX:\n";
    for(int i=0;i<n;i++) for(int j=0;j<m;j++) cin>>Max[i][j];

    cout<<"ALLOC:\n";
    for(int i=0;i<n;i++) for(int j=0;j<m;j++) cin>>Alloc[i][j];

    cout<<"AVAIL:\n";
    for(int j=0;j<m;j++) cin>>Avail[j];

    // Compute NEED
    for(int i=0;i<n;i++)
        for(int j=0;j<m;j++)
            Need[i][j] = Max[i][j] - Alloc[i][j];

    vector<int> work = Avail;
    vector<bool> finish(n,false);
    vector<int> safeSeq;

    int count = 0;
    while(count < n){
        bool found = false;

        for(int i=0;i<n;i++){
            if(!finish[i]){
                bool canRun = true;

                for(int j=0;j<m;j++){
                    if(Need[i][j] > work[j]){
                        canRun = false;
                        break;             
                    }
                }

                if(canRun){
                    for(int j=0;j<m;j++)
                        work[j] += Alloc[i][j];

                    finish[i] = true;
                    safeSeq.push_back(i);
                    count++;
                    found = true;
                    break;                 
                }
            }
        }

        if(!found){
            cout<<"\nUNSAFE STATE\n";
            return;
        }
    }

    cout<<"\nSAFE SEQUENCE: ";
    for(int i=0;i<n;i++){
        cout<<"P"<<safeSeq[i]+1;
        if(i<n-1) cout<<" -> ";
    }
    cout<<"\n";
}
