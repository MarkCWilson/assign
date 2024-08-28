class Profile { 
private:
	vector<vector<int> > _m;
public:
	int get(int r, int c) const;
	vector<int> getrow (int r) const;
	Profile();
	Profile(int N);	
	Profile(vector<vector<int>> v);
	Profile(string file);
    Profile(int N, int m); // IC	
    Profile(int N, int m, double phi, vector<int> ref); // Mallows
	void print() const;
	int numvoters() const;	
	int numcands() const;
	bool prefers (int r, int a, int b) const;
	void set(int r, int c, int n);
	void invert();
};