#include<bits/stdc++.h> 
#include<limits.h>
#include "Functions.h"

using namespace std; 

//Find stops of a route and fill RouteStops
void ReadNwFile(){

    ifstream file("Data_input/NWDescFile");
    string str;

    int firstline = 0;
    string routeno="";
   	long long start_pos;
    long long num_stops= 0;

    while (getline(file, str)) {

        //if a new route starting
        if(str.find("route:")==0){
          
          //Store previous route data  
        	if(routeno!=""){
        		Routes[routeno] = {INF , num_stops, INF, start_pos};
        	}

          //Get routeno

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
        //Get route stops
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

          //If firstline of route, store both stops else store only second stop

          if(firstline==1){
              start_pos = RouteStops.size();
              RouteStops.push_back(words[0]);
              num_stops++;
          }

          RouteStops.push_back(words[1]);
          num_stops++;    
        }
    }

    Routes[routeno] = {INF , num_stops, INF, start_pos};

    file.close();

}

//File sorted on tripId and time
void TripFile(){

    ifstream file("Data_input/tripData_tripIDsorted_date1");
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
       	
       	if(prevRoute=="")
       	{
       		prevRoute = route;
       	}

        //If new route, update info of previous route
       	if(route!=prevRoute){
        	Routes[prevRoute].num_trips = trips;
        	Routes[prevRoute].pointer_trip = start_pos;

       		start_pos = maxtillnow+1;
       		trips = 0;
       		
       		prevRoute = route;
       		
       	}

        //New trip
       	if(tripID!=prevTrip){

       		prevTrip = tripID;
       		int pos = tripID.find_last_of("_");
       		tripno = stoi(tripID.substr(pos+1,tripID.size()-pos-1));

       		current_stopno = 0;

       		//First stops not same, trip stretching to next day, skip that trip
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
  //Stores real size of StopTimes
  StopTimes_size = maxtillnow+1;
    
  file.close();
   
}

int create_DataStructures(){

	ReadNwFile();
	TripFile();

}