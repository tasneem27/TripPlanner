#include<bits/stdc++.h>

extern std::string source;
extern std::string destination;
extern std::string d_time;
extern time_t departure_time;
extern int K;
extern int max_transfers;


struct Node
{
	long long num_trips;
	long long num_stops;
	long long pointer_trip;
	long long pointer_stop;
};

extern long long INF;
extern std::unordered_map<std::string,Node> Routes;
extern std::vector<std::string> RouteStops;
extern long long curr_size;
extern std::vector<time_t> StopTimes;
extern long long StopTimes_size;
extern std::unordered_map<std::string,std::vector<std::string>> StopRoutes;
extern std::unordered_map<std::string,std::vector<std::pair<time_t,std::string>>> StopsCostLabels;
extern std::unordered_map<std::string,time_t> EarliestArrivals;
extern std::ofstream result_out;
