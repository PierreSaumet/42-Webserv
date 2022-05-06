/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psaumet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/01 11:07:34 by psaumet           #+#    #+#             */
/*   Updated: 2021/06/01 11:07:42 by psaumet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_HPP
# define ERROR_HPP

# include <iostream>
# include <ios>
# include <sstream>
# include <exception>
# include <string>
# include <string.h>

# define RED "\033[1;31m"
# define GREEN "\033[1;32m"
# define BLUE "\033[1;34m"
# define CLEAR "\033[0m"

class Error: public std::exception
{
    public:
        Error(int number = 0, std::string const &line="", int level=0) throw();
    
	    virtual ~Error() throw();

        virtual const char* what() const throw();
		int get_level() const throw();
 
    protected:
        int _number_error;               	//Numéro de l'erreur
        std::string _line_error;            //Description de l'erreur
        int _level_error;               	//Niveau de l'erreur

};


#endif		