#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
using namespace std;

#include "rankmatrix.h"
#include "extra.h"

using namespace Extra;

//constructors

RankMatrix::RankMatrix(int N, int m)
{
       _m.resize(N, vector<double>(m,0));
}

//accessors

void RankMatrix::print() // must be more idiomatic way, inherit somehow
{
	Extra::print(_m);
}

double RankMatrix::get(int r, int c)  const
{
	return _m[r][c];// need to do range checking for robustness
}

int RankMatrix::numrows() const
{
	return _m.size();
}

int RankMatrix::numcols() const
{
	return _m[0].size();
}

//mutators

void RankMatrix::set(int r, int c, double f)
{
	_m[r][c] = f; // need to do range checking for robustness
}

RankMatrix RankMatrix::add(RankMatrix p2) 
{
	int N = this->numrows();
	int m = this->numcols();
	RankMatrix p = *this;
	// no dimension checking
	{
		for (int i = 0; i < N; i++) 
		{
			for(int j = 0; j < m; j++)
			{
				p.set(i,j, p.get(i,j) + p2.get(i,j));
			}
		}		
	} 
	return p;
}

void RankMatrix::scale(double x) 
{
	int N = this->numrows();
	int m = this->numcols();
	// no dimension checking
	{
		for (int i = 0; i < N; i++) 
		{
			for(int j = 0; j < m; j++)
			{
				_m[i][j] = _m[i][j]*x;
			}
		}		
	} 
}

void RankMatrix::cdf() // gives cumulative probability of getting item at least that good
{
	int N = this->numrows();
	int m = this->numcols();
	RankMatrix p = *this;
	// no dimension checking
	{
		for (int i = 0; i < N; i++) 
		{
			for(int j = 1; j < m; j++)
			{			
				_m[i][j] = _m[i][j] + _m[i][j-1]; // clever?
			}
		}		
	} 
}


//functions

vector<double> RankMatrix::utilscore (vector<double> w) // score given weight vector
// no checking for bad input, as usual
{	
	RankMatrix M = *this;
	int n = _m.size(); // number of agents
	int m = _m[0].size(); // number of items
	vector<double> u(n,0);
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			u[i] = u[i] + w[j]*_m[i][j];
		}	
	}
	return u;	
}
