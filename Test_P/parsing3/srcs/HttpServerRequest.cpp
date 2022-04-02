/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServerRequest.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psaumet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/01 11:07:34 by psaumet           #+#    #+#             */
/*   Updated: 2021/06/01 11:07:42 by psaumet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Headers/HttpServer.hpp"


/*
**	DONC on parse la requete et on recupere les info
*/
void	HttpServer::ft_parser_requete( int len_msg, const char  *msg )
{
	std::cout << "Dans parser requete " << std::endl;

	std::string request_http(msg);
	std::cout << "msg = " << request_http << std::endl;
	std::cout << "len = " << request_http.length() << std::endl;
	std::cout << "len = " << len_msg << std::endl;



	if (request_http.compare(0, 4, "GET ") == 0)
	{
		this->ft_get(request_http, len_msg);
		std::cout << "fin de ft_get(), on return " << std::endl;
		return ;
	}
	else if (request_http.compare(0, 5, "POST ") == 0)
		this->ft_post(request_http, len_msg);
	else if (request_http.compare(0, 7, "DELETE ") == 0)
		this->ft_delete(request_http, len_msg);
	else
	{
		std::cout << "ERROR dans la method error 405" << std::endl;
	}
	// on regarder si c'est get et la taille sup a 1023 si oui erreur 431
	// if (request_http.compare(0, 4, "GET ") == 0 && len_msg > 1023)
	// {
	// 	std::cout << RED << "PUTAIN ERREUR 431 car get et trop grand " << CLEAR << std::endl;
	// 	this->_header_requete[0].error = true;
	// 	this->_header_requete[0].num_error = 431; 
	// }

	// on trouve la fin du header de la requete HTTP.
	std::string::iterator	it_b = request_http.begin();
	std::string::iterator	it_end_request;
	int i =0;
	for(; it_b != request_http.end(); it_b++)
	{
		std::string end_request(it_b, it_b + 4);
		if (end_request == "\r\n\r\n")
		{
			it_end_request = it_b;
			break ;
		}
		// std::cout << "*it = " << *it_b << std::endl;
		i++;
	}
	std::cout << RED << "ICI " << CLEAR << std::endl;
	std::string size_header(request_http.begin(), it_end_request);
	std::cout << RED << "ICI =)" << CLEAR << std::endl;
	if (size_header.size() >= 1024)		// server apache 8000 max
	{
		std::cout << RED << "ICI 2" << CLEAR << std::endl;
		// throw error ou affiche la page 413 ?
		// throw Error(10, "Error, in recieved header, the header size is greater than 8000.", 2);
		std::cout << "ERREUR size du header trop grand return une erreur 413 entity too large" << std::endl;

		// alors si get return 431
	}
	else
	{
		std::cout << RED << "ICI 3" << CLEAR << std::endl;
		std::cout << "Taille du header bon = " << size_header.size() << std::endl;
		// test savoir quelle method
		if (this->_header_requete.empty() == true)
		{
			std::cout << "Notre container header est vide, on en cree un et on recupere les informations" << std::endl;
			this->_header_requete.push_back(t_header_request());

			this->_header_requete[0].method = this->ft_check_methods_header(size_header);
			if (this->_header_requete[0].method.empty() == true)
				throw Error(11, "Error, in recieved header, the method used is not correct.", 2);
			std::cout << "On a la requete :" << this->_header_requete[0].method << "-" <<  std::endl;
		
			this->_header_requete[0].path = this->ft_check_path_header(size_header);
			if (this->_header_requete[0].path.empty() == true)
				throw Error(12, "Error, in recieved header, the path is not correct.", 2);;
			std::cout << "le path = " << this->_header_requete[0].path << std::endl;
			
			// erruer avec le /flavicon.ico
			if (this->_header_requete[0].path == "/flavicon.ico ")
			{
				std::cout << "merde flavicon " << std::endl;
				this->_header_requete[0].path = "/ ";
			}


			std::cout << " Taille DU PATH = " << this->_header_requete[0].path.length() << std::endl;

			// donc si la taille du path est superieur a 1024 on  va dire
			if (this->_header_requete[0].path.length() > 1024)
			{
				// doit setup error 414 si GET
				std::cout << "path superierur a 1024" << std::endl;
			}
			else
			{
				std::cout << "taille path ok" << std::endl;
			}
			// EUH a quoi ca sert ca ?
			// if (this->_header_requete[0].path == "/page2.html")
			// {
			// 	std::cout << "YES" << std::endl;
			// 	exit(EXIT_SUCCESS);
			// }

			this->_header_requete[0].protocol = this->ft_check_protocol_header(size_header);
			if (this->_header_requete[0].protocol.empty() == true)
				throw Error(13, "Error, in recieved header, the protocol is not correct.", 2);
			std::cout << "le protocol = " << this->_header_requete[0].protocol << "-" << std::endl;

			this->_header_requete[0].host = this->ft_check_host_header(size_header);
			if (this->_header_requete[0].host.empty() == true)
				throw Error(14, "Error, in recieved header, the host is not correct.", 2);			
			std::cout << "le host = " << this->_header_requete[0].host << "-" << std::endl;

			std::cout << GREEN << "On a bien recu une demande " << CLEAR << std::endl;
		}
		else
		{
			std::cout << "Probleme le container qui recupere la header de la requete n'est pas vide. " << std::endl;
			std::cout << "Il faut le supprimer apres avoir fait traite une demande." << std::endl;
		}

	}
	std::cout << "i = " << i << std::endl;
}

