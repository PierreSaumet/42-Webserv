/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psaumet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/01 11:07:34 by psaumet           #+#    #+#             */
/*   Updated: 2021/06/01 11:07:42 by psaumet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Parsing.hpp"

#include <stdlib.h>			//exit

/*
**clang++ -g -o parse_test main.cpp Parsing.cpp Parsing_get_set.cpp Parsing_utils.cpp Parsing_check.cpp Parsing_find.cpp Error.cpp && valgrind --leak-check=full ./parse_test ./conf_files/test.conf 
*/

void		ft_error(std::string message)
{
	std::cout << message << std::endl;
	exit(EXIT_FAILURE);
}

/*
**	clang++ -o parse_test main.cpp Parsing.cpp Parsing_utils.cpp && ./parse_test test.conf
*/

int			main(int argc, char **argv)
{
	if (argc != 2)
		ft_error("ERROR, should only give one parameter !\n");
	else
	{
		// start parsing
		Parsing(std::string(argv[1]));
	}
	exit(EXIT_SUCCESS);
}