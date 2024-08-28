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
#include "rankmatrix.h"
using namespace AssignRules;
using namespace MeasuresAssign;
using namespace Extra;

RankMatrix SimulateAssignBos::analyse_rm_bos(Profile p, Profile tb, char rule) 
{

	int N = p.numvoters(); // square case, no checking
	Assign q(N);
	RankMatrix M(N,N);	

	q = BostonKU(p,tb,rule);	
	M = rankdist(p,q);
	return M;
		
}	

vector<double> SimulateAssignBos::analyse_bos(Profile p, Profile tb, char rule, string output) 
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
	
	
	q = BostonKU(p,tb,rule);
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
		cout << rule << " results " << endl;	
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


void SimulateAssignBos::simulate_bos(int N, int n, double phi, char tie, char reftype) 
{ 
	
	cout.precision(15); // 15 digit output
	
	vector<vector<double> > outN;
	vector<vector<double> > outBob;
	vector<vector<double> > outBop;
	
	vector<double> row(7,0);
	
	Profile p;
	Profile tb;
	
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
		tb = Profile(N,N,0.0,ref); // standard version
		
		if (tie == 'c') // Condorcet version
		{			
			for (int r = 0; r < N; r++)
			{
			 	for (int c = 0; c < N; c++)
			 	{
			 		tb.set(r,c, (r+c)%N);
			 	}
			}
		}
		
		
		
		// now start computing
		row = analyse_bos(p, tb, 'N', "summ"); 
		outN.push_back(row);
		row = analyse_bos(p, tb, 'x', "summ"); 
		outBob.push_back(row);
		row = analyse_bos(p, tb, 'y', "summ"); 
		outBop.push_back(row);
	}
		
	ostringstream filename1;
	ostringstream filename2;
	ostringstream filename3;
	
   
    filename1 << "../../../outputs/numerical_results/FPW2021/matrixextreme/mallows/util/"<<N<<"n/" << phi << "_Bos";
    ofstream file1;
    file1.open(filename1.str());
    cout.rdbuf(file1.rdbuf());
	print(outN);	
	file1.close();	

	filename2 << "../../../outputs/numerical_results/FPW2021/matrixextreme/mallows/util/"<<N<<"n/" << phi<<"_Bob"; 
	ofstream file2(filename2.str());
	cout.rdbuf(file2.rdbuf());
	print(outBob);	
	file2.close();
		
	filename3 << "../../../outputs/numerical_results/FPW2021/matrixextreme/mallows/util/"<<N<<"n/" << phi<<"_Bop";
	ofstream file3(filename3.str());
	cout.rdbuf(file3.rdbuf());
	print(outBop);	
	file3.close();
	
}

void SimulateAssignBos::simulate_rm_bos(int N, int n, double phi, char tie, char reftype) 
{ 
	
	cout.precision(15); // 15 digit output
	vector<vector<double> > out;
	RankMatrix MBos(N,N);
	RankMatrix MBob(N,N);
	RankMatrix MBop(N,N);
	
	
	Profile p;
	Profile tb;
	
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
			 if (tie == 'c') // Condorcet version
			 {
			 	tb = Profile(N,N,0.0,ref); // hack to generate unanimous profile
			 	for (int r = 0; r < N; r++)
			 	{
			 		for (int c = 0; c < N; c++)
			 		{
			 			tb.set(r,c, (r+c)%N);
			 		}
			 	}
			 }
			 else
			 {
			 	tb = Profile(N,N,phi,ref); // correlated version
			 }
		
		MBos = MBos.add(analyse_rm_bos(p, tb, 'N')); 
		MBob = MBob.add(analyse_rm_bos(p, tb, 'x')); 
		MBop = MBop.add(analyse_rm_bos(p, tb, 'y')); 	
	}
	// this part doesn't work right? - should not need so many objects - it works however
	
	ostringstream filename1;
	ostringstream filename2;
	ostringstream filename3;
	   
    filename1 << "../../../outputs/numerical_results/FPW2021/matrixextreme/mallows/rankdist/"<<N<<"n/" << phi << "_Bos";
    ofstream file1;
    file1.open(filename1.str());    
    cout.rdbuf(file1.rdbuf());
	MBos.print();
	file1.close();
	
	filename2 << "../../../outputs/numerical_results/FPW2021/matrixextreme/mallows/rankdist/"<<N<<"n/" << phi<<"_Bob"; 
	ofstream file2(filename2.str());
	cout.rdbuf(file2.rdbuf());
	MBob.print();
	file2.close();
	
	filename3 << "../../../outputs/numerical_results/FPW2021/matrixextreme/mallows/rankdist/"<<N<<"n/" << phi<<"_Bop";
	ofstream file3(filename3.str());
	cout.rdbuf(file3.rdbuf());
	MBop.print();
	file3.close();
		
}