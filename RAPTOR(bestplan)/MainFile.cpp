#include <bits/stdc++.h>
#include "globals.h"
#include "Functions.h"
#include "DS.cpp"
#include "Raptor.cpp"

long long INF = 100000;
int K = 3;
int max_transfers = K-1;

unordered_map<string,Node> Routes;
vector<string> RouteStops;
long long curr_size = 1000000;
vector<time_t> StopTimes(curr_size);
long long StopTimes_size;
unordered_map<string,vector<string>> StopRoutes;
unordered_map<string,vector<pair<time_t,string>>> StopsCostLabels;
unordered_map<string,time_t> EarliestArrivals;

string source,destination,d_time;
time_t departure_time;

std::ofstream result_out;

int main(){
	result_out.open ("Outfiles/Results2last.csv");

	cout<<"Enter source: ";
	cin>>source;

	cout<<"Enter destination: ";
	cin>>destination;

	cout<<"Enter departure time: ";
	cin>>d_time;
	/*source = "60";
	destination = "4";
	d_time = "13:00";--*/

	departure_time = splitTime(d_time);

	create_DataStructures();
	main_Algo();
	
	
	/*------ifstream file("/home/user/PycharmProjects/MLC/Data/stage_list_random200")*/;
	/*ifstream file("Results.csv");

	string str;
	
	create_DataStructures();
   	while (getline(file, str)) {
        string word;
 		stringstream s(str); 

 		//string words[3];
 		int wordno = 0;

        while (getline(s, word, ',')) { 
        	if(wordno==0)
        		source = word;
        	if(wordno==1)
        		destination = word;
        	if(wordno==2)
        		d_time = word;
        	wordno++;
        }
        //if(d_time=="23:00")
        //	continue;
        
        departure_time = splitTime(d_time);
 			
 			/*Routes.clear();
			RouteStops.clear();
			curr_size = 1000000;
			StopTimes.clear();
			StopTimes.resize(curr_size);
			StopRoutes.clear();
			StopsCostLabels.clear();
			EarliestArrivals.clear();

			//create_DataStructures();
		main_Algo();
    }*/
    
    
    result_out.close();
	
}
