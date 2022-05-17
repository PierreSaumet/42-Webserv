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

#include "../Headers/Parsing.hpp"
#include "../Headers/HttpServer.hpp"
#include "../Headers/Error.hpp"
#include "../Headers/CGI_exec.hpp"

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
		std::string name("./srcs/base.conf");
		HttpServer server(name);
    }
    else if (argc == 2)
    {
        std::string name = argv[1];
		HttpServer server(name);
	}
	else
	{
		std::cerr << RED << "Error, in the argument given." << CLEAR << std::endl;
		return (1);
	}
	if (&signal != 0)
	{
		std::cout << RED << "Our webserv wish you a goodbye..." << CLEAR << std::endl;
		return (1);
	}
    return (0);
}