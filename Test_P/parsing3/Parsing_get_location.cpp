#include "Headers/Parsing.hpp"
# include <iostream>


/*
**
**	on doit avoir
autoindex					ok
root						ok
error_page
dav_method					ok
upload_path					ok
client_body_buffer_size		ok
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
		{
			std::cout << "go root  = " << scope_location[i] << std::endl;
			
			if (this->ft_get_root_location(i, scope_location, index_server, index_location))
			{
				std::cout << "Erreur, dans get root location" << std::endl;
				return (-1);
			}
			std::cout << "FIN DE ROOT root location = " << this->_servers[index_server].location[index_location].root_location << std::endl;
			std::cout << "i = " << i << " et scope_location[i] = " << scope_location[i] << std::endl;
			i += 2;
		}
		else if (scope_location[i] == "dav_methods")
		{
			std::cout << "go dav_methods = " << scope_location[i] << std::endl;
			
			i = this->ft_get_methods_location(i, scope_location, index_server, index_location);
			if (i == 0)
			{
				std::cout << "Erreur dans get methods location" << std::endl;
				return (-1);
			}
			std::cout << "FIN DE METHODS LOCATION = " << this->_servers[index_server].location[index_location].methods_location.size() << std::endl;
		}
		else if (scope_location[i] == "autoindex")
		{
			std::cout << "go autoindex = " << scope_location[i] << std::endl;
			if (this->ft_get_autoindex_location(i, scope_location, index_server, index_location))
			{
				std::cout << "Erreur dans get autoidnex location" << std::endl;
				return (-1);
			}
			std::cout << "FIN DE GET AUTOINDEX LOCATION = " <<  this->_servers[index_server].location[index_location].autoindex_location << std::endl;
			i += 2;
		}
		else if (scope_location[i] == "upload_store")
		{
			std::cout << "go upload_store = " << scope_location[i] << std::endl;
			if (this->ft_get_upload_store_location(i, scope_location, index_server, index_location))
			{
				std::cout << "Erreur dans get upload store location " << std::endl;
				return (-1);
			}
			std::cout << "FIN DE GET upload store location = " << this->_servers[index_server].location[index_location].upload_store_location << std::endl;
			i += 2;
		}
		else if (scope_location[i] == "client_body_buffer_size")
		{
			std::cout << "go client_body_buffer_size = " << scope_location[i] << std::endl;
			if (this->ft_get_buffer_size_location(i, scope_location, index_server, index_location))
			{
				std::cout << "Erreur dans get buffer size locatuon " << std::endl;
				return (-1);
			}
			std::cout << "FIN DE GET get buffer location = " << this->_servers[index_server].location[index_location].buffer_size_location << std::endl;
			i += 2;
		
		}
		else if (scope_location[i] == "error_page")
		{
			std::cout << "go error_page = " << scope_location[i] << std::endl;
			i = ft_get_error_location(i, scope_location, index_server, index_location);
			if (i < 0)
			{
				std::cout << "Erreur dans get erreur location " << std::endl;
				return (-1);
			}
			std::cout << "Fin de get error location = " << this->_servers[index_server].location[index_location].error_location.size() << std::endl;
		}
		else
		{
			std::cout << "ELSE LOCATION  = " << scope_location[i] << std::endl;
			break;
		}
		//i++;
	}




	(void)k;
	(void)tmp;
	(void)index_server;

	std::cout << "GOOD" << std::endl;
	std::cout << "\n\n" << std::endl;
	return (0);
}


/*
**	ft_get_error( size_t k, std::vector<std::string> tmp, size_t index_server ):
**		This function will check the information given in the 'error_page' directive.
**		It will check the error codes indicated and it will check if the files exist.
**		It will save the data in a std::map<int, std::string>  container.
**
**	==> Returns the new increamentation of the parsing, otherwise returns -1 if an error occurs.
*/
size_t          Parsing::ft_get_error_location( size_t k, std::vector<std::string> tmp, size_t index_server, size_t index_location )
{
	// on incremente k on passe errr_page
	k += 1;
	while (tmp[k][tmp[k].size() - 1] != ';')
	{
		std::cout << "\tLA tmp[k] = " << tmp[k] << std::endl;
		int y = 0;
		while (tmp[k][y])
		{
			if (isdigit(tmp[k][y]))
			{
				y++;
			}
			else
			{
				std::cout << "Error: error_page directive should only have numbers then a directory!" << std::endl;
				return (-1);
				//break;
			}
		}
		int error_code = std::strtol(tmp[k].c_str(), NULL, 10);
		if (this->ft_check_code_error(error_code) == 1)
		{
			std::cout << "EUh il faut quitter? " << std::endl;
			std::cout << "car = " << this->ft_check_code_error(error_code) << std::endl;
			return (-1);
		}
		this->_servers[index_server].location[index_location].error_location.insert(std::pair<int, std::string>(error_code, "NULL"));
		k++;
	}
	if (tmp[k][0] != '.' || tmp[k][1] != '/')
	{
		std::cout << "Error, error_page directive should end with a directory or file" << std::endl;
		return (-1);
	}
	std::string address = tmp[k].substr(0, tmp[k].size() - 1);
	struct stat buffer;
	if (stat(address.c_str(), &buffer) != 0)
	{
		std::cout << "Error, error_page directive, directory doesn't exist!" << std::endl;
		return (-1);
	}

	if (this->_servers[index_server].location[index_location].error_location.size() > 1)		// several error pages.
	{
		// on ajute l'addresse a toutes les erreurs
		std::map<int, std::string>::iterator it = this->_servers[index_server].location[index_location].error_location.begin();
		for (it = this->_servers[index_server].location[index_location].error_location.begin(); it != this->_servers[index_server].location[index_location].error_location.end(); it++)
		{
			struct stat buff;

			if (it->second == "NULL")
				it->second = address;

			std::size_t found = it->second.find(".html");
			if (found == std::string::npos)
			{
				std::stringstream ss;
				std::string check_c;
				ss << it->first;
				ss >> check_c;
				check_c.append(".html");
				if (it->second[it->second.size() - 1] != '/')
					it->second.append("/");
				it->second.append(check_c);
			}
			if (stat(it->second.c_str(), &buff) < 0)
			{
				std::cout << "Error, error_page directive, cannot find the error file" << std::endl;
				return (-1);
			}
			if (buff.st_size == 0)
			{
				std::cout << "Error, error_page directive, file is empty" << std::endl;
				return (-1);
			}
		}
	}
	else		// only one error page
	{
		std::map<int, std::string>::iterator it = this->_servers[index_server].location[index_location].error_location.begin();
		if (it->second == "NULL")
			it->second = address;
		struct stat buff;
		std::size_t found = it->second.find(".html");
		if (found == std::string::npos)
		{
			std::stringstream ss;
			std::string check_c;
			ss << it->first;
			ss >> check_c;
			check_c.append(".html");
			if (it->second[it->second.size() - 1] != '/')
				it->second.append("/");
			it->second.append(check_c);
		}
		if (stat(it->second.c_str(), &buff) < 0)
		{
			std::cout << "Error, error_page directive, cannot find the error file" << std::endl;
			return (-1);
		}
		if (buff.st_size == 0)
		{
			std::cout << "Error, error_page directive, file is empty" << std::endl;
			return (-1);
		}
	}
	k++;
	return (k);
}


