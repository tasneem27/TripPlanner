#include<bits/stdc++.h> 
#include<algorithm>
#include<limits.h>
#include"Functions.h"

using namespace std; 

vector<string> MarkedStops;
unordered_map<string,string> RoutestoScan;


//current_trip starts from 0 to num_trips-1
long long EarliestTrip(string route, string stop, int current_trip, time_t current_time){

	//if(current_trip == Routes[route].num_trips)
	//	current_trip--;

	long long current_trip_startpos = Routes[route].pointer_trip + (current_trip* Routes[route].num_stops);
	long long current_trip_stoppos = find(RouteStops.begin()+Routes[route].pointer_stop,RouteStops.begin()+
						Routes[route].pointer_stop+Routes[route].num_stops,stop) - RouteStops.begin()-Routes[route].pointer_stop;
	current_trip_stoppos += current_trip_startpos;

	long long stop_at = Routes[route].pointer_trip;


	int tripno = current_trip+1;

	while(current_trip_stoppos >= stop_at){
		//----wcout<<"StopTime: "<<wtime(StopTimes[current_trip_stoppos])<<endl;
		if(difftime(StopTimes[current_trip_stoppos],current_time)>0){
			tripno--;
			current_trip_stoppos -= Routes[route].num_stops;
		}
		else{
			if(tripno == current_trip+1)
				return -1;
			return tripno;
		}
	}

	return tripno;
}

time_t minTime(time_t t1,time_t t2){
	if(difftime(t1,t2)>0)
		return t2;
	return t1;
}


void scanRoutes(int k){
	MarkedStops.clear();

	unordered_map<string,string>::iterator itr;
	for(itr = RoutestoScan.begin(); itr!= RoutestoScan.end(); itr++){
		string route = itr->first;
		string u = itr->second;
		//---cout<<route<<" "<<u<<endl;
		string parent;
		//Find earliest trip to catch 
		//int t = EarliestTrip(route,u,Route[route].num_trips,StopsCostLabels[u][k-1].first)
		//if(t==-1)
		//	continue;

		long long tr = Routes[route].num_trips-1;
		bool first = true;

		long long pos = find(RouteStops.begin()+Routes[route].pointer_stop,RouteStops.begin()+
						Routes[route].pointer_stop+Routes[route].num_stops,u) - RouteStops.begin();

		long long last_pos = Routes[route].pointer_stop+Routes[route].num_stops-1;

		for(int i=pos; i<=last_pos; i++){
			string stop = RouteStops[i];
			//---cout<<"Stops: "<<stop<<" "<<pos<<" "<<last_pos<<" tripno: "<<tr<<" "<<Routes[route].num_trips<<endl;
			long long index = Routes[route].pointer_trip + (tr* Routes[route].num_stops) + (i - Routes[route].pointer_stop);


			if(!first){
				time_t min = minTime(EarliestArrivals[stop],EarliestArrivals[destination]);
				if(difftime(StopTimes[index],min)<0){
					StopsCostLabels[stop][k].first = StopTimes[index];
					EarliestArrivals[stop] = StopTimes[index];
					//if(stop==u){
					StopsCostLabels[stop][k].second = parent;
					//}

					//else{
					//	StopsCostLabels[stop][k].second = RouteStops[i-1];					
					//}

					MarkedStops.push_back(stop);
				}
			}

			if(difftime(StopsCostLabels[stop][k-1].first,StopTimes[index])<=0){
				first = false;
				long long prev_tr = tr;
				tr = EarliestTrip(route,stop,tr,StopsCostLabels[stop][k-1].first);
				if(tr==-1)
					break;
				if(tr!=prev_tr){
					parent = stop+"-"+route;
				}
			}

		}


	}
}

