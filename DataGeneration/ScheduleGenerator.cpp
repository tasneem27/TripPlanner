#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <bits/stdc++.h> 
#include <locale>
#include <codecvt>

using namespace std;

unordered_map<string,vector<float>> speeds;
unordered_map<string,vector<float>> freq;
time_t time1;

time_t splitTime(string ticket_time){
    
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


int findWindow(time_t t) {

  float diff = difftime(t,time1);
  diff = diff/3600;
  return int(diff)-5;

}

void getSpeeds(){
  ifstream file("GeneratedFiles/SpeedModsFile");
  string str;

  while (getline(file, str)) {
    istringstream ss(str); 
    string words[19];
    int wordno=0;
    string routeno;

    do { 
      if(wordno==0)
        ss >> routeno;
      else
        ss >> words[wordno-1];
      
      wordno++;
    } while (ss); 


    vector<float> speedmod;
    for(int i=0;i<19;i++)
      speedmod.push_back(stof(words[i]));

    speeds[routeno]=speedmod;
  }
}

void getFreq(){
  ifstream file("GeneratedFiles/FreqModsFile");
  string str;

  while (getline(file, str)) {
    istringstream ss(str); 
    string words[19];
    int wordno=0;
    string routeno;

    do { 
      if(wordno==0)
        ss >> routeno;
      else
        ss >> words[wordno-1];
      
      wordno++;
    } while (ss); 

    vector<float> freqmod;
    for(int i=0;i<19;i++)
      freqmod.push_back(stof(words[i]));

    freq[routeno]=freqmod;
    //cout<<routeno<<freq[routeno][0]<<endl;
  }
}


void GenerateTrips(string routeno,vector<pair<string,float>> RouteStages,string date){

  //cout<<"Route: "<<routeno<<endl;
  float default_freq = freq[routeno][0];
  int tripno=0;
  time_t starttime = splitTime("06:00:00");
  time_t endtime = splitTime("23:00:00");
  //time_t prevtime = starttime;

  while(starttime<=endtime){
    tripno++;
    string tripID = routeno+"_"+to_string(tripno);

    std::wstring string_to_convert=wtime(starttime);
    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;
    std::string converted_str = converter.to_bytes( string_to_convert );

    cout<<tripID<<","<<RouteStages[0].first.substr(0,RouteStages[0].first.find("_"))<<","<<converted_str<<endl;


    time_t currtime = starttime;
    for(int i=1;i<RouteStages.size();i++){
      //cout<<RouteStages[i].first<<" "<<RouteStages[i].second<<" "<<speeds[RouteStages[i].first][0]<<" "<<freq[routeno][0]<<endl;
      int window = findWindow(currtime);
      float default_speed1 = speeds[RouteStages[i-1].first][0];
      float default_speed2 = speeds[RouteStages[i].first][0];   
      float dist = RouteStages[i].second;  

      float avg_speed = default_speed1+speeds[RouteStages[i-1].first][window]+default_speed2+speeds[RouteStages[i].first][window];
      avg_speed = avg_speed/2;

      float time_taken = (dist/avg_speed)*3600;

      /*float hours = int(time_taken);
      float mins = (time_taken-hours)*60;
      float secs = int((mins - int(mins))*60);
      mins = int(mins);

      struct tm *tmp = gmtime(&currtime);
      tmp->tm_hour+=hours;
      tmp->tm_min+=mins;
      tmp->tm_sec+=secs;
      currtime = mktime(tmp);*/

      currtime = currtime+time_taken;
      
      string_to_convert=wtime(currtime);
      converted_str = converter.to_bytes( string_to_convert );
      //cout<<converted_str<<endl;

      cout<<tripID<<","<<RouteStages[i].first.substr(0,RouteStages[i].first.find("_"))<<","<<converted_str<<endl;

    }

    //string_to_convert=wtime(currtime);
    //converted_str = converter.to_bytes( string_to_convert );
    //cout<<converted_str<<endl;

    int window = findWindow(currtime);
    float freq_new = (default_freq+freq[routeno][window])*60;

    /*float hours = int(freq_new);
    float mins = (freq_new-hours)*60;
    float secs = int((mins - int(mins))*60);
    mins = int(mins);

    struct tm *tmp = gmtime(&starttime);
    tmp->tm_hour+=hours;
    tmp->tm_min+=mins;
    tmp->tm_sec+=secs;
    starttime = mktime(tmp);*/

    starttime = starttime+freq_new;

  }

  
}





int main ()
{

  getSpeeds();
  getFreq();

  //Generating one week tripData
  for(int day=1;day<=7;day++){
    string date = to_string(day);

    set<string> routes_dir;

  	ifstream file("GeneratedFiles/NWDescFile");
    string str;

    int firstline = 0;
    vector<pair<string,float>> RouteStages;
  	string routeno="";
    time1 = splitTime("00:00:00");

    while (getline(file, str)) {
      if(str.find("route:")==0){
    			
          if(routeno!="")
            GenerateTrips(routeno,RouteStages,date);
    			
          firstline = 0;
          routeno = str.substr(str.find(":")+2,str.find(" ")-str.find(":")-2)+"_"+date;
      		RouteStages.clear();
    		
      }
    		
      else{

    			firstline++;
          
          istringstream ss(str); 
          string words[3];
          int wordno=0;

      		do { 
          		ss >> words[wordno];
          		wordno++;
      		} while (ss); 


    			if(firstline==1){
            RouteStages.push_back(make_pair(words[0]+"_"+date,0));
    			}
    			
          RouteStages.push_back(make_pair(words[1]+"_"+date,stof(words[2])));
    			
    		}
    }

    //For last route
    GenerateTrips(routeno,RouteStages,date);
  }
}