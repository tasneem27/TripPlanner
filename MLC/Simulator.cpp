//current_trip starts from 0 to num_trips-1
time_t EarliestTrip(string getin_stop,string route, string getoff_stop,time_t current_time){

	//if(current_trip == Routes[route].num_trips)
	//	current_trip--;
	int current_trip = Routes[route].num_trips-1;

	long long current_trip_startpos = Routes[route].pointer_trip + (current_trip* Routes[route].num_stops);
	long long current_trip_stoppos = find(RouteStops.begin()+Routes[route].pointer_stop,RouteStops.begin()+
						Routes[route].pointer_stop+Routes[route].num_stops,getin_stop) - RouteStops.begin()-Routes[route].pointer_stop;
	current_trip_stoppos += current_trip_startpos;

	long long dest_pos = find(RouteStops.begin()+Routes[route].pointer_stop,RouteStops.begin()+
						Routes[route].pointer_stop+Routes[route].num_stops,getoff_stop) - RouteStops.begin()-Routes[route].pointer_stop;

	long long stop_at = Routes[route].pointer_trip;


	int tripno = current_trip+1;
	//cout<<"Intial tripno: "<<tripno<<endl;
	while(current_trip_stoppos >= stop_at){
		//----wcout<<"StopTime: "<<wtime(StopTimes[current_trip_stoppos])<<endl;
		if(difftime(StopTimes[current_trip_stoppos],current_time)>0){
			tripno--;
			current_trip_stoppos -= Routes[route].num_stops;
		}
		else{
			if(tripno == current_trip+1){
				return 0;
			}
			//cout<<"Tripno: "<<tripno<<endl;
			time_t arrtime = StopTimes[Routes[route].pointer_trip + (tripno* Routes[route].num_stops) + dest_pos];			
			return arrtime;
		}
	}
	//cout<<"Tripno: "<<tripno<<endl;

	time_t arrtime = StopTimes[Routes[route].pointer_trip + (tripno* Routes[route].num_stops) + dest_pos];			
	return arrtime;
	//return tripno;
}