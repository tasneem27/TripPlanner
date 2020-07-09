#include<bits/stdc++.h> 
#include<limits.h>
#include "Functions.h"

using namespace std; 

void ReadNwFile(){
	ifstream file("Data/NWDescFile");
    string str;

    int firstline = 0;
    string routeno="";
   	long long start_pos;
    long long num_stops= 0;

    while (getline(file, str)) {
        if(str.find("route:")==0){
            
        	if(routeno!=""){
        		Routes[routeno] = {INF , num_stops, INF, start_pos};
        	}

            firstline = 0;
            num_stops = 0;


            istringstream ss(str); 
            string words[4];
            int wordno=0;

            do { 
                ss >> words[wordno];
                wordno++;
            } while (ss); 

            routeno = words[1];

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
                start_pos = RouteStops.size();
                RouteStops.push_back(words[0]);
                num_stops++;

            	if(StopRoutes.find(words[0])==StopRoutes.end()){
             		StopRoutes.insert(pair<string,vector<string> >(words[0], vector<string>()));
            		StopsCostLabels.insert(pair<string,vector<pair<time_t,string>>> (words[0],vector<pair<time_t,string>>(K+1,make_pair(numeric_limits<time_t>::max(),"INFINITY"))));
            		EarliestArrivals[words[0]] = numeric_limits<time_t>::max();
            	}

             	StopRoutes[words[0]].push_back(routeno);

            }

            RouteStops.push_back(words[1]);
            num_stops++;

            if(StopRoutes.find(words[1])==StopRoutes.end()) {
             	StopRoutes.insert(pair<string,vector<string> >(words[1], vector<string>()));
            	StopsCostLabels.insert(pair<string,vector<pair<time_t,string>>> (words[1],vector<pair<time_t,string>>(K+1,make_pair(numeric_limits<time_t>::max(),"INFINITY"))));
               	EarliestArrivals[words[1]] = numeric_limits<time_t>::max();

            }

            StopRoutes[words[1]].push_back(routeno);

            
        }
    }

    Routes[routeno] = {INF , num_stops, INF, start_pos};

    
   	int total_routes = 0;
    unordered_map<string,Node>::iterator itr;
    for(itr = Routes.begin(); itr!=Routes.end(); itr++){
    	total_routes++;
    	int stops = itr->second.num_stops;
    	//---cout<<"Route: "<<itr->first<<" "<<stops<<endl;

    	/*---for(int i=itr->second.pointer_stop;i<itr->second.pointer_stop+stops;i++){
    		cout<<RouteStops[i]<<" ";
    	}
    	cout<<endl;*/
    }

    //---cout<<"Total routes"<<total_routes<<endl;

    file.close();

}

//File sorted on tripId and time
void TripFile(){

	ifstream file("Data/tripData");
	string str;

    string prevRoute="";
    string prevTrip="";
    string prevStop="";
    time_t prevTime=0;
    bool skipTrip = false;

    long long trips = 0, start_pos = 0, num_stops, tripno, current_stopno;
    long long maxtillnow = -1;

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
       	num_stops = Routes[route].num_stops;
       	//cout<<num_stops;

       	
       	if(prevRoute=="")
       	{
       		prevRoute = route;
       	}

       	if(route!=prevRoute){
        	//cout<<trips
        	Routes[prevRoute].num_trips = trips;
        	Routes[prevRoute].pointer_trip = start_pos;

       		start_pos = maxtillnow+1;
       		trips = 0;
       		
       		prevRoute = route;
       		
       	}

       	if(tripID!=prevTrip){
       		//---cout<<tripID<<" "<<trips<<endl;
       		prevTrip = tripID;
       		int pos = tripID.find_last_of("_");
       		tripno = stoi(tripID.substr(pos+1,tripID.size()-pos-1));

       		current_stopno = 0;

       		//First stops not same, trip stretching to next day
       		if(words[1]!=RouteStops[Routes[route].pointer_stop]){
       			skipTrip = true;
       		}
       		else{
       			trips++;
       			skipTrip = false;
       			long long index = start_pos + ((tripno-1)*num_stops) + current_stopno; 
       			if(index > maxtillnow)
       				maxtillnow = index;

       			if(index>=curr_size){
       				curr_size*=2;
       				StopTimes.resize(curr_size);
       			}

       			StopTimes[index] = splitTime(words[2]);
       			current_stopno++;
       		}
       		
       	}
       	
       	else{
       		if(skipTrip)
       		 	continue;
       		else{
       			long long index = start_pos + ((tripno-1)*num_stops) + current_stopno; 
       			if(index > maxtillnow)
       				maxtillnow = index;
				
				if(index>=curr_size){
       				curr_size*=2;
       				StopTimes.resize(curr_size);
       			}

       			StopTimes[index] = splitTime(words[2]);
       			current_stopno++;
       		}

       	} 		
    }

	Routes[prevRoute].num_trips = trips;
    Routes[prevRoute].pointer_trip = start_pos;
    
    //----cout<<maxtillnow<<endl;
    StopTimes_size = maxtillnow+1;
    
    int loc = Routes["9_U"].pointer_trip +  (92*27) + 9;
    time_t temp = StopTimes[loc];
    //----wcout<<wtime(temp);

    file.close();
   
}

int create_DataStructures(){

	ReadNwFile();
	TripFile();

}