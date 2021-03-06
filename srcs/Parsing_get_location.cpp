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

#include "../Headers/Parsing.hpp"
#include <iostream>

/*
**	ft_find_directive_location( size_t k, std::vector<std::string> tmp, size_t index_server ):
**		This function will check if the location block is correct.
**		Then, it will get data from the directive
**
**	==>	It returns 0 if an error occurs, otherwise it returns k.
*/
size_t			Parsing::ft_find_directive_location( size_t k, std::vector<std::string> tmp, size_t index_server )
{
	if (this->_servers[index_server].location.size() == 0)
		this->_servers[index_server].nbr_location = 1;
	else
		this->_servers[index_server].nbr_location += 1;

	this->_servers[index_server].location.push_back(t_location());
	if (tmp[k] != "location" || tmp[k + 2] != "{")
		throw Error(62, "Error,  in 'location' directive, it needs 'location' + '/' + '{'.", 1);
	if (tmp[k + 1][0] != '/')
		throw Error(62, "Error,  in 'location' directive, it needs 'location' + '/' + '{'.", 1);

	size_t index_location = this->_servers[index_server].location.size() - 1;	
	this->_servers[index_server].location[index_location].name_location = tmp[k + 1].substr(0, tmp[k + 1].size());
	
	k += 2;
	std::vector<std::string> scope_location = this->ft_get_scope_location(k, tmp);
	std::map<std::string, bool>            	location_dir;
	location_dir.insert(std::pair<std::string, bool>("dav_methods", false));
	
	size_t 									i = 0;
	while (i < scope_location.size())
	{
		std::map<std::string, bool>::iterator it = location_dir.begin();
		if (it->first == scope_location[i])
		{
			if (it->second == false)
				it->second = true;
			else
				throw Error(7, "Error, in 'location bloc', it has doublon.", 1);
		}
		i++;
	}
	std::map<std::string, bool>::iterator it = location_dir.begin();
	if (it->second == false)
	{
		if (it->first == "dav_methods")
			throw Error(12, "Error, in 'location bloc', 'dav_methods' directive is missing.", 1);
	}

	i = 1;
	while (i < scope_location.size())
	{
		if (scope_location[i] == "root")
		{
			if (this->ft_find_root_location(i, scope_location, index_server, index_location))
				return (0);
			i += 2;
		}
		else if (scope_location[i] == "dav_methods")
		{
			i = this->ft_find_methods_location(i, scope_location, index_server, index_location);
			if (i == 0)
				return (0);
		}
		else if (scope_location[i] == "autoindex")
		{
			if (this->ft_find_autoindex_location(i, scope_location, index_server, index_location))
				return (0);
			i += 2;
		}
		else if (scope_location[i] == "index")
		{
			if (this->ft_find_index_location(i, scope_location, index_server, index_location))
				return (0);
			i += 2;
		}
		else if (scope_location[i] == "error_page")
		{
			i = ft_find_error_location(i, scope_location, index_server, index_location);
			if (i == 0)
				return (0);
		}
		else if (scope_location[i] == "return")
		{
			if (this->ft_find_return_location(i, scope_location, index_server, index_location))
				return (true);
			i += 3;
		}
		else
		{
			if (scope_location[i] == "}")
				break;
			else
				throw Error(14, "Error, directive unrecognized dans location.", 1);
		}
	}
	k += i + 1;
	return (k);
}

/*
**	ft_find_return_location( size_t k, std::vector<std::string> tmp, size_t index_server, size_t index_location)
**		This function find return directive from a bloc's location.
**		It checks the given code if it is between 301 and 308.
**
**	==>	If an error occurs, throw an Error message. Otherwise it returns 0.
*/
bool		Parsing::ft_find_return_location( size_t k, std::vector<std::string> tmp, size_t index_server, size_t index_location)
{

	k += 1;
	std::vector<std::string> 	number;
	number.push_back("300");
	number.push_back("301");
	number.push_back("302");
	number.push_back("303");
	number.push_back("304");
	number.push_back("307");
	number.push_back("308");

	for (std::vector<std::string>::iterator it = number.begin(); it != number.end(); ++it)
	{
		if (tmp[k].compare(*it) == 0)
		{
			this->_servers[index_server].location[index_location].code_return_location = tmp[k];
			k += 1;
			size_t pos = tmp[k].find(";");
			if (pos == std::string::npos)
				throw Error(0, "Error, in 'return' location's bloc directive, it should have ';' at the end.", 0);
			if (pos != tmp[k].size() - 1)
				throw Error(0, "Error, in 'return' location's bloc directive, it should have only 1 ';' at the end.", 0);
			if (tmp[k][0] != '/')
				throw Error(0, "Error, in 'return' location's bloc directive, it should start with '/'.", 0);
			this->_servers[index_server].location[index_location].return_location = tmp[k];
			this->_servers[index_server].location[index_location].return_location.erase(this->_servers[index_server].location[index_location].return_location.size() - 1, 1);
			return (false);
		}
	}
	throw Error(0, "Error, in 'return' location's bloc directive, error's number is not correct.", 0);
}

