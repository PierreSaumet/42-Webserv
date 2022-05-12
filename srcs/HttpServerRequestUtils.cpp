/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServerRequestUtils.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psaumet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/01 11:07:34 by psaumet           #+#    #+#             */
/*   Updated: 2021/06/01 11:07:42 by psaumet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Headers/HttpServer.hpp"

/*
**	std::string			HttpServer::ft_parsing_path_get_request( void ):
**		This function setup 'this->_header_requete[0].query_string' and put 
**	all data from a GET request, the data after ?.
**	Return emtpy if there is no data.
**		Alos, setup 'this->_header_requete[0].data' which is a container
**	with a key and value;
*/
std::string			HttpServer::ft_parsing_path_get_request( void )
{
	std::cout << RED << "Dans ft_parsing_path_get_request : FONCTION A CHANGER " << CLEAR << std::endl;
	size_t		pos_cursor = this->_header_requete[0].path.find("?");
	this->_header_requete[0].query_string = "";
	if (pos_cursor == std::string::npos)
		return ("");
	else
	{
		if (pos_cursor == this->_header_requete[0].path.size() - 1)
			return ("");
		std::string tmp = "";
		pos_cursor++;
		size_t len = this->_header_requete[0].path.length();
		while (this->_header_requete[0].path[pos_cursor])
		{
			if (this->_header_requete[0].path.compare(pos_cursor, 1, "=") == 0)
			{
				this->_header_requete[0].data.insert(std::pair<std::string, std::string>(tmp, "NULL"));
				tmp.clear();
			}
			else if (this->_header_requete[0].path.compare(pos_cursor, 1, "&") == 0 || pos_cursor == len - 1)
			{
				if (pos_cursor == len - 1)
					tmp.append(this->_header_requete[0].path, pos_cursor, 1);
				std::map<std::string, std::string>::iterator it_b = this->_header_requete[0].data.begin();
				for (; it_b != this->_header_requete[0].data.end(); it_b++)
				{
					if (it_b->second == "NULL")
					{
						tmp = ft_clean_path_get_request(tmp); // a refaire
						it_b->second = tmp;
						this->_header_requete[0].query_string.append(it_b->first);
						this->_header_requete[0].query_string.append("=");
						this->_header_requete[0].query_string.append(it_b->second);
						this->_header_requete[0].query_string.append("&");
					}
				}
				tmp.clear();
			}
			else
				tmp.append(this->_header_requete[0].path, pos_cursor, 1);
			pos_cursor++;
		}
	}
	if (this->_header_requete[0].query_string.empty())
		return ("");
	this->_header_requete[0].query_string.erase(this->_header_requete[0].query_string.end() - 1);
	return (this->_header_requete[0].query_string);
}


/*
**	Cette fonction compare le port de la requete avec le port des servers pour connaitre
**	quel server on utilise.
**	retourne -1 si une erreur sinon retourne le numero de l'emplacement du server.
*/
int HttpServer::ft_find_index_server( void ) // A DEPLACER
{
	std::string 	port_request = this->_header_requete[0].host;
	size_t			pos = port_request.find(":");
		
	port_request.erase(port_request.begin(), port_request.begin() + pos + 1);
	if (this->_servers.size() > 1)
	{
		size_t			y = 0;

		while (y < this->_servers.size())
		{
			std::stringstream 	ss;
			std::string			port_server;

			ss << this->_servers[y].port_server;
			ss >> port_server;
			if (port_server == port_request)
				return (y);
			y++;
		}
		return (-1);

	}
	std::stringstream 	ss;
	std::string			port_server;

	ss << this->_servers[0].port_server;
	ss >> port_server;
	if (port_server == port_request)
		return (0);
	return (-1);
}



int		HttpServer::ft_redirection( void )
{

	std::cout << GREEN << "Dans ft_redirection " << CLEAR << std::endl;
	if (this->_header_requete[0].location == true)
	{
		if (this->_servers[this->_num_serv].location[this->_num_loc].return_location.empty() == true)
		{
			std::cout << "On a pas de  redirection dans le bloc location : " << this->_servers[this->_num_serv].location[this->_num_loc].name_location << std::endl;
			this->_header_requete[0].return_used = false;
			return (0);
		}
		std::cout << "On a une redirection dans le bloc location : " << this->_servers[this->_num_serv].location[this->_num_loc].name_location << std::endl;
		this->_header_requete[0].return_used = true;
		this->_header_requete[0].path.erase(0, this->_servers[this->_num_serv].location[this->_num_loc].name_location.size());
		this->_header_requete[0].path.insert(0, this->_servers[this->_num_serv].location[this->_num_loc].return_location);
		return (1);
	}
	return (0);
}



