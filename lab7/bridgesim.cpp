#include<bits/stdc++.h>
using namespace std;
#include"bridge.cpp"

class event{
public:
	Bridge* bridge;
	mssg msg;
	Lan* lan;
};

class flow_event{
public:
	Bridge* bridge;
	Lan* lan;
	pair<string,string> msg;
};

class bridgesim{
	//map<string,vector<string> > topo, host;
public:
	map<string,Lan*> host;
	vector<Bridge*> bridge;
	vector<Lan*> lan;
	vector<pair<string,string> > vec;
	int trace;

	// Constructor
	bridgesim(map<string,vector<string> > top, map<string,vector<string> > hos, set<string> la, int tr, vector<pair<string,string> > arr){
		//topo = top;
		//host = hos;
		trace = tr;
		//bridge.resize(top.size());
		for(int i=0;i<top.size();i++){
			bridge.push_back(new Bridge(""));
		}
		for(int i=0;i<la.size();i++){
			lan.push_back(new Lan(""));
		}
		//cout<<lan.size()<<endl;
		int k=0;
		//cout<<lan[0]->self<<endl;
		for(set<string>::iterator itr = la.begin(); itr != la.end(); itr++){
			(lan[k])->self = *itr;
			//cout<<lan[k]->self<<" ";
			k++;
		}
		//cout<<endl;
		k = 0;
		//cout<<"here3"<<endl;
		//cout<<(bridge[0])->dist<<endl;
		for(map<string,vector<string> >::iterator itr = top.begin(); itr != top.end(); itr++){

			(bridge[k])->self = itr->first;
			(bridge[k])->dist = 0;
			(bridge[k])->root = itr->first;

			for(int i=0;i<(itr->second).size();i++){
				string lan_name = (itr->second)[i];
				for(int j=0;j<lan.size();j++){
					if((lan[j])->self == lan_name){
						(bridge[k])->topo[lan[j]] = 2; // Initially all root port
						(lan[j])->topo.push_back(bridge[k]);
					}
				}
			}
			k++;
			//bridge.push_back(&bd);
		}

		//To be checked
		for(map<string,vector<string> >::iterator itr = hos.begin(); itr != hos.end(); itr++){
			for(int i=0;i<(itr->second).size();i++){
				for(int j=0;j<lan.size();j++){
					if((lan[j])->self == itr->first){
						host[(itr->second)[i]] = lan[j];
					}
				}
			}
		}

		vec = arr;

	}

