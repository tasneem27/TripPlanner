#include "Functions.h"
#include "globals.h"

//Checks if given time t is between time1 and time2
bool checkRange(time_t t, time_t time1, time_t time2) {
    if (difftime(t,time1)>=0 && difftime(time2,t)>0)
        return true;
    else
    	return false;
}

//Finds the window in which time belongs
//window starts from 0
int getWindow(time_t time){
	// string s = time[0]+time[1];
	// int hour = stoi(s);
	// s = time[2]+time[3];
	// int min = stoi(s);
	//struct tm *tmp = gmtime(&time);
	struct tm *tmp = std::localtime(&time);
	int hour = tmp->tm_hour;
	int min = tmp->tm_min;
	int window = ((hour*60)+min)/interval_duration;
	//wcout<< hour<< " "<<min<<" "<<window<<endl;
	return window;
}

int SecondbestWindow(time_t time){
	struct tm *tmp = std::localtime(&time);
	int hour = tmp->tm_hour;
	int min = tmp->tm_min;
	int sec = tmp->tm_sec;
	int window = ((hour*60)+min)/interval_duration;

	if(window-1<0)
		return window+1;
	else if(window+1>=num_intervals)
		return window-1;
	else{
		//wcout<<"In second window: "<<wtime(time)<<" "<<window<<" ";
		//double time1 = (hour*60*60)+(min*60)+sec;
		if(((splitTime("00:00:00")+(interval_duration*(window+1)*60)-time)/60)<(interval_duration/2)){
			//cout<<window+1<<endl;
			return window+1;

		}
		else{
			//cout<<window-1<<endl;
			return window-1;
		}
	}
}


std::string getRoute(std::string tripid){
	return tripid.substr(0, tripid.find("_"));
}

std::string getDirection(std::string tripid){
    unsigned first = tripid.find("_");
    unsigned last = tripid.find("_",first+1);
    return tripid.substr (first+1,last-first-1);
}

std::string getDate(std::string tripid){
	unsigned first = tripid.find("_");
    unsigned second = tripid.find("_",first+1);
    unsigned third = tripid.find("_",second+1);
    return tripid.substr (second+1,third-second-1);
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
