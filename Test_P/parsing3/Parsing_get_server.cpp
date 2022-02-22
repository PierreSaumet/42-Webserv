#include "Headers/Parsing.hpp"
# include <iostream>


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
			return (-1);
		}
		this->_servers[index_server].error_server.insert(std::pair<int, std::string>(error_code, "NULL"));
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