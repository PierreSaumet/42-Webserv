#include "Headers/Parsing.hpp"
# include <iostream>


/*
**
**	on doit avoir
autoindex
root
error_page
dav_method
upload_path
client_body_buffer_size
*/

/*
**	location /// { ==>fonctionne}
*/
size_t			Parsing::ft_get_location( size_t k, std::vector<std::string> tmp, size_t index_server )
{
	std::cout << "\n\ndans location" << std::endl;
	std::cout << "k = " << k << std::endl;
	std::cout << "tmp[k] = " << tmp[k] << std::endl;
	std::cout << "index = " << index_server << std::endl;

	// on ajoute un bloc location
	this->_servers.push_back(Parsing::t_server());

	this->_servers[index_server].location.push_back(Parsing::t_location());
	// this->_servers[index_server].location++;
	std::cout << " TAILLE DE location dans le server = " << this->_servers[index_server].location.size() << std::endl;
	// on verifie qu'il y a / et quelaue chose apres et {
	if (tmp[k] != "location" || tmp[k + 2] != "{")
	{
		std::cout << "Error, in 'location' directive, it needs 'location' + '/' + '{'." << std::endl;
		return (-1);
	}
	if (tmp[k + 1][0] == '/')
	{
		size_t i = 1;							// peut etre pas obligatoire
		while (isalpha(tmp[k + 1][i]))
			i++;
		if (tmp[k + 1].size() - 1 == i)
			if (tmp[k + 1][i + 1] != '/')
			{
				std::cout << "Error, in 'location' directive, it needs 'location' + '/' + '{'." << std::endl;
				return (-1);
			}
	}
	else if (tmp[k + 1][0] != '/')
	{
		std::cout << "Error, in 'location' directive, it needs 'location' + '/' + '{'." << std::endl;
		return (-1);
	}
	// et on recupere le nom 
	size_t index_location = 0;				//	== a changer
	this->_servers[index_server].location[index_location].name_location = tmp[k + 1].substr(0, tmp[k + 1].size());
	std::cout << "test nom = " << this->_servers[index_server].location[index_location].name_location << std::endl;


	// on recupere le scope pour travailler
	k += 2;
	std::cout << "tmp = " << tmp[k] << std::endl;
	std::vector<std::string>    scope_location = this->ft_get_scope(k);
	std::cout << "size_du scope location = " << scope_location.size() << std::endl;

	// on parcourt le scope pour recuperer les donnees avec des fonctions
	size_t i = 1;
	while (i < scope_location.size())
	{
		std::cout << "DANS boucle scope location" << std::endl;
		if (scope_location[i] == "root")
			std::cout << "go root  = " << scope_location[i] << std::endl;
		else if (scope_location[i] == "dav_methods")
			std::cout << "go dav_methods = " << scope_location[i] << std::endl;
		else if (scope_location[i] == "autoindex")
			std::cout << "go autoindex = " << scope_location[i] << std::endl;
		else if (scope_location[i] == "upload_path")
			std::cout << "go upload_path = " << scope_location[i] << std::endl;
		else if (scope_location[i] == "client_body_buffer_size")
			std::cout << "go client_body_buffer_size = " << scope_location[i] << std::endl;
		else if (scope_location[i] == "error_page")
			std::cout << "go error_page = " << scope_location[i] << std::endl;
		else
			std::cout << "autre  = " << scope_location[i] << std::endl;
		i++;
	}




	(void)k;
	(void)tmp;
	(void)index_server;

	std::cout << "GOOD" << std::endl;
	std::cout << "\n\n" << std::endl;
	return (0);
}