size_t HttpServer::check_location( std::string path, std::string name_location )
{
	size_t 	i = 0;
	(void)i;
	std::cout << "\nFonction check_location avec le path = -" << path << "-" << " dans le bloc - " << name_location << std::endl;

	// On regarde si ca existe
	struct stat buff;
	if (stat(path.c_str(), &buff) < 0)
	{
		std::cout << "existe pas " << std::endl;
		sleep(2);
		return (1);
	}
	else if (S_ISREG(buff.st_mode))
	{
		std::cout << "FICHIER donc bon" << std::endl;
		return (0);
	}
	else if (S_ISDIR(buff.st_mode))
	{
		if (path[path.size() - 1] == '/')
		{
			if (this->_servers[this->_num_serv].location[this->_num_loc].index_location.empty() == false)	// if index location, return index location
			{
				this->_header_requete[0].path.append(this->_servers[this->_num_serv].location[this->_num_loc].index_location);
				return (0);
			}
			if (this->_servers[this->_num_serv].location[this->_num_loc].autoindex_location ==  true)		// if autoindex location, return autoindex location
			{
				this->_header_requete[0].autoindex = true;
				return (0);
			}
			// Adding index of server
			std::string tmp = this->_servers[this->_num_serv].index_server;
			if (tmp[0] == '.')
				tmp.erase(0, 1);
			this->_header_requete[0].path.append(tmp);
			return (0); 
		}
		return (2); // 403
	}
	else
		return (1); // 404
}


size_t 	HttpServer::ft_check_access_path( void )
{
	std::cout << GREEN <<  "\n\nDans verifie le droit duchemin" << CLEAR << std::endl;

	if (this->_header_requete[0].path.find("/flavicon.ico") != std::string::npos)
	{
		if (this->_header_requete[0].path.compare("/flavicon.ico") == 0)
		{
			this->_header_requete[0].path.clear();
			this->_header_requete[0].path.insert(0, this->_servers[this->_num_serv].root_server);
			this->_header_requete[0].path.append("/flavicon.ico");
			return (0);
		}
		std::cout << "404 flavicon" << std::endl;
		exit(1);
		return (1); // 404
	}
	if (this->_header_requete[0].location == false)
	{
		std::cout << "pas de location donc on compare la demande avec l'acces de l'index" << std::endl;
		std::cout << "path = " << this->_header_requete[0].path << std::endl;

		struct stat		buff_path;
		if (this->_header_requete[0].path == "/")			// On demande l'index on retourne 0
		{
			this->_header_requete[0].path.insert(0, this->_servers[this->_num_serv].root_server);
			this->_header_requete[0].path.append(this->_servers[this->_num_serv].index_server);
			if (stat(this->_header_requete[0].path.c_str(), &buff_path) < 0)
				return (1); // 404
			return (0); // good
		}
		std::string		path_index_server = this->_servers[this->_num_serv].index_server;
		if (path_index_server[0] == '.')
			path_index_server.erase(0, 1);
		struct stat		buff_index_server;

		path_index_server.insert(0, this->_servers[this->_num_serv].root_server);	
		stat(path_index_server.c_str(), &buff_index_server);
		this->_header_requete[0].path.insert(0, this->_servers[this->_num_serv].root_server);
		if (stat(this->_header_requete[0].path.c_str(), &buff_path) < 0)
			return (1); // 404
		if (buff_path.st_dev == buff_index_server.st_dev && buff_path.st_ino == buff_index_server.st_ino)
			return (0);
		else
			return (2); // 403
	}
	else
	{
		if (this->_servers[this->_num_serv].location[this->_num_loc].root_location.empty() == false)
		{
			std::cout << "path = " << this->_header_requete[0].path << std::endl;
			if (this->_servers[this->_num_serv].location[this->_num_loc].name_location != "/") // on supprime le nom du bloc location et on met le root location
			{
				if (this->_servers[this->_num_serv].location[this->_num_loc].name_location[this->_servers[this->_num_serv].location[this->_num_loc].name_location.size() - 1] == '/')	
					this->_header_requete[0].path.erase(0, this->_servers[this->_num_serv].location[this->_num_loc].name_location.size() - 1);
				else
					this->_header_requete[0].path.erase(0, this->_servers[this->_num_serv].location[this->_num_loc].name_location.size());
			}
			std::cout << "path = " << this->_header_requete[0].path << std::endl;
			this->_header_requete[0].path.insert(0, this->_servers[this->_num_serv].location[this->_num_loc].root_location);
			std::cout << "path = " << this->_header_requete[0].path << std::endl;
			// exit(1);
		}
		this->_header_requete[0].path.insert(0, this->_servers[this->_num_serv].root_server);
		return (this->check_location(this->_header_requete[0].path, this->_servers[this->_num_serv].location[this->_num_loc].name_location ));
	}
	
	std::cout << "pas bon " << std::endl;
	exit(1);

}



