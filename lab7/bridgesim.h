class event{
public:
	Bridge* bridge;
	mssg msg;
	Lan* lan;
};

class bridgesim{
public:
	map<string,Lan*> host;
	vector<Bridge*> bridge;
	vector<Lan*> lan;
	int trace;
	void run();
};