size_t			HttpServer::ft_get( std::string request_http, int len_msg)
{
	std::cout << "Dans get : " << std::endl;

	if (this->_header_requete.empty() == true)
	{
		this->_header_requete.push_back(t_header_request());

		if (len_msg > 1023)
		{
			std::cout << RED << "PUTAIN ERREUR 431 car get et trop grand " << CLEAR << std::endl;
			this->_header_requete[0].error = true;
			this->_header_requete[0].num_error = 431; 

			// il faut regarder dans les error_page si cette erreur est specifie
			//	regarder si on est dans root et errors ou location
			if (this->ft_setup_error_header(request_http, len_msg) == 0)
				return (0);
			else 
				return (1);

		}
		else
		{
			std::cout << "pas 'erreur 431" << std::endl;
			std::cout << "request = " << request_http << std::endl;
		}
	}
	else
	{
		std::cout << RED << "Probleme le container qui recupere la header de la requete n'est pas vide. " << std::endl;
		std::cout << "Il faut le supprimer apres avoir fait traite une demande." << CLEAR << std::endl;
	}



	return (0);
}


size_t			HttpServer::ft_setup_error_header( std::string request_http, int len_msg )
{
	(void)request_http;
	(void)len_msg;
	// donc on a une erreur.
	// 1) il faut regarder ou on se trouve dans la requete si on est dans du root ou dans du location
	// 2) regarder si l'erreur donne est deja configurer dans la directive error_page
	// 3) si oui, on setup le header avec le fichier
	// 4) si non, on regarde si on a le fichier de l'erreur en question.
	// 5) si le fichier existe on setup le header et on revoit
	// 6) si non, on setup le bon header

	// 1
	std::cout << "display un truc = " << this->_servers[0].host_server << std::endl;
	//	1: il y a des location si oui, on cherche si on est dedans...
	if (this->_servers[0].nbr_location > 0)
	{
		std::cout << "il y a des loctions : " << this->_servers[0].nbr_location << std::endl;
		size_t i = 0;
		while ( i < this->_servers[0].nbr_location)
		{
			std::cout << "affiche le nom de la location : " << this->_servers[0].location[i].name_location << std::endl;
			
			size_t found = request_http.find("/");
			size_t size_name_location = this->_servers[0].location[i].name_location.length();
			if (found == std::string::npos)
			{
				std::cout << "ERREUR euh pas normal ne trouve pas / dans la requete a traiter "<< std::endl;
				exit(EXIT_FAILURE);
			}
			else
			{
				if (request_http.compare(found, size_name_location, this->_servers[0].location[i].name_location) == 0)
				{
					std::cout << "OUI on se trouve dans un dossier location a continuer " << std::endl;
					
					// go checker si l'erreur est setup
					if (this->_header_requete[0].error == true)
					{
						if (this->_header_requete[0].num_error)
						{

							std::map<int, std::string>::iterator it = this->_servers[0].location[i].error_location.begin();
							// on cherche si l'erreur a ete idique dans une directive error_page
							for (; it != this->_servers[0].location[i].error_location.end(); it++)
							{
								std::cout << "on display les erreur dans location... " << it->first << std::endl;
								std::cout << "et nous = " << this->_header_requete[0].num_error << std::endl;
								if (it->first >= 0 && this->_header_requete[0].num_error == (size_t)it->first)
								{
									std::cout << "OUI ILS OSNT EGAL" << std::endl;
									std::cout << "euh second = " << it->second << std::endl;
									// on met dans le body le fichier a recuperer
									this->_header_requete[0].body_error = it->second;
									std::cout << " on a mit le fichiera recuperer pour l'error." << std::endl;
									return (0);
								}
								else
								{
									std::cout << "euh non " << std::endl;
								}
							}
							std::cout << RED << "l'erreur n'a pas ete trouve dans error_page " << CLEAR << std::endl;
							this->_header_requete[0].body_error = "/";
							this->_header_requete[0].body_error.insert(0, this->_servers[0].location[i].name_location);
							// this->_header_requete[0].body_error = this->_servers[0].location[i].name_location;
							this->_header_requete[0].body_error.insert(0, this->_servers[0].root_server);
							std::cout << "euh le body error = " << this->_header_requete[0].body_error << std::endl;
							// this->_header_requete[0].body_error = this->_servers[0].root_server;
							// this->_header_requete[0].body_error = "NULL";
						}
						else
						{
							std::cout << "pas normal on ne doit pas etre la num error pas setup = false" << std::endl;
							exit(EXIT_FAILURE);
						}
					}
					else
					{
						std::cout << "pas normal on ne doit pas etre la error = false" << std::endl;
						exit(EXIT_FAILURE);
					}

					return (0);
				}
				else
				{
					std::cout << "euh non pas dans un location " << std::endl;
					std::cout << " REKT FRAGILE " << std::endl;
					exit(EXIT_FAILURE);
				}
			}
			i++;
		}
		// this->_servers[index_server].location[index_location].name_location
	}
	else
	{
		std::cout << "il y a pas de location" << std::endl;
	}
	return (0);
}


