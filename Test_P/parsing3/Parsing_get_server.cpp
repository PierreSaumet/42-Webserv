/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing_get_server.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psaumet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/01 11:07:34 by psaumet           #+#    #+#             */
/*   Updated: 2021/06/01 11:07:42 by psaumet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Headers/Parsing.hpp"

/*
**
*/
bool			Parsing::ft_check_directive_server( std::vector<std::string> scope_server, size_t server_size )
{
	std::map<std::string, bool>            	serv_dir;
	size_t 									count = 0;
	size_t 									k = 0;
	server_size = scope_server.size();
	serv_dir.insert(std::pair<std::string, bool>("listen", false));
	serv_dir.insert(std::pair<std::string, bool>("server_name", false));
	serv_dir.insert(std::pair<std::string, bool>("error_page", false));
	serv_dir.insert(std::pair<std::string, bool>("root", false));
	serv_dir.insert(std::pair<std::string, bool>("dav_methods", false));
	serv_dir.insert(std::pair<std::string, bool>("index", false));
	serv_dir.insert(std::pair<std::string, bool>("autoindex", false));
	serv_dir.insert(std::pair<std::string, bool>("upload_store", false));
	serv_dir.insert(std::pair<std::string, bool>("client_body_buffer_size", false));
	serv_dir.insert(std::pair<std::string, bool>("cgi_path", false));
	
	while (k < scope_server.size())
	{
		for (std::map<std::string, bool>::iterator it_b = serv_dir.begin(); it_b != serv_dir.end(); it_b++)
		{
			if (it_b->first == scope_server[k])
			{
				count++;
				if (it_b->second == false)
					it_b->second = true;
				else
					throw Error(7, "Error, in 'server block', it has a doublon.", 1);
			}
		}
		if (scope_server[k] == "location")
		{
			std::vector<std::string>    scope_location = this->ft_get_scope(k + 1);
			k = k + scope_location.size();
		}
		k++;
	}



	
	for (std::map<std::string, bool>::iterator it_b = serv_dir.begin(); it_b != serv_dir.end(); it_b++)
	{
		if (it_b->second == false)
		{
			if (it_b->first == "listen")
			{
				std::cout << "MISS listen" << std::endl;
				return (true);			
			}
		}
	}
	std::cout << " COUT =  " << count << std::endl;
	if (count < 6)
	{
		std::cout << "Error, miss : ";
		for (std::map<std::string, bool>::iterator it_b = serv_dir.begin(); it_b != serv_dir.end(); it_b++)
		{
			if (it_b->second == false)
				std::cout << "\"" << it_b->first << "\" ";
		}
		std::cout << "\t directives in a bloc server before a location bloc" << std::endl;
		return (true);
	}
	///////////////// A DECOUPER FIN FONCTION 1 //////////////////////////////////////////


	std::cout << "scope_server[k] == " << scope_server[k - 1] << std::endl;


	// std::cout << "STOP " << std::endl;
	// return (true);


	serv_dir.clear();

	this->_servers.push_back(Parsing::t_server());
	return (false);
}



