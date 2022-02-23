/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing_get_location.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psaumet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/01 11:07:34 by psaumet           #+#    #+#             */
/*   Updated: 2021/06/01 11:07:42 by psaumet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Headers/Parsing.hpp"
#include <iostream>

/*
**	ft_get_location( size_t k, std::vector<std::string> tmp, size_t index_server ):
**		This function will check if the location block is correct.
**		Then, it will get data from the directive
**
**	==>	It returns 0 if an error occurs, otherwise it returns k.
*/
size_t			Parsing::ft_get_location( size_t k, std::vector<std::string> tmp, size_t index_server )
{
	// Adding a block server
	this->_servers.push_back(Parsing::t_server());

	if (this->_servers[index_server].location.size() == 0)
		this->_servers[index_server].nbr_location = 1;
	else
		this->_servers[index_server].nbr_location += 1;

	this->_servers[index_server].location.push_back(Parsing::t_location());
	if (tmp[k] != "location" || tmp[k + 2] != "{")
		throw Error(62, "Error,  in 'location' directive, it needs 'location' + '/' + '{'.", 1);
	if (tmp[k + 1][0] != '/')
		throw Error(62, "Error,  in 'location' directive, it needs 'location' + '/' + '{'.", 1);
	// Getting name of the location
	size_t index_location = this->_servers[index_server].location.size() - 1;	
	this->_servers[index_server].location[index_location].name_location = tmp[k + 1].substr(0, tmp[k + 1].size());
	
	// Getting the scope of the location block
	k += 2;
	std::vector<std::string>    scope_location = this->ft_get_scope(k);

	// Going through the scope to get data
	size_t i = 1;
	while (i < scope_location.size())
	{
		if (scope_location[i] == "root")
		{
			if (this->ft_get_root_location(i, scope_location, index_server, index_location))
				return (0);
			i += 2;
		}
		else if (scope_location[i] == "dav_methods")
		{
			i = this->ft_get_methods_location(i, scope_location, index_server, index_location);
			if (i == 0)
				return (0);
		}
		else if (scope_location[i] == "autoindex")
		{
			if (this->ft_get_autoindex_location(i, scope_location, index_server, index_location))
				return (0);
			i += 2;
		}
		else if (scope_location[i] == "upload_store")
		{
			if (this->ft_get_upload_store_location(i, scope_location, index_server, index_location))
				return (0);
			i += 2;
		}
		else if (scope_location[i] == "client_body_buffer_size")
		{
			if (this->ft_get_buffer_size_location(i, scope_location, index_server, index_location))
				return (0);
			i += 2;
		}
		else if (scope_location[i] == "index")
		{
			if (this->ft_get_index_location(i, scope_location, index_server, index_location))
				return (0);
			i += 2;
		}
		else if (scope_location[i] == "error_page")
		{
			i = ft_get_error_location(i, scope_location, index_server, index_location);
			if (i == 0)
				return (0);
		}
		else
		{
			if (scope_location[i] == "}")
				break;
			else
				throw Error(14, "Error, directive unrecognized.", 1);
		}
	}
	k += i + 1;
	return (k);
}

/*
**	ft_get_error( size_t k, std::vector<std::string> tmp, size_t index_server ):
**		This function will check the information given in the 'error_page' directive fron a location blok.
**		It will check the error codes indicated and it will check if the files exist.
**		It will save the data in a std::map<int, std::string>  container.
**
**	==>	If an error occurs, throw an Error message. Otherwise it returns k.
*/
size_t          Parsing::ft_get_error_location( size_t k, std::vector<std::string> tmp, size_t index_server, size_t index_location )
{
	k += 1;
	while (tmp[k][tmp[k].size() - 1] != ';')
	{
		std::cout << "\tLA tmp[k] = " << tmp[k] << std::endl;
		int y = 0;
		while (tmp[k][y])
		{
			if (isdigit(tmp[k][y]))
				y++;
			else
				throw Error(44, "Error: in 'error_page' directive, it should only have numbers then a directory!", 1);
		}
		int error_code = std::strtol(tmp[k].c_str(), NULL, 10);
		if (this->ft_check_code_error(error_code) == 1)
			return (0);
		this->_servers[index_server].location[index_location].error_location.insert(std::pair<int, std::string>(error_code, "NULL"));
		k++;
	}
	if (tmp[k][0] != '.' || tmp[k][1] != '/')
		throw Error(45, "Error, in 'error_page' directive should end with a directory or file", 1);
	std::string address = tmp[k].substr(0, tmp[k].size() - 1);
	struct stat buffer;
	if (stat(address.c_str(), &buffer) != 0)
		throw Error(46, "Error, in 'error_page' directive, the directory doesn't exist!", 1);
	if (this->_servers[index_server].location[index_location].error_location.size() > 1)		// several error pages.
	{
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
				throw Error(47, "Error, in 'error_page' directive, it cannot find the error file.", 1);
			if (buff.st_size == 0)
				throw Error(48, "Error, in 'error_page' directive, the file is empty.", 1);
		}
	}
	else		// Case there is only 1 error page
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
			throw Error(47, "Error, in 'error_page' directive, it cannot find the error file.", 1);
		if (buff.st_size == 0)
			throw Error(48, "Error, in 'error_page' directive, the file is empty.", 1);
	}
	k++;
	return (k);
}