void			HttpServer::ft_post(std::string request_http, int len_msg)
{
	(void)request_http;
	(void)len_msg;
}
void			HttpServer::ft_delete(std::string request_http, int len_msg)
{
	(void)request_http;
	(void)len_msg;
}




std::string		HttpServer::ft_check_host_header( std::string header )
{
	size_t pos;

	if ((pos = header.find("Host: ", 0)) == std::string::npos)
	{
		std::cout << "ERREUR NE TROUBE PAS LE HOST DANS LE HEADER" << std::endl;
		return (NULL);
	}
	else
	{
		size_t pos_user;
		if ((pos_user = header.find("User-Agent:", pos)) == std::string::npos)
		{
			std::cout << "ERREUR NE TROUBE PAS LE HOST DANS LE HEADER" << std::endl;
			return (NULL);
		}
		else
		{
			std::string tmp(header, pos + 6, pos_user - (pos + 6));
			for (size_t i = 0; i < this->_data->ft_get_nbr_servers(); i++)
			{
				// std::cout << "this->_servers[i].host_server =" << this->_servers[i].host_server <<std::endl;
				// std::cout << "tmp =" << tmp << std::endl;
				// std::cout << "i" << std::endl;
				if (tmp.compare(0, 9, this->_servers[i].host_server) == 0)
				{
					std::cout << "ILS SONT EGAUX les host" << std::endl;

					std::stringstream ss;
					ss << this->_servers[i].port_server;
					std::string port;
					ss >> port;
					if (tmp.compare(10, 4, port) == 0)
					{
						std::cout<< "Ils sont egaux les port " << std::endl;
						return (tmp);
					}
					else
					{
						if (this->_data->ft_get_nbr_servers() > 1)
						{
							std::cout << "euh nbr serv = " << this->_data->ft_get_nbr_servers() << std::endl;
							for (size_t y = 0; y < this->_data->ft_get_nbr_servers(); y++)
							{
								std::stringstream ss2;
								std::string port2;
								// ss.str("");
								// ss.flush();
								// port = "";
								// ss >> port;
								std::cout << "port lol = " << port2 << std::endl;
								std::cout << RED << "y = " << y << " et port server = " << this->_servers[y].port_server << CLEAR << std::endl;
								ss2 << this->_servers[y].port_server;
								// std::string port;
								ss2 >> port2;
								std::cout << "MERDE = " << port2 << " et y = " << y << std::endl;
								if (tmp.compare(10, 4, port2) == 0)
								{
									std::cout<< "Ils sont egaux les port " << std::endl;
									return (tmp);
								}
								ss2.str("");
								ss2.flush();
								port2 = "";
							}
						}
						std::cout << "port pas egaux ? " << std::endl;
						std::cout << " tmp = -" << tmp << "- et nous = -" << port << "-" << std::endl;
						throw Error(666, "Erreur test lol, ", 666);
					}
				}
				else
				{
					std::cout << "les host ne sont pas egaux" << std::endl;
					throw Error(666, "Erreur test lol 2, ", 666);
				}
				// if (this->_servers[i].host_server == tmp)
				// 	return (tmp);
			}
			std::cout << "ERREUR ICI " << std::endl;
			exit(1);
			return (std::string(header, pos + 6, pos_user - (pos + 6)));
		}
	}

}

