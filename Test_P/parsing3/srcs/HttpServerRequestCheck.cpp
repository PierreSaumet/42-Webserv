/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServerRequestCheck.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psaumet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/01 11:07:34 by psaumet           #+#    #+#             */
/*   Updated: 2021/06/01 11:07:42 by psaumet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Headers/HttpServer.hpp"


std::string		HttpServer::ft_check_pathhttp_header( std::string header )
{
	// recupere le path complet, celui de l'ordinateur et du fichier demande 
	//	le fichier demander se retrouve dans le header et on enleve les valeur apres le -?-

	size_t		pos_backslash = header.find("/");

	if (pos_backslash == std::string::npos)
	{
		std::cout << RED << "dans ft_check_path_header, ne trouve pas le -/-: dans le header de la requete" << CLEAR << std::endl;
		return ("");
	}
	else
	{
		size_t pos_end;
		if (this->_header_requete[0].method == "GET")
			pos_end = header.find("?", pos_backslash);
		if (this->_header_requete[0].method == "POST")
		{
			pos_end = header.find("HTTP/1.1", pos_backslash);
			pos_end -= 1; // on supprime l'espace de fin
		}

		if (pos_end == std::string::npos)
		{
			std::cout << RED << "dans ft_check_path_header, ne trouve pas -?-: dans le header de la requete" << CLEAR << std::endl;
			
			// erreur pour trouver img/chien donc on retourne simplement le path
			std::cout << "on va retourner le path ? ..." << std::endl;
			return (this->_header_requete[0].path);
			return ("");
		}
		else
		{
			std::string file_name(header, pos_backslash, pos_end - pos_backslash);

			char	cwd[256];
			if (getcwd(cwd, sizeof(cwd)) == NULL)
			{
				perror("getcwd() error");		// retour ne error
				return ("");
			}
			else
			{
				std::string 	total(cwd);
				std::string 	tmp = this->_servers[0].root_server;

				size_t pos = tmp.find("."); 
				tmp.erase(pos, 1);
				total.append(tmp);
				total.append(file_name);
				return (total);
			}
		}
	}
}

std::string		HttpServer::ft_check_accept_header( std::string header )
{
	size_t		pos = header.find("Accept: ");
	if (pos == std::string::npos)
	{
		std::cout << RED << "dans ft_check_accpet, ne trouve pas ACCEPT: dans le header de la requete" << CLEAR << std::endl;
		return ("");
	}
	{
		size_t	pos_end = header.find("\r\n", pos);
		if (pos_end == std::string::npos)
		{
			std::cout << RED << "dans ft_check_accpet, ne trouve pas \r\n: dans le header de la requete" << CLEAR << std::endl;
			return ("");
		}
		else
		{
			std::string tmp(header, pos + 8, pos_end - (pos + 8));
			return (tmp); 
		}
	}
}



int				HttpServer::ft_check_method_allowed_header( std::string request_http, std::string method )
{
	if (this->_servers[0].nbr_location > 0)
	{
		std::cout << "il y a des locations : " << this->_servers[0].nbr_location << std::endl;
		size_t i = 0;
		while ( i < this->_servers[0].nbr_location)
		{
			std::cout << "affiche le nom de la location : " << this->_servers[0].location[i].name_location << std::endl;
			
			size_t found = request_http.find("/");
			// size_t size_name_location = this->_servers[0].location[i].name_location.length();
			if (found == std::string::npos)
			{
				std::cout << "ERREUR euh pas normal ne trouve pas / dans la requete a traiter "<< std::endl;
				exit(EXIT_FAILURE);
			}
			else
			{
				std::string tmp = this->_servers[0].location[i].name_location;
				if (tmp[tmp.size() - 1] != '/')
					tmp.append("/");
				std::cout << "tmp = " << tmp << std::endl;
				if (request_http.find(tmp) != std::string::npos)
				{
					std::cout << "OUI on se trouve dans un dossier location a continuer   11 " << std::endl;
					
					// 
					std::vector<std::string>::iterator	it_b = this->_servers[0].location[i].methods_location.begin();
					for (; it_b != this->_servers[0].location[i].methods_location.end(); it_b++)
					{
						if (*it_b == method)
						{
							std::cout << " la method est autorisee dans location " << std::endl;
							return (0);		// oui method fonctionne
						}
					}
					std::cout << " la method n'est PAS autorisee dans location " << std::endl;
					return (1);		// la method n'est pas autorisee
				}
			}
			i++;
		}
	}
	// exit(1);
	std::cout << "on va chercher dans le root" << std::endl;
	std::vector<std::string>::iterator  it_b = this->_servers[0].methods_server.begin();
	for (; it_b != this->_servers[0].methods_server.end(); it_b++)
	{
		if (*it_b == method)
		{
			std::cout << " la method est autorisee dans root " << std::endl;
			return (0);		// oui ca marche
		}
	}
	std::cout << " la method n'est PAS autorisee dans root " << std::endl;
	return (1);
}


/*
**	std::string		ft_check_protocol_header( std::string header )
**		This function checks the version of the HTTP protocol used by the client's request.
**	
**		Attention ne epeut pas retour ner NULL
*/
std::string		HttpServer::ft_check_protocol_header( std::string header )
{
	size_t pos;

	if ((pos = header.find("HTTP/1.1")) == std::string::npos)
	{
		// A FAIRE: creer une erreur propre.
		std::cout << "ERREUR NE TROUBE PAS LE PROTOCOL DANS LE HEADER" << std::endl;
		return ("");
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
		// A FAIRE: creer une erreur propre.
		std::cout << "ERREUR NE TROUVE PAS de / dans le path du HEADER DE LA REQUETE \n";
		return ("");
	}
	else
	{
		size_t pos_http;
		if ((pos_http = header.find("HTTP/1.1\r\n")) == std::string::npos)
		{
			std::cout << "ERREUR NE TROUVE PAS  HTTP protocol dans le PATH du HEADER DE LA REQUETE \n";
			return ("");
		}
		else
		{
			if (pos > pos_http)
			{
				// A FAIRE: creer une erreur propre.
				std::cout << "ERREUR le path doit etre avant le http version dans le header du client \n";
				return ("");
			}
			std::string tmp(header, pos, pos_http - pos - 1);
			// on setup aussi le request_uri
			this->_header_requete[0].request_uri = tmp;
			// on setup aussi scriptfilename
			size_t pos_tmp = tmp.find("?");
			std::string tmp2(tmp, 0, pos_tmp);
			this->_header_requete[0].script_file_name = tmp2;
			

			if (tmp.size() != 1)							// on rajoute le root au debut de la string
				tmp.insert(0, this->_servers[0].root_server);
			return (tmp);
		}
	}
}


