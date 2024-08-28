#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <random>
#include <unordered_set>
using namespace std;
#include "strictpref.h"
#include "profile.h"
#include "extra.h"

//constructors

Profile::Profile() // empty profile
{
	
}

Profile::Profile(int N) // empty profile with N voters - profile is a vector of vectors
{
     _m.resize(N, vector<int>(0)); 
}

Profile::Profile(vector<vector<int>> v)
{
	int N = v.size();
	_m.resize(N, vector<int>(0));
	for(int i = 0; i < N; i++)
	{
		_m[i] = v[i];	
	}

}

Profile::Profile(string file) //reads profile in CSV format
{ // should add checking for bad input (e.g. repeated elements) 
	ifstream infile;
	string line;
	
	infile.open(file);
	
	if(infile.is_open())
	{
		while(!infile.eof()) 
		{							
		    std::getline(infile,line); 
		    if (!line.empty())
		    {		    
		    	_m.push_back(vector<int>(0));	
		    	istringstream iss(line);   			
	    		string token;	    		
	    
	    		while(std::getline(iss,token,','))
	    		{	       
	        		_m.back().push_back(atoi(token.c_str())); // push numbers into vector
	    		}	   
			}			
		}		
 	infile.close();
	}
	else 
	{
		cout << "Unable to open file";	
	}	
} 

Profile::Profile(int N, int m) // IC generator with N voters and m candidates
{
	mt19937 twister;
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	twister.seed(seed);
	srand(time(0));
	// RNG now set up

    _m.resize(N, vector<int>(m,0));  
       
   		vector<int> id(m,0); //identity permutation
   	 	for(int j = 0; j < m; j++) 
    		{
     	 	  id[j] = j;         		
    		}
   	 	for(int i = 0; i < N; i++) // generate orders independently over agents
    		{
    			shuffle(id.begin(), id.end(), twister); 
       		 	_m[i] = id;	
   			}

}


Profile::Profile(int m, int N, double phi, vector<int> ref) // Mallows generator with N voters and m
// candidates, parameter phi, reference order ref
// uses RIM from Doignon et al 2004 as described in Lu-Boutilier 2014

{
	mt19937 twister;
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	twister.seed(seed);
	srand(time(0));
	
	_m.resize(N, vector<int>(m,0));  
	
    Profile p;
    
    double t = 0;
    	for (int i = 0; i < N; i++)
    	{      
    	    vector<int> v;
    		for (int k = 0; k < m; k++) 
    		{ 	    		
    			vector<double> weights;
    			for(int l = 0; l <= k; l++) 
    			{
       				weights.push_back(pow(phi, k-l));
   				}
   				discrete_distribution<long> d(weights.begin(), weights.end());
    			
    			int pos = d(twister);
    			v.insert(v.begin()+ pos, ref[k]); //efficiency issue, runs slow?
    		}
    		_m[i] = v;
    	}
    	
}


/*
Profile::SP(int a, int b) // randomly generate single-peaked, from Toby Walsh 2015 arXiv paper
{
	vector<int> v;
	if (a==b)
	{
		return v;
	}
	else
	{
	*****
	}

}

*/

//accessors

vector<int> Profile::getrow (int r) const
// need to do range checking for robustness
{
	vector<int> row;
	for (int j = 0; j < _m[0].size(); j++) 
	{
		row.push_back(_m[r][j]);		
	}
	return row;
}

int Profile::get(int r, int c) const
{
	return _m[r][c];
}

void Profile::print() const  // print in matrix format
{
	for (int i = 0; i < _m.size(); i++) 
	{
		cout << _m[i][0];
		for (int j = 1; j < _m[i].size(); j++) 
		{
			cout << " " << _m[i][j];
		}
	cout << endl;
	}
}

int Profile::numvoters() const // number of agents
{ 
	return _m.size();
}

int Profile::numcands() const // number of candidates
{ 
	unordered_set<int> cands;
	for (int i = 0; i < _m.size(); i++) 
		{
			for (int j = 0; j < _m[i].size(); j++) 
				{
					cands.insert(_m[i][j]);
				}
		}
	return cands.size();	
}

bool Profile::prefers (int r, int a, int b) const
// does voter with inverse preference invpref prefer a to b?
// assumes a,b are integers 0..m-1 and profile gives INVERSE preferences
{
	
	return _m[r][a] < _m[r][b];
}

//mutators

void Profile::set(int r, int c, int i) // initialize a particular element
	{
		_m[r][c] = i;	
	}

void Profile::invert() // invert preference lists to order by item not preference
{
	vector<vector<int> > temp = _m;
	
	for(int i = 0; i < _m.size(); i++)
	{
		for (int j = 0; j < _m[i].size(); j++)
		{
			_m[i][temp[i][j]] = j;
		}
	}

}