#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <bits/stdc++.h> 

using namespace std;

unordered_map<string,float> stage_speed;
unordered_map<string,int> stage_count;

bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}


float RandomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

//Recording speed of buses at stages and counts
void findRouteDetails(vector<string> prevRouteDetails){
	//cout<<"In function"<<endl;

	for (auto it = prevRouteDetails.rbegin(); it != prevRouteDetails.rend(); ++it)
	{
			
		istringstream ss(*it); 
		string words[4];
		int wordno=0;

  		// Traverse through all words 
    	do { 
        // Read a word  
        	ss >> words[wordno];
        	// Print the read word 
        	//cout <<"start"<<words[wordno]<<"end"<<endl; 
        	wordno++;
        	// While there is more to read 
    	} while (ss); 

		//cout<<words[1]<<" "<<words[0]<<" "<<words[2]<<" "<<words[3]<<endl;

    float speed = (stof(words[2])*60)/stof(words[3]);

    if (stage_count.find(words[1]) != stage_count.end()) {
      stage_speed[words[1]]+=speed;
      stage_count[words[1]]+=1;
    } 
    else{
      stage_speed[words[1]]=speed;
      stage_count[words[1]]=1;
    }

    if (stage_count.find(words[0]) != stage_count.end()) {
      stage_speed[words[0]]+=speed;
      stage_count[words[0]]+=1;
    } 
    else{
      stage_speed[words[0]]=speed;
      stage_count[words[0]]=1;
    }

    
	}
}





int main ()
{

  for(int day=1;day<=7;day++){

    stage_speed.clear();
    stage_count.clear();

  	set<string> routes_dir;

  	ifstream file("GeneratedFiles/newBusData_nf");
    string str;

    int firstline = 0;
  	string prevStage;
  	vector<string> prevRouteDetails;
  	string routeno;
    float travel_time = 0;

   

    	while (getline(file, str)) {
    		if(str.find("route:")==0){
    			
    			firstline = 0;
    			string oldroute;
    			routeno = str.substr(str.find(":")+2,str.find(" ")-str.find(":")-2);
      		oldroute = routeno;

      		if(routes_dir.find(routeno+"_U")!=routes_dir.end())
      		{
      			routeno = routeno+"_D";
      			replace(str,oldroute, routeno);
      			//cout << str <<"\n";

      			findRouteDetails(prevRouteDetails);
      			prevRouteDetails.clear();

      		}
      		else{
      			prevRouteDetails.clear();
      			routeno = routeno+"_U";
  				replace(str,oldroute, routeno);
      			//cout << str <<"\n"; 		
      		}
      	
      		routes_dir.insert(routeno);
          travel_time=0;



    		}
    		else{

    			if(routeno.find("_D") != std::string::npos)
    				continue;

    			firstline++;

  			istringstream ss(str); 
  			string words[6];
  			int wordno=0;

    			// Traverse through all words 
      		do { 
          		// Read a word  
          		ss >> words[wordno];
          		// Print the read word 
          		//cout <<"start"<<words[wordno]<<"end"<<endl; 
          		wordno++;
          		// While there is more to read 
      		} while (ss); 

    			//cout << str << " find:"<<str.find(" ")<<"\n";

    			if(firstline==1){
  				prevStage = words[0];
    			}
    			
    			if(words[4]=="T"){
            travel_time+=stof(words[2]);
    				string new_line = prevStage+" "+words[1]+" "+words[3]+" "+to_string(travel_time)+"\n";
    				

            float speed = (stof(words[3])*60)/travel_time;


            if (stage_count.find(prevStage) != stage_count.end()) {
              stage_speed[prevStage]+=speed;
              stage_count[prevStage]+=1;
            } 
            else{
              stage_speed[prevStage]=speed;
              stage_count[prevStage]=1;
            }
            
            if (stage_count.find(words[1]) != stage_count.end()) {
              stage_speed[words[1]]+=speed;
              stage_count[words[1]]+=1;
            } 
            else{
              stage_speed[words[1]]=speed;
              stage_count[words[1]]=1;
            }

            prevStage = words[1];
    				//cout<<new_line;

    				prevRouteDetails.push_back(new_line);

            travel_time = 0;
    			}
          else{
            travel_time+=stof(words[2]);
          }

    		}
    	}


      for (auto i : stage_speed){ 
        float avg_speed = i.second/stage_count[i.first];  
        cout<<i.first<<"_"<<day<<" "<<avg_speed<<" ";


        float speedmods[18];
        float fastless = 0.1*avg_speed;
        float fastmore = 0.01*avg_speed;
        float slowmore = 0.25*avg_speed;

        //18 intervals: starting from 6 to 7 am , 7 to 8,...
        for(int i=0;i<18;i++){
            if(i<2 || (i>=8 && i<=9) || (i>=15))
            {
              speedmods[i] = RandomFloat(-fastmore,fastless);
            }
            else{
              speedmods[i] = -1*RandomFloat(0,slowmore);
            }
            cout<<speedmods[i]<<" ";
          }

          cout<<endl;
      }
  }
}