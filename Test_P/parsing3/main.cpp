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
#include "Headers/HttpServer.hpp"
#include "Headers/Error.hpp"
#include <iostream>

/*
**	 main(int argc, char **argv):
**		This is the 'main' function.
*/
int             main(int argc, char **argv)
{
	if (signal(SIGINT, HttpServer::handler_signal) == SIG_ERR)
		exit(EXIT_FAILURE);
    if (argc == 1)
    {
		std::string name("./confs/test1.conf");
		Parsing test1 = Parsing(name);
    }
    else if (argc == 2)
    {
        std::string name = argv[1];
		 Parsing test1 = Parsing(name);
		//HttpServer server_1(name);
	}
	else
	{
		std::cerr << RED << "Error, in the argument given." << CLEAR << std::endl;
	}
	// Testing signal
	// raise(SIGINT);
	// std::cout << "int sig = " << HttpServer::int_signal << std::endl;

	// if (&signal != 0)
	// {
	// 	std::cout << "ERREUR AHAHAH" << std::endl;
	// }

    return (0);
}