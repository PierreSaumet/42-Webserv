/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psaumet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/01 11:07:34 by psaumet           #+#    #+#             */
/*   Updated: 2021/06/01 11:07:42 by psaumet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Headers/Error.hpp"

Error::Error( int number, std::string const &line, int level) throw() :
_number_error(number), _line_error(line), _level_error(level) {
	
	return ;
}

const char* Error::what() const throw() {

	std::cerr << RED << "Lvl: " << this->_level_error << " nbr: " << this->_number_error << " - " << CLEAR;
    return (this->_line_error.c_str());
}

int         Error::get_level() const throw() {

    return (this->_level_error);
}

Error::~Error() throw() {

    return ;
} 