bool Parsing::ft_find_directive_server( size_t k, std::vector<std::string> scope_server, size_t i )
{

	while (k < scope_server.size())
	{
		if (scope_server[k] == "listen")
		{
			std::cout << "go listen" << std::endl;
			if (this->ft_get_listen(k, scope_server, i))
				return (true);
			std::cout << "\thost = " << this->_servers[i].host_server << " et port = " << this->_servers[i].port_server << std::endl;
			k += 2;
		}
		else if (scope_server[k] == "server_name")
		{
			std::cout << "go server_name " << std::endl;
			if (this->ft_get_server_name(k, scope_server, i))
				return (true);
			std::cout << "\tserver_name = " << this->_servers[i].name_server << std::endl;
			k += 2;
		}
		else if (scope_server[k] == "autoindex")
		{
			std::cout << "go autoindex " << std::endl;
			if (this->ft_get_autoindex(k, scope_server, i))
				return (true);
			std::cout << "\tautoindex = " << this->_servers[i].autoindex_server << std::endl;
			k += 2;
		}
		else if (scope_server[k] == "root")
		{
			std::cout << "go root" << std::endl;
			if (this->ft_get_root(k ,scope_server, i))
				return (true);
			std::cout << "\troot = " << this->_servers[i].root_server << std::endl;
			k += 2;
		}
		else if (scope_server[k] == "error_page")
		{
			std::cout << "go error_page " << std::endl;
			k = this->ft_get_error(k, scope_server, i);
			if (k == 0)
				return (true);
			//k += 3;
		}
		else if (scope_server[k] == "dav_methods")
		{
			std::cout << "go dav_methods " << std::endl;
			k = this->ft_get_methods(k, scope_server, i);
			if (k == 0)
			{
				std::cout << "ERROR DANS DAV METHODS" << std::endl;
				return (true);
			}
			std::cout << "\tmedhods 1 = " << this->_servers[i].methods_server[0] << std::endl;
			//k += 2;
		}
		else if (scope_server[k] == "client_body_buffer_size")
		{
			std::cout << "go client_body_buffer_size" << std::endl;
			if (this->ft_get_buffer_size(k, scope_server, i))
				return (true);
			k += 2;
		}
		else if (scope_server[k] == "cgi_path")
		{
			std::cout << "go cgi path" << std::endl;
			if (this->ft_get_cgi_path(k, scope_server, i))
				return (true);
			k += 2;
		}
		else if (scope_server[k] == "upload_store")
		{
			std::cout << "go ft_get_upload_store " << std::endl;
			if (this->ft_get_upload_store(k, scope_server, i))
				return (true);
			k += 2;
		}
		else if (scope_server[k] == "server" && scope_server[k + 1] == "{")
		{
			std::cout << "Error, a bloc server cannot have another bloc server inside." << std::endl;
			return (true);
		}
		else if (scope_server[k] == "location")
		{
			std::cout << "go location " << std::endl;
			std::cout << "k = " << k << std::endl;
			k = ft_get_location( k, scope_server, i);
			std::cout << "fin de location : k = " << k << std::endl;
			if (k == 0)
				return (true);
			// return (true);
			// break;
			// k += 2;
		}
		else if (scope_server[k] == "index")
		{
			std::cout << "go index " << std::endl;
			if (this->ft_get_index(k, scope_server, i))
			{
				return (true);
			}
			k += 2;
		}
		else
		{
			std::cout << "\n\nDANS ELSE, tout est parse, il manque location et k == ." << scope_server[k] << std::endl;
			if (scope_server[k] == "}")
				break;
			else
			{
				std::cout << "INSTRUCTION NON RECONNU la" << std::endl;
				return (true);
			}
			//std::cout << " EUH ERROR " << scope_server[k] << " et k = " << k << std::endl;
			//if ()
			//return (true);
			//exit(EXIT_FAILURE);
			//break;
		}
	}
	return (false);
}


