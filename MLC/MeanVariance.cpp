#include<bits/stdc++.h>
//#include "globals.h"
#include "Functions.h"

string mvfile;
string startfile;
string start_week_file;
time_t time1,time2;

//Calculate standard deviation
double SD(vector<double> list, double mean, int N){
	double sqsum=0;
	for(int i=0;i<N;i++){
		sqsum += ((list[i]-mean)*(list[i]-mean));
	}
	return sqrt(sqsum/N);
}

void MeanVariance(vector<unordered_map<string,vector<double>>> interval_stop_list, string route){

	//cout<<"Route: "<<route<<endl<<endl;
	for(int i=0;i<num_intervals;i++){
		//cout<<" INTERVAL NUMBER: "<<i<<endl;

    	unordered_map<string,vector<double>>::iterator itr;

    	for (itr = interval_stop_list[i].begin(); itr != interval_stop_list[i].end(); ++itr) 
    	{
 			//cout<<" Stop pair: "<<itr->first<<endl;
 			double sum = 0;
 			int count = 0;
 			for(int j=0; j<itr->second.size(); j++)
 			{
 				count++;
 				sum+=itr->second[j];
 				//cout<<itr->second[j]<<" ";
 			}  

 			double mean = sum/count;
 			double sd = SD(itr->second,mean,count);
 			//cout<<"  Mean: "<<mean<<" SD: "<<sd;
 			fr_out<<route<<","<<i<<","<<itr->first<<","<<mean<<","<<sd<<",\n";
 			//cout<<endl; 		
    	}
	}
}



void MeanVariance_main(){

	mvfile = "GeneratedFiles/MVfile_interval"+to_string(interval_duration)+".csv";
	startfile ="GeneratedFiles/BusStart_interval"+to_string(interval_duration)+".csv";
	start_week_file = "GeneratedFiles/BusStart_week_interval"+to_string(interval_duration)+".csv";

  fr_out.open (mvfile.c_str());
	//fr_out1.open (startfile.c_str());
  fr_out2.open(start_week_file.c_str());

	time1 = splitTime("00:00:00");
	time2 = splitTime("06:00:00");

	ifstream file("Data_input/tripData_tripIDsorted");
	string str;

    string prevRoute="";
    string prevTrip="";
    string prevStop="";
    time_t prevTime=0;
    bool skipTrip = false;

    vector<unordered_map<string,vector<double>>> interval_stop_list (num_intervals);

    unordered_map<string,vector<string>> StartTheBus;

   	while (getline(file, str)) {
        string word;
 		stringstream s(str); 
 		string route;
 		int counter = 0;

 		string words[3];
 		int wordno = 0;

        while (getline(s, word, ',')) { 
        	words[wordno] = word;
        	wordno++;
        }
            
        string tripID = words[0];
       	route = getRoute(tripID)+"_"+getDirection(tripID);
        string date = getDate(tripID);

       	if(prevRoute=="")
       	{
       		prevRoute = route;
          if(StartTheBus.find(route+"_"+date)==StartTheBus.end()){
            StartTheBus.insert(pair<string,vector<string> >(route+"_"+date, vector<string>()));
       		//fr_out1<<route<<",";
        }

       	}
       	if(route!=prevRoute){
          
          //Iterate start the bus and store
          for (auto k : StartTheBus){
            fr_out2<<k.first<<",";
            for(int sz = 0;sz<k.second.size();sz++)
              fr_out2<<k.second[sz]<<",";

            fr_out2<<"\n";
          } 
          


          StartTheBus.clear();
       		//fr_out1<<"\n";
       		//Do calculations and store
       		MeanVariance(interval_stop_list,prevRoute);

       		//Clear interval_stop_list
       		for(int i=0;i<num_intervals;i++){
       			interval_stop_list[i].clear();
       		}

       		prevRoute = route;

          if(StartTheBus.find(route+"_"+date)==StartTheBus.end()){
            StartTheBus.insert(pair<string,vector<string> >(route+"_"+date, vector<string>()));
       		//fr_out1<<route<<",";

       		//break;	//COMMENT IT
       	}
      }

       	if(tripID!=prevTrip){
       		prevTrip = tripID;
       		prevStop = words[1];
       		prevTime = splitTime(words[2]);
       		
       		if(checkRange(prevTime,time1,time2)){
   				skipTrip = true;
   			  }
   			  else{
   				skipTrip = false;

          StartTheBus[route+"_"+date].push_back(words[2]);
   				//fr_out1<<words[2]<<",";

   			  }			
       	}

       	else{
       		
       		if(skipTrip)
       			continue;

       		string currStop = words[1];
       		time_t currTime = splitTime(words[2]);

       		string stop_pair = prevStop+"_"+currStop;
       		double time_diff = difftime(currTime,prevTime);

       		time_t midTime = prevTime + (time_diff/2);

       		//wcout<<wtime(prevTime)<<" "<<wtime(midTime)<<endl;
       		//cout<<words[2]<<" "<<gmtime(&currTime)->tm_hour<<" ";
       		int window = getWindow(midTime);
       		//cout<<window<<endl;

       		if(interval_stop_list[window].find(stop_pair)==interval_stop_list[window].end()){
				interval_stop_list[window].insert(pair<string,vector<double> >(stop_pair, vector<double>()));
       		}
       		interval_stop_list[window][stop_pair].push_back(time_diff);
     
       		prevStop = currStop;
       		prevTime = currTime;
       	} 			
    }

    MeanVariance(interval_stop_list,prevRoute); 
     //Iterate start the bus and store
          for (auto k : StartTheBus){
            fr_out2<<k.first<<",";
            for(int sz = 0;sz<k.second.size();sz++)
              fr_out2<<k.second[sz]<<",";

            fr_out2<<"\n";
          } 
          
    file.close();
    fr_out.close();
    //fr_out1.close();
    fr_out2.close(); 
}
