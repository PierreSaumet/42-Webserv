/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psaumet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/01 11:07:34 by psaumet           #+#    #+#             */
/*   Updated: 2021/06/01 11:07:42 by psaumet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Headers/HttpServer.hpp"


#include <stdio.h>




size_t			HttpServer::ft_upload_file( std::string request_http )
{
	std::cout << GREEN << "Dans ft_upload_file : \n" << CLEAR <<  std::endl;
	sleep(2);

	std::cout << "REQUEST = " << request_http << std::endl;

	sleep(5);
	
	size_t pos = request_http.find("filename=");				// on cherche le nom du fichier
	if (pos == std::string::npos)
		return (400);
	size_t pos_end = request_http.find("\"", pos + 10);
	if (pos_end == std::string::npos)
		return (400);
	
	// on recupere le nom du fichier
	std::string name(request_http, pos + 10 , pos_end - (pos + 10 ));
	std::cout << "name = " << name << std::endl;
	// std::string ret = ft_where_to_upload(this->_header_requete[0].path);
	std::string ret = "";
	exit(1);
	if (ret == "404")
		return (404);
	else if (ret == "403")
		return (403);
	else if (ret == "400")
		return (400);
	else if (ret == "413")
		return (413);
	else if (ret == "431")
		return (431);
	// exit(1);

	// exit(1);
	name.insert(0, "/");
	// name.insert(0, ft_where_to_upload(this->_header_requete[0].path));

	std::cout << "ICI name = " << name << std::endl;
	// exit(1);
	struct stat buff;
	if (lstat(name.c_str(), &buff) == 0)		// fichier deja uploader
		return (200);
	
	//  on recupere le body
	pos = request_http.find("\r\n\r\n");		// on cherche la fin du header
	std::string body_tmp(request_http, pos);
	body_tmp.erase(0, 4);   					// supprime \r\n\r\n
	pos = body_tmp.find("\r\n");				// on cherche la fin de ligne de boundary
	std::string boundary(body_tmp, 0, pos);		// on cree une string boundary
	boundary.append("--");						// on ajoute -- pour la fin
	std::cout << "boundary = " << boundary << std::endl;

	pos = body_tmp.find("\r\n\r\n");			// separation du texte inutile ? et des donnees
	body_tmp.erase(0, pos + 4);
	std::cout << "body_tmp = " << body_tmp << std::endl;

	// test pour les grosses donnees			// PAS FAIT ENCORE
	pos= body_tmp.find(boundary);
	if (pos == std::string::npos)
	{
		std::cout << "OUPS erreur ne trouve pas :" << std::endl;
		std::cout << "boundary = " << boundary << std::endl;
		// dans ce cas, il faut couper le fichier en plusioeurs morceaux
		exit(1);
	}
	std::cout << "pos = " << pos <<std::endl;
	
	std::string BODY(body_tmp, 0, pos);
	BODY.erase( BODY.size() - 2, 2);

	std::fstream file_out;
	file_out.open(name.c_str(), std::ios_base::out);		// on cree le fichier
	if (!file_out.is_open())							// on peut pas creer le fichier
	{
		file_out.close();
		return (500);
	}
	else
	{
		file_out << BODY;
		std::cout << "CA MARCHE il faut mettre non pas 200 mais 201" << std::endl;
		file_out.close();
	}
	return (201);
}


/*
**	Functions a deplacer dans un autre fichier
*/
void	HttpServer::ft_clean_socket_clients( void )
{
	std::cout << "Dans clean socket clients." << std::endl;
	std::vector<t_client_socket>::iterator it_b;
	std::vector<t_client_socket>::iterator it_e = this->_all_client_socket.end();

	for (it_b = this->_all_client_socket.begin(); it_b != it_e; it_b++)
	{
		std::cout << "\t dans la boucle client" << std::endl;
		close(it_b->client_socket);
	}
	return ;
}

void	HttpServer::ft_clean_socket_servers( void )
{
	std::cout << "Dans clean socket servers." << std::endl;
	std::vector<t_http_server>::iterator it_b;
	std::vector<t_http_server>::iterator it_e = this->_http_servers.end();

	for (it_b = this->_http_servers.begin(); it_b != it_e; it_b++)
	{
		std::cout << "\t dans la boucle serv" << std::endl;
		close(it_b->sock);
	}
	return ;
}