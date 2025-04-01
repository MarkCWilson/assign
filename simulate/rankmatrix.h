#pragma once
class RankMatrix { 
private:
	vector<vector<double> > _m;
public:
	RankMatrix(int N, int m);
	double get(int r, int c) const;
	vector<int> getrow (int r) const;
	void print();
	int numrows() const;	
	int numcols() const;
	void set(int r, int c, double f);
	vector<double> utilscore (vector<double> w); // score given weight vector
	RankMatrix add (RankMatrix M);
	void scale(double x);
	void cdf();
};

