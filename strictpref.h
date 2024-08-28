class StrictPref { 
private:
	vector<int> _v;
public:
	int get(int r, int c) const;
//	vector<int> getrow (int r) const;
	StrictPref();
	StrictPref(int m);	
	StrictPref(vector<int> w);
    
    int get(int c);
	void print() const;
	int numcands() const;
	bool prefers (int a, int b) const;
	void set(int c, int n);
	void invert();
};

