#include "../Headers/Parsing.hpp"
#include <iostream>


void		Parsing::put_name_server( void ) {

	char letter = 'a';
	for (std::vector<t_server>::iterator it = this->_servers.begin(); it != this->_servers.end(); it++)
	{
		std::string name = "random_serv_";
		if (it->name_server.empty() == true)
		{
			name.push_back(letter);
			it->name_server = name;
			letter++;
		}		
	}

}