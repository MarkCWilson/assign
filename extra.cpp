#include <iostream>
#include <string>
#include <vector>
#include "extra.h"

using namespace std;

vector<double>  Extra::add(vector<double> v, vector<double> w) 
{
	vector<double> x;
	if(v.size() == w.size())
	{		
		for(int i = 0; i < v.size(); i++)
		{
			x.push_back(v[i] + w[i]);
		}
	}
	else
	{
		x = v;
	}
	return x;
}