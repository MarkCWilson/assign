#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <queue>
#include <stack>
using namespace std;
#include "profile.h"
#include "assign.h"
#include "assign_rules.h"
#include "extra.h"


Assign AssignRules::Imposed(const Profile &p, vector<int> v) 
{  
	return Assign(v);
}

Assign AssignRules::SD(const Profile &p, vector<int> v, int c) // need to check v is perm
{
//using agent order v for the first c
    int N = p.numvoters();
	int m = v.size();
	Assign assign = Assign(m);
	vector<bool> avail = vector<bool>(m, true);
	for (int i = 0; i < c; i++)
	{
		int j = 0;
		while (avail[p.get(v[i],j)] == false)
		{
			j++;
		} // pointing to correct item now (need error checking if no objects left)
		int a = p.get(v[i],j);
		avail[a] = false;
		assign.set(v[i], a);
	}
// using agent order determined by first agent in profile for the rest
	v = p.getrow(0);
	for (int i = 0; i < m; i++)
	{
		if(assign.get(v[i]) == -1) // not assigned yet
		{		
			int j = 0;
			while (avail[p.get(v[i],j)] == false)
			{
				j++;
			} // pointing to correct item now (need error checking if no objects left)
			int a = p.get(v[i],j);
			avail[a] = false;
			assign.set(v[i], a);
		}			
	}    
	return assign;
}

Assign AssignRules::SD(const Profile &p, vector<int> v) 
{
	return AssignRules::SD(p, v, p.numvoters());
}


Assign AssignRules::SD(const Profile &p) 
{
	vector<int> v = p.getrow(0);
	return AssignRules::SD(p, v);
}


Assign AssignRules::SDS(const Profile &p, vector<int> v) 
// stack version
{
	int m = v.size();
	Assign assign = Assign(m);
	vector<bool> avail = vector<bool>(m, true);
	vector<int> bid(m,-1);
	int a = -1;
	int i = -1;

    stack<int> Q = stack<int>();
    for (int i = v.size() - 1; i >=0; i--) 
    {
    	Q.push(v[i]);
    }

	
	while (!Q.empty())
	{
	    i = Q.top();
	    bid[i] = bid[i]+1;
	    a = p.get(i, bid[i]); 
	   
	    //cout << "agent number" << i << " bid item " << a << endl;

		if (avail[a]) 
		{
			avail[a] = false;
			assign.set(i, a);
			Q.pop();
		}
		else
		{
			;
		}
	}	
		
	return assign;
}


Assign AssignRules::RSD(const Profile &p) 
{
    mt19937 twister;
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	twister.seed(seed);
	srand(time(0)); // RNG now set up
	
	int m = p.numvoters();
	vector<int> v(m,-1);

	
	for(int j = 0; j < m; j++) 
    {
        v[j] = j;         		
    }
    
    shuffle(v.begin(), v.end(), twister); 

	return AssignRules::SDS(p, v);
}

Assign AssignRules::TTC(const Profile &p, Assign assign) // not sure of efficiency, maybe use deque for pref orders
{
	int N = p.numvoters(); // no check for bad input 
	vector<bool> left = vector<bool>(N,true); // agents left
	vector<bool> avail = vector<bool>(N,true); // items left
	vector<int> out = vector<int>(N,-1); // current outneighbor of each agent
	vector<bool> seen;
	int it;
	int j;
	int t;
	
	int numleft = N; // number of agents left
	
	Assign ao = assign;
	ao.invert();
	
	vector<int> top = vector<int>(N,-1); // best item still available for each agent
	for (int i = 0; i < N; i++)
	{
		top[i] = 0; // index, not item
	}
	
	while(numleft > 0) // guard to stop infinite loop
	{
	
		//cout << " number of agents left " << numleft << endl;
	//	cout << " left vector " << endl;
		//Extra::print(left);
	//	cout << " avail vector" << endl;
	//	Extra::print(avail);
	//	cout << "current assignment" << endl;
	  //  assign.print();
		
	
		for(int i = 0; i < N ; i++) // run through remaining agents only
		{			
			seen = vector<bool>(N,false);
			
			if (left[i]) // run through remaining agents only
			{	
				 // index of best remaining item, needs updating
				
			//	cout << " agent " << i << " new top index " << top[i] << " item " << p.get(i,top[i]) << endl;
				j = i;
				while (left[j] && !seen[j])  //follow to find cycle
					{
						seen[j] = true;
					//	cout << "old j = " << j << endl;
						out[j] = ao.get(p.get(j,top[j])); // outneighbor of j
						j = out[j];
					//	cout << "new j = " << j << endl;
					}
				// we now have  cycle even if only of length 1, or are pointing to already gone node
				// reassign items along the cycle and mark agents as gone, or do nothing
				// now j is  first node seen in the cycle
				
				while (left[j]) 
				{
				//	cout << "REALLOCATING" << endl;
				   // cout << "old j = " << j << endl;
				    it = p.get(j,top[j]);
					assign.set(j, it);
					avail[it] = false;
					ao.set(it, j);
					left[j] = false;
					numleft--;
					j = out[j];
					//cout << "new j = " << j << endl;
				}
				for (int k = 0;  k < N; k++) // looks ugly - must be a more elegant way
				{
					if(left[k]) 
					{
						t = top[k];
					  	while (avail[p.get(k,t)] == false)// update top so new digraph
						{
							t++;				
						}	
						top[k] = t;
					}	
					
				}
			//	cout << " new assignment " << endl;
			//	assign.print();
			//	cout << " new inverse assignment " << endl;
			//	ao.print();
			//	cout << " new left " << endl;
			//	Extra::print(left);
			//	cout << " new avail " << endl;
			//	Extra::print(avail);
			//	cout << " new top " << endl; 
			//	Extra::print(top);
						
			}	
		}
	
	}
	

	return assign;
}

