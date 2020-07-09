#include "Functions.h"
#include "globals.h"


std::string getRoute(std::string tripid){
	return tripid.substr(0, tripid.find("_"));
}

std::string getDirection(std::string tripid){
    unsigned first = tripid.find("_");
    unsigned last = tripid.find("_",first+1);
    return tripid.substr (first+1,last-first-1);
}

time_t splitTime(std::string ticket_time){
    struct tm tm1 = {0};
    ticket_time = "1970-01-01 "+ticket_time;
    strptime(ticket_time.c_str(), "%Y-%m-%d %H:%M:%S", &tm1);
    time_t t = mktime(&tm1);
    return t;
}

std::wstring wtime(const time_t &t) {
    std::tm tm = *std::localtime(&t);
    std::wstringstream wss;
    wss << std::put_time(&tm, L"%T");
    return wss.str();
}