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

# include <exception>
# include <string>

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

/*
**	Classification
**		Level_error	=	0	==>	Configuration file
**			number_error	=	1	==>	file doesn't exist.
**							=	2	==>	file is empty.
**							=	3	==> file must terminate with '.conf'.
							=	4	==> file have a wrong name or a bad ending.












*/		

#endif