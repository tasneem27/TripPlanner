#include<bits/stdc++.h> 
#include<cmath>
#include "Functions.h"
#include "Simulator.cpp"
#include "RealTimes.cpp"
//#include "globals.h"
using namespace std; 
  
#define cost_struct pair<double,pair<double,int>> 
#define label pair<string,cost_struct>

int max_transfers = 3;
bool comp1(label l1, label l2) 
{ 
    return (l1.second.first > l2.second.first); 
} 

bool comp2(cost_struct c1, cost_struct c2){
	if(c1.first<c2.first)
		return true;
	else if (c1.first==c2.first)
		return (c1.second.first<c2.second.first);

	return false;
}

// A hash function used to hash a pair of any kind 
struct hash_pair { 
    template <class T1, class T2, class T3, class T4> 
    size_t operator()(const pair<T1,pair< T2,pair<T3,T4>>>& p) const
    { 
        auto hash1 = hash<T1>{}(p.first); 
        auto hash2 = hash<T2>{}(p.second.first); 
        auto hash3 = hash<T3>{}(p.second.second.first);
        auto hash4 = hash<T3>{}(p.second.second.second);

        return hash1 ^ hash2 ^ hash3 ^ hash4; 
    } 
}; 

priority_queue<label,vector<label>,decltype(&comp1)> pq(comp1);
unordered_map<label,label,hash_pair> pm;
unordered_map<string,vector<cost_struct>> cl;


bool isDominated(cost_struct cv, vector<cost_struct> costs){
	//if(costs.size()==0)
	//	return false;

	/*for(int i=0;i<costs.size();i++){
		cout<<"Costs: "<<costs[i].first<<" "<<costs[i].second<<endl;
		if(cv.first<costs[i].first || cv.second<costs[i].second)
			return false;
		
	}

	return true;*/

	for(int i=0;i<costs.size();i++){
		//cout<<"Costs: "<<costs[i].first<<" "<<costs[i].second<<endl;
		if(round(costs[i].first)<=round(cv.first) && round(costs[i].second.first)<=round(cv.second.first) && round(costs[i].second.second)<=round(cv.second.second)){
			//cout<<"Returning true"<<endl;
			return true;
		}
	}
	return false;
}

void removeDominated(string nv){
	vector<cost_struct> costs = cl[nv];

	int n = costs.size();
	vector<cost_struct> R;
	R.push_back(costs[0]);

	int k=1;

	for(int i=1; i<n; i++){

		bool dom = false;
		for(int j=0;j<R.size();j++){
			if(R[j].first<=costs[i].first && R[j].second.first<=costs[i].second.first
				&& R[j].second.second<=costs[i].second.second){
				dom=true;
				break;
			}	
		}

		if(dom==false){
			k = k+1;
			R.push_back(costs[i]);
		}
	}

	vector<cost_struct> M;
	M.push_back(R[k-1]);

	for(int i=k-2; i>=0; i--){

		bool dom = false;
		for(int j=0;j<M.size();j++){
			if(M[j].first<=R[i].first && M[j].second.first<=R[i].second.first
				&& M[j].second.second<=R[i].second.second){
				dom=true;
				break;
			}	
		}

		if(dom==false){
			M.push_back(R[i]);
		}
	}

	cl[nv] = M;


	//for(int i=0;i<cl[nv].size();i++)	
	//	cout<<cl[nv][i].first<<":"<<cl[nv][i].second<<" ";

}

bool isTransfer(string n){
	if (n[0]=='t')
		return true;
	return false;
}


