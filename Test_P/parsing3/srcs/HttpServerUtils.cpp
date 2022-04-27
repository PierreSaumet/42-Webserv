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
	std::cout << GREEN << "Dans ft_where_to_upload : " << CLEAR << std::endl;
	std::cout << "path = " << path << std::endl;
	
	std::string tmp = "";
	struct stat buff;
	size_t count = 0;
	size_t i = path.find("/");				// cas requete action="/" erreur
	if (i == std::string::npos)
		return ("400");
	if (path[path.size() -1] == '/')		// si le dernier est '/' on supprime
		path.erase(path.size() - 1, 1);	
	while (i != std::string::npos)			// on compte les '/' pour connaitre le nbr de dossiers
	{
		count++;
		i = path.find("/", i + 1);
	}

	// on regarde ou on se trouve, on debute par un bloc locationn
	if (this->_servers[0].nbr_location > 0)
	{
		i = 0;
		while (i < this->_servers[0].nbr_location)
		{
			tmp = this->_servers[0].location[i].name_location;
			tmp.insert(0, this->_servers[0].root_server);
			if (path.compare(0, tmp.size(), tmp) == 0)				// Oui on est dans un bloc location
			{
				if (stat(path.c_str(), &buff) != 0)					// on verifie que le path existe
					return ("404");
				if (path.compare(0, this->_servers[0].location[i].upload_store_location.size(), this->_servers[0].location[i].upload_store_location) == 0)
				{
					// le path est egal a un dossier uploadstore d'un bloc location
					std::cout << "PATH = upload store location" << std::endl;
					if (this->_servers[0].location[i].buffer_size_location == 0) // upload_store n'est pas setup donc max 1mb
					{
						if ( std::strtol(this->_header_requete[0].content_length.c_str(), NULL, 10) > 1000000)
						{
							std::cout << "superieur donc erreur 413 body trop gros" << std::endl;
							return ("413");
						}
						else
						{
							std::cout << "taille bonne " << std::endl;
							return (this->_servers[0].location[i].upload_store_location);
						}
					}
					else
					{
						if ( std::strtol(this->_header_requete[0].content_length.c_str(), NULL, 10) > (long)this->_servers[0].location[i].buffer_size_location)
						{
							std::cout << "superieur donc erreur 413 body trop gros" << std::endl;
							return ("413");
						}
						else
							return (this->_servers[0].location[i].upload_store_location);
					}
				}
				else
				{
					if (S_ISREG(buff.st_mode))	// c'est un fichier donc erreur
						return ("400");
					if (path == tmp)			// le path est directement egal a un location bloc
					{
						std::cout << "LA" << std::endl;
						std::cout << "this->_requete content length = " << this->_header_requete[0].content_length << std::endl;
						std::cout << "this->_servers[0].location[i].client max = " << this->_servers[0].location[i].buffer_size_location << std::endl;
						sleep(2);
						if (this->_servers[0].location[i].buffer_size_location == 0) // pas setup donc on met 1mb soit 1 000 000
						{
							if ( std::strtol(this->_header_requete[0].content_length.c_str(), NULL, 10) > 1000000)
							{
								std::cout << "superieur donc erreur 413 body trop gros" << std::endl;
								return ("413");
								exit(1);
							}
							else
							{
								std::cout << "taille bonne " << std::endl;
								return (this->_servers[0].location[i].upload_store_location);
							}
							// if (this->_header_requete[0].content_length)
						}
						else
						{
							std::cout << "upload store est instaure" << std::endl;
							if ( std::strtol(this->_header_requete[0].content_length.c_str(), NULL, 10) > (long)this->_servers[0].location[i].buffer_size_location)
							{
								std::cout << "superieur donc erreur 431 body trop gros" << std::endl;
								return ("413");
							}
							else
								return (this->_servers[0].location[i].upload_store_location);
						}
						
						return (this->_servers[0].location[i].upload_store_location);
					}
					else
						return ("403");			// different donc erreur		
				}
			}
			i++;
		}
	}
	// std::cout << "On est pas dans un bloc location " << std::endl;
	if (stat(path.c_str(), &buff) != 0)
	{
		std::cout << "addresse existe pas " << std::endl;
		return ("404");
	}
	if (S_ISREG(buff.st_mode))			// c'est un fichier
	{
		if (count > 2)					// qui est contenu dans un dossier
		{
			tmp = path;
			if (path.compare(0, this->_servers[0].upload_store_server.size(), this->_servers[0].upload_store_server) == 0)
			{
				// on est dans le dossier du upload store du root donc OK
				std::cout << "ICI 2" << std::endl;
				std::cout << "this->_requete content length = " << this->_header_requete[0].content_length << std::endl;
				std::cout << "this->_servers[0].location[i].client max = " << this->_servers[0].buffer_size_server << std::endl;
				sleep(2);
				return (this->_servers[0].upload_store_server);
			}
			else
			{
				// on est ailleurs donc pas le droit
				return ("403");
			}
		}
		else
		{
			std::cout << "c'est qu'un fichier donc on copare ave script name " << std::endl;
			std::string tmp = path;
			path.erase(0, this->_servers[0].root_server.size());
			std::cout << "path = " << path << std::endl;
			if (path.compare(this->_header_requete[0].referer) != 0)
			{
				std::cout << "DIFFERENT AVEC LE REFERER DONCERRUER DANS ACTION" << std::endl;
				return ("403");
				exit(1);
			}
			else
			{
				std::cout << "GOOD =)" << std::endl;
				std::cout << "ICI 2" << std::endl;
				std::cout << "this->_requete content length = " << this->_header_requete[0].content_length << std::endl;
				std::cout << "this->_servers[0].Sclient max = " << this->_servers[0].buffer_size_server << std::endl;
				if (this->_servers[0].buffer_size_server == 0) // pas setup donc on met 1mb soit 1 000 000
				{
					if ( std::strtol(this->_header_requete[0].content_length.c_str(), NULL, 10) > 1000000)
					{
						std::cout << "superieur donc erreur 431 body trop gros" << std::endl;
						return ("413");
						exit(1);
					}
					else
					{
						std::cout << "taille bonne " << std::endl;
						return (this->_servers[0].upload_store_server);
					}
					// if (this->_header_requete[0].content_length)
				}
				else
				{
					std::cout << "upload store est instaure" << std::endl;
					if ( std::strtol(this->_header_requete[0].content_length.c_str(), NULL, 10) > (long)this->_servers[0].buffer_size_server)
					{
						std::cout << "superieur donc erreur 431 body trop gros" << std::endl;
						return ("413");
					}
					else
						return (this->_servers[0].upload_store_server);
				}
			}
			std::cout << "\nOn a le referer : " << this->_header_requete[0].referer << "\n" <<  std::endl;

		}
	}
	else if (S_ISDIR(buff.st_mode))
	{
		std::cout << "c'est un dossier on regrde si c'est upload" << std::endl;
		if (path.compare(0, this->_servers[0].upload_store_server.size(), this->_servers[0].upload_store_server) != 0)
		{
			std::cout << RED << "ce n'est pas le upload store donc erreur" <<  CLEAR << std::endl;
			return ("403");
		}
		else
		{
			std::cout <<  GREEN << "c'est bon c'est l'addresse de pload store" << CLEAR << std::endl;
			if (this->_servers[0].buffer_size_server == 0) // pas setup donc on met 1mb soit 1 000 000
			{
				if ( std::strtol(this->_header_requete[0].content_length.c_str(), NULL, 10) > 1000000)
				{
					std::cout << "superieur donc erreur 431 body trop gros" << std::endl;
					return ("413");
				}
				else
				{
					std::cout << "taille bonne " << std::endl;
					return (this->_servers[0].upload_store_server);
				}
				// if (this->_header_requete[0].content_length)
			}
			else
			{
				std::cout << "upload store est instaure" << std::endl;
				if ( std::strtol(this->_header_requete[0].content_length.c_str(), NULL, 10) > (long)this->_servers[0].buffer_size_server)
				{
					std::cout << "superieur donc erreur 431 body trop gros" << std::endl;
					return ("413");
				}
				else
					return (this->_servers[0].upload_store_server);
			}
			return (this->_servers[0].upload_store_server);
		}
	}
	std::cout << "pas normal" << std::endl;
	return ("");
}



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
	std::string ret = ft_where_to_upload(this->_header_requete[0].path);
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
	name.insert(0, ft_where_to_upload(this->_header_requete[0].path));

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