/*
**	ft_get_buffer_size( size_t k, std::vector<std::string> tmp, size_t index_server ):
**		This function will check the information given in the 'client_body_buffer_size' directive.
**		The information given is between 8000 (8k) and 16000 (16k) maximum.
**		The information will be used for the 'POST' command.
**
**	==> Returns 1 if an error occurs, otherwise returns 0.
*/
bool			Parsing::ft_get_buffer_size( size_t k, std::vector<std::string> tmp, size_t index_server )
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
		this->_servers[index_server].buffer_size_server = buffer_size * 1000;
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
			this->_servers[index_server].buffer_size_server = buffer_size;
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
**	ft_get_cgi_path( size_t k, std::vector<std::string> tmp, size_t index_server ):
**		This function will check the information given in the 'cgi_path' directive.
**		The information given is an fodler where we can find the data for the cgi.
**
**	==> Returns 1 if an error occurs, otherwise returns 0.
*/
bool			Parsing::ft_get_cgi_path( size_t k, std::vector<std::string> tmp, size_t index_server )
{
	struct stat buffer;
	size_t  	len;
	
	k += 1;
	len = tmp[k].size();
	if (tmp[k][len] != '\0')
	{
		std::cout << "Error, in 'cgi_path' directive, it should end with '\0'" << std::endl;
		return (true);
	}
	if (tmp[k][len - 1] != ';')
	{
		std::cout << "Error, in 'cgi_path' directive, it should end with ';'" << std::endl;
		return (true);
	}
	if (tmp[k][0] != '.' || tmp[k][1] != '/')
	{
		std::cout << "Error, in 'cgi_path' directive, it should start with './'" << std::endl;
		return (true);
	}
	this->_servers[index_server].cgi_path_server = tmp[k].substr(0, len - 1);
	if (stat(this->_servers[index_server].cgi_path_server.c_str(), &buffer) == -1)
	{
		std::cout << "Error, 'cgi_path' directive doesn't exist!" << std::endl;
		return (true);
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
bool			Parsing::ft_get_upload_store( size_t k, std::vector<std::string> tmp, size_t index_server )
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
	this->_servers[index_server].upload_store_server = tmp[k].substr(0, len - 1);
	if (stat(this->_servers[index_server].upload_store_server.c_str(), &buffer) == -1)
	{
		std::cout << "Error, 'upload_store' directive doesn't exist!" << std::endl;
		return (true);
	}
	return (false);
}

/*
**	ft_get_error( size_t k, std::vector<std::string> tmp, size_t index_server ):
**		This function will check the information given in the 'error_page' directive.
**		It will check the error codes indicated and it will check if the files exist.
**		It will save the data in a std::map<int, std::string>  container.
**
**	==> Returns the new increamentation of the parsing, otherwise returns -1 if an error occurs.
*/
size_t          Parsing::ft_get_error( size_t k, std::vector<std::string> tmp, size_t index_server )
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
				return (0);
				//break;
			}
		}
		int error_code = std::strtol(tmp[k].c_str(), NULL, 10);
		if (this->ft_check_code_error(error_code) == 1)
		{
			std::cout << "EUh il faut quitter? " << std::endl;
			std::cout << "car = " << this->ft_check_code_error(error_code) << std::endl;
			return (0);
		}
		this->_servers[index_server].error_server.insert(std::pair<int, std::string>(error_code, "NULL"));
		k++;
	}
	if (tmp[k][0] != '.' || tmp[k][1] != '/')
	{
		std::cout << "Error, error_page directive should end with a directory or file" << std::endl;
		return (0);
	}
	std::string address = tmp[k].substr(0, tmp[k].size() - 1);
	struct stat buffer;
	if (stat(address.c_str(), &buffer) != 0)
	{
		std::cout << "Error, error_page directive, directory doesn't exist!" << std::endl;
		return (0);
	}

	if (this->_servers[index_server].error_server.size() > 1)		// several error pages.
	{
		// on ajute l'addresse a toutes les erreurs
		std::map<int, std::string>::iterator it = this->_servers[index_server].error_server.begin();
		for (it = this->_servers[index_server].error_server.begin(); it != this->_servers[index_server].error_server.end(); it++)
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
				return (0);
			}
			if (buff.st_size == 0)
			{
				std::cout << "Error, error_page directive, file is empty" << std::endl;
				return (0);
			}
		}
	}
	else		// only one error page
	{
		std::map<int, std::string>::iterator it = this->_servers[index_server].error_server.begin();
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
			return (0);
		}
		if (buff.st_size == 0)
		{
			std::cout << "Error, error_page directive, file is empty" << std::endl;
			return (0);
		}
	}
	k++;
	return (k);
}

/*
**	ft_get_methods( size_t k, std::vector<std::string> tmp, size_t index_server ):
**		This function will check the information given in the 'dav_methods' directive.
**		It only accept 3 methods: 'DELETE', 'GET' and 'POST'
**
**	==> Returns the next directive if no error occurs, otherwise display an error message and it returns 0.
*/
size_t          Parsing::ft_get_methods( size_t k, std::vector<std::string> tmp, size_t index_server )
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
			this->_servers[index_server].methods_server.push_back("DELETE");
		else if (tmp[k].compare("POST") == 0)
			this->_servers[index_server].methods_server.push_back("POST");
		else if (tmp[k].compare("GET") == 0)
			this->_servers[index_server].methods_server.push_back("GET");
		else
		{
			if (tmp[k].compare("DELETE;") == 0)
				this->_servers[index_server].methods_server.push_back("DELETE");
			else if (tmp[k].compare("POST;") == 0)
				this->_servers[index_server].methods_server.push_back("POST");
			else if (tmp[k].compare("GET;") == 0)
				this->_servers[index_server].methods_server.push_back("GET");
			else
			{
				std::cout << "Error, in 'dav_methods' directive, it can only have DELETE POST and GET methods!" << std::endl;
				return (0);
			}
			break;
		}
		k++;
	}
	if (this->_servers[index_server].methods_server.size() > 3 || this->_servers[index_server].methods_server.size() == 0)
	{
		std::cout << "Error, in 'dav_methods' directive, it can only have 3 methods maximum!" << std::endl;
		return (0);
	}
	k++;
	if (tmp[k] == "GET" || tmp[k] == "GET;" || tmp[k] == "DELETE" || tmp[k] == "DELETE;" || tmp[k] == "POST" || tmp[k] == "POST;")
	{
		std::cout << "Error, in 'dav_methods' directive, methods are not correct." << std::endl;
		return (0);
	}

	// on verifie les doublons
	size_t int_del = 0;
	size_t int_get = 0;
	size_t int_post = 0;
	std::vector<std::string>::iterator it;
	for (it = this->_servers[index_server].methods_server.begin(); it != this->_servers[index_server].methods_server.end(); it++)
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
			std::cout << "Error, in 'dav_methods' directive, this method is unkonwn." << std::endl;
			return (0);
		}
		if (int_del > 1 || int_post > 1 || int_get > 1)
		{
			std::cout << "Error, in 'dav_methods' directive, it should not have doublons." << std::endl;
			return (0);
		}
	}
	return (k);
}

