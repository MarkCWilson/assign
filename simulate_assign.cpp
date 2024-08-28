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
#include "rankmatrix.h"
using namespace AssignRules;
using namespace MeasuresAssign;
using namespace Extra;

RankMatrix SimulateAssign::analyse_rm(Profile p, vector<int> v, char rule) 
{

	int N = p.numvoters(); // square case, no checking
	Assign q(N);
	RankMatrix M(N,N);
	
	switch (rule)
	{
		case 'S':
		{
			q = SD(p, v);	
			break;
		}
		case 'R':
		{
			q = SD(p);	
			break;
		}
		case 'T':
		{
			q = Assign(v);
			q = TTC(p, q);
			break;
		}
		case 'N':
		{
			q = Boston(p, v);
			break;	
		}
		case 'Z':
		{
			q = Boston(p, v,'x');
			break;	
		}
		case 'W':
		{
			q = Boston(p, v, 'y');
			break;	
		}
		case 'A':
		{
			q = BostonAd(p, v);	
			break;
		}
		case 'B':
		{
			q = Boston(p, v, 'b');	
			break;
		}
		case 'X':
		{
			q = BostonAd(p, v,'x');	
			break;
		}
		case 'Y':
		{
			q = BostonAd(p, v,'y');	
			break;
		}
		case 'I':
		{
			q = Imposed(p,v);
			break;
		}
				
	}
	
	M = rankdist(p,q);
	return M;
		
}	


vector<double> SimulateAssign::analyse(Profile p, vector<int> v, char rule, string output) 
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
	
	
	switch (rule)
	{
		case 'S':
		{
			q = SD(p, v);	
			break;
		}
		case 'R':
		{
			q = SD(p);	
			break;
		}
		case 'T':
		{
			q = Assign(v);
			q = TTC(p, q);
			break;
		}
		case 'N':
		{
			q = Boston(p, v);
			break;	
		}
		case 'Z':
		{
			q = Boston(p, v,'x');
			break;	
		}
		case 'W':
		{
			q = Boston(p, v,'y');
			break;	
		}
		case 'B':
		{
			q = Boston(p, v,'b');
		}
		case 'A':
		{
			q = BostonAd(p, v);	
			break;
		}
		case 'X':
		{
			q = BostonAd(p, v,'x');	
			break;
		}
		case 'Y':
		{
			q = BostonAd(p, v,'y');	
			break;
		}
		case 'I':
		{
			q = Imposed(p,v);
			break;
		}
	}
	
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
	
	//out.push_back(obmatnorm(M));
		
				
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


