#include<bits/stdc++.h>
using namespace std;
class event{
public:
	int f;
};
int main(){
	// cout<<"a"+to_string(12)<<endl;
	// set<string> s;
	// s.insert("abs");
	// s.insert("bac");
	// s.insert("dlt");
	// for(set<string>::iterator itr = s.begin();itr != s.end(); itr++){
	// 	cout<<*itr<<endl;
	// }
	// vector<int> vec;
	// vec.resize(vec.size()+1);
	// map<int,string> mp;
	// //if(mp[1].is_empty())
	// event* e; e->f=1;
	// map<int,event*> mp1;
	// mp1[1] = e;
	// cout<<(mp1[1])->f<<endl;
	int trace, n_bridges;
	std::cin>>trace>>n_bridges;
	
	map<string,vector<string> > topo, host;
	set<string> la;
	
	for(int i=0;i<n_bridges;i++){
		string name,b;
		int cnt=0;
		while(getline(cin, name, ' ')){
			if(!cnt){
				cnt++;
			}
			else{
				topo["B"+to_string(i+1)].push_back(name);
				cout<<"B"+to_string(i+1)<<": "<<name<<endl;
				if(la.find(name) == la.end()){
					la.insert(name);
				}
			}
		}
	}
}