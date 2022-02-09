/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing_get_set.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psaumet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/01 11:07:34 by psaumet           #+#    #+#             */
/*   Updated: 2021/06/01 11:07:42 by psaumet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Parsing.hpp"


/*
**	Getters
*/
std::string			Parsing::get_name_of_file( void ) const {

	return (this->_name_of_file);
}

std::string			Parsing::get_ip_server( void ) const {

	return (this->_ip_server);
}

int					Parsing::get_port( void ) const {

	return (this->_port);
}

std::string			Parsing::get_root( void ) const {

	return (this->_root);
}

int					Parsing::get_autoindex( void ) const {

	return (this->_autoindex);
}

std::string			Parsing::get_index( void ) const {

	return (this->_index);
}

std::map<int, std::string>	Parsing::get_error_page( void ) const {

	return (this->_error_page);
}

/*
**	Setters

**		SUREMENT USELESS
*/
void				Parsing::set_name_of_file( const std::string &new_name )
{
	this->_name_of_file = new_name;
	return ;
}