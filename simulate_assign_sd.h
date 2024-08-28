namespace SimulateAssignSD
{		
	vector<double> analyse_sd(Profile p, vector<int> v, int c, string output);
	RankMatrix analyse_rm_sd(Profile p, vector<int> v, int c);
	void simulate_sd(int N, int n, double phi, char reftype);
	void simulate_rm_sd(int N, int n, double phi, char reftype);
}
