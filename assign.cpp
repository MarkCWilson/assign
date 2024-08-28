#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
using namespace std;

#include "assign.h"
#include "extra.h"

using namespace Extra;

// only for integer assignments - need to redo for fractional

//constructors

Assign::Assign() // empty allocation no agents
{
	
}

Assign::Assign(int m) // empty allocation m agents
{
        _v.resize(m,-1); 
    
}

Assign::Assign(vector<int> alloc) //integer allocation
{
	_v.resize(alloc.size(),-1);
	for (int i = 0; i < alloc.size(); i++) 
	{
		_v[i] = alloc[i];
	}
}

Assign::Assign(vector<double> alloc) //fractional allocation
{
	_v.resize(alloc.size(),-1);
	for (int i = 0; i < alloc.size(); i++) 
	{
		_v[i] = alloc[i];
	}
}

Assign::Assign(string file) 
{
	ifstream infile;
	infile.open(file);
	if(!infile.fail())
	{
 // need to fill in yet	
	}
}

//accessors

void Assign::print()  // print in row vector format
{
	cout << _v[0];
	for (int i = 1; i < _v.size(); i++) 
	{
		cout << " " << _v[i];	
	}
	cout << endl;
}

int Assign::get(int i) // extract allocation of agent i
{
	return _v[i];// need to do range checking for robustness
}


int Assign::size() // number of agents
{
	return _v.size();
}

//mutators

void Assign::set(int i, int a) // set agent i allocation to n
//only integer allocations so far
// need to do range checking for robustness
{
	_v[i] = a;
}

void Assign::invert() // list by item not agent, assumes perm
{
	vector<int> temp = _v;
	
	for(int i = 0; i < _v.size(); i++)
	{		
		_v[temp[i]] = i;
			
	}
}


//functions

bool Assign::is_good() // checks whether all allocated elements are different
{
	int m = _v.size();
	int c = 0;
	vector<int>::iterator it;
	for (int i = 0; i < m; i++)
	{
		sort(_v.begin(), _v.end());	
		it = adjacent_find (_v.begin(), _v.end());
	}
	return (it == _v.end());
}