Assign AssignRules::TTC(const Profile &p) // random profile
{
	mt19937 twister;
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	twister.seed(seed);
	srand(time(0)); // RNG now set up
	
	int m = p.numvoters();
	vector<int> v(m,-1);

	for(int j = 0; j < m; j++) 
    {
        v[j] = j;         		
    }
    
    shuffle(v.begin(), v.end(), twister); 
    
	return AssignRules::TTC(p, Assign(v));
}

Assign AssignRules::BostonQ(const Profile &p, vector<int> v) 
// naive sequential version
{
	int m = v.size();
	Assign assign = Assign(m);
	vector<bool> avail = vector<bool>(m, true);
	vector<int> bid(m,-1);
	int a = -1;
	int i = -1;

    queue<int> Q = queue<int>();
    for (int e: v)
    {
    	Q.push(e);
    }

	
	while (!Q.empty())
	{
	    i = Q.front();
	    bid[i] = bid[i] + 1; // bid for next most preferred object
	    a = p.get(i, bid[i]); 
	   
	    //cout << "agent number" << i << " bid item " << a << endl;

		if (avail[a]) 
		{
			avail[a] = false;
			assign.set(i, a);
			Q.pop();
		}
		else
		{
			Q.pop();
			Q.push(i);
		}
	}	
		
	return assign;
}

Assign AssignRules::Boston(const Profile &p, vector<int> v, char flip)
// simultaneous  version
{
	int m = v.size(); // number of agents/items
	Assign assign = Assign(m); //initial empty assignment
	vector<bool> avail = vector<bool>(m, true); // is item available
	vector<int> bid(m,-1); //rank of item bid for by agent
	vector<int> biditem(m,-1); // actual item bid for by agent
	
	int a = -1; // item
	int i = -1; // agent index
	int c = 0; //how many people assigned so far
	int j; // general variable
	int r = 0; // round number

    while (c < m) 
    {
    	r = r+1; // counting number of rounds
    	//cout << " round number " << r << endl;
    	
    	for (int i = 0; i < m; i++) //deciding on bids - order of agents is unimportant
    	{
    		j = v[i]; //next agent
    		if (assign.get(j) == -1) //agent has no item yet
    		{
    			bid[j] = bid[j] + 1;
    			biditem[j] = p.get(j, bid[j]); 
    			//cout << "agent number " << j << " bid rank " << bid[j] << " bid item " << biditem[j] << endl;   			
    		}
    		//cout <<"biditems: \n";
    		//Assign(biditem).print();
    	
    	}
    	
    	if (flip == 'x' && r > 1) // Bobo
    		{
    			std::reverse(v.begin(), v.end());
    		}
    	else if (flip == 'y' && r == 2) // Bopper
    		{
    			std::reverse(v.begin(), v.end());
    		}
    	else if (flip == 'b' && r == 2) // block reversing 
    		{	
    			int a = 0; // start index of block
    			int i = 0; // runs though block
    			while (i < m)
    			{
    				//cout <<"block start item: " << p.get(a, 0) << "\n";
    				//cout <<"a = " << a << "\n";
    				i = a;
    				while(i < m && p.get(a,0)==p.get(i,0) )
    				{
    					i++; // now i points to start of next block
    				//	cout <<"new i = " << i << "\n";
    				}
    				std::reverse(v.begin()+a, v.begin()+i);
    				//cout << "current v = ";
    				//Assign(v).print();
    				a = i;
    				//cout <<"new a = " << a << "\n";
    			}    				
    		}
        else
        	{
        	;
        	}
    	
    	
    	for (int i = 0; i < m; i++) // assigning items - order of agents is important
    	{
    		j = v[i];
    		if (assign.get(j) == -1)
    		{
    				a = biditem[j];
    				if (avail[a])
    				{
    					assign.set(j,a); //take the item
    					avail[a] = false;
    					c = c+1;   		
    					//cout << "agent number " << j << " takes item " << a << endl;	
    					//cout << "total number assigned so far " << c << endl;
    				}
    		}
    	}
    	
    }
	//cout << "number of rounds " << r << endl;
	return assign;
}