/*
**	ft_get_buffer_size( size_t k, std::vector<std::string> tmp, size_t index_server ):
**		This function will check the information given in the 'client_body_buffer_size' directive from a location block.
**		The information given is between 8000 (8k) and 16000 (16k) maximum.
**		The information will be used for the 'POST' command.
**
**	==>	If an error occurs, throw an Error message. Otherwise it returns 0.
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
		throw Error(57, "Error, in 'client_body_buffer_size' directive, it should only be digits.", 1);
	if (tmp[k][i] == 'k' && tmp[k][i + 1] == ';' && i + 1 == tmp[k].size() - 1 && tmp[k][i + 2] == '\0')
	{
		buffer_size = std::strtol(tmp[k].c_str(), NULL, 10);
		if (buffer_size < 8 || buffer_size > 16)
			throw Error(58, "Error, in 'client_body_buffer_size' directive, buffer size must be between 8k and 16k.", 1);
		this->_servers[index_server].location[index_location].buffer_size_location = buffer_size * 1000;
	}
	else
	{
		if (tmp[k][i] == ';' && i + 1 == tmp[k].size() && tmp[k][i + 1] == '\0')
		{
			buffer_size = std::strtol(tmp[k].c_str(), NULL, 10);
			if (buffer_size < 8000 || buffer_size > 16000)
				throw Error(59, "Error, in 'client_body_buffer_size' directive, buffer size must be between 8000 and 16000.", 1);
			this->_servers[index_server].location[index_location].buffer_size_location = buffer_size;
		}
		else
			throw Error(60, "Error, in 'client_body_buffer_size' directive, informations are corrupted.", 1);
	}
	return (false);
}

/*
**	ft_get_upload_store( size_t k, std::vector<std::string> tmp, size_t index_server ):
**		This function will check the information given in the 'upload_store' directive from a location block.
**		The information given is an fodler where we can find files uploaded.
**
**	==>	If an error occurs, throw an Error message. Otherwise it returns 0.
*/
bool			Parsing::ft_get_upload_store_location( size_t k, std::vector<std::string> tmp, size_t index_server, size_t index_location )
{
	struct stat buffer;
	size_t  	len;
	
	k += 1;
	len = tmp[k].size();
	if (tmp[k][len] != '\0')
		throw Error(49, "Error,	in 'upload_store' directive, it should end with '\0'.", 1);
	if (tmp[k][len - 1] != ';')
		throw Error(50, "Error, in 'upload_store' directive, it should end with ';'.", 1);
	if (tmp[k][0] != '.' || tmp[k][1] != '/')
		throw Error(51, "Error, in 'upload_store' directive, it should start with './'.", 1);
	this->_servers[index_server].location[index_location].upload_store_location = tmp[k].substr(0, len - 1);
	if (stat(this->_servers[index_server].location[index_location].upload_store_location.c_str(), &buffer) == -1)
		throw Error(52, "Error, in  'upload_store' directive , the folder doesn't exist!", 1);
	return (false);
}

/*
**	ft_get_autoindex( size_t k, std::vector<std::string> tmp, size_t index_server):
**      This function will checks the informations given in the 'autoindex' directive from a location bloc.
**      It should find "on;" or "off;".
**
**	==>	If an error occurs, throw an Error message. Otherwise it returns 0.
*/
bool            Parsing::ft_get_autoindex_location( size_t k, std::vector<std::string> tmp, size_t index_server, size_t index_location )
{
	k += 1;
	size_t  len = tmp[k].size();
	if (tmp[k][len] != '\0')
		throw Error(36, "Error, in 'autoindex' directive it should end with '\0'.", 1);
	if (tmp[k][len - 1] != ';')
		throw Error(37, "Error, in 'autoindex' directive it should end with ';'.", 1);
	if (tmp[k].compare("on;") != 0 && tmp[k].compare("off;") != 0)
		throw Error(38, "Error, in 'autoindex' directive it should be 'on' or 'off'.", 1);
	if (tmp[k].compare("on;") == 0)
		 this->_servers[index_server].location[index_location].autoindex_location = true;
	else
		this->_servers[index_server].location[index_location].autoindex_location = false;
	return (false);
}


