namespace SimulateAssignBos
{		
	vector<double> analyse_bos(Profile p, Profile tb, char rule, string output);
	RankMatrix analyse_rm_bos(Profile p, Profile tb, char rule);
	void simulate_bos(int N, int n, double phi, char tie, char reftype);
	void simulate_rm_bos(int N, int n, double phi, char tie, char reftype);		
}
