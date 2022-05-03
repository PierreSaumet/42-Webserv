/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServerError.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psaumet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/01 11:07:34 by psaumet           #+#    #+#             */
/*   Updated: 2021/06/01 11:07:42 by psaumet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Headers/HttpServer.hpp"




/*
**	size_t		ft_setup_error_header( std::string request_http, int len_msg )
**
**	<!> This function is used if there is an error in the header or any error.
**	IT IS NOT FINISHED.
*/
int			HttpServer::ft_setup_error_header( std::string request_http, int len_msg )
{
	(void)len_msg;
	std::cout << GREEN << "Dans ft_setup_error_header" << CLEAR << std::endl;
	// exit(1);
	if (this->_servers[this->_num_serv].nbr_location > 0)
	{
		std::cout << "il y a des loctions : " << this->_servers[this->_num_serv].nbr_location << std::endl;
		size_t i = 0;
		while ( i < this->_servers[this->_num_serv].nbr_location)
		{
			std::cout << "affiche le nom de la location : " << this->_servers[this->_num_serv].location[i].name_location << std::endl;
			
			size_t found = request_http.find("/");
			// size_t size_name_location = this->_servers[this->_num_serv].location[i].name_location.length();
			if (found == std::string::npos)
			{
				std::cout << "ERREUR euh pas normal ne trouve pas / dans la requete a traiter "<< std::endl;
				exit(EXIT_FAILURE);
			}
			else
			{
				// std::cout << "request http = " << request_http << std::endl;
				// std::cout << "nom lcoation = " << this->_servers[this->_num_serv].location[i].name_location << std::endl;
				// std::cout << "found = " << found << std::endl;
				std::string tmp = this->_servers[this->_num_serv].location[i].name_location;
				if (tmp[tmp.size() - 1] != '/')
					tmp.append("/");
				// std::cout << "tmp = " << tmp << std::endl;
				if (request_http.find(tmp) != std::string::npos)
				{
					std::cout << "OUI on se trouve dans un dossier location a continuer   11 " << std::endl;
					
					// go checker si l'erreur est setup
					if (this->_header_requete[0].error == true)
					{
						if (this->_header_requete[0].num_error)
						{

							std::map<int, std::string>::iterator it = this->_servers[this->_num_serv].location[i].error_location.begin();
							// on cherche si l'erreur a ete idique dans une directive error_page
							for (; it != this->_servers[this->_num_serv].location[i].error_location.end(); it++)
							{
								std::cout << "on display les erreur dans location... " << it->first << std::endl;
								if (it->first >= 0 && this->_header_requete[0].num_error == (size_t)it->first)
								{
									std::cout << "OUI ils sont egaux, le num_error et l'erreur qui a ete setup dans le fichier de conf" << std::endl;
									std::cout << "euh second = " << it->second << std::endl;
									// on met dans le body le fichier a recuperer
									this->_header_requete[0].body_error = it->second;
									std::cout << " on a mis le fichier a recuperer pour l'error." << std::endl;
									std::cout << "\nbody error = " << this->_header_requete[0].body_error;
									if (i == 0)
										return (-1);
									else
										return (i);
								}
								else
								{
									std::cout << "euh non " << std::endl;
								}
							}
							std::cout << RED << "l'erreur n'a pas ete trouve dans error_page " << CLEAR << std::endl;
							this->_header_requete[0].body_error = "/";
							this->_header_requete[0].body_error.insert(0, this->_servers[this->_num_serv].location[i].name_location);
							// this->_header_requete[0].body_error = this->_servers[this->_num_serv].location[i].name_location;
							this->_header_requete[0].body_error.insert(0, this->_servers[this->_num_serv].root_server);
							std::cout << "euh le body error = " << this->_header_requete[0].body_error << std::endl;
							// this->_header_requete[0].body_error = this->_servers[this->_num_serv].root_server;
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
			}
			i++;
		}
		std:: cout << RED << " la pierre ca ne correspond pas des location  " << CLEAR << std::endl;

		std::map<int, std::string>::iterator it = this->_servers[this->_num_serv].error_server.begin();
		// on cherche si l'erreur a ete idique dans une directive error_page
		for (; it != this->_servers[this->_num_serv].error_server.end(); it++)
		{
			std::cout << "on display les erreur setup du server... " << it->first << std::endl;
			if (it->first >= 0 && this->_header_requete[0].num_error == (size_t)it->first)
			{
				std::cout << "OUI ils sont egaux, le num_error et l'erreur qui a ete setup dans le fichier de conf" << std::endl;
				std::cout << "euh second = " << it->second << std::endl;
				// on met dans le body le fichier a recuperer
				this->_header_requete[0].body_error = it->second;
				std::cout << " on a mis le fichier a recuperer pour l'error." << std::endl;
				std::cout << "\nbody error = " << this->_header_requete[0].body_error;
				return (0);
			}
			else
			{
				std::cout << "euh non " << std::endl;
			}
		}
		std:: cout << RED << " la pierre que faire ?  " << CLEAR << std::endl;
		this->_header_requete[0].body_error = "/";
		
		// this->_header_requete[0].body_error = this->_servers[this->_num_serv].location[i].name_location;
		this->_header_requete[0].body_error.insert(0, this->_servers[this->_num_serv].root_server);
		// this->_servers[index_server].location[index_location].name_location
	}
	else
	{
		std::cout << "il y a pas de location, on cherche les erreurs dans root" << std::endl;
		std:: cout << RED << " la pierre ca ne correspond pas des location  " << CLEAR << std::endl;

		std::map<int, std::string>::iterator it = this->_servers[this->_num_serv].error_server.begin();
		// on cherche si l'erreur a ete idique dans une directive error_page
		for (; it != this->_servers[this->_num_serv].error_server.end(); it++)
		{
			std::cout << "on display les erreur setup du server... " << it->first << std::endl;
			if (it->first >= 0 && this->_header_requete[0].num_error == (size_t)it->first)
			{
				std::cout << "OUI ils sont egaux, le num_error et l'erreur qui a ete setup dans le fichier de conf" << std::endl;
				std::cout << "euh second = " << it->second << std::endl;
				// on met dans le body le fichier a recuperer
				this->_header_requete[0].body_error = it->second;
				std::cout << " on a mis le fichier a recuperer pour l'error." << std::endl;
				std::cout << "\nbody error = " << this->_header_requete[0].body_error;
				return (0);
			}
			else
			{
				std::cout << "euh non " << std::endl;
			}
		}
		std:: cout << RED << " la pierre que faire ?  " << CLEAR << std::endl;
		this->_header_requete[0].body_error = "/";
		
		// this->_header_requete[0].body_error = this->_servers[this->_num_serv].location[i].name_location;
		this->_header_requete[0].body_error.insert(0, this->_servers[this->_num_serv].root_server);
		
	}
	return (0);
}