/*
**	ft_get_root( size_t k, std::vector<std::string> tmp, size_t index_server ):
**		This function will checks the informations given in the 'root' directive from a location block.
**			It is the same fonction as 'ft_get_root'.
**		It will also checks if the folder exists.
**
**	==>	If an error occurs, throw an Error message. Otherwise it returns 0.
*/
bool            Parsing::ft_get_root_location( size_t k, std::vector<std::string> tmp, size_t index_server, size_t index_location )
{
	struct stat buffer;
	size_t  	len;
	
	k += 1;
	len = tmp[k].size();
	if (tmp[k][len] != '\0')
		throw Error(32, "Error, in 'root' directive, it should end with '\0'.", 1);
	if (tmp[k][len - 1] != ';')
		throw Error(33, "Error, in 'root' directive, it should end with ';'.", 1);
	if (tmp[k][0] != '.' || tmp[k][1] != '/')
		throw Error(34, "Error, in 'root' directive, it should start with './'.", 1);
	this->_servers[index_server].location[index_location].root_location = tmp[k].substr(0, len - 1);
	if (stat(this->_servers[index_server].location[index_location].root_location.c_str(), &buffer) == -1)
		throw Error(35, "Error, in 'root' directive doesn't exist!.", 1);
	return (false);
}

/*
**	ft_get_methods( size_t k, std::vector<std::string> tmp, size_t index_server ):
**		This function will check the information given in the 'dav_methods' directive from a location block.
**		It only accept 3 methods: 'DELETE', 'GET' and 'POST'
**
**	==>	If an error occurs, throw an Error message. Otherwise it returns k.
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
				throw Error(39, "Error, in 'dav_methods' directive, it can only have DELETE POST and GET methods!", 1);
			break;
		}
		k++;
	}
	if (this->_servers[index_server].location[index_location].methods_location.size() > 3 || this->_servers[index_server].location[index_location].methods_location.size() == 0)
		throw Error(40, "Error, in 'dav_methods' directive, it can only have 3 methods maximum!", 1);
	k++;
	if (tmp[k] == "GET" || tmp[k] == "GET;" || tmp[k] == "DELETE" || tmp[k] == "DELETE;" || tmp[k] == "POST" || tmp[k] == "POST;")
		throw Error(41, "Error, in 'dav_methods' directive, methods are not correct.", 1);
	
	// Checking doublons.
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
			throw Error(42, "Error, in 'dav_methods' directive, this method is unkonwn.", 1);
		if (int_del > 1 || int_post > 1 || int_get > 1)
			throw Error(43, "Error, in 'dav_methods' directive, it should not have doublons.", 1);
	}
	return (k);
}

/*
**	ft_get_index_location( size_t k, std::vector<std::string> tmp, size_t index_server ):
**		This function will check the information given in the 'index' directive from a location block.
**		The file should exist and not be empty.
**
**	==>	If an error occurs, throw an Error message. Otherwise it returns 0.
*/
bool			Parsing::ft_get_index_location( size_t k, std::vector<std::string> tmp, size_t index_server, size_t index_location )
{
	struct stat buffer;
	size_t  	len;
	
	k += 1;
	len = tmp[k].size();
	if (tmp[k][len] != '\0')
		throw Error(26, "Error, in 'index' directive, it should end with '\0'.", 1);
	if (tmp[k][len - 1] != ';')
		throw Error(27, "Error, in 'index' directive, it should end with ';'.", 1);
	if (tmp[k].size() <= 6)
		throw Error(28, "Error, in 'index' directive, it should have a proper name and end with '.html'.", 1);
	if (tmp[k].compare(tmp[k].size() - 6, 6, ".html;") !=  0)
		throw Error(29, "Error, in 'index' directive, it should end with '.html'.", 1);
	this->_servers[index_server].location[index_location].index_location = tmp[k].substr(0, len - 1);
	if (stat(this->_servers[index_server].location[index_location].index_location.c_str(), &buffer) == -1)
		throw Error(30, "Error, in 'index' directive, file doesn't exist.", 1);
	if (buffer.st_size == 0)
		throw Error(31, "Error, in 'index' directive, file is empty.", 1);
	return (false);
}
