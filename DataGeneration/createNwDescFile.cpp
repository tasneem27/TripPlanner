#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <bits/stdc++.h> 

using namespace std;

bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}


void findRouteDetails(vector<string> prevRouteDetails){
	//cout<<"In function"<<endl;

	for (auto it = prevRouteDetails.rbegin(); it != prevRouteDetails.rend(); ++it)
	{
			
		istringstream ss(*it); 
		string words[3];
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

		cout<<words[1]<<" "<<words[0]<<" "<<words[2]<<endl;
	}
}

//Output to file GenearatedFiles/NWDescFile
int main ()
{

    set<string> routes_dir;
    
    //Original Mumbai Navigator busdata file
    ifstream file("newBusData_nf");
    
    string str;
    int firstline = 0;
    string prevStage;
    vector<string> prevRouteDetails;
    string routeno;


    //Read busdata file and generate Nw desc file
    //Generate stage to stage distances and store only stage information
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
    			cout << str <<"\n";

    			findRouteDetails(prevRouteDetails);
    			prevRouteDetails.clear();
    		}

    		else{
    			prevRouteDetails.clear();
    			routeno = routeno+"_U";
				  replace(str,oldroute, routeno);
    			cout << str <<"\n"; 		
    		}
    	
    		routes_dir.insert(routeno);
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
  			
        //If stage
  			if(words[4]=="T"){
  				string new_line = prevStage+" "+words[1]+" "+words[3]+"\n";
  				prevStage = words[1];
  				cout<<new_line;
  				prevRouteDetails.push_back(new_line);
  			}
  		}
  	}
}