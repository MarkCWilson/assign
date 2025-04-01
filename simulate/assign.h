class Assign{

private:
	vector<int> _v;
public:
	int get(int i);
	void set(int i, int a);
	int size();
	Assign();
	Assign(vector<int> alloc);
	Assign(vector<double> alloc);
	Assign(int m);
	Assign(string file);
    void print();
    bool is_good();
    void invert();
};
