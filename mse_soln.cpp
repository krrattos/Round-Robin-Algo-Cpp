/*Mayank Singh
  2111CS07
  Mtech CSE 1st yr*/
#include<bits/stdc++.h>
using namespace std;
vector<vector<int>>ans;         //to hold answers. 
unordered_map<int,bool>ready;     //a map to know if a process has arrived or not. ready=0 ie process not arrived
int current_time=0;  //the current time
int context=0;
int ready_processes=0;
queue<int>ready_q;      //queue to find the next process to execute

void find_ready_processes(vector<pair<int,int>>&input , int process)       //function to update the ready map to know which processes are currently ready to execute
{
    ready_processes=0;
    typedef pair<int,int>pr;
    priority_queue<pr,vector<pr>,greater<pr>>pq;
    for(int i=0;i<process;i++)
    {
        if(input[i].first<=current_time)
            {
                if(ready[i]==0)
                pq.push({input[i].first,i});
                ready[i]=1;
            }
    }
    while(!pq.empty())
    {
        ready_q.push(pq.top().second);
        pq.pop();
    }
    for(int i=0;i<process;i++)
    {
        if(ready[i]==1)
        ready_processes++;
    }
}

vector<pair<int,int>> compute_gantt(vector<pair<int,int>> input,int process,int tq)
{
    ready.clear();
    int net_bt=0;      //it is the total sum of all the burst times
    current_time=0;
    ready_processes=0;
   

    for(int i=0;i<process;i++)
    {
        ready[i]=0;
        net_bt+=input[i].second;
    }
    vector<pair<int,int>>gantt;     //the gantt chart where first parameter is process number, second is the current time

    find_ready_processes(input,process);
    while(net_bt>0)
    {
        
        if( ready_processes>0 )
        {
            int x=ready_q.front();
            ready_q.pop();

            if(input[x].second>=tq)
            {
                current_time+=tq;
                net_bt=net_bt-tq;
                input[x].second-=tq;
            }
            else
            {
                net_bt=net_bt-input[x].second;
                current_time+=input[x].second;
                input[x].second=0;
            }
            // int x=find_min(input,process);
            if(gantt.size()!=0&&gantt[gantt.size()-1].first!=x)
            context++;
            gantt.push_back({x,current_time});
            // input[x].second--;
            find_ready_processes(input,process);
            if(input[x].second>0)
                ready_q.push(x);
            // net_bt--;
        }
        else
        current_time++; 
    }
    return gantt;
}

void compute_completion_time(vector<pair<int,int>>&gantt)
{
     for(int i=gantt.size()-1;i>=0;i--)
    {
        if(ans[gantt[i].first][3]==-1)
            ans[gantt[i].first][3]=gantt[i].second;
    }
    return;
}

void compute_remaining_parameters(vector<pair<int,int>>&input, vector<pair<int,int>>&gantt, int process)
{
    for(int i=0;i<process;i++)
    {
        ans[i][2]=ans[i][3]-input[i].first;
        ans[i][1]=ans[i][2]-input[i].second;
    }
    for(int i=0;i<gantt.size();i++)
    {
        if(ans[gantt[i].first][0]==-1)
        {
            ans[gantt[i].first][0]=gantt[i].second-1;
        }
    }
    return;
}
vector<double> compute_avg(int process)
{
    vector<double>avg(4);
    for(int i=0;i<process;i++)
    {
        avg[0]+=ans[i][0];
        avg[1]+=ans[i][1];
        avg[2]+=ans[i][2];
        avg[3]+=ans[i][3];
    }
    avg[0]=avg[0]/process;
    avg[1]=avg[1]/process;
    avg[2]=avg[2]/process;
    avg[3]=avg[3]/process;
    return avg;
}

int main()
{
    int process=0;      //count of number of processes
    context=0;
    int tq=0;
    cout<<"\n********************************************************************************************************\nEnter Number of Processes (P): ";
        cin>>process;
        while(process<4||process>10)
        {
            cout<<"\nEnter Valid Number of Processes P in range 4 to 10 :";
            cin>>process;
        }
        vector<pair<int,int>>input(process);        //to hold the processes 

        ans.clear();
        ans.resize(process,vector<int>(4,-1));      //columns are : response time, waiting time, turn around time, completion time 

        bool flag=0;
        typedef pair<int,int>pr;
        priority_queue<pr,vector<pr>,greater<pr>>pq;    //pair to temporarily hold processes in ascending arrival times

        for(int i=0;i<process;i++)      //taking inputs
        {
            cout<<"\nEnter arrival time and burst time of process P"<<i<<": ";
            cin>>input[i].first;
            if(input[i].first==0)
            flag=1;
            while(input[i].first<0)
            {
                cout<<"\nEnter valid positive Arrival Time of process P"<<i<<": ";
                cin>>input[i].first;
            }
            cin>>input[i].second;
            while(input[i].second<0)
            {
                cout<<"\nEnter valid positive Burst Time of process P"<<i<<": ";
                cin>>input[i].second;
            }
            pq.push({input[i].first,i});
        }
        if(flag==0)
        {
            cout<<"There is no process with arrival time of 0\nExiting.....";
            return 0;
        }

        cout<<"\nEnter Time Quantum: ";
        cin>>tq;
        while(tq<=0)
        {
            cout<<"\nEnter Valid Time Quantum ";
            cin>>tq;
        }
        vector<pair<int,int>>gantt; //the gantt chart
        vector<double>avg(4);       //array to hold averages
       
        gantt=compute_gantt(input,process,tq);
        compute_completion_time(gantt);
        compute_remaining_parameters(input,gantt,process);
        avg=compute_avg(process);
        ///////////////////////////////outputs:///////////////////////////////
         cout<<"\n********************************************************************************************************\nOutputs:\n";
        cout<<"\nGantt Chart:\n0";
        for(int i=0;i<gantt.size();i++)
        cout<<" -> |P"<<gantt[i].first<<"| <- "<<gantt[i].second;

        cout<<"\n\n\nP      AT    BT   TAT    WT\n--------------------------------------------------";

        while(!pq.empty())
        {
            int i=pq.top().second;
            pq.pop();
            cout<<"\nP"<<i<<setw(6)<<input[i].first<<setw(6)<<input[i].second<<setw(6)<<ans[i][2]<<setw(6)<<ans[i][1];
        }
        cout<<"\n--------------------------------------------------";
    //    cout<<"\n\nAverage Response Time: "<<avg[0];
       cout<<"\n\nAverage Waiting Time: "<<avg[1];
       cout<<"\nAverage Turnaround Time: "<<avg[2];
       cout<<"\nTotal Context Switches: "<<context;
    //    cout<<"\nAverage Completion Time: "<<avg[3];       
       cout<<"\n********************************************************************************************************";
    
    return 0;
}