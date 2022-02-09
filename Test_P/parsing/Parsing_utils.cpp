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
**	All private functions from Parsing.hpp
*/

/*
**	Parsing::delete_comm():
**		Delete all sentences starting with '#' from the std::string &line given.
**		Return line without the #'s lines.
*/
std::string				Parsing::delete_comm( std::string &line )		// Need to delete //
{
	std::string tmp(line);
	size_t			pos = 0;
	size_t			pos_end = 0;
	while (1)
	{
		pos = tmp.find("#");
		if (pos == std::string::npos)
			break;
		pos_end = tmp.find("\n", pos);
		tmp.erase(pos , pos_end - pos);
	}
	return (tmp);
}

/*
**	Parsing::check_code_error_file()
**		Check if the file for the error_code given in the configuration file
**		is correct and exist. It use std::map<int, std::string> _error_page
**		If there is a problem return 1 otherwise 0.
*/
bool					Parsing::check_code_error_file( std::string &line ) // Need to delete //
{
	// on parcourt toutes les error
	std::map<int, std::string>::iterator it_b = this->_error_page.begin();
	std::map<int, std::string>::iterator it_e = this->_error_page.end();
	size_t pos = 0;
	std::string get_code;
	for (it_b = this->_error_page.begin(); it_b != it_e; it_b++)
	{
		//std::cout << it_b->first << " => " << it_b->second << std::endl;
		pos = it_b->second.find(".html", pos);
		if (pos == std::string::npos)
		{
			std::cout << "kek " << it_b->second << std::endl;
			//break
			get_code = std::to_string(it_b->first);
			//std::cout << "get code = " << get_code << std::endl;
			get_code.append(".html");
			//std::cout << "get code = " << get_code << std::endl;
			it_b->second.append(get_code);
			//std::cout << "du coup = " << it_b->second << std::endl;
			if (check_file_and_empty_exit(it_b->second) == 1)
				return (1);
			// std::fstream			fs;
			// fs.open(it_b->second);
			// if (fs.is_open() == 0)
			// {
			// 	fs.close();
			// 	std::cout << "file dosent exist : " << it_b->second << std::endl;
			// 	//exit(EXIT_FAILURE);
			// }
			// else
			// {
			// 	std::cout << "\t= ) file exist " << it_b->second << std::endl;
			// 	if (fs.peek() == std::fstream::traits_type::eof())
			// 	{
			// 		std::cout << "Error, file is empty : " << it_b->second << std::endl;
			// 	}
			// 	fs.close();
			// }
		}
		else	// l'addresse du fichier possede .html
		{
			//std::cout << "good" << std::endl;
			size_t pos = 0;
			pos = it_b->second.find(std::to_string(it_b->first), pos);
			if (pos == std::string::npos)
			{
				std::cout << "CACA" << std::endl;
				size_t pos_back = it_b->second.find_last_of("/", pos);
				size_t pos_dot = it_b->second.find_last_of(".", pos);
				pos = 0;
				std::cout << "pos_b " << pos_back << " et " << pos_dot << std::endl;
				if (pos_dot - pos_back == 4)
				{
					std::cout << "YES" << std::endl;
					it_b->second.replace(pos_back + 1, 3, std::to_string(it_b->first));
				}
				std::cout << "it_first = " << it_b->first << " second = " << it_b->second << std::endl;
			}
			std::cout << "pos = " << pos << " et it_fsit = " << it_b->first << std::endl;
			//if (std::to_string(it_b->first).)
			if (check_file_and_empty_exit(it_b->second) == 1)
				return (1);
			// std::fstream			fs;
			// fs.open(it_b->second);
			// if (fs.is_open() == 0)
			// {
			// 	fs.close();
			// 	std::cout << "file dosent exist : " << it_b->second << std::endl;
			// 	//exit(EXIT_FAILURE);
			// }
			// else
			// {
			// 	std::cout << "\t= ) file exist " << it_b->second << std::endl;
			// 	if (fs.peek() == std::fstream::traits_type::eof())
			// 	{
			// 		std::cout << "Error, file is empty : " << it_b->second << std::endl;
			// 	}
			// 	fs.close();
			// }
			
		}
		pos = 0;

	}	
	
	return (false);
}

/*
**	Parsing::check_code_error():
**		Check if the given code is allowed.
**		Use client_code_error and server_code_error.
*/
bool					Parsing::check_code_error( int code ) const
{
	// if (server_code_error(code) == 1 || client_code_error(code) == 1)
	// 	return (1);
	// else
	// 	return (0);
	if (code > 399 && code < 500)
		return (client_code_error(code));
	else if (code > 499 && code < 600)
		return (server_code_error(code));
	else
	{
		throw Error(8, "Error, code error not correct", 1);
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
bool					Parsing::server_code_error( int code ) const
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
bool					Parsing::client_code_error( int code ) const
{
	if ((code >= 400 && code <= 417) || (code >= 421 && code <= 426) 
			|| code == 428 || code == 429 || code == 431 || code == 451 )		// || code == 499 pour NGINX
		return (false);
	else
		return (true);
}


/*
**	check_extension():
**		Check if the configuration file terminates with .conf
**			return 0 if ok and 1 if error.
*/
bool				Parsing::check_extension( void ) const {

	if (get_name_of_file().empty())
		throw Error(1, "Error, empty name file", 0);
	else
	{
		std::string		to_find = ".conf";
		std::size_t		pos_to_find = this->_name_of_file.find(to_find);
		if (pos_to_find != std::string::npos)		// try to find .conf
		{
			if (this->_name_of_file.compare(pos_to_find, 6, ".conf\n") != 0)
				throw Error(0, "Error, configuration file should end by \'.conf' !", 0);
		}
		else
			throw Error(0, "Error, configuration file should end by \'.conf' !", 0);
	}
	return (false);
}

/*
**	check_exit():
**		Check if the file exist or not
**		Return 1 if error 0 if ok
*/
bool				Parsing::check_exist( void ) const {

	std::fstream	fs;
	std::string tmp(this->_name_of_file.begin(), this->_name_of_file.end() - 1); 
	
	fs.open(tmp);
	if (fs.is_open() == 0)
	{
		fs.close();
		throw Error(2, "Error, file doesn't exist!", 0);
	}
	fs.close();
	return (false);
}

/*
**	check_empty():
**		Check if the file is empty or not
**		Return 1 if error 0 if ok
*/
bool				Parsing::check_empty( void ) const
{
	std::string tmp(this->_name_of_file.begin(), this->_name_of_file.end() - 1); 
	std::ifstream file(tmp);
	
	if (file.peek() == std::ifstream::traits_type::eof())
	{
		file.close();
		throw Error(3, "Error configuration file empty", 0);
	}
	file.close();
	return (false);
}
