namespace AssignRules
{ 
	Assign SD(const Profile &p, vector<int> v, int c); 
	Assign SD(const Profile &p, vector<int> v); 
	Assign SD(const Profile &p); 
	Assign SDS(const Profile &p, vector<int> v); 
	Assign RSD(const Profile &p); 
	Assign TTC(const Profile &p, Assign v);
	Assign TTC(const Profile &p);
	Assign Boston(const Profile &p, vector<int> v, char flip);
	Assign Boston(const Profile &p, vector<int> v);
	Assign BostonQ(const Profile &p, vector<int> v);
	Assign BostonAd(const Profile &p, vector<int> v, char flip);
	Assign BostonAd(const Profile &p, vector<int> v);
    Assign BostonKU(const Profile &p, const Profile &tb, char flip);  
	Assign Imposed(const Profile &p, vector<int> v);
};
