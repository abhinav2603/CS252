#include<bits/stdc++.h>
using namespace std;
struct mssg{
	string root, self; int dist;
};
class Bridge;
class Lan{
public:
	vector<Bridge*> topo;
	string desg_bridge, self, root; 
	vector<string> host;
	int dist;
	Lan(string name){
		self = name;
	}
};

class Bridge{
public:
	map<Lan*,int> topo;
	Lan* rlan;
	string root, self; int dist;
	map<string,Lan*> forwardingTable;
	Bridge(string s){
		self = s;
		root = s;
		dist = 0; 
		rlan = NULL;
	}
	void print_port(){
		cout<<self<<":";
		for(map<Lan*,int>::iterator itr = topo.begin(); itr != topo.end(); itr++){
			string func;
			if(itr->second == 0){
				func = "NP";
			}
			else if(itr->second == 1){
				func = "RP";
			}
			else{
				func = "DP";
			}
			cout<<" "<<(itr->first)->self<<"-"<<func;
		}
		cout<<endl;
	}

	void print_table(int n){
		int k = 0;
		for(map<string,Lan*>::iterator itr = forwardingTable.begin(); itr != forwardingTable.end(); itr++){
			cout<<itr->first<<" | "<<(itr->second)->self;
			if(!n || k<forwardingTable.size()-1){
				cout<<endl;
			}
			k++;
		}
	}
};