void SimulateAssign::simulate(int N, int n, double phi) 
{ 
	
	cout.precision(15); // 15 digit output
	vector<vector<double> > outS;
	vector<vector<double> > outR;
	vector<vector<double> > outT;
	vector<vector<double> > outN;
	vector<vector<double> > outA;
	vector<vector<double> > outZ;
	vector<vector<double> > outW;
	vector<vector<double> > outB;
	vector<vector<double> > outX;
	vector<vector<double> > outY;
	
	vector<int> v(N,-1);
	vector<double> row(10,0);
	
	for(int i = 0; i < N; i++) 
	{
		v[i] = i;
	}
	
	Profile p;
	for(int i = 0; i < n; i++) 
	{	
		
		if (phi == 1.0) 
		{
			p = Profile(N,N); 	
		}
		else
		{
			 p = Profile(N,N,phi,v);
		}
		row = analyse(p, v, 'S', "summ"); 
		outS.push_back(row);
		row = analyse(p, v, 'R', "summ"); 
		outR.push_back(row);
		row = analyse(p, v, 'T', "summ"); 
		outT.push_back(row);
		row = analyse(p, v, 'N', "summ"); 
		outN.push_back(row);
		row = analyse(p, v, 'A', "summ"); 
		outA.push_back(row);
		row = analyse(p, v, 'Z', "summ"); 
		outZ.push_back(row);
		row = analyse(p, v, 'W', "summ"); 
		outW.push_back(row);
		row = analyse(p, v, 'B', "summ"); 
		outB.push_back(row);	
		row = analyse(p, v, 'X', "summ"); 
		outX.push_back(row);
		row = analyse(p, v, 'Y', "summ"); 
		outY.push_back(row);
	}
		
	ostringstream filename1;
	ostringstream filename2;
	ostringstream filename3;
	ostringstream filename4;
	ostringstream filename5;
	ostringstream filename6;
	ostringstream filename7;
	ostringstream filename8;
	ostringstream filename9;
	ostringstream filename10;
    
    //basef = "../../../outputs/numerical_results/FPW2021/matrixextreme/mallows/"
    //basef = "/Users/mwilson/Dropbox/Mark_research/simulation/outputs/numerical_results/FPW2021/matrixextreme/mallows/"
    //cout << basef << "\n";
    //filename1 <<basef<<"util/"<<N<<"n/"<< phi<<"_SD";
    filename1 << "../../../outputs/numerical_results/FPW2021/matrixextreme/mallows/util/"<<N<<"n/" << phi << "_SD";
    ofstream file1;
    file1.open(filename1.str());
    cout.rdbuf(file1.rdbuf());
	print(outS);	
	file1.close();
	
	filename5 << "../../../outputs/numerical_results/FPW2021/matrixextreme/mallows/util/"<<N<<"n/" << phi << "_RSD";
    ofstream file5(filename5.str());
    cout.rdbuf(file5.rdbuf());
	print(outR);	
	file5.close();
	
	filename2 << "../../../outputs/numerical_results/FPW2021/matrixextreme/mallows/util/"<<N<<"n/" << phi<<"_TTC"; 
	ofstream file2(filename2.str());
	cout.rdbuf(file2.rdbuf());
	print(outT);	
	file2.close();
		
	filename3 << "../../../outputs/numerical_results/FPW2021/matrixextreme/mallows/util/"<<N<<"n/" << phi<<"_B";
	ofstream file3(filename3.str());
	cout.rdbuf(file3.rdbuf());
	print(outN);	
	file3.close();
	
	filename4 << "../../../outputs/numerical_results/FPW2021/matrixextreme/mallows/util/"<<N<<"n/" << phi<<"_AB";
	ofstream file4(filename4.str());
	cout.rdbuf(file4.rdbuf());
	print(outA);	
	file4.close();
	
	filename6 << "../../../outputs/numerical_results/FPW2021/matrixextreme/mallows/util/"<<N<<"n/" << phi<<"_Z";
	ofstream file6(filename6.str());
	cout.rdbuf(file6.rdbuf());
	print(outZ);	
	file6.close();
	
	filename7 << "../../../outputs/numerical_results/FPW2021/matrixextreme/mallows/util/"<<N<<"n/" << phi<<"_RB";
	ofstream file7(filename7.str());
	cout.rdbuf(file7.rdbuf());
	print(outW);	
	file7.close();
	
	filename10 << "../../../outputs/numerical_results/FPW2021/matrixextreme/mallows/util/"<<N<<"n/" << phi<<"_BRB";
	ofstream file10(filename10.str());
	cout.rdbuf(file10.rdbuf());
	print(outB);	
	file10.close();
	
	
	filename8 << "../../../outputs/numerical_results/FPW2021/matrixextreme/mallows/util/"<<N<<"n/" << phi<<"_X";
	ofstream file8(filename8.str());
	cout.rdbuf(file8.rdbuf());
	print(outX);	
	file8.close();
	
	filename9 << "../../../outputs/numerical_results/FPW2021/matrixextreme/mallows/util/"<<N<<"n/" << phi<<"_Y";
	ofstream file9(filename9.str());
	cout.rdbuf(file9.rdbuf());
	print(outY);	
	file9.close();
		
}