void PrintJourney(){
	
	result_out<<source<<","<<destination<<","<<d_time<<",";
	//cout<<source<<" "<<destination<<endl;
	cout<<endl;
	cout<<"Source: "<<source<<" Destination: "<<destination<<" Departure_time: "<<d_time<<endl<<endl; 
	for(int k=1;k<=K;k++){
		cout<<"Plan with "<<k<<" trips"<<endl;

		if(StopsCostLabels[destination][k].second=="INFINITY")
		{
			cout<<"\t No plans"<<endl;
			result_out<<"-1"<<",";
		}

		else{
			wcout<<"\t Best time taken to reach destination: "<<round((StopsCostLabels[destination][k].first-departure_time)/60)<<" minutes"<<endl<<endl;
			
			double travel_time = (StopsCostLabels[destination][k].first - departure_time)/60;
			result_out<<travel_time<<",";

			stack<string> st;
			int i = k;
			st.push(destination);
			string node = destination;
			while(i>0){
				string temp = StopsCostLabels[node][i].second;
				st.push(temp);
				node = temp.substr(0,temp.find("-"));
				i--;
			}
			//cout<<"Source: "<<source<<endl;
			while(!st.empty()){
				size_t pos = st.top().find("-");
				if(pos==string::npos){
					cout<<"\t Get down at destination: "<<st.top();
				}
				else{
					string stop = st.top().substr(0,pos);
					string route = st.top().substr(pos+1,st.top().size()-pos-1);
					cout<<"\t Take bus "<<route<<" at stop "<<stop<<" then"<<endl;
				}
				//cout<<st.top()<<" ";
				st.pop();
			}
		}
		cout<<endl<<endl;
	}
	result_out<<"\n";
}


int main_Algo(){

	if(StopsCostLabels.find(source)==StopsCostLabels.end() ||
		StopsCostLabels.find(destination)==StopsCostLabels.end()){
		cout<<"BAD: "<<source<<" "<<destination<<endl;
		return 0;
	}

	unordered_map<string,vector<pair<time_t,string>>>::iterator cost_itr;
	for(cost_itr = StopsCostLabels.begin(); cost_itr!= StopsCostLabels.end(); cost_itr++){
		cost_itr->second = vector<pair<time_t,string>>(K+1,make_pair(numeric_limits<time_t>::max(),"INFINITY"));
		EarliestArrivals[cost_itr->first] = numeric_limits<time_t>::max();	
	}






	StopsCostLabels[source][0].first = departure_time;
	StopsCostLabels[source][0].second = "undef";
	EarliestArrivals[source] = departure_time;
	MarkedStops.push_back(source);

	for(int k=1;k<=K;k++){

		/*unordered_map<string,vector<string>>::iterator cost_itr;
		for(cost_itr = StopsCostLabels.begin(); cost_itr!= StopsCostLabels.end(); cost_itr++){
			cost_itr.second[k] = cost_itr.second[k-1];
		}*/



		RoutestoScan.clear();
		for(int i=0;i<MarkedStops.size();i++){
			string marked_stop = MarkedStops[i];
			//All routes serving a marked stop
			for(int j=0;j<StopRoutes[marked_stop].size();j++){
				
				string route = StopRoutes[marked_stop][j];
				
				if (RoutestoScan.find(route)!= RoutestoScan.end()){

					long long pos1 = find(RouteStops.begin()+Routes[route].pointer_stop,RouteStops.begin()+
						Routes[route].pointer_stop+Routes[route].num_stops,marked_stop) - RouteStops.begin();
					long long pos2 = find(RouteStops.begin()+Routes[route].pointer_stop,RouteStops.begin()+
						Routes[route].pointer_stop+Routes[route].num_stops,RoutestoScan[route]) - RouteStops.begin();

					if(pos1 < pos2){
						RoutestoScan[route] = marked_stop;
					}
					//RoutestoScan[route] = min(marked_stop,RoutestoScan[route]);
				}

				else{
					RoutestoScan[route] = marked_stop;
				}
			}	
		}

		scanRoutes(k);

		if(MarkedStops.size()==0)
			break;	
	}


	PrintJourney();
}
