#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>
#include <random>
using namespace std;

#include "extra.h"
#include "profile.h"
#include "assign.h"
#include "assign_rules.h"
#include "measures_assign.h"
#include "simulate_assign.h"
#include "simulate_assign_bos.h"
#include "simulate_assign_sd.h"
#include "rankmatrix.h"
using namespace AssignRules;
using namespace MeasuresAssign;
using namespace Extra;

RankMatrix SimulateAssignSD::analyse_rm_sd(Profile p, vector<int> v, int c) 
{

	int N = p.numvoters(); // square case, no checking
	Assign q(N);
	RankMatrix M(N,N);	

	q = SD(p,v,c);	
	M = rankdist(p,q);
	return M;
		
}	

vector<double> SimulateAssignSD::analyse_sd(Profile p, vector<int> v, int c, string output) 
{
	
	vector<double> out;
	int N = p.numvoters(); // square case, no checking
	Assign q(N);
	RankMatrix M(N,N);
	
	vector<double> w(N,0); // Borda weights
	for (int i = 0; i < N; i++) 
	{
		w[i] = (N-1.0-i)/(N-1.0);
	}
	
	vector<double> wp(N,0); // plurality weights
	wp[0] = 1;
	
	
	q = SD(p,v,c);
	M = rankdist(p,q);
	
	vector<double> u = M.utilscore(w);
    out.push_back(utilwelf(u));
	out.push_back(egalwelf(u));
	out.push_back(maxwelf(u));
	out.push_back(nashwelf(u));
	out.push_back(orderbias(u));

	
	u = M.utilscore(wp);
	out.push_back(utilwelf(u));
	out.push_back(egalwelf(u));
	out.push_back(maxwelf(u));
	out.push_back(orderbias(u));
		
				
	if(output == "verb") 
	{
		cout  << " results " << endl;	
		q.print();
		cout << "UWBorda = " << out[0] << endl;
		cout << "EWBorda = " << out[1] << endl;	
		cout << "maxBorda = " << out[2] << endl;	
		cout << "Nash Borda = " << out[3] << endl;	
		cout << "order bias Borda = " << out[4] << endl;
		cout << "UWplur = " << out[5] << endl;
		cout << "EWplur = " << out[6] << endl;
		cout << "maxplur = " << out[7] << endl;	
		cout << "order bias plur = " << out[8] << endl;	
	
		return out;
	}
	else if (output == "summ") 
	{
		return out;		
	}	
	return out;	
}	


void SimulateAssignSD::simulate_sd(int N, int n, double phi, char reftype) 
{ 
	
	cout.precision(15); // 15 digit output
	
	vector<vector<double> > outS0;
	vector<vector<double> > outS1;
	vector<vector<double> > outSm;
	
	vector<double> row(7,0);
	
	Profile p;
	vector<int> v = vector<int>(N,0);
	for(int j = 0; j < N; j++) // standard order of agents
    		{
        		v[j] = j;         		
    		}	
	
	vector<int> ref(N,0);
	if (reftype == 'r') // reverse reference order
	{
		for (int j = 0; j < N; j++)
			 	{
			 		ref[j] = N-1-j;
			 	} //  non-identity choice for reference permutation for Mallows	
	}
	else
	{
		for(int j = 0; j < N; j++) // WLOG initial reference order is identity permutation
    		{
        		ref[j] = j;         		
    		}		
	}
	
	
	
	for(int i = 0; i < n; i++) 
	{	
		
		p = Profile(N,N,phi,ref);
		cout<< "done to profile" << endl;
				
		// now start computing
		row = analyse_sd(p, v, 0, "verb"); 
		cout<< "done to SD0" << endl;
		outS0.push_back(row);
		row = analyse_sd(p, v, 1, "verb"); 
		cout<< "done to SD1" << endl;
		outS1.push_back(row);
		row = analyse_sd(p, v, N, "verb"); 
		cout<< "done to SDN" << endl;
		outSm.push_back(row);
	}
		
	ostringstream filename1;
	ostringstream filename2;
	ostringstream filename3;
	
   
    filename1 << "../../../outputs/numerical_results/FPW2021/matrixextreme/mallows/util/"<<N<<"n/" << phi << "_SD0";
    ofstream file1;
    file1.open(filename1.str());
    cout.rdbuf(file1.rdbuf());
	print(outS0);	
	file1.close();	

	filename2 << "../../../outputs/numerical_results/FPW2021/matrixextreme/mallows/util/"<<N<<"n/" << phi<<"_SD1"; 
	ofstream file2(filename2.str());
	cout.rdbuf(file2.rdbuf());
	print(outS1);	
	file2.close();
		
	filename3 << "../../../outputs/numerical_results/FPW2021/matrixextreme/mallows/util/"<<N<<"n/" << phi<<"_SDN";
	ofstream file3(filename3.str());
	cout.rdbuf(file3.rdbuf());
	print(outSm);	
	file3.close();
	
}

void SimulateAssignSD::simulate_rm_sd(int N, int n, double phi, char reftype) 
{ 
	
	cout.precision(15); // 15 digit output
	vector<vector<double> > out;
	RankMatrix MS0(N,N);
	RankMatrix MS1(N,N);
	RankMatrix MSm(N,N);
	
	Profile p;
	vector<int> v(N,0);
	for (int i = 0; i < N; i++)
	{
		v[i] = i; // standard order of agents
	}
	
	vector<int> ref(N,0);
	if (reftype == 'r') // reverse reference order
	{
		for (int j = 0; j < N; j++)
			 	{
			 		ref[j] = N-1-j;
			 	} //  non-identity choice for reference permutation for Mallows	
	}
	else
	{
		for(int j = 0; j < N; j++) // WLOG initial reference order is identity permutation
    		{
        		ref[j] = j;         		
    		}		
	}
	
	
	
	for(int i = 0; i < n; i++) 
	{			
		p = Profile(N,N,phi,ref);
			
		MS0 = MS0.add(analyse_rm_sd(p, v, 0)); 
		MS1 = MS1.add(analyse_rm_sd(p, v, 1)); 
		MSm = MSm.add(analyse_rm_sd(p, v, N)); 	
	}
	// this part doesn't work right? - should not need so many objects - it works however
	
	ostringstream filename1;
	ostringstream filename2;
	ostringstream filename3;
	   
    filename1 << "../../../outputs/numerical_results/FPW2021/matrixextreme/mallows/rankdist/"<<N<<"n/" << phi << "_SD0";
    ofstream file1;
    file1.open(filename1.str());    
    cout.rdbuf(file1.rdbuf());
	MS0.print();
	file1.close();
	
	filename2 << "../../../outputs/numerical_results/FPW2021/matrixextreme/mallows/rankdist/"<<N<<"n/" << phi<<"_SD1"; 
	ofstream file2(filename2.str());
	cout.rdbuf(file2.rdbuf());
	MS1.print();
	file2.close();
	
	filename3 << "../../../outputs/numerical_results/FPW2021/matrixextreme/mallows/rankdist/"<<N<<"n/" << phi<<"_SDN";
	ofstream file3(filename3.str());
	cout.rdbuf(file3.rdbuf());
	MSm.print();
	file3.close();
		
}