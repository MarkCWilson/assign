namespace Extra
{
	template <typename T>
	void print(std::vector<T> v) // print a vector as row
	{
		std::cout << v[0];
		for (int i = 1; i < v.size(); i++) 
		{
			std::cout << "," << v[i];	
		}
		std::cout << std::endl;
	}	
	
	template <typename T>
	void print(std::vector<std::vector<T> > v)  // print a matrix
	{
		int n = v.size();
		for (int i = 0; i < n; i++) 
		{
			std::cout << v[i][0];
			for (int j = 1; j < v[i].size(); j++) 
			{
				std::cout << "," << v[i][j];	
			}
			std::cout << std::endl;
		}
	std::cout << std::endl;	
	}	
	
	
	std::vector<double>  add(std::vector<double> v, std::vector<double> w); //need to check size

	
	
	
		
}