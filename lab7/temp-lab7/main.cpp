#include<bits/stdc++.h>
//#include<bridge.h>
#include "bridgesim.cpp"
using namespace std;

int main(){
	int trace=0, n_bridges=5, n_trans;
	cin>>trace>>n_bridges;
	//cout<<"fundo"<<endl;
	map<string,vector<string> > topo, host;
	set<string> la;
	//cout<<n_bridges<<endl;
	string b;
	for(int i=0;i<n_bridges;i++){
		vector<string> v;
		string name;
		cin>>b;
		int cnt=0;
		getline(cin, name);
		//cout<<name<<endl;
		int j=0,k=0;
		for(k=0;k<name.size();k++){
			if(name[k] == ' '){
				v.push_back(name.substr(j,k-j));
				j=k+1;
			}
		}
		v.push_back(name.substr(j,k-j));
		//cout<<v.size()<<endl;
		for(int l=1;l<v.size();l++){
			topo["B"+to_string(i+1)].push_back(v[l]);
			if(la.find(v[l]) == la.end()){
				la.insert(v[l]);
			}
		}
	}
			// if(!cnt){
			// 	cnt++;
			// }
			// cout<<name<<endl;
			// else{
			// 	topo["B"+to_string(i+1)].push_back(name);
			// 	//cout<<"B"+to_string(i+1)<<": "<<name<<endl;
			// 	if(la.find(name) == la.end()){
			// 		la.insert(name);
			// 	}
			// }
		//cout<<"func"<<endl;
	// for(map<string,vector<string> >:: iterator itr = topo.begin(); itr != topo.end(); itr++){
	// 	cout<<"Bridge: "<<itr->first<<endl;
	// 	for(int i=0;i<itr->second.size();i++){
	// 		cout<<itr->second[i]<<" ";
	// 	}
	// 	cout<<endl;
	// }
	//cout<<topo.size()<<endl;

	for(set<string>::iterator itr=la.begin(); itr != la.end(); itr++){
		vector<string> v;
		string name;
		cin>>b;
		getline(cin, name);
		//cout<<name<<endl;
		int j=0,k=0;
		for(k=0;k<name.size();k++){
			if(name[k] == ' '){
				v.push_back(name.substr(j,k-j));
				j=k+1;
			}
		}
		v.push_back(name.substr(j,k-j));

		for(int l=1;l<v.size();l++){
			host[*itr].push_back(v[l]);
		}
	}
	// for(map<string,vector<string> >:: iterator itr = host.begin(); itr != host.end(); itr++){
	// 	cout<<"Lan: "<<itr->first<<endl;
	// 	for(int i=0;i<itr->second.size();i++){
	// 		cout<<itr->second[i]<<" ";
	// 	}
	// 	cout<<endl;
	// }
	//cout<<"here"<<endl;
	//bridgesim obj(topo, host, la, trace);
	//obj.run();
	/**cin>>n_trans;
	for(int i=0;i<n_trans;i++){

	}**/
	cin>>n_trans;
	vector<pair<string,string> > vp(n_trans);
	for(int i=0;i<n_trans;i++){
		cin>>vp[i].first>>vp[i].second;
	}
	// for(int i=0;i<n_trans;i++){
	// 	cout<<vp[i].first<<" "<<vp[i].second<<endl;
	// }
	bridgesim obj(topo, host, la, trace, vp);
	obj.run();
	cout<<endl;
}