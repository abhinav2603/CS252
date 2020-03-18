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
	Lan(string name);
};
class Bridge{
public:
	map<Lan*,int> topo;
	string root, self; int dist;
	Lan* rlan;
	map<string,Lan*> forwardingTable;
	Bridge(string s);
	void print_table();
	void print_port();
};