cost_struct getAccurateCost(label lu, string nu, string nv, string departure_time){
	cost_struct cu = lu.second;
	//string nu = lu.first;

	cost_struct cv;
	if (isTransfer(nu) && !isTransfer(nv)){
		string route = nv.substr(1,nv.find("-")-1);
		/*if(nu == source){
			pair<double,double> res = EstrealWaitTime(route);
			cv.first = res.first;
			cv.second.first = res.second;
			cv.second.second = 0;
		}


		else{*/
			time_t nu_arrival = splitTime(departure_time)+cu.first;
			int interval = getWindow(nu_arrival);
			int second_interval = SecondbestWindow(nu_arrival);

			string routenode = nv.substr(1,nv.size()-1);
			
			double first_mean = wait_times[routenode][interval];
			double second_mean = wait_times[routenode][second_interval];

			time_t window1_mean_time = splitTime("00:00:00")+((interval_duration*interval)+(interval_duration/2))*60;
			time_t window2_mean_time = splitTime("00:00:00")+((interval_duration*second_interval)+(interval_duration/2))*60;
		
			double weight1 = abs(window2_mean_time-nu_arrival);
			double weight2 = abs(window1_mean_time-nu_arrival);

			double mean = (weight1*first_mean)+(weight2*second_mean);
			mean = round(mean/(weight1+weight2));
			//cout<<"FURTHER WAIT TIMES: "<<nv<<" "<<mean<<" "<<first_mean<<" "<<second_mean<<endl;
			cv = cu;
			//cv.push_back[cu[0]];
			//cv.push_back[cu[1]];
			cv.first += mean;

			//cout<<"In cost function: "<<routenode<<" "<<mean<<endl;

		//}
		return cv;


	}

	else if(isTransfer(nv) && !isTransfer(nu)){
		
		/*vector<double> cv;
		cv.push_back[cu[0]];
		cv.push_back[cu[1]];*/

		cv = cu;
		cv.second.second++;
		//cout<<"Alight "<<endl;

		return cv;
	}

	else{
		string stop1 = nu.substr(nu.find("-")+1,nu.size()-nu.find("-")-1);
		string stop2 = nv.substr(nv.find("-")+1,nv.size()-nv.find("-")-1);
		string route = nu.substr(1,nu.find("-")-1);
		string key = route+"-"+stop1+"_"+stop2;

		//cout<<"BOTH ROUTE NODES: ";
		//cout<<stop1<<" "<<stop2<<" "<<route<<" "<<key<<endl;
		
		time_t nu_arrival = splitTime(departure_time)+cu.first;
		int interval = getWindow(nu_arrival);
		int second_interval = SecondbestWindow(nu_arrival);
		
		double first_mean = mean_times[key][interval];
		double second_mean = mean_times[key][second_interval];

		double first_var = var_times[key][interval];
		double second_var = var_times[key][second_interval];

		time_t window1_mean_time = splitTime("00:00:00")+((interval_duration*interval)+(interval_duration/2))*60;
		time_t window2_mean_time = splitTime("00:00:00")+((interval_duration*second_interval)+(interval_duration/2))*60;
	
		double weight1 = abs(window2_mean_time-nu_arrival);
		double weight2 = abs(window1_mean_time-nu_arrival);

		double mean = (weight1*first_mean)+(weight2*second_mean);
		mean = round(mean/(weight1+weight2));

		double var = (weight1*first_var)+(weight2*second_var);
		var = var/(weight1+weight2);

		cv = cu;
		cv.first += mean;
		cv.second.first += var;
		
		//cout<<"In cost function: "<<key<<" "<<mean<<" "<<var<<endl;

		return cv;
	}
}

void PrintOutput(string line){
	//---vector<pair<int,pair<int,int>>> output(max_transfers,make_pair(0,make_pair(0,0)));
	//---result_out<<line<<",";
	int transfers = 0;

	time_t dtime = splitTime(departure_time);
	for(int i=0;i<cl[destination].size();i++){
		stack<string> path;


		cout<<endl<<"Plan "<<i+1<<":"<<endl;
		transfers = 0;
		cout<<"\t Mean travel time: "<<cl[destination][i].first<<" mins; Variance: "<<round(cl[destination][i].second.first)<<" mins ";
		
        result_out<<source<<","<<destination<<","<<departure_time<<","<<cl[destination][i].first<<","<<round(cl[destination][i].second.first)<<",";


		//cout<<cl[destination][i].second.second<<" ";

		//---output[cl[destination][i].second.second-1].first++;
		//---output[cl[destination][i].second.second-1].second.first+=cl[destination][i].first;
		//---output[cl[destination][i].second.second-1].second.second+=cl[destination][i].second.first;


		label l = make_pair(destination,cl[destination][i]);
		bool justtransfered = false;
		while(pm.find(l)!=pm.end()){
			//cout<<l.first<<" ";
			if(l.first[0]=='t')
			{
				transfers++;
				justtransfered = true;
				if(transfers==1)
				{
					path.push("Destination stop: "+l.first.substr(1,l.first.size()-1));
				}
				else{
					path.push("Get down at: "+l.first.substr(1,l.first.size()-1));
				}

				//---cout<<"l.first: "<<l.first<<" "<<l.second.first<<" "<<l.second.second.first<<" "<<l.second.second.second<<endl;
			}

			label l2 = pm[l];
			l = l2;

			if(justtransfered){
				//nu.substr(1,nu.find("-")-1)
				path.push("Take route: "+l.first.substr(1,l.first.find("-")-1));
				justtransfered = false;
				//---cout<<"l.first: "<<l.first<<" "<<l.second.first<<" "<<l.second.second.first<<" "<<l.second.second.second<<endl;

			}
			//---else
				//---cout<<"l.first: "<<l.first<<" "<<l.second.first<<" "<<l.second.second.first<<" "<<l.second.second.second<<endl;

		}
		
		//cout<<l.first<<" ";
		if(l.first[0]=='t')
		{
			transfers++;
			path.push("Start at stop: "+l.first.substr(1,l.first.size()-1));
			//---cout<<"l.first: "<<l.first<<" "<<l.second.first<<" "<<l.second.second.first<<" "<<l.second.second.second<<endl;


		}
		cout<<" Transfers: "<<transfers-2<<endl;
		result_out<<transfers-2<<",";
		cout<<endl;
		//cout<<"Stack trace: "<<endl;

		string getin_stop, take_route, getoff_stop=""; 
		string top;
		time_t currtime = splitTime(departure_time);

		while (!path.empty()) { 

			if(getoff_stop!=""){
				getin_stop = getoff_stop;
			}
			else{
				top = path.top();
				getin_stop = top.substr(top.find(":")+2,top.size()-top.find(":")-2);
        		cout <<"\t "<<top<<endl; 
        		path.pop();
        	}

        	if(!path.empty()){
        		top = path.top();
				take_route = top.substr(top.find(":")+2,top.size()-top.find(":")-2);
        		cout <<"\t "<<top<<endl; 
        		path.pop();
        	}
			
			if(!path.empty()){
        		top = path.top();
				getoff_stop = top.substr(top.find(":")+2,top.size()-top.find(":")-2);
        		cout <<"\t "<<top<<endl; 
        		path.pop();
        	}

        	currtime = EarliestTrip(getin_stop,take_route,getoff_stop,currtime);
        }

        wcout<<endl<<"\t =>Actual Time taken: "<<round((currtime-dtime)/60)<<" minutes"<<endl; 
        result_out<<round((currtime-dtime)/60)<<"\n";

    }

    /*----for(int i=0;i<max_transfers;i++){
    	if(output[i].first==0){
    		output[i].second.first=-1;
    		output[i].second.second=-1;

    	}
    	else{
    		output[i].second.first=output[i].second.first/output[i].first;
    		output[i].second.second=output[i].second.second/output[i].first;	
    	}

    	//---result_out<<output[i].second.first<<","<<output[i].second.second<<",";
    	cout<<"i: "<<i<<" "<<output[i].second.first<<" "<<output[i].second.second<<endl;
    }*/
    //---result_out<<"\n"; 
	
}

