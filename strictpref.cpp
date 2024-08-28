#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <random>
#include <unordered_set>
using namespace std;
#include "extra.h"
#include "strictpref.h"

//constructors

StrictPref::StrictPref() // empty pref
{
	
}

StrictPref::StrictPref(int m) // dummy "pref" with m candidates
{
     _v.resize(m, 0); 
}

StrictPref::StrictPref(vector<int> w) // profile from vector
{
	_v = vector<int>(w);
}

// accessors

void StrictPref::print() const  // print in row format
{
	for (int j = 1; j < _v.size(); j++) 
	{
		cout << " " << _v[j];
	}
	cout << endl;
}

int StrictPref::numcands() const // number of candidates
{ 
	
	return _v.size();	
}

int StrictPref::get(int c) // initialize a particular element
	{
		return _v[c];	
	}

bool StrictPref::prefers (int a, int b) const
// does voter with inverse preference prefer a to b?
// assumes a,b are integers 0..m-1 and profile gives INVERSE preferences
{
	
	return _v[a] < _v[b];
}


// mutators

void StrictPref::set(int c, int i) // initialize a particular element
	{
		_v[c] = i;	
	}

void StrictPref::invert() // invert preference list to order by item not preference
{
	vector<int> temp = _v;
	
	for (int j = 0; j < _v.size(); j++)
	{
		_v[temp[j]] = j;
	}
}