Assign AssignRules::Boston(const Profile &p, vector<int> v)

{
	return AssignRules::Boston(p,v,'n');

}

Assign AssignRules::BostonAd(const Profile &p, vector<int> v, char flip) //not done yet
// simultaneous  version
{
	int m = v.size();
	Assign assign = Assign(m);
	vector<bool> avail = vector<bool>(m, true);
	vector<int> bid(m,-1);
	vector<int> biditem(m,-1);
	
	int a = -1;
	int i = -1;
	int c = 0; //how many people assigned so far
	int j;
	int r = 0;

    while (c < m) 
   
    {
   		r = r+1; // counting number of rounds
   		//cout << " round number " << r << endl;
    	for (int i = 0; i < m; i++)
    	{
    		j = v[i]; //next agent
    		if (assign.get(j) == -1) //agent has no item yet
    		{
    			bid[j] = bid[j] + 1; // normal bid for Naive Boston
    			while (!avail[p.get(j, bid[j])]) // adaptive case, keep going till find next available item
    			{
    				bid[j] = bid[j] + 1; 
    			}
    			biditem[j] = p.get(j, bid[j]); 
    		   // cout << "agent number" << j << "bid rank " << bid[j] << " bid item " << biditem[j] << endl;
    			
    		}
    	
    	}
    	
    	if (flip == 'x' && r > 1) // Bobo
    		{
    			std::reverse(v.begin(), v.end());
    		}
    	else if (flip == 'y' && r == 2) // Bopper
    		{
    			std::reverse(v.begin(), v.end());
    		}
    	else 
    		{			// usual Boston
    		};
    	
    	
    	for (int i = 0; i < m; i++)
    	{
    		j = v[i];
    		if (assign.get(j) == -1 )
    		{
    				a = biditem[j];
    				if (avail[a])
    				{
    					assign.set(j,a); //take the item
    					avail[a] = false;
    					c = c+1;   		
    					//cout << "agent number" << j << " takes item " << a << endl;	
    					//cout << "total number assigned so far " << c << endl;
    				}
    		}
    	}
    	
    }
	//cout << "number of rounds " << r << endl;
	return assign;
}

Assign AssignRules::BostonAd(const Profile &p, vector<int> v)

{
	return AssignRules::BostonAd(p,v,'n');

}


Assign AssignRules::BostonKU(const Profile &p, const Profile &tb, char flip)

{
	int m = p.numcands(); // number of items
	int N = p.numvoters(); // number of agents
	Assign assign = Assign(m); //initial empty assignment
	vector<bool> avail = vector<bool>(m, true); // is item available
	vector<int> bid(m,-1); //rank of item bid for by agent
	vector<int> biditem(m,-1); // actual item bid for by agent
	
	int a = -1; // item
	int i = -1; // agent index
	int c = 0; //how many people assigned so far
	int j; // general variable
	int r = 0; // round number
	vector<int> v; // temporary for tiebreak of an item

    while (c < m) 
    {
    	r = r+1; // counting number of rounds
    	//cout << "round number " << r << endl;
    	
    	for (int j = 0; j < m; j++) //deciding on bids - order of agents is unimportant
    	{
    		if (assign.get(j) == -1) //agent has no item yet
    		{
    			bid[j] = bid[j] + 1;
    			biditem[j] = p.get(j, bid[j]); 
    			//cout << "agent number " << j << " bid rank " << bid[j] << " bid item " << biditem[j] << endl;   			
    		}
    		//cout <<"biditems: \n";
    		//Assign(biditem).print();
    	
    	}
    	
    	
     	for (int i = 0; i < m; i++) // loop through items to assign them
    	{
    		if(avail[i])
    		{
    			v = tb.getrow(i);
    			if (flip == 'y' && r > 1) // Bopper
    			{
    				std::reverse(v.begin(), v.end());
    			}
    			else if (flip == 'x' && r%2==0) // Bobo
    			{
    				std::reverse(v.begin(), v.end());
    			}
    		
    			for (int j = 0; j < N; j++) // go through agents in i's tiebreak order
    			{
    				if (biditem[v[j]]==i && avail[i])
    				{
    					assign.set(v[j],i); //take the item	
    					c = c+1; 
    					avail[i] = false;
    					//cout << "agent number " << v[j] << " takes item " << i << endl;	
    					//cout << "total number assigned so far " << c << endl;   				
    				}    						
    			}
    		}
    	}		      		    		
	}
	return assign;
}



