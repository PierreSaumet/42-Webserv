/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing_utils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psaumet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/01 11:07:34 by psaumet           #+#    #+#             */
/*   Updated: 2021/06/01 11:07:42 by psaumet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Parsing.hpp"


/*
**	find_root()
**		Get the root of the server from a std::string &line
**		The name is a std::string
**		Throw an error if "server_name" is not find.
**		Otherwise return std::string name_server;
*/
size_t			Parsing::find_root( std::string &line, std::vector<Parsing::t_server> &myvect, int index, size_t new_pos )
{
	size_t 				pos = 0;
	size_t				pos_end = 0;
	size_t				pos_n = 0;
	std::string			tmp;
	struct stat 		buffer;

	pos = line.find("root");
	if (pos == std::string::npos)
	{
		// euh pas de root pas grave ?
		return pos;
		throw Error(4, "Error, no root name", 1);
	}
	// add 5 for -root -
	pos = pos + 4;
	std::cout << "pos = " << pos << " et =" << line[pos] << std::endl;
	if (isspace((int)line[pos]) == 0)
		throw Error(25, "Error, root is not correct!", 1);
	while (isspace((int)line[pos]) != 0)
		pos++;
	std::cout << "pos = " << pos << " et =" << line[pos] << std::endl;
	if (line[pos] != '/')
		throw Error(26, "Error, root should start by '/'!", 1);
	pos = line.find_first_not_of(" \t\n\v\r\f", pos);
	pos_end = line.find(";", pos);
	pos_n = line.find("\n", pos);
	if( pos_n <= pos_end)
		throw Error(99, "Error, need an ';' in configuration file", 1);

	tmp = line.substr(pos, pos_end - pos).c_str();
	tmp.insert(0, ".");
	if (stat(tmp.c_str(), &buffer) != 0)
		throw Error(27, "Error, cannot open directory from root!", 1);
	myvect[index]._root_server = line.substr(pos, pos_end - pos).c_str();
	return (pos);
	//return (line.substr(pos, pos_end - pos).c_str());
}


/*
**	find_ip_port()
**		Get the ip and the port from a std::string &line
**		The ip is a std::string and port an int.
**		Throw an error if "listen" is not find.
**		Otherwise return std::string ip_server;
*/
size_t			Parsing::find_ip_port( std::string &line, std::vector<Parsing::t_server> &myvect, int index, size_t new_pos )
{
	size_t 			pos = 0;
	size_t			pos_host = 0;
	size_t			pos_port = 0;
	size_t			pos_end = 0;
	size_t			pos_n = 0;
	std::string		tmp;

	// On cherche listen
	pos = line.find("listen", new_pos);
	if (pos == std::string::npos)		// listen existe pas
		throw Error(0, "Error, no listen in the configuration file", 1);
	// on verifie dans les scopes location si listen
	std::string loc_name = "location";
	std::string tmp_loc = this->get_scope_v2(line, loc_name, 0);
	std::cout << "LOC tmp = " << tmp_loc << std::endl;
	
	// donc on a le scoe location
	


	// if (line[pos - 1] != '\t')
	// 	throw Error(1, "Error, bad listen in the configuration file", 1);
	pos = pos + 6;
	std::cout << "line[pos] = " << line[pos] << " et pos = " << pos << std::endl;
	// while (line[pos] == '\t' || line[pos] == ' ')
	// 	pos++;

	pos = this->_ft_is_blank(line, pos);
	
	std::cout << "XXD line[pos] = " << line[pos] << " et pos = " << pos << std::endl;
	if (isdigit(line[pos]) == 0)
		throw Error(1, "Error, no host or bad host in the configuration file", 1);
	std::cout << "XXD line[pos] = " << line[pos] << " et pos = " << pos << std::endl;
	
	
	pos = line.find_first_not_of(" \t\n\v\r\f", pos);
	pos_host = line.find_first_of("0123456789", pos);;
	if (pos_host == std::string::npos)
		throw Error(1, "Error, no host in the configuration file", 1);
	pos_port = line.find_first_of(":", pos);
	if (pos_port == std::string::npos)
		throw Error(1, "Error, no host in the configuration file", 1);
	if (pos_host >= pos_port)
		throw Error(-1, "Error,  in the configuration file", 1);

	pos_end = line.find(";", pos);
	pos_n = line.find("\n", pos);
	
	if (pos_n <= pos_end)
		throw Error(99, "Error, need an ';' in configuration file", 1);
	myvect[index]._port_server = std::strtol(line.substr(pos_port + 1, pos_end - pos_port).c_str(), NULL, 10);
	if (myvect[index]._port_server < 0 || myvect[index]._port_server > 65535)
		throw Error(2, "Error, bad port. Should be between 0 and 65535", 1);
	
	tmp = line.substr(pos, pos_port - pos).c_str();
	if (tmp.compare("127.0.0.1") != 0)
		throw Error(1, "Error, host should be 127.0.0.1", 1);
	myvect[index]._host_server = tmp;
	//std::cout << "fin de find host = " << myvect[index]._host_server << std::endl;
	return (pos);
}


