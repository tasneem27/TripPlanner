#include<bits/stdc++.h>
extern int interval_duration;
extern int num_intervals;
extern std::string source;
extern std::string destination;
extern std::string departure_time;
extern std::ofstream fr_out,fr_out1,fr_out2;
extern std::unordered_map<std::string,std::vector<std::string>> adj;
extern long long INF;
extern std::unordered_map<std::string,std::vector<double>> mean_times;
extern std::unordered_map<std::string,std::vector<double>> var_times;
extern std::unordered_map<std::string,std::vector<double>> wait_times;
extern std::ofstream result_out;


struct Node
{
	long long num_trips;
	long long num_stops;
	long long pointer_trip;
	long long pointer_stop;
};

extern std::unordered_map<std::string,Node> Routes;
extern std::vector<std::string> RouteStops;
extern long long curr_size;
extern std::vector<time_t> StopTimes;
extern long long StopTimes_size;

extern std::unordered_map<std::string,std::vector<time_t>> RealTime;