int ShortestPath_main(string line){
	//pq.clear();
	pm.clear();
	cl.clear();
	cout<<source<<" "<<destination<<" "<<departure_time<<endl;

	ifstream file("GeneratedFiles/Waitfile_interval"+to_string(interval_duration)+".csv");
	string str;

   	while (getline(file, str)) {
        string word;
 		stringstream s(str); 
 		string route;

 		vector<double> wait_times_list;
 		int wordno = 0;

        while (getline(s, word, ',')) { 
        	if(wordno==0){
        		route=word;
        	}
        	else{
        		wait_times_list.push_back(round(stof(word)));
        	}
        	wordno++;
        }
       	wait_times[route]=wait_times_list;
    }

    file.close();

	ifstream file2("GeneratedFiles/MVfile_interval"+to_string(interval_duration)+".csv");

	int lineno = 0;
   	while (getline(file2, str)) {
   		lineno++;
        string word;
 		stringstream s(str); 
 		string route;

 		vector<string> start_times;
 		int wordno = 0;
 		int interval;
 		double mean,var;

        while (getline(s, word, ',')) { 
        	if(wordno==0){
        		route=word+"-";
        	}
        	else if(wordno==1){
        		interval = stoi(word);
        	}
        	else if(wordno==2){
        		route+=word;
        	}
        	else if(wordno==3){
        		mean = round(stof(word)/60);
        	}
        	else if(wordno==4){
        		var = stof(word)/60;        		
        	}
        	wordno++;
 		}
 		if(mean_times.find(route)==mean_times.end()){
 			mean_times.insert(pair<string,vector<double> >(route, vector<double>(num_intervals,INF)));
 			//var_times.insert(pair<string,vector<double> >(route, vector<double>(num_intervals,INF))); 		
 		}

 		if(var_times.find(route)==var_times.end()){
 			var_times.insert(pair<string,vector<double> >(route, vector<double>(num_intervals,INF))); 		 			
 		}
		mean_times[route][interval]=mean;    	
		var_times[route][interval]=var;
	}

	file2.close();

	int round = 0;


 	cost_struct cost = make_pair(0,make_pair(0,0));
	label ls = make_pair(source,cost);
	if(cl.find(source)==cl.end()){
		cl[source] = vector<cost_struct>();
	}

	cl[source].push_back(cost);

	pq.push(ls);

	while (pq.size()!=0){

		label lu = pq.top();
		pq.pop();

		string nu = lu.first;
		//cout<<"nu: "<<nu<<endl;
		for(int i=0;i<adj[nu].size();i++){

			string nv = adj[nu][i];

			//cout<<"nv: "<<nv<<endl;
			cost_struct cv = getAccurateCost(lu,nu,nv,departure_time);
			//double cv[2] = {cv_vec[0],cv_vec[1]};

			vector<cost_struct> costs = cl[nv];

			if (isDominated(cv,costs)){
				continue;
			}

			if(cv.second.second>max_transfers){
				continue;
			}
				
			if(cl.find(nv)==cl.end()){
				cl[nv] = vector<cost_struct>();
			}
				
			//cout<<"New label pushed: "<<nv<<" "<<cv.first<<" "<<cv.second<<endl;
			//cl.insert({nv,cv});
			cl[nv].push_back(cv);
			
			removeDominated(nv);

			label lv = make_pair(nv,cv);
			pm[lv] = lu;
			//pm.insert({lv,lu});
			pq.push(lv);


		}
	}

	

	cout<<endl<<"Output: "<<endl;

	PrintOutput(line);

	// for(int i=0;i<cl[destination].size();i++){
	//	cout<<cl[destination][i].first<<" "<<cl[destination][i].second<<endl;
	// }
}