#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <bits/stdc++.h> 

using namespace std;

float RandomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}


int main ()
{

  set<string> routes_dir;

  //Generating one week Freq modifications
  for(int day=1;day<=7;day++){


  	ifstream file("GeneratedFiles/NWDescFile");
    string str;

    int firstline = 0;
  	string prevStage;
  	vector<string> prevRouteDetails;
  	string routeno;

    while (getline(file, str)) {
    		if(str.find("route:")==0){
    			routeno = str.substr(str.find(":")+2,str.find(" ")-str.find(":")-2);

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

          cout<<routeno<<"_"<<day<<" "<<words[3]<<" ";
          float freq = stof(words[3]);
          float freqmods[18];
          //float fastless = 0.25*freq;
          //float fastmore = 0.1*freq;
          //float slowmore = 0.5*freq;

          for(int i=0;i<18;i++){
            if(i<2 || (i>=8 && i<=9) || (i>=15))
            {
              freqmods[i] = RandomFloat(0,3);
            }
            else{
              freqmods[i] = RandomFloat(0,3);
            }
            cout<<freqmods[i]<<" ";
          }
          cout<<endl;
    		}
    		else{
    				continue;
    		}
    }
    file.close();
  }
}