#include <vector>
#include <iostream>
#include <cmath>
using namespace std;
#include "profile.h"
#include "assign.h"
#include "extra.h"
#include "rankmatrix.h"
#include "measures_assign.h"



RankMatrix MeasuresAssign::rankdist(Profile p, Assign q) //for discrete assignments only
{
	int n = p.numvoters();
	int m = p.numcands();
	
	RankMatrix M = RankMatrix(n,m);
	
	for (int i = 0; i < n; i++) 
	{
		for (int j = 0; j < m; j++)
		{
			M.set(i, j, int(q.get(i) == p.get(i,j)));
		}
	}
	//Extra::print(M); d
	return M;
}

double MeasuresAssign::utilwelf (vector<double> u) 
{
	double uw = 0.0;
	int N = u.size();
	for (int i = 0; i<N; i++)
	{
		uw = uw + u[i];
	}	
	return uw/N;
}

double MeasuresAssign::egalwelf (vector<double> u) 
{
	double ew = u[0];
	int N = u.size();
	for (int i = 1; i<N; i++)
	{
		if (u[i] < ew)
		{
			ew = u[i];
		}
	}	
	return ew;
}

double MeasuresAssign::maxwelf (vector<double> u) 
{
	double mw = u[0];
	int N = u.size();
	for (int i = 1; i<N; i++)
	{
		if (u[i] > mw)
		{
			mw = u[i];
		}
	}	
	return mw;
}

double MeasuresAssign::nashwelf (vector<double> u) 
{
	double nw = 1.0;
	int N = u.size();
	for (int i = 0; i<N; i++)
	{
		nw = nw*u[i]; 
	}	
	return nw; // unnormalized by product for now, may have overflow problems?
}

double MeasuresAssign::orderbias (vector<double> u) 
{
	double ma = u[0];
	double mi = u[0];
	int N = u.size();
	for (int i = 1; i<N; i++)
	{
		if (u[i] < mi)
		{
			mi = u[i];
		}
		if (u[i] > ma)
		{
			ma = u[i];
		}
	}	
	return (ma - mi);
}

double MeasuresAssign::obmatnorm (RankMatrix M)
{
	int N = M.numrows();
	double c,t, t2;
	t = 0.0;
	t2 = 0.0;
	
	for (int j = 0; j < N; j++)
	{
		c = 0.0;
		t2 = t2 + M.get(0, j);
		for (int i = 0; i < N; i++)	
		{
			c = c + M.get(i,j);	
		}
		c = c/N;
		for (int i = 0; i < N; i++)	
		{	
			double oi = M.get(i,j) - c;
			M.set(i,j,oi);					
		}
		for (int i = 0; i< N; i++)
		{
			t = t + M.get(i,j)*M.get(i,j);

		}	
	}
	return (sqrt(t/N)/t2);
	
}
