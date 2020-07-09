#include <bits/stdc++.h>
#include "globals.h"
#include "MeanVariance.cpp"
#include "EstWaitingTime.cpp"
#include "createGraph.cpp"
#include "ShortestPath.cpp"
#include "DS.cpp"

int interval_duration,num_intervals;
string source,destination,departure_time;
std::ofstream fr_out,fr_out1,fr_out2;
unordered_map<string,vector<string>> adj;
long long INF = 100000;
unordered_map<string,vector<double>> mean_times;
unordered_map<string,vector<double>> var_times;
unordered_map<string,vector<double>> wait_times;
std::ofstream result_out;

//RAPTOR DS to get actual time taken
unordered_map<string,Node> Routes;
vector<string> RouteStops;
long long curr_size = 1000000;
vector<time_t> StopTimes(curr_size);
long long StopTimes_size;

//Use real waiting time for first stop
unordered_map<string,vector<time_t>> RealTime;

int main(){
	
	cout<<"Enter interval duration (in mins): ";
	cin>>interval_duration;
	
	num_intervals = (24*60)/interval_duration;


	//Initialization
		//Raptor
	//Fills Routes, RouteStops and StopTimes
	create_DataStructures();
	
	//Generates mean,variance for travel and wait times for stops in buses in interval
	ifstream ifile;
   	ifile.open("GeneratedFiles/MVfile_interval"+to_string(interval_duration)+".csv");
   	if(ifile) {
      //cout<<"file exists";
   	}
   	else{ 
		MeanVariance_main();
		EstWaitTime_main();
	}

	//Create transportation graph with route and transfer nodes
	createGraph_main();

	//Output file
	result_out.open ("Outfiles/OutputFileRealdata_60.csv");

	
	/*Uncomment if give manual source, destination and departure time*/

	
	cout<<"Enter source: ";
	cin>>source;
	source = "t"+source;

	cout<<"Enter destination: ";
	cin>>destination;
	destination = "t"+destination;

	cout<<"Enter departure time: ";
	cin>>departure_time;

	FillDs();
 	ShortestPath_main("");


	/*Next lines (till 93) takes inputs from file and calls shortest path*/
	/*ifstream file("Results.csv");
	string str;

   	while (getline(file, str)) {
        string word;
 		stringstream s(str); 

 		int wordno = 0;

        while (getline(s, word, ',')) { 
        	if(wordno==0)
        		source = "t"+word;
        	if(wordno==1)
        		destination = "t"+word;
        	if(wordno==2)
        		departure_time = word;
        	//departure_time = "07:00";
        	wordno++;
        }

        //Using departure time, fill RealTime Ds to store latest bus time that
        //reached a route stop
       	FillDs();

       	//Run modified dijsktra on graph
 		ShortestPath_main(str);
 	}
	*/

 	result_out.close();
}