/*
**	ft_get_autoindex( size_t k, std::vector<std::string> tmp, size_t index_server):
**      This function will checks the informations given in the 'autoindex' directive.
**      It should find "on;" or "off;".
**
**     ==> Returns 0 if no problem happens, otherwise returns 1.
*/
bool            Parsing::ft_get_autoindex( size_t k, std::vector<std::string> tmp, size_t index_server )
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
		this->_servers[index_server].autoindex_server = true;
	else
		this->_servers[index_server].autoindex_server = false;
	return (false);
}

/*
**	ft_get_root( size_t k, std::vector<std::string> tmp, size_t index_server ):
**		This function will checks the informations given in the 'root' directive.
**		It will also checks if the folder exists.
**
**	==> Returns 0 if no problem happens, otherwise returns 1
*/
bool            Parsing::ft_get_root( size_t k, std::vector<std::string> tmp, size_t index_server )
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
	this->_servers[index_server].root_server = tmp[k].substr(0, len - 1);
	if (stat(this->_servers[index_server].root_server.c_str(), &buffer) == -1)
	{
		std::cout << "Error, 'root' directive doesn't exist!" << std::endl;
		return (true);
	}
	return (false);
}

/*
**	ft_get_server_name( size_t k, std::vector<std::string> tmp, size_t index_server ):
**		This function will checks the information given in the 'server_name' directive.
**		It should only have digit and alphabetic characters.
**
**	==>	Returns 0 if no problem happens, otherwise returns 1.
*/
bool         Parsing::ft_get_server_name( size_t k, std::vector<std::string> tmp, size_t index_server )
{
	k += 1;
	size_t  len = tmp[k].size();
	if (tmp[k][len] != '\0')
	{
		std::cout << "Error, in 'server_name' directive, it should end with '\0'" << std::endl;
		return (true);
	}
	size_t i;
	i = 0;
	while (isalnum(tmp[k][i]))
		i++;
	if (i + 1 < len)
	{
		std::cout << "Error, in 'server_name' directive, it should only have alphanumeric characters" << std::endl;
		return (true);
	}
	if (tmp[k][len - 1] != ';')
	{
		std::cout << "Error, in 'server_name' directive, it should end with ';'" << std::endl;
		return (true);
	}
	this->_servers[index_server].name_server = tmp[k].substr(0, len - 1);
	return (false);
}

