#include<bits/stdc++.h> 
using namespace std; 

void addEdge(string u, string v) 
{ 
    if(adj.find(u)==adj.end())
        adj.insert(pair<string,vector<string> >(u, vector<string>()));
    
    adj[u].push_back(v); 

} 
  
// A utility function to print the adjacency list 
// representation of graph 
void printGraph() 
{ 
    int nodes = 0;
    int tnodes = 0;
    int rnodes = 0;

    unordered_map<string,vector<string>>::iterator itr;
    for (itr = adj.begin(); itr != adj.end(); ++itr) 
    { 
        nodes++;
        string v = itr->first;

        if(v[0]=='t')
            tnodes++;
        else{
            rnodes++;
            //cout<<v<<endl;
        }

        cout << "\n Adjacency list of vertex " << v << "\n head "; 
        for (auto x : adj[v]) 
           cout << "-> " << x; 
        printf("\n");
    } 

    cout<<"Total nodes: "<<nodes<<" "<<tnodes<<" "<<rnodes<<endl;
} 
  
// Driver code 
int createGraph_main() 
{ 

    ifstream file("Data_input/NWDescFile");
    string str;
    string prevNode;

    int firstline = 0;
    string routeno="";
    int rnodes=0;


    unordered_map<string,int> fromfile;

    while (getline(file, str)) {
        if(str.find("route:")==0){
            firstline = 0;
            istringstream ss(str); 
            string words[4];
            int wordno=0;

            do { 
                ss >> words[wordno];
                wordno++;
            } while (ss); 

            routeno = words[1];
            prevNode = "";

            fromfile[routeno]=0;
        }
        
        else{

            firstline++;
        
            istringstream ss(str); 
            string words[3];
            int wordno=0;
            string routeNode; 
            string transferNode;

            do { 
                ss >> words[wordno];
                wordno++;
            } while (ss); 

            if(firstline==1){
                routeNode = "r"+routeno+"-"+words[0];
                transferNode = "t"+words[0];
                addEdge(routeNode,transferNode);
                addEdge(transferNode,routeNode);

                string routeNode2 = "r"+routeno+"-"+words[1];
                string transferNode2 = "t"+words[1];
                addEdge(routeNode2,transferNode2);
                addEdge(transferNode2,routeNode2);


                addEdge(routeNode,routeNode2);
                prevNode=routeNode2;

                rnodes+=2;
                fromfile[routeno]+=2;
            }

            else{
                routeNode = "r"+routeno+"-"+words[1];
                transferNode = "t"+words[1];
                addEdge(routeNode,transferNode);
                addEdge(transferNode,routeNode); 

                addEdge(prevNode,routeNode);
                prevNode=routeNode;

                rnodes++;
                fromfile[routeno]+=1;
            }
           
            
        }
    }


   // printGraph(); 

    //cout<<"Routenodes: "<<rnodes<<endl;
    file.close();

    /*
    FINDING ROUND TRIPS
    unordered_map<string,int> fromgraph;


    unordered_map<string,vector<string>>::iterator itr;
    for (itr = adj.begin(); itr != adj.end(); ++itr) 
    { 
        //cout<<"hello";
        string v = itr->first;
        //cout<<v<<endl;
        if(v[0]=='r')
        {
            string route = v.substr(1, v.find("-")-1);
            cout<<"L"<<route;
            if(fromgraph.find(route)==fromgraph.end())
                fromgraph[route]=1;
            else
                fromgraph[route]++;
        }

    }

    unordered_map<string,int>::iterator it;


    cout<<"COMPARE..............."<<endl;
    for (it = fromfile.begin(); it != fromfile.end(); ++it) 
    {
        if(fromfile[it->first]!=fromgraph[it->first])
            cout<<"\""<<it->first<<"\",";
    } 

    */
    return 0; 

} 
