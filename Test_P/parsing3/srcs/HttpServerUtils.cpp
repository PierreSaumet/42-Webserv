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


std::string				HttpServer::ft_where_to_upload( std::string path)
{
	if (this->_servers[0].nbr_location > 0)
	{
		std::cout << "il y a des locations : " << this->_servers[0].nbr_location << std::endl;
		size_t i = 0;
		while ( i < this->_servers[0].nbr_location)
		{
			std::cout << "affiche le nom de la location : " << this->_servers[0].location[i].name_location << std::endl;
			
			size_t found = path.find("/");
			if (found == std::string::npos)
			{
				std::cout << "ERREUR euh pas normal ne trouve pas / dans la requete a traiter "<< std::endl;
				exit(EXIT_FAILURE);
			}
			else
			{
				std::string tmp = this->_servers[0].location[i].name_location;
				tmp.insert(0, this->_servers[0].root_server);
				// if (tmp[tmp.size() - 1] != '/')
				// 	tmp.append("/");
				std::cout << "tmp = " << tmp << std::endl;			// tmp = nom de location
				std::cout << "path = " << path << "\n" << std::endl;
				if (path.compare(tmp) == 0)
				{
					std::cout << "OUI on se trouve dans un dossier location on retourne l'addresse ou uploader " << std::endl;
					
					if (this->_servers[0].location[i].upload_store_location.empty() == true)
					{
						std::cout << "OUI vide doit retourner une erreur" << std::endl;
						return ("");
						// exit(1);
					}
					
					return (this->_servers[0].location[i].upload_store_location);
					
				}
			}
			i++;
		}
	}
	// exit(1);
	std::cout << "Ca veut dire qu'on doit uploader dans le root" << std::endl;
	return (this->_servers[0].upload_store_server);
	
}



size_t			HttpServer::ft_upload_file( std::string request_http )
{
	std::cout << GREEN << "Dans ft_upload_file : \n" << CLEAR <<  std::endl;
		size_t pos = request_http.find("filename=");				// on cherche le nom du fichier
	if (pos == std::string::npos)
		return (500);
	size_t pos_end = request_http.find("\"", pos + 10);
	if (pos_end == std::string::npos)
		return (500);
	
	// on recupere le nom du fichier
	std::string name(request_http, pos + 10 , pos_end - (pos + 10 ));
	if (ft_where_to_upload(this->_header_requete[0].path) == "")
		return (500);
	name.insert(0, "/");
	name.insert(0, ft_where_to_upload(this->_header_requete[0].path));

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