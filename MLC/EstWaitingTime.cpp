#include<bits/stdc++.h> 
#include "Functions.h"
//#include "globals.h"

/*
TEMP
*/


unordered_map<string,vector<string>> bus_start_list;
unordered_map<string,vector<int>> route_stop_buscounts;
unordered_map<string,vector<double>> mean_times_local;

string freqfile;

void FindWaitTime(vector<string> routesStops,string routeno){
	
    for(int i=0;i<routesStops.size();i++){
        fr_out<<routeno+"-"+routesStops[i]<<",";

		for(int j=0;j<num_intervals;j++){
			double wij=0;
            int count=0;
            for(int day=1;day<=7;day++){

			     double fij = double(route_stop_buscounts[routeno+"_"+to_string(day)+"-"+routesStops[i]][j])/interval_duration;
			     //cout<<" Wait "<<routeno+"-"+routesStops[i]<<" "<<j<<" "<<route_stop_buscounts[routeno+"-"+routesStops[i]][j]<<endl;
			     //cout<<fij<<" ";
                 if(fij!=0){
				    wij += (0.5/fij);
                    count++;
                    //cout<<"wij now: "<<wij<<" "<<count<<endl;

                }
            }
			//cout<<wij<<" "<<count;
            if(count!=0)
                wij = wij/count;
            else
                wij = INF;
            //cout<<routeno+"-"+routesStops[i]<<" "<<wij<<endl;
            fr_out<<wij<<",";
		}
		fr_out<<"\n";
	}
}


void FindFreq(vector<string> routesStops, string routeno){
	
    for(int day=1;day<=7;day++){
	   for(int i=0;i<bus_start_list[routeno+"_"+to_string(day)].size();i++){
		
		time_t prevTime;
		for(int j=0;j<routesStops.size();j++){
			time_t arrTime;
			int arrInterval;
			if(j==0){
				arrTime = splitTime(bus_start_list[routeno+"_"+to_string(day)][i]);
				arrInterval = getWindow(arrTime);

			}

			else{
				int interval = getWindow(prevTime);

				double mean_time;
				string search_key = routeno+"-"+routesStops[j-1]+"_"+routesStops[j];
				
				if(mean_times_local[search_key][interval]!=INF){
					arrTime = prevTime+mean_times_local[search_key][interval];
					arrInterval = getWindow(arrTime);

				}

				else if(mean_times_local[search_key][interval+1]!=INF){
					arrTime = prevTime+mean_times_local[search_key][interval+1];
					arrInterval = getWindow(arrTime);

				}

				else{
					cout<<"Error: route="<<routeno<<" bus_start="<<bus_start_list[routeno][i]<<" at_stop_index="<<j<<endl;
					break;
				}
			}

			string key = routeno+"_"+to_string(day)+"-"+routesStops[j];
			if(route_stop_buscounts.find(key)==route_stop_buscounts.end())
 				route_stop_buscounts.insert(pair<string,vector<int> >(key, vector<int>(num_intervals,0)));

			route_stop_buscounts[key][arrInterval]++;
			//cout<<key<<" "<<arrInterval<<" "<<route_stop_buscounts[key][arrInterval]<<endl;
			prevTime = arrTime;
		}
	}
    
    }
	
    FindWaitTime(routesStops,routeno);
}

int  EstWaitTime_main(){
	freqfile ="GeneratedFiles/Waitfile_interval"+to_string(interval_duration)+".csv";	

	fr_out.open (freqfile.c_str());

	//cout<<"Before open";
	ifstream file("GeneratedFiles/BusStart_week_interval"+to_string(interval_duration)+".csv");
	string str;
	//cout<<"After open";
   	while (getline(file, str)) {
        string word;
 		stringstream s(str); 
 		string route;

 		vector<string> start_times;
 		int wordno = 0;

        while (getline(s, word, ',')) { 
        	if(wordno==0){
        		route=word;
        	}
        	else{
        		start_times.push_back(word);
        	}
        	wordno++;
        }
        bus_start_list[route]=start_times;
    }

    file.close();

	ifstream file2("GeneratedFiles/MVfile_interval"+to_string(interval_duration)+".csv");

   	while (getline(file2, str)) {
        string word;
 		stringstream s(str); 
 		string route;

 		vector<string> start_times;
 		int wordno = 0;
 		int interval;
 		double mean;

        while (getline(s, word, ',')) { 
        	if(wordno==0){
        		route=word+"-";
        	}
        	else if(wordno==1){
        		interval = stoi(word);
        	}
        	else if(wordno==2){
        		route+=word;
        	}
        	else if(wordno==3){
        		mean = stof(word);
        	}
        	wordno++;
 		}
 		if(mean_times_local.find(route)==mean_times_local.end()){
 			mean_times_local.insert(pair<string,vector<double> >(route, vector<double>(num_intervals,INF)));
 		}
		mean_times_local[route][interval]=mean;    	

	}

	file2.close();

	ifstream file3("Data_input/NWDescFile");
    string prevNode;

    int firstline = 0;
    string routeno="";
    vector<string> routesStops;


    while (getline(file3, str)) {
        if(str.find("route:")==0){
        	//Do something with prevRoute
        	if(routesStops.size()!=0){
        		FindFreq(routesStops,routeno);
                //break;
            }

        	routesStops.clear();

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

        }
        
        else{

            firstline++;
        
            istringstream ss(str); 
            string words[3];
            int wordno=0;
            

            do { 
                ss >> words[wordno];
                wordno++;
            } while (ss); 

            if(firstline==1){
               routesStops.push_back(words[0]);
               routesStops.push_back(words[1]);

            }

            else{
               routesStops.push_back(words[1]);                
            }
           
            
        }
    }

    FindFreq(routesStops,routeno);

    file3.close();
    fr_out.close();
}