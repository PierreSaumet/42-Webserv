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
**	size_t		ft_setup_error_header( std::string request_http)
**
**	<!> This function is used if there is an error in the header or any error.
**	IT IS NOT FINISHED.
*/
int			HttpServer::ft_setup_error_header( std::string request_http)
{
	std::cout << GREEN << "Dans ft_setup_error_header" << CLEAR << std::endl;
	
	if (this->_servers[this->_num_serv].nbr_location > 0)
	{
		std::cout << "il y a des loctions : " << this->_servers[this->_num_serv].nbr_location << std::endl;
		exit(1);
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
		// std::cout << "pas de location" << std::endl;
		// std::cout << "Error _page = " << this->_servers[this->_num_serv].folder_error << std::endl;
		// std::cout << "On a setup une erreur ? = " << this->_header_requete[0].error << std::endl;
		// std::cout << "On a une erreur numero = " << this->_header_requete[0].num_error << std::endl;
		// Cas ou il y a pas de location donc on met le root
		this->_header_requete[0].body_error.append(this->_servers[this->_num_serv].root_server);
		
		std::map<int, std::string>::iterator it = this->_servers[this->_num_serv].error_server.begin();
		for (; it != this->_servers[this->_num_serv].error_server.end(); it++)
		{
			if (it->first >= 0 && this->_header_requete[0].num_error == (size_t)it->first)
			{
				this->_header_requete[0].body_error.append(it->second);
				if (this->_header_requete[0].body_error[this->_header_requete[0].body_error.size() -1 ] != '/')
					this->_header_requete[0].body_error.append("/");
				std::cout << "\nbody error contient = " << this->_header_requete[0].body_error << std::endl;
				return (0);
			}
		}
	}
	// on met le body error a 0
	this->_header_requete[0].body_error.clear();
	std::cout << " body error = " << this->_header_requete[0].body_error << std::endl;
	return (0);
}



std::string		HttpServer::ft_find_error_html( void )
{
	std::cout << GREEN << "\n On est dans ft_find_error_html " <<  CLEAR << std::endl;
	std::stringstream 	ss;
	std::string			path_error;
	struct stat 		buff;
	DIR 				*dir = NULL;
	struct dirent  		*file = NULL;

	std::cout << "body error = " << this->_header_requete[0].body_error << std::endl;
	
	if (stat(this->_header_requete[0].body_error.c_str(), &buff) < 0)
		return (ft_create_error());

	ss << this->_header_requete[0].num_error;
	ss >> path_error;
	path_error.resize(4);
	if ((dir = opendir(this->_header_requete[0].body_error.c_str())) != NULL)
	{
		while ((file = readdir(dir)) != NULL)
		{
			if (file->d_type == DT_REG)
			{
				if (path_error.compare(0, 3, file->d_name, 0, 3) == 0)
				{
					std::cout << "fichier = " << file->d_name << std::endl;
					if (this->_header_requete[0].body_error[this->_header_requete[0].body_error.size() - 1] != '/')
						this->_header_requete[0].body_error.append("/");
					this->_header_requete[0].body_error.append(file->d_name);
					
					if (stat(this->_header_requete[0].body_error.c_str(), &buff) < 0)
						return (ft_create_error());
					path_error.clear();
					path_error =  ft_get_file(this->_header_requete[0].body_error);
					path_error.insert(0, this->ft_get_end_header());
					path_error.insert(0, this->ft_get_content_length(buff, 0, 0));
					path_error.insert(0, this->ft_get_server_name());
					path_error.insert(0, this->ft_get_date());
					path_error.insert(0, this->ft_get_charset());
					path_error.insert(0, this->ft_get_content_type());
					if(this->_header_requete[0].num_error == 405)
						path_error.insert(0, this->ft_get_allow());
					path_error.insert(0, this->ft_get_status(true));
					closedir(dir);
					return (path_error);
				}
			}
		}
	}
	closedir(dir);
	return (ft_create_error()); // test
}