/*
**	find_name_server()
**		Get the name of the server from a std::string &line
**		The name is a std::string
**		Throw an error if "server_name" is not find.
**		Otherwise return std::string name_server;
*/
size_t		Parsing::find_name_server( std::string &line, std::vector<t_server> &myvect, int index, size_t new_pos ) 
{
	size_t 				pos = 0;
	size_t				pos_end = 0;
	size_t				pos_n = 0;

	pos = line.find("server_name", new_pos);
	if (pos == std::string::npos)
		throw Error(3, "Errer, no server_name", 1);
	// add 12 for -server_name -
	pos = pos + 11;
	//std::cout << "line[pos = " << line[pos] << std::endl;
	if (isspace((int)line[pos]) == 0)
		throw Error(15, "Error, server_name is not correct!", 1);
	pos = line.find_first_not_of(" \t\n\v\r\f", pos);
	pos_end = line.find(";", pos);
	pos_n = line.find("\n", pos);
	if (pos_n <= pos_end)
		throw Error(99, "Error, need an ';' in configuration file", 1);
	//std::cout << " kek = " << line.substr(pos, pos_end - pos).c_str() << std::endl;

	std::string tmp = line.substr(pos, pos_end - pos).c_str();
	myvect[index]._name_server = tmp;

	//std::cout << " SERVER NAME TMP = " << tmp << std::endl;
	return (pos);
	//return (line.substr(pos, pos_end - pos).c_str());
}


/*
**	find_autoindex()
**		Get the autoindex of the server from a std::string &line
**		The autoindex is a int
**		0 or 1;
*/
size_t				Parsing::find_autoindex( std::string &line, std::vector<Parsing::t_server> &myvect, int index, size_t new_pos )
{
	size_t 				pos = 0;
	size_t				pos_end = 0;
	size_t				pos_n = 0;
	pos = line.find("autoindex", pos);
	if (pos == std::string::npos)
	{
		// euh pas de autoindex pas grave ?
		return pos;
		throw Error(5, "Error, no autoindex!", 1);
	}
	// check before  a rajouter partout
	size_t tmp = pos; 
	while (line[pos] != ';')
		pos--;
	pos++;
	while (isspace((int)line[pos]) != 0)
		pos++;
	std::cout << "line pos = " << line[pos] << std::endl;
	if (pos != tmp)
		throw Error(39, "Error, autoindex not valid!", 1);
	//exit(EXIT_FAILURE);
	
	// add 10 for -root -
	pos = pos + 9;
	pos = line.find_first_not_of(" \t\n\v\r\f", pos);
	pos_end = line.find(";", pos);
	pos_n = line.find("\n", pos);
	if( pos_n <= pos_end)
		throw Error(99, "Error, need an ';' in configuration file", 1);
	if (line.compare(pos, 3, "on;") == 0)	// oui
	{
		myvect[index]._autoindex_server = true;
		return (pos);
	}
	else if (line.compare(pos, 4, "off;") == 0)
	{
		myvect[index]._autoindex_server = false;
		return (pos);
	}
	else
		throw Error(39, "Error, autoindex not valid!", 1);
}