/*
**	std::string		ft_check_host_header( std::string header )
**		This function checks for the presence of 'Host' in the client request.
**
**		et retourne une string contenant le host et le port.
**		exmeple: 127.0.0.1:8080
**			==> ne marche pas si 8081 			A FAIRE
*/
std::string		HttpServer::ft_check_host_header( std::string header )
{
	size_t pos;

	pos = 0;
	if ((pos = header.find("Host: ", 0)) == std::string::npos)
	{
		// A FAIRE: creer une erreur propre.
		std::cout << "ERREUR NE TROUVE PAS LE HOST DANS LE HEADER" << std::endl;
		return ("");
	}
	else
	{
		size_t pos_end;
		if ((pos_end = header.find("\r\n", pos)) == std::string::npos)
		{
			// A FAIRE: creer une erreur propre.
			std::cout << "ERREUR NE TROUVE PAS LE USER-AGENT DANS LE HEADER" << std::endl;
			return ("");
		}
		else
		{
			// on verifie que la position de host est avant la position de user-agent.
			if (pos > pos_end)
			{
				// A FAIRE: creer une erreur propre.
				std::cout << "ERREUR HOST doit etre avant USER-AGENT" << std::endl;
				return ("");
			}
			// on recupere les informations apres Host et avant User-agent
			std::string tmp(header, pos + 6, pos_end - (pos + 6));
			for (size_t i = 0; i < this->_data->ft_get_nbr_servers(); i++)
			{
				// on parcourt nos servers pour verifier que le host de la requete est
				//	bien pour un de nos servers.
				if (tmp.compare(0, 9, this->_servers[i].host_server) == 0)
				{
					// les host de la requete et de notre server sont egaux
					//	ex: 127.0.0.1
					// std::cout << "ILS SONT EGAUX les host" << std::endl;

					// on convertit le port (std::size_t) en std::string.
					std::stringstream ss;
					ss << this->_servers[i].port_server;
					std::string port;
					ss >> port;
					if (tmp.compare(10, 4, port) == 0)
					{
						// les ports sont bon, on retourne la string au complete
						//		ex : 127.0.0.1:8080
						// std::cout<< "Ils sont egaux les port cas 1" << std::endl;
						return (tmp);
					}
					else
					{
						//	Les ports ne correspondent pas.
						if (this->_data->ft_get_nbr_servers() > 1)
						{
							// Cas ou on a plusieurs servers
							std::cout << "euh nbr serv = " << this->_data->ft_get_nbr_servers() << std::endl;
							for (size_t y = 0; y < this->_data->ft_get_nbr_servers(); y++)
							{
								// on parcourt chaque servers pour voir si on a un port
								//	qui correspond a la requete du client.
								std::stringstream ss2;
								std::string port2;
								std::cout << RED << "y = " << y << " et port server = " << this->_servers[y].port_server << CLEAR << std::endl;
								ss2 << this->_servers[y].port_server;
								ss2 >> port2;
								std::cout << "Punaise = " << port2 << " et y = " << y << std::endl;
								if (tmp.compare(10, 4, port2) == 0)
								{
									std::cout<< "Ils sont egaux les port cas 2" << std::endl;
									return (tmp);
								}
								ss2.str("");
								ss2.flush();
								port2 = "";
							}
						}
						std::cout << "port pas egaux ? " << std::endl;
						std::cout << " tmp = -" << tmp << "- et nous = -" << port << "-" << std::endl;
						throw Error(666, "Erreur test lol, ", 666);			// A FAIRE au propre
					}
				}
				else
				{
					std::cout << "les host ne sont pas egaux" << std::endl;
					throw Error(666, "Erreur test lol 2, ", 666);		// A FAIRE au propre
				}
				// if (this->_servers[i].host_server == tmp)
				// 	return (tmp);
			}
			std::cout << "ERREUR ICI " << std::endl;			// A FAIRE au propre
			exit(1);
			return (std::string(header, pos + 6, pos_end - (pos + 6)));
		}
	}
}