/*
**	ft_get_buffer_size( size_t k, std::vector<std::string> tmp, size_t index_server ):
**		This function will check the information given in the 'client_body_buffer_size' directive.
**		The information given is between 8000 (8k) and 16000 (16k) maximum.
**		The information will be used for the 'POST' command.
**
**	==> Returns 1 if an error occurs, otherwise returns 0.
*/
bool			Parsing::ft_get_buffer_size_location( size_t k, std::vector<std::string> tmp, size_t index_server, size_t index_location )
{
	size_t		i;
	int			buffer_size;

	k += 1;
	i = 0;
	buffer_size = 0;
	while (isdigit(tmp[k][i]))
		i++;
	if (i == 0)
	{
		std::cout << "Error, in 'client_body_buffer_size' directive, it should only be digits." << std::endl;
		return (true);
	}
	if (tmp[k][i] == 'k' && tmp[k][i + 1] == ';' && i + 1 == tmp[k].size() - 1 && tmp[k][i + 2] == '\0')
	{
		buffer_size = std::strtol(tmp[k].c_str(), NULL, 10);
		if (buffer_size < 8 || buffer_size > 16)
		{
			std::cout << "Error, in 'client_body_buffer_size' directive, buffer size must be between 8k and 16k." << std::endl;
			return (true);
		}
		this->_servers[index_server].location[index_location].buffer_size_location = buffer_size * 1000;
	}
	else
	{
		if (tmp[k][i] == ';' && i + 1 == tmp[k].size() && tmp[k][i + 1] == '\0')
		{
			buffer_size = std::strtol(tmp[k].c_str(), NULL, 10);
			if (buffer_size < 8000 || buffer_size > 16000)
			{
				std::cout << "Error, in 'client_body_buffer_size' directive, buffer size must be between 8000 and 16000." << std::endl;
				return (true);
			}
			this->_servers[index_server].location[index_location].buffer_size_location = buffer_size;
		}
		else
		{
			std::cout << "Error, in 'client_body_buffer_size' directive, informations are corrupted." << std::endl;
			return (true);
		}
	}
	return (false);
}