std::string		HttpServer::ft_check_protocol_header( std::string header )
{
	size_t pos;

	if ((pos = header.find("HTTP/1.1")) == std::string::npos)
	{
		std::cout << "ERREUR NE TROUBE PAS LE PROTOCOL DANS LE HEADER" << std::endl;
		return (NULL);

	}
	else
	{
		return (std::string(header, pos, 8));
	}
}

std::string		HttpServer::ft_check_path_header( std::string header )
{
	size_t	pos;
	if ((pos = header.find_first_of("/", 0)) == std::string::npos)
	{
		std::cout << "ERREUR NE TROUVE PAS LE PATH DANS LE HEADER DE LA REQUETE \n";
		return (NULL);
	}
	else
	{
		size_t pos_http;
		if ((pos_http = header.find("HTTP")) == std::string::npos)
		{
			std::cout << "ERREUR NE TROUVE PAS LE PATH DANS LE HEADER DE LA REQUETE \n";
			return (NULL);
		}
		else
		{
			std::string tmp(header, pos, pos_http - pos);
			
			return (tmp);
		}
	}
}

std::string		HttpServer::ft_check_methods_header( std::string header )
{
	// std::string::iterator	it_b = header.begin();
	// std::string::iterator	it_end_method;

	size_t pos;
	if ((pos = header.find("GET", 0)) == std::string::npos)
	{
		if ((pos = header.find("POST", 0)) == std::string::npos)
		{
			if ((pos = header.find("DELETE", 0)) == std::string::npos)
			{
				std::cout << "check methos header probleme return null rien a ete trouve" << std::endl;
				return (NULL);
			}
			else
				return (std::string(header, pos, 6));;
		}
		else
			return (std::string(header, pos, 4));

	}
	else
		return (std::string(header, pos, 3));

	return (NULL);
}