/*
**	find_replace and ft_clean_path get request
**		on doit les deplacer ailleurs.
**	clean path get request est a ameliorer. c'est du brut force LOL
*/
std::string		find_replace( std::string to_search, std::string to_replace, std::string data )
{
	size_t pos = data.find(to_search);

	while (pos != std::string::npos)
	{
		data.replace(pos, to_search.size(), to_replace);
		pos = data.find(to_search, pos + to_replace.size());
	}
	return (data);
}
// URL ENCODING
std::string		HttpServer::ft_clean_path_get_request( std::string tmp )  // A TERMINER
{
	std::map<std::string, std::string> url_coding;

	// url_coding.insert(std::pair<std::string , std::string>("+", " ")); // space	
	url_coding.insert(std::pair<std::string , std::string>("%21", "!")); 	
	url_coding.insert(std::pair<std::string , std::string>("%22", "\"")); 	
	url_coding.insert(std::pair<std::string , std::string>("%23", "#")); 	
	url_coding.insert(std::pair<std::string , std::string>("%24", "$")); 
	url_coding.insert(std::pair<std::string , std::string>("%25", "%")); 	
	url_coding.insert(std::pair<std::string , std::string>("%26", "&")); 
	url_coding.insert(std::pair<std::string , std::string>("%27", "'")); 	
	url_coding.insert(std::pair<std::string , std::string>("%28", "(")); 	
	url_coding.insert(std::pair<std::string , std::string>("%29", ")")); 	
	url_coding.insert(std::pair<std::string , std::string>("%2A", "*"));
	url_coding.insert(std::pair<std::string , std::string>("%2B", "+"));
	url_coding.insert(std::pair<std::string , std::string>("%2C", ","));
	url_coding.insert(std::pair<std::string , std::string>("%2D", "-"));
	url_coding.insert(std::pair<std::string , std::string>("%2E", "."));
	url_coding.insert(std::pair<std::string , std::string>("%2F", "/"));
	url_coding.insert(std::pair<std::string , std::string>("%3A", ":"));
	url_coding.insert(std::pair<std::string , std::string>("%3B", ";"));
	url_coding.insert(std::pair<std::string , std::string>("%3C", "<"));
	url_coding.insert(std::pair<std::string , std::string>("%3D", "="));
	url_coding.insert(std::pair<std::string , std::string>("%3E", ">"));
	url_coding.insert(std::pair<std::string , std::string>("%3F", "?"));
	url_coding.insert(std::pair<std::string , std::string>("%40", "@"));
	url_coding.insert(std::pair<std::string , std::string>("%5B", "["));
	url_coding.insert(std::pair<std::string , std::string>("%5C", "\\"));
	url_coding.insert(std::pair<std::string , std::string>("%5D", "]"));
	url_coding.insert(std::pair<std::string , std::string>("%5E", "^"));
	url_coding.insert(std::pair<std::string , std::string>("%5F", "_"));
	url_coding.insert(std::pair<std::string , std::string>("%E2", "€"));
	url_coding.insert(std::pair<std::string , std::string>("%60", "`"));

	std::map<std::string, std::string>::iterator it_find = url_coding.begin();
	for (; it_find != url_coding.end(); it_find++)
	{
		tmp = find_replace(it_find->first, it_find->second, tmp);
	}
	return (tmp);

	return (tmp);
}