/*
**	ft_find_error( size_t k, std::vector<std::string> tmp, size_t index_server ):
**		This function will check the information given in the 'error_page' directive fron a location blok.
**		It will check the error codes indicated and it will check if the files exist.
**		It will save the data in a std::map<int, std::string>  container.
**
**	==>	If an error occurs, throw an Error message. Otherwise it returns k.
*/
size_t          Parsing::ft_find_error_location( size_t k, std::vector<std::string> tmp, size_t index_server, size_t index_location )
{
	k += 1;
	if (!isdigit(tmp[k][0]))
		throw Error(0, "Error, in 'error_page' location's bloc directive, code error is not correct.", 1);
	while (tmp[k][tmp[k].size() - 1] != ';')
	{
		int y = 0;
		
		while (tmp[k][y])
		{
			if (isdigit(tmp[k][y]))
				y++;
			else
				throw Error(44, "Error: in 'error_page' location's bloc directive, it should only have numbers then a directory!", 1);
		}
		int error_code = std::strtol(tmp[k].c_str(), NULL, 10);
		if (this->ft_check_code_error(error_code) == 1)
			throw Error(0, "Error, in 'error_page' location's bloc directive, code error is not correct.", 1);
		this->_servers[index_server].location[index_location].error_location.insert(std::pair<int, std::string>(error_code, "NULL"));
		k++;
	}
	if (tmp[k][0] != '/')
		throw Error(45, "Error, in 'error_page' location's bloc directive, it should start with '/'.", 1);

	std::string address = tmp[k].substr(0, tmp[k].size() - 1);
	this->_servers[index_server].location[index_location].folder_error = address;
	std::map<int, std::string>::iterator it = this->_servers[index_server].location[index_location].error_location.begin();
	for (it = this->_servers[index_server].location[index_location].error_location.begin(); it != this->_servers[index_server].location[index_location].error_location.end(); it++)
	{
		if (it->second == "NULL")
			it->second = address;
	}
	k++;
	return (k);

}

/*
**	ft_find_autoindex( size_t k, std::vector<std::string> tmp, size_t index_server):
**      This function will checks the informations given in the 'autoindex' directive from a location bloc.
**      It should find "on;" or "off;".
**
**	==>	If an error occurs, throw an Error message. Otherwise it returns 0.
*/
bool            Parsing::ft_find_autoindex_location( size_t k, std::vector<std::string> tmp, size_t index_server, size_t index_location )
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
**	ft_find_root( size_t k, std::vector<std::string> tmp, size_t index_server ):
**		This function will checks the informations given in the 'root' directive from a location block.
**			It is the same fonction as 'ft_find_root'.
**		It will also checks if the folder exists.
**
**	==>	If an error occurs, throw an Error message. Otherwise it returns 0.
*/
bool            Parsing::ft_find_root_location( size_t k, std::vector<std::string> tmp, size_t index_server, size_t index_location )
{
	size_t  	len;
	
	k += 1;
	len = tmp[k].size();
	if (tmp[k][len] != '\0')
		throw Error(32, "Error, in 'root' location's bloc directive, it should end with '\0'.", 1);
	if (tmp[k][len - 1] != ';')
		throw Error(33, "Error, in 'root' location's blocd irective, it should end with ';'.", 1);
	if (tmp[k][0] != '/')
		throw Error(34, "Error, in 'root' location's bloc directive, it should start with '/'.", 1);
	size_t pos = tmp[k].find(".");
	if (pos != std::string::npos)
		throw Error(35, "Error, in 'root' location's bloc directive, it should not have a dot '.'", 1);
	this->_servers[index_server].location[index_location].root_location = tmp[k].substr(0, len - 1);
	
	if (this->_servers[index_server].location[index_location].root_location[this->_servers[index_server].location[index_location].root_location.size() - 1] == '/')
		this->_servers[index_server].location[index_location].root_location.erase(this->_servers[index_server].location[index_location].root_location.size()-1, 1);
	return (false);
}

/*
**	ft_find_methods( size_t k, std::vector<std::string> tmp, size_t index_server ):
**		This function will check the information given in the 'dav_methods' directive from a location block.
**		It only accept 3 methods: 'DELETE', 'GET' and 'POST'
**
**	==>	If an error occurs, throw an Error message. Otherwise it returns k.
*/
size_t          Parsing::ft_find_methods_location( size_t k, std::vector<std::string> tmp, size_t index_server, size_t index_location )
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
			{
				if (tmp[k] == ";")
					break ;	
				throw Error(39, "Error, in 'dav_methods' location's bloc  directive, it can only have DELETE POST and GET methods!", 1);
			}
			break;
		}
		k++;
	}
	if (this->_servers[index_server].location[index_location].methods_location.size() > 3 || this->_servers[index_server].location[index_location].methods_location.size() == 0)
		throw Error(40, "Error, in 'dav_methods' directive, it can only have 3 methods maximum!", 1);
	k++;
	if (tmp[k] == "GET" || tmp[k] == "GET;" || tmp[k] == "DELETE" || tmp[k] == "DELETE;" || tmp[k] == "POST" || tmp[k] == "POST;")
		throw Error(41, "Error, in 'dav_methods' directive, methods are not correct.", 1);
	
	size_t int_del = 0;
	size_t int_get = 0;
	size_t int_post = 0;
	std::vector<std::string>::iterator it;
	for (it = this->_servers[index_server].location[index_location].methods_location.begin(); it != this->_servers[index_server].location[index_location].methods_location.end(); it++)
	{
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
**	ft_find_index_location( size_t k, std::vector<std::string> tmp, size_t index_server ):
**		This function will check the information given in the 'index' directive from a location block.
**		The file should exist and not be empty.
**
**	==>	If an error occurs, throw an Error message. Otherwise it returns 0.
*/
bool			Parsing::ft_find_index_location( size_t k, std::vector<std::string> tmp, size_t index_server, size_t index_location )
{
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
	return (false);
}
