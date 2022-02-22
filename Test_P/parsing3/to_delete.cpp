#include "Headers/Parsing.hpp"
#include <iostream>


void		Parsing::display_all( void ) {


	std::cout << "\n\n\n NOMBRE DE SERVER = " << this->_nbr_servers << std::endl;

	std::cout << "\n Serveur name = " << this->_servers[0].name_server << std::endl;
	std::cout << "\n Host server = " << this->_servers[0].host_server << std::endl;
	std::cout << "\n error server = " << this->_servers[0].methods_server.size() << std::endl;
	std::cout << "\n location server = " << this->_servers[0].location.size() << std::endl;
	std::cout << "\n location server name 0 = " << this->_servers[0].location[0].name_location << std::endl;
	std::cout << "\n location server name 1 = " << this->_servers[0].location[1].name_location << std::endl;
}