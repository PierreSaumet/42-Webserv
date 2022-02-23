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

#include "Headers/Parsing.hpp"
#include <iostream>

/*
**	 main(int argc, char **argv):
**		This is the 'main' function.
*/
int             main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "Error, need one argument!" << std::endl;
        return (1);
    }
    else
    {
        std::string name = argv[1];
		Parsing test1 = Parsing(name);
    }
    return (0);
}