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

#include "Headers/Parsing.hpp"
#include <iostream>

/*
**	Getters need to be moved to Parsing_getters.cpp
*/
std::vector<t_server>			Parsing::ft_get_servers( void )
{
	return (this->_servers);
}

std::string						Parsing::ft_get_name_of_file( void )
{
	return (this->_name_of_file);
}

size_t							Parsing::ft_get_nbr_servers( void )
{
	return (this->_nbr_servers);
}

size_t							Parsing::ft_get_error( void )
{
	return (this->_error);
}

/*
**	ft_delete_comm( std::string &line ):
**		This function will delete all commentaries (starting with #) from the configuration file.
**
**	==> Returns the new string "tmp" which will contain all the configuration file.
*/
std::string                     Parsing::ft_delete_comm( std::string &line )
{
	std::string                 tmp = line;
	size_t                      pos = 0;
	size_t                      pos_end = 0;
	while (1)
	{
		pos = tmp.find('#', pos);
		if (pos == std::string::npos)
			break;
		pos_end = tmp.find('\n', pos);
		tmp.erase(pos, pos_end - pos);
	}
	return (tmp);
}

/*
**	ft_get_scope( size_t index ):
**		This function will return the scope starting and the given 'index' argument.
**
**	==> It will return a std::vector<std::string> which contains the scope.
*/
std::vector<std::string>        Parsing::ft_get_scope( size_t index )
{
	std::vector<std::string>::iterator      it_b;
	std::vector<std::string>::iterator      it_e;
	std::vector<std::string>::size_type 	size_data = this->_data.size();
	size_t                  				i = index;
	size_t                  				count = 0;

	while (i++ < size_data)
	{
		if (this->_data[i] == "{")
		{
			if (count == 0)
				it_b = this->_data.begin() + i;
			count++;
		}
		if (this->_data[i] == "}" && count != 0)
		{
			count--;
			if (count == 0)
			{
				it_e = this->_data.begin() + i + 1;
				break;
			}
		}
	}
	std::vector<std::string>    tmp(it_b, it_e);
	return (tmp);
}

/*
**	ft_check_code_serv( int code ) const:
**		Check if the given code is a server code.
**
**	==>	If an error occurs, throw an Error message. Otherwise it returns 0.
*/
bool			Parsing::ft_check_code_serv( int code ) const
{
	if ((code >= 500 && code <= 508) || code == 510 || code == 511)
		return (false);
	else
		throw Error(101, "Error, wrong code server.", 1);
}

/*
**	ft_check_code_client( int code ) const:
**		Check if the given code is a client code.
**
**	==>	If an error occurs, throw an Error message. Otherwise it returns 0.
*/
bool			Parsing::ft_check_code_client( int code ) const
{
	if ((code >= 400 && code <= 417) || (code >= 421 && code <= 426) 
			|| code == 428 || code == 429 || code == 431 || code == 451 )		// || code == 499 pour NGINX
		return (false);
	else
		throw Error(100, "Error, wrong code client.", 1);
}

/*
**	ft_check_code_error( int code ) const:
**		This function check the given argument and it uses the corresponding function.
**
**	==>	It returns ! if an error occurs.
*/
bool			Parsing::ft_check_code_error( int code ) const
{
	if (code > 399 && code < 500)
		return (this->ft_check_code_client(code));
	else if (code > 499 && code < 600)
		return (this->ft_check_code_serv(code));
	else
		return (true);
}