/*
**	ft_get_upload_store( size_t k, std::vector<std::string> tmp, size_t index_server ):
**		This function will check the information given in the 'upload_store' directive.
**		The information given is an fodler where we can find files uploaded.
**
**	==> Returns 1 if an error occurs, otherwise returns 0.
*/
bool			Parsing::ft_get_upload_store_location( size_t k, std::vector<std::string> tmp, size_t index_server, size_t index_location )
{
	struct stat buffer;
	size_t  	len;
	
	k += 1;
	len = tmp[k].size();
	if (tmp[k][len] != '\0')
	{
		std::cout << "Error, in 'upload_store' directive, it should end with '\0'" << std::endl;
		return (true);
	}
	if (tmp[k][len - 1] != ';')
	{
		std::cout << "Error, in 'upload_store' directive, it should end with ';'" << std::endl;
		return (true);
	}
	if (tmp[k][0] != '.' || tmp[k][1] != '/')
	{
		std::cout << "Error, in 'upload_store' directive, it should start with './'" << std::endl;
		return (true);
	}
	this->_servers[index_server].location[index_location].upload_store_location = tmp[k].substr(0, len - 1);
	if (stat(this->_servers[index_server].location[index_location].upload_store_location.c_str(), &buffer) == -1)
	{
		std::cout << "Error, 'upload_store' directive doesn't exist!" << std::endl;
		return (true);
	}
	return (false);
}


/*
**	ft_get_autoindex( size_t k, std::vector<std::string> tmp, size_t index_server):
**      This function will checks the informations given in the 'autoindex' directive.
**      It should find "on;" or "off;".
**
**     ==> Returns 0 if no problem happens, otherwise returns 1.
*/
bool            Parsing::ft_get_autoindex_location( size_t k, std::vector<std::string> tmp, size_t index_server, size_t index_location )
{
	k += 1;
	size_t  len = tmp[k].size();
	if (tmp[k][len] != '\0')
	{
		std::cout << "Error, in 'autoindex' directive it should end with '\0'" << std::endl;
		return (true);
	}
	if (tmp[k][len - 1] != ';')
	{
		std::cout << "Error, in 'autoindex' directive it should end with ';'" << std::endl;
		return (true);
	}
	if (tmp[k].compare("on;") != 0 && tmp[k].compare("off;") != 0)
	{
		std::cout << "Error, in 'autoindex' directive it should be 'on' or 'off' " << std::endl;
		return (true);
	}
	if (tmp[k].compare("on;") == 0)
		 this->_servers[index_server].location[index_location].autoindex_location = true;
	else
		this->_servers[index_server].location[index_location].autoindex_location = false;
	return (false);
}