	void run(){
		int time = 0;
		vector<vector<event> > event_train;
		vector<event> v;
		event_train.push_back(v);
		bool eq = false;
		//cout<<"fund1"<<endl;
		vector<vector<int> > temp(bridge.size());
		for(int i=0;i<bridge.size();i++){
			for(map<Lan*,int>::iterator itr = bridge[i]->topo.begin(); itr != bridge[i]->topo.end(); itr++){
				temp[i].push_back(itr->second);
			}
		}
		//cout<<"reached"<<endl;
		while(!eq){
			//RESETTING the desg_bridge for each LAN
			//cout<<"fundoo"<<endl;
			bool f=true;
			for(int i=0;i<lan.size();i++){
				(lan[i])->desg_bridge = "";
			}
			vector<event> v1;
			event_train.push_back(v1);
			//cout<<"fund2"<<" "<<event_train[0].size()<<endl;
			//Perform the events in the event train at time "time"
			for(int i=0;i<event_train[time].size();i++){

				Bridge* bridge = event_train[time][i].bridge;
				mssg m = event_train[time][i].msg;
				Lan* lan = (event_train[time][i].lan); // pointer to the lan

				if(bridge != NULL && bridge->topo[lan]){//If port is functioning
					if((bridge->root > m.root) || ((bridge->root == m.root) && (bridge->dist > m.dist+1))){
						bridge->root = m.root;
						bridge->dist = m.dist+1;
						bridge->rlan = lan;
					}
				}
				if(lan->desg_bridge == ""){
					lan->desg_bridge = m.self;
					lan->dist = m.dist;
					lan->root = m.root;
				}
				else{
					if(lan->dist > m.dist){
						lan->desg_bridge = m.self;
						lan->dist = m.dist;
						lan->root = m.root;
					}
					else if(lan->dist == m.dist && lan->root > m.root){
						lan->desg_bridge = m.self;
						lan->dist = m.dist;
						lan->root = m.root;
					}
					else if(lan->dist == m.dist && lan->root == m.root && lan->desg_bridge > m.self){
						lan->desg_bridge = m.self;
						lan->dist = m.dist;
						lan->root = m.root;
					}
				}
			}
			/**
			cout<<"---------Printing Lan desg_bridges-------------"<<endl;
			for(int i=0;i<lan.size();i++){
				cout<<lan[i]->self<<" "<<lan[i]->desg_bridge<<endl;
			}
			cout<<"-------------ENDED------------------"<<endl;**/

			// for(int i=0;i<bridge.size();i++){
			// 	cout<<bridge[i]->self<<" "<<bridge[i]->topo.size()<<endl;
			// 	for(map<Lan*,int>::iterator itr = bridge[i]->topo.begin(); itr != bridge[i]->topo.end(); itr++){
			// 		cout<<itr->first->self<<" "<<itr->second<<" ";

			// 	}
			// 	cout<<endl;
			// }
			
			bool init = false;
			//CHANGING THE STATE OF PORTS
			for(int i=0;i<bridge.size();i++){
				if(bridge[i]->rlan != NULL){
					init = true;
					for(map<Lan*,int>::iterator itr = (bridge[i])->topo.begin(); itr != (bridge[i])->topo.end(); itr++){
						if((bridge[i])->self == (itr->first)->desg_bridge){
							
							// if(itr->second != 2){
							// 	cout<<"f1"<<endl;
							// 	f = false;
							// }
							itr->second = 2;
						}
						else{
							
							// if(itr->second != 0){
							// 	cout<<"f2"<<endl;
							// 	f = false;
							// }
							itr->second = 0;
						}
					}
					if(bridge[i]->topo.find(bridge[i]->rlan) != bridge[i]->topo.end()){

						// if((bridge[i])->topo[(bridge[i])->rlan] != 1){
						// 	cout<<"f3"<<endl;
						// 	f = false;
						// }
						// if(bridge[i]->self == "B1"){
						// 	cout<<"wrongly done"<<endl;
						// }
						(bridge[i])->topo[(bridge[i])->rlan] = 1;
					}
				}
			}
			//cout<<"fund4"<<endl;
			//If eq doesn't changes that means steady state has been reached
			//eq = (temp == bridge);
			//cout<<"Checker:  "<<(temp==bridge)<<endl;

			for(int i=0;i<bridge.size();i++){
				int j=0;
				for(map<Lan*,int>::iterator itr = bridge[i]->topo.begin(); itr != bridge[i]->topo.end(); itr++){
					//cout<<i<<" "<<j<<" "<<temp[i][j]<<" "<<itr->second<<endl;
					if(temp[i][j] != itr->second){
						f = false;
					}
					j++;
				}
			}
			//cout<<f<<endl;
			eq = f;
			if(!init){
				//cout<<"-------------potter-------------"<<endl;
				eq = init;
			}

			// for(int i=0;i<bridge.size();i++){
			// 	cout<<bridge[i]->self<<" "<<bridge[i]->topo.size()<<endl;
			// 	for(map<Lan*,int>::iterator itr = bridge[i]->topo.begin(); itr != bridge[i]->topo.end(); itr++){
			// 		cout<<itr->first->self<<" "<<itr->second<<" ";

			// 	}
			// 	cout<<endl;
			// }

			//Adding new messages to the event queue after finding the OFF ports
			for(int i=0;i<bridge.size();i++){
				//cout<<bridge[i]->self<<endl;
				for(map<Lan*,int>::iterator itr = (bridge[i])->topo.begin(); itr != (bridge[i])->topo.end(); itr++){
					if(itr->second && (bridge[i]->rlan != itr->first)){// PORT is onn and is not the root lan
						if(itr->first->topo.size() == 1){
							event e = * (new event());
							e.lan = itr->first;
							e.bridge = NULL;
							e.msg.root = (bridge[i])->root;
							e.msg.dist = (bridge[i])->dist;
							e.msg.self = (bridge[i])->self;
							event_train[time+1].push_back(e);
						}
						for(int j=0;j<(itr->first)->topo.size();j++){
							if(((itr->first)->topo[j])->self != (bridge[i])->self){
								event e = * (new event());
								e.lan = itr->first;
								e.bridge = (itr->first)->topo[j];
								e.msg.root = (bridge[i])->root;
								e.msg.dist = (bridge[i])->dist;
								e.msg.self = (bridge[i])->self;
								event_train[time+1].push_back(e);

								//cout<<(e.bridge)->self<<" "<<e.lan->self<<" "<<e.msg.self<<" "<<e.msg.root<<" "<<e.msg.dist<<endl;
							}
						}
					}
				}
			}
			for(int i=0;i<bridge.size();i++){
				int j=0;
				for(map<Lan*,int>::iterator itr = bridge[i]->topo.begin(); itr != bridge[i]->topo.end(); itr++){
					temp[i][j] = itr->second;
					j++;
				}
			}
			//cout<<"fund5"<<eq<<endl;
			time++;
			if(trace){
				cout<<"-----------PRINTING EVENT TRAIN-------------"<<endl;
				for(int i=0;i<event_train[time].size();i++){
					event e = event_train[time][i];
					cout<<"Bridge: "<<e.bridge->self<<endl<<"MESSAGE: "<<e.msg.root<<" "<<e.msg.dist<<" "<<e.msg.self<<endl<<"Lan: "<<e.lan->self<<endl<<endl;
				}
				cout<<"------------------Ended----------------------"<<endl;
			}

			// cout<<"-----------PRINTING STATUS OF PORTS----------"<<endl;
			// for(int i=0;i<bridge.size();i++){
			// 	(bridge[i])->print_port();
			// }
			// cout<<"------------------ENDED--------------------"<<endl;
		}

		for(int i=0;i<bridge.size();i++){
			(bridge[i])->print_port();
		}

		///-----------------------------------------------------------------------------------------
		///--------------------DATA FLOW AFTER SPANNING TREE HAS BEEN BUILT------------------------
		//------------------------------------------------------------------------------------------
		time = 0;
		vector<vector<flow_event> > event_train2;
		vector<flow_event> fl;
		event_train2.push_back(fl);

		for(int i=0;i<vec.size();i++){
			string start = (vec[i]).first, end = (vec[i]).second;
			Lan* lan = host[start];
			bool possible = false;
			vector<flow_event> fl1;
			event_train2.push_back(fl1);

			//cout<<lan->self<<endl;
			//map<string,Bridge*>::iterator iter = la

			for(int j=0;j<lan->topo.size();j++){
				flow_event e = * (new flow_event());
				e.bridge = lan->topo[j];
				e.msg = {start,end};
				e.lan = lan;
				if(e.bridge->topo[lan]){// If port is functioning
					event_train2[time+1].push_back(e);
				}
			} 
			time++;
			if(trace){
				cout<<"      Current time is: "<<time<<endl;
				cout<<"-------------Printing Event Train------------"<<endl;
				for(int l=0;l<event_train2[time].size();l++){
					flow_event e = event_train2[time][l];
					cout<<"To Bridge: "<<e.bridge->self<<endl;
					cout<<"Message: "<<e.msg.first<<" to "<<e.msg.second<<endl;
					cout<<"From Lan: "<<e.lan->self<<endl<<endl;
				}
				cout<<"-------------------ENDING--------------------"<<endl;
			}
			//cout<<"fundo"<<endl;
			while(!possible){
				bool eq = true;
				vector<flow_event> fl2;
				event_train2.push_back(fl2);

				for(int j=0;j<event_train2[time].size();j++){

					flow_event e = event_train2[time][j];
					map<string,Lan*>::iterator itr = e.bridge->forwardingTable.find(e.msg.second);
					// if(e.msg.second == "H9" && e.bridge->self == "B5"){
					// 	cout<<"/////////////////////SHUT UP///////////////////"<<endl;
					// 	if(itr != e.bridge->forwardingTable.end()){
					// 		cout<<itr->second->self<<" "<<e.lan->self<<endl;
					// 	}
					// }
					//int cnt = 0;
					if(itr != e.bridge->forwardingTable.end()){// If bridge has learnt already about the port
						if(itr->second->self != e.lan->self){
							for(int k=0;k<(itr->second)->topo.size();k++){
								if((itr->second)->topo[k]->self != e.bridge->self){
									flow_event e1 = *(new flow_event());
									e1.msg = e.msg;
									e1.lan = itr->second;
									e1.bridge = (itr->second)->topo[k];
									if(e1.bridge->topo[e1.lan]){
										event_train2[time+1].push_back(e1);
									}
								}
							}
						}
					}
					else{
						for(map<Lan*,int>::iterator itr2=e.bridge->topo.begin(); itr2 != e.bridge->topo.end(); itr2++){

							if(itr2->second){// If port is functioning
								for(int k=0;k<(itr2->first)->topo.size();k++){
									if((itr2->first)->topo[k]->self != e.bridge->self && itr2->first->self != e.lan->self){
										flow_event e1 = *(new flow_event());
										e1.msg = e.msg;
										e1.lan = itr2->first;
										e1.bridge = (itr2->first)->topo[k];
										if(e1.bridge->topo[e1.lan]){
											event_train2[time+1].push_back(e1);
										}
									}
								}
							}
						}
					}
					if(e.bridge->forwardingTable[e.msg.first] != e.lan){
						eq = false;
						e.bridge->forwardingTable[e.msg.first] = e.lan;
					}
				}
				//cout<<"here"<<endl;
				possible = eq;
				time++;
				if(trace){
					cout<<"      Current time is: "<<time<<endl;
					cout<<"-------------Printing Event Train------------"<<endl;
					for(int l=0;l<event_train2[time].size();l++){
						flow_event e = event_train2[time][l];
						cout<<"To Bridge: "<<e.bridge->self<<endl;
						cout<<"Message: "<<e.msg.first<<" to "<<e.msg.second<<endl;
						cout<<"From Lan: "<<e.lan->self<<endl<<endl;
					}
					cout<<"-------------------ENDING--------------------"<<endl;
				}
			}
			//cout<<"fundo1"<<endl;
			for(int i=0;i<bridge.size()-1;i++){
				cout<<bridge[i]->self<<":"<<endl;
				cout<<"HOST ID | FORWARDING PORT"<<endl;
				bridge[i]->print_table(0);
			}
			if(i < vec.size()-1){
				cout<<bridge[bridge.size()-1]->self<<":"<<endl;
				cout<<"HOST ID | FORWARDING PORT"<<endl;
				bridge[bridge.size()-1]->print_table(0);
			}
			else{
				cout<<bridge[bridge.size()-1]->self<<":"<<endl;
				cout<<"HOST ID | FORWARDING PORT"<<endl;
				bridge[bridge.size()-1]->print_table(1);
			}
		cout<<endl;
		}
	}

};