/*
**  ft_get_listen( size_t km std::vector<std::string> tmp, size_t idnex_server ):
**      This function will checks the informations given in the 'listen' directive.
**      It should find a 'host' equal to '127.0.0.1' or 'localhost', an 'port' > 1 and < 65535.
**
**     ==> Returns 0 if no problem happens, otherwise returns 1.
*/
bool         Parsing::ft_get_listen( size_t k, std::vector<std::string> tmp, size_t index_server) 
{
	k += 1;
	size_t      len = tmp[k].size();
	if (tmp[k][len] != '\0')
	{
		std::cout << "Error, in 'listen directive' it should end with \0" << std::endl;
		return (true);
	}
	if (tmp[k][len - 1] != ';')
	{
		std::cout << "Error, in 'listen directive' it should end with ';'" << std::endl;
		return (true);
	}
	if (tmp[k].find(":", 0) == std::string::npos)
	{
		std::cout << "Error, in 'listen directive' it should have a ':' between host and port" << std::endl;
		return (true);
	}
	if (len > 15)
	{
		std::cout << "Error, in 'listen directive' it has bad host or port" << std::endl;
		return (true);
	}
	if (tmp[k].compare(0, 10, "127.0.0.1:") != 0 && tmp[k].compare(0, 10, "localhost:") != 0)
	{
		std::cout << "Error, in 'listen directive' host should be 127.0.0.1 or 'localhost'" << std::endl;
		return (true);
	}
	size_t i = 9;
	while (++i < tmp[k].size())
	{
		if (isdigit(tmp[k][i]) == 0)
			if (i < tmp[k].size() - 1)
			{
				std::cout << "Error, in 'listen directive' port should only have digit and a semicolon at the end"<< std::endl;
				return (true);
			}
	}
	this->_servers[index_server].host_server = tmp[k].substr(0, 9);
	this->_servers[index_server].port_server = std::strtol(tmp[k].substr(10, 4).c_str(), NULL, 10);
	if (this->_servers[index_server].port_server < 0 || this->_servers[index_server].port_server > 65535)
	{
		std::cout << "Error, in 'listen directive'  port should be between 0 and 65535" << std::endl;
		return (true);
	}

	if (index_server > 0)
	{
		size_t count = 0;
		while (count < index_server)
		{
			if (this->_servers[index_server].port_server == this->_servers[count].port_server)
			{

				std::cout << "Error, in 'listen directive', bloc servers have the same port." << std::endl;
				std::cout << " 0 = " << this->_servers[count].port_server << std::endl;
				std::cout << " et index_server = " << index_server << " et = " << this->_servers[index_server].port_server << std::endl;
				return (true);
			}
			count++;
		}
	}
	// std::cout << "PARFAIT" << std::endl;
	return false;
}




bool					Parsing::ft_check_code_error( int code ) const
{
	// if (server_code_error(code) == 1 || client_code_error(code) == 1)
	// 	return (1);
	// else
	// 	return (0);
	if (code > 399 && code < 500)
		return (this->ft_check_code_client(code));
	else if (code > 499 && code < 600)
		return (this->ft_check_code_serv(code));
	else
	{
		std::cout << "ERROR, code pas bon MERDE" << std::endl;
		return (1);
		//exit(EXIT_FAILURE);
		//throw Error(8, "Error, code error not correct", 1);
		// std::cout << "ERROR CODE PAS VALIDE" << std::endl;
		// exit(EXIT_FAILURE);
	}
	//return ( client_code_error(code));
}

/*
**	Parsing::server_code_error():
**		Check iff the given code is a server code.
**		if not, return 1.
*/
bool					Parsing::ft_check_code_serv( int code ) const
{
	if ((code >= 500 && code <= 508) || code == 510 || code == 511)
		return (false);
	else
		return (true);
}

/*
**	Parsing::client_code_error():
**		Check if the given code is a client code.
**		If not, return 1;
*/
bool					Parsing::ft_check_code_client( int code ) const
{
	if ((code >= 400 && code <= 417) || (code >= 421 && code <= 426) 
			|| code == 428 || code == 429 || code == 431 || code == 451 )		// || code == 499 pour NGINX
		return (false);
	else
		return (true);
}


bool					Parsing::ft_get_index( size_t k, std::vector<std::string> tmp, size_t index_server )
{
	struct stat buffer;
	size_t  	len;
	
	k += 1;
	len = tmp[k].size();
	if (tmp[k][len] != '\0')
	{
		std::cout << "Error, in 'index' directive, it should end with '\0'" << std::endl;
		return (true);
	}
	if (tmp[k][len - 1] != ';')
	{
		std::cout << "Error, in 'index' directive, it should end with ';'" << std::endl;
		return (true);
	}
	std::cout << "longueur de index = " << tmp[k].size() << std::endl;
	if (tmp[k].size() <= 6)
	{
		std::cout << "Error, in 'index' directive, it should have a proper name and end with '.html'." << std::endl;
		return (true);
	}
	std::cout << "tmp[k] de truc = " << tmp[k][tmp[k].size() - 6] << std::endl;
	if (tmp[k].compare(tmp[k].size() - 6, 6, ".html;") !=  0)
	{
		std::cout << "Error, in 'index' directive, it should end with '.html'." << std::endl;
		return (true);
	}
	this->_servers[index_server].index_server = tmp[k].substr(0, len - 1);
	if (stat(this->_servers[index_server].index_server.c_str(), &buffer) == -1)
	{
		std::cout << "Error, 'index' directive, file doesn't exist." << std::endl;
		return (true);
	}
	if (buffer.st_size == 0)
	{
		std::cout << "Error, in 'index' directive, file is empty." << std::endl;
		return (true);
	}
	return (false);
}