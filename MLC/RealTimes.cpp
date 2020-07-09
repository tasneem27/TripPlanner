#include<bits/stdc++.h> 
#include<limits.h>
#include "Functions.h"

void FillDs(){
	RealTime.clear();
	ifstream file("Data_input/tripData_timesorted");
	string str;

    string prevRoute="";
    string prevTrip="";
    string prevStop="";
    time_t prevTime=0;
    bool skipTrip = false;

    long long trips = 0, start_pos = 0, num_stops, tripno, current_stopno;
    long long maxtillnow = -1;


    time_t d_time = splitTime(departure_time);

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
        time_t arrTime = splitTime(words[2]);
        if(arrTime > d_time)
        	break;

        string tripID = words[0];
       	route = getRoute(tripID)+"_"+getDirection(tripID);
       	
		long long current_stoppos = find(RouteStops.begin()+Routes[route].pointer_stop,RouteStops.begin()+
						Routes[route].pointer_stop+Routes[route].num_stops,words[1]) - RouteStops.begin()-Routes[route].pointer_stop;
      
      	if(RealTime.find(route)==RealTime.end()){
      		RealTime.insert(pair<string,vector<time_t> >(route, vector<time_t>(Routes[route].num_stops,0)));
		} 	
       	
		RealTime[route][current_stoppos] = splitTime(words[2]);

     	
    }
}


pair<double,double> EstrealWaitTime(string route){

	long long source_pos = find(RouteStops.begin()+Routes[route].pointer_stop,RouteStops.begin()+
						Routes[route].pointer_stop+Routes[route].num_stops,source.substr(1,source.size()-1)) - RouteStops.begin()-Routes[route].pointer_stop;
     
	int j=-1;
    /* for(int i = 0;i<source_pos;i++){
     	if(RealTime[route][i]>RealTime[route][i+1]){
     		j=i;
     		//break;
     	}
     }*/

    for(int i = source_pos-1;i>=0;i--){
     	if(RealTime[route][i]>RealTime[route][i+1]){
     		j=i;
     		break;
     	}
    }  

	time_t nu_arrival = splitTime(departure_time);
	int interval = getWindow(nu_arrival);
	int second_interval = SecondbestWindow(nu_arrival);
	//---cout<<"interval: "<<interval<<" "<<second_interval<<endl;
	time_t window1_mean_time = splitTime("00:00:00")+((interval_duration*interval)+(interval_duration/2))*60;
	time_t window2_mean_time = splitTime("00:00:00")+((interval_duration*second_interval)+(interval_duration/2))*60;
	//---wcout<<"widow time: "<<wtime(window1_mean_time)<<" "<<wtime(window2_mean_time)<<endl;
	double weight1 = abs(window2_mean_time-nu_arrival);
	double weight2 = abs(window1_mean_time-nu_arrival);

    if(j==-1){
    	string routenode = route+"-"+source.substr(1,source.size()-1);
		double first_mean = wait_times[routenode][interval]*2;
		double second_mean = wait_times[routenode][second_interval]*2;
		
		double mean = ((weight1*first_mean)+(weight2*second_mean))/(weight1+weight2);
		mean = RealTime[route][source_pos]+(mean*60)-nu_arrival;
		if(mean<0)
			mean=0;
		double var = 0;
		//----cout<<"First part: "<<mean/60<<" "<<route<<" "<<first_mean<<" "<<second_mean;
		//---wcout<<wtime(RealTime[route][source_pos])<<endl;
		return make_pair(round(mean/60),var);
    }

    else{

    	double first_mean = 0;
    	double second_mean = 0;
    	double first_var = 0;
    	double second_var = 0;

    	for(int i=j;i<source_pos;i++){
    		string stop1 = RouteStops[Routes[route].pointer_stop+i];
    		string stop2 = RouteStops[Routes[route].pointer_stop+i+1];
    		string key = route+"-"+stop1+"_"+stop2;
			first_mean += mean_times[key][interval];
			second_mean += mean_times[key][second_interval];

			first_var += var_times[key][interval];
			second_var += var_times[key][second_interval];
    	}
    	//---cout<<"first_mean: "<<first_mean<<"second_mean: "<<second_mean<<" "<<weight1<<" "<<weight2<<endl;
    	double mean = (weight1*first_mean)+(weight2*second_mean);
		mean = round(mean/(weight1+weight2));

		double var = (weight1*first_var)+(weight2*second_var);
		var = var/(weight1+weight2);
		//---wcout<<"Initial mean: "<<mean<<" trip arrival"<<wtime(RealTime[route][j])<<" "<<j<<endl;
		mean = RealTime[route][j]+(mean*60)-nu_arrival;
		//---wcout<<"Mean before: "<<mean/60<<" "<<wtime(RealTime[route][j])<<" ";
		//---cout<<route<<" "<<j<<endl;
		if(mean<0)
			mean = 0;

		//---cout<<"Real wait: "<<mean<<" "<<var<<endl;
		return make_pair(round(mean/60),var);


    }

}