void SimulateAssign::simulate_rm(int N, int n, double phi) 
{ 
	
	cout.precision(15); // 15 digit output
	vector<vector<double> > out;
	vector<int> v(N,-1);
	RankMatrix MS(N,N);
	RankMatrix MR(N,N);
	RankMatrix MT(N,N);
	RankMatrix MN(N,N);
	RankMatrix MA(N,N);
	RankMatrix MZ(N,N);
	RankMatrix MW(N,N);
	RankMatrix MB(N,N);
	RankMatrix MX(N,N);
	RankMatrix MY(N,N);
	
	for(int i = 0; i < N; i++) 
	{
		v[i] = i;
	}
	
	
	Profile p;
	for(int i = 0; i < n; i++) 
	{	
		
		if (phi == 1.0) 
		{
			p = Profile(N,N); // faster generation	
		}
		else
		{
			 p = Profile(N,N,phi,v);
		}
		
		MS = MS.add(analyse_rm(p, v, 'S')); 
		MR = MR.add(analyse_rm(p, v, 'R')); 
		MT = MT.add(analyse_rm(p, v, 'T')); 
		MN = MN.add(analyse_rm(p, v, 'N')); 
		MA = MA.add(analyse_rm(p, v, 'A'));
		MZ = MZ.add(analyse_rm(p, v, 'Z'));
		MW = MW.add(analyse_rm(p, v, 'W'));
		MB = MB.add(analyse_rm(p, v, 'B'));
		MX = MX.add(analyse_rm(p, v, 'X'));
		MY = MY.add(analyse_rm(p, v, 'Y')); 	
	}
	// this part doesn't work right? - should not need so many objects - it works however
	

	ostringstream filename1;
	ostringstream filename2;
	ostringstream filename3;
	ostringstream filename4;
	ostringstream filename5;
	ostringstream filename6;
	ostringstream filename7;
	ostringstream filename8;
	ostringstream filename9;
	ostringstream filename10;
   
    filename1 << "../../../outputs/numerical_results/FPW2021/matrixextreme/mallows/rankdist/"<<N<<"n/" << phi << "_SD";
    ofstream file1;
    file1.open(filename1.str());    
    cout.rdbuf(file1.rdbuf());
	MS.print();
	//cout << obmatnorm(MS);
	file1.close();
	
	filename2 << "../../../outputs/numerical_results/FPW2021/matrixextreme/mallows/rankdist/"<<N<<"n/" << phi<<"_TTC"; 
	ofstream file2(filename2.str());
	cout.rdbuf(file2.rdbuf());
	MT.print();
	//cout << obmatnorm(MT);
	file2.close();
	
	filename3 << "../../../outputs/numerical_results/FPW2021/matrixextreme/mallows/rankdist/"<<N<<"n/" << phi<<"_B";
	ofstream file3(filename3.str());
	cout.rdbuf(file3.rdbuf());
	MN.print();
//	cout << obmatnorm(MN);
	file3.close();
	
	filename4 << "../../../outputs/numerical_results/FPW2021/matrixextreme/mallows/rankdist/"<<N<<"n/" << phi<<"_AB";
	ofstream file4(filename4.str());
	cout.rdbuf(file4.rdbuf());
	MA.print();
//	cout << obmatnorm(MA);
	file4.close();
	
	filename5 << "../../../outputs/numerical_results/FPW2021/matrixextreme/mallows/rankdist/"<<N<<"n/" << phi << "_R";
    ofstream file5(filename5.str());
    cout.rdbuf(file5.rdbuf());
	MR.print();
//cout << obmatnorm(MR);
	file5.close();
	
	filename6 << "../../../outputs/numerical_results/FPW2021/matrixextreme/mallows/rankdist/"<<N<<"n/" << phi << "_Z";
    ofstream file6(filename6.str());
    cout.rdbuf(file6.rdbuf());
	MZ.print();
//	cout << obmatnorm(MZ);
	file6.close();
	
	filename7 << "../../../outputs/numerical_results/FPW2021/matrixextreme/mallows/rankdist/"<<N<<"n/" << phi << "_RB";
    ofstream file7(filename7.str());
    cout.rdbuf(file7.rdbuf());
	MW.print();
//	cout << obmatnorm(MW);
	file7.close();
	
	filename10 << "../../../outputs/numerical_results/FPW2021/matrixextreme/mallows/rankdist/"<<N<<"n/" << phi << "_BRB";
    ofstream file10(filename10.str());
    cout.rdbuf(file10.rdbuf());
	MB.print();
//	cout << obmatnorm(MB);
	file10.close();
	
	filename8 << "../../../outputs/numerical_results/FPW2021/matrixextreme/mallows/rankdist/"<<N<<"n/" << phi << "_X";
    ofstream file8(filename8.str());
    cout.rdbuf(file8.rdbuf());
	MX.print();
//	cout << obmatnorm(MX);
	file8.close();
	
	filename9 << "../../../outputs/numerical_results/FPW2021/matrixextreme/mallows/rankdist/"<<N<<"n/" << phi << "_Y";
    ofstream file9(filename9.str());
    cout.rdbuf(file9.rdbuf());
	MY.print();
//	cout << obmatnorm(MY);
	file9.close();
		
}