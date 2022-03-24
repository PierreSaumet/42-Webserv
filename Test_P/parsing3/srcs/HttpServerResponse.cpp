/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServerResponse.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psaumet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/01 11:07:34 by psaumet           #+#    #+#             */
/*   Updated: 2021/06/01 11:07:42 by psaumet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Headers/HttpServer.hpp"



std::string		HttpServer::ft_setup_header( void )
{
	// cette fonction doit voir si le path de la requete est valide et existe.
	// ouvrir le fichier, verifier qu'il ne soit pas vide.
	// setup up le header avec le numero de la reponse du server.
	std::string filename(this->_servers[0].index_server.c_str());
	FILE *input_file = NULL;
	std::string res;
	std::string file_contents;

	// on verifier si le path est l'index
	if (this->_header_requete[0].path == "/ ")
	{
		std::cout << "display index" << std::endl;	
		struct stat buff;
		if (stat(filename.c_str(), &buff) < 0)
		{
			std::cout << "Error, la page demande n'exite pas." << std::endl;
			return (NULL);
		}
		if (buff.st_size == 0)
		{
			std::cout << "Error, la page demande est vide. " << std::endl;
			return (NULL);
		}
		input_file = fopen(filename.c_str(), "r");
		if (input_file == NULL)
		{
			std::cout << "Error, pour ouvrir la page demande avec fopen." << std::endl;
			return (NULL);
		}

		// stat(filename.c_str(), &sb);
		res.resize(buff.st_size + 100);
		std::cout << "taille buffer = " << buff.st_size << std::endl;
		
		fread(const_cast<char*>(res.data()), buff.st_size, 1, input_file);

		fclose(input_file);
		file_contents = res;
		std::cout << "la taille putain = " << file_contents.size() << std::endl;

		// file_contents.insert(0, file_contents.c_str());
		file_contents.insert(0, "\r\n\r\n");
		file_contents.insert(0, "879");
		file_contents.insert(0, "Content-Length: ");
		file_contents.insert(0, "HTTP/1.1 200 OK\r\nContent-Type: text/html; Charset=UTF-8\r\nServer: Webserv\r\n");
		// HTTP/1.1 200 OK\r\nContent-Type: text/html; Charset=UTF-8\r\nServer: Webserv\r\n == 80
		// file_contents.insert(81, "Content-Length: ");
		// Content-Length: == 16
		// file_contents.insert(98, file_contents.c_str());
		// 3 lol
		

		std::cout << "file contents = " << file_contents << std::endl;
		sleep(1);
	}
	return (NULL);
}