/*
**	ft_get_root( size_t k, std::vector<std::string> tmp, size_t index_server ):
**		This function will checks the informations given in the 'root' directive FROM a location bloc.
**			It is the same fonction as 'ft_get_root'.
**		It will also checks if the folder exists.
**
**	==> Returns 0 if no problem happens, otherwise returns 1
*/
bool            Parsing::ft_get_root_location( size_t k, std::vector<std::string> tmp, size_t index_server, size_t index_location )
{
	struct stat buffer;
	size_t  	len;
	
	k += 1;
	len = tmp[k].size();
	if (tmp[k][len] != '\0')
	{
		std::cout << "Error, in 'root' directive, it should end with '\0'" << std::endl;
		return (true);
	}
	std::cout << "ICI" << std::endl;
	if (tmp[k][len - 1] != ';')
	{
		std::cout << "Error, in 'root' directive, it should end with ';'" << std::endl;
		return (true);
	}
	if (tmp[k][0] != '.' || tmp[k][1] != '/')
	{
		std::cout << "Error, in 'root' directive, it should start with './'" << std::endl;
		return (true);
	}
	this->_servers[index_server].location[index_location].root_location = tmp[k].substr(0, len - 1);
	if (stat(this->_servers[index_server].location[index_location].root_location.c_str(), &buffer) == -1)
	{
		std::cout << "Error, 'root' directive doesn't exist!" << std::endl;
		return (true);
	}
	return (false);
}



/*
**	ft_get_methods( size_t k, std::vector<std::string> tmp, size_t index_server ):
**		This function will check the information given in the 'dav_methods' directive.
**		It only accept 3 methods: 'DELETE', 'GET' and 'POST'
**
**	==> Returns the next directive if no error occurs, otherwise display an error message and it returns 0.
*/
size_t          Parsing::ft_get_methods_location( size_t k, std::vector<std::string> tmp, size_t index_server, size_t index_location )
{
	std::vector<std::string> methods;

	methods.push_back("DELETE");
	methods.push_back("GET");
	methods.push_back("POST");
	k += 1;
	while (1)
	{
		

		std::cout << "Dans la boucle " << std::endl;
		if (tmp[k].compare("DELETE") == 0)
			this->_servers[index_server].location[index_location].methods_location.push_back("DELETE");
		else if (tmp[k].compare("POST") == 0)
			this->_servers[index_server].location[index_location].methods_location.push_back("POST");
		else if (tmp[k].compare("GET") == 0)
			this->_servers[index_server].location[index_location].methods_location.push_back("GET");
		else
		{
			if (tmp[k].compare("DELETE;") == 0)
				this->_servers[index_server].location[index_location].methods_location.push_back("DELETE");
			else if (tmp[k].compare("POST;") == 0)
				this->_servers[index_server].location[index_location].methods_location.push_back("POST");
			else if (tmp[k].compare("GET;") == 0)
				this->_servers[index_server].location[index_location].methods_location.push_back("GET");
			else
			{
				std::cout << "Error, in 'dav_methods' directive, it can only have DELETE POST and GET methods! 1" << std::endl;
				return (0);
			}
			break;
		}
		k++;
	}
	if (this->_servers[index_server].location[index_location].methods_location.size() > 3 || this->_servers[index_server].location[index_location].methods_location.size() == 0)
	{
		std::cout << "Error, in 'dav_methods' directive, it can only have 3 methods maximum! 2" << std::endl;
		return (0);
	}
	k++;
	if (tmp[k] == "GET" || tmp[k] == "GET;" || tmp[k] == "DELETE" || tmp[k] == "DELETE;" || tmp[k] == "POST" || tmp[k] == "POST;")
	{
		std::cout << "Error, in 'dav_methods' directive, methods are not correct. 3" << std::endl;
		return (0);
	}

	// on verifie les doublons
	size_t int_del = 0;
	size_t int_get = 0;
	size_t int_post = 0;
	std::vector<std::string>::iterator it;
	for (it = this->_servers[index_server].location[index_location].methods_location.begin(); it != this->_servers[index_server].location[index_location].methods_location.end(); it++)
	{
		std::cout << "it = " << *it << std::endl;
		if (*it == "DELETE")
			int_del++;
		else if (*it == "POST")
			int_post++;
		else if (*it == "GET")
			int_get++;
		else
		{
			std::cout << "Error, in 'dav_methods' directive, this method is unkonwn 4." << std::endl;
			return (0);
		}
		if (int_del > 1 || int_post > 1 || int_get > 1)
		{
			std::cout << "Error, in 'dav_methods' directive, it should not have doublons. 5" << std::endl;
			return (0);
		}
	}
	return (k);
}
