/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServerRequestUtils.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psaumet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/01 11:07:34 by psaumet           #+#    #+#             */
/*   Updated: 2021/06/01 11:07:42 by psaumet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Headers/HttpServer.hpp"


std::string			HttpServer::ft_parsing_path_get_request( void )
{
	std::cout << RED << "Dans ft_parsing_path_get_request : FONCTION A CHANGER " << CLEAR << std::endl;
	size_t		pos_cursor = this->_header_requete[0].path.find("?");
	this->_header_requete[0].query_string = "";
	if (pos_cursor == std::string::npos)
		return ("");
	else
	{
		std::cout << "\tIl y a des donnees a parser dans la requete GET." << std::endl;
		std::cout << "pos_cursor = " << pos_cursor << std::endl;
		std::cout << "taille patg = " << this->_header_requete[0].path.size() << std::endl;
		if (pos_cursor == this->_header_requete[0].path.size() - 1)
			return ("");

		std::string tmp = "";


		
		pos_cursor++;
		size_t len = this->_header_requete[0].path.length();
		while (this->_header_requete[0].path[pos_cursor])
		{
			if (this->_header_requete[0].path.compare(pos_cursor, 1, "=") == 0)
			{
				this->_header_requete[0].data.insert(std::pair<std::string, std::string>(tmp, "NULL"));
				tmp.clear();
			}
			else if (this->_header_requete[0].path.compare(pos_cursor, 1, "&") == 0 || pos_cursor == len - 1)
			{
				if (pos_cursor == len - 1)
					tmp.append(this->_header_requete[0].path, pos_cursor, 1);
				std::map<std::string, std::string>::iterator it_b = this->_header_requete[0].data.begin();
				for (; it_b != this->_header_requete[0].data.end(); it_b++)
				{
					if (it_b->second == "NULL")
					{
						// On change les characteres speciaux
						tmp = ft_clean_path_get_request(tmp);
						it_b->second = tmp;

						// On ajoute les donnees a this->_header_requete[0].query_string
						this->_header_requete[0].query_string.append(it_b->first);
						this->_header_requete[0].query_string.append("=");
						this->_header_requete[0].query_string.append(it_b->second);
						this->_header_requete[0].query_string.append("&");
					}
				}
				tmp.clear();
			}
			else
				tmp.append(this->_header_requete[0].path, pos_cursor, 1);
			pos_cursor++;
		}
	}
	if (this->_header_requete[0].query_string.empty())
		return ("");
	this->_header_requete[0].query_string.erase(this->_header_requete[0].query_string.end() - 1);
	return (this->_header_requete[0].query_string);
}


/*
**	Cette fonction compare le port de la requete avec le port des servers pour connaitre
**	quel server on utilise.
**	retourne -1 si une erreur sinon retourne le numero de l'emplacement du server.
*/
int HttpServer::ft_find_index_server( void ) // A DEPLACER
{
	std::string 	port_request = this->_header_requete[0].host;
	size_t			pos = port_request.find(":");
		
	port_request.erase(port_request.begin(), port_request.begin() + pos + 1);
	if (this->_servers.size() > 1)
	{
		size_t			y = 0;

		while (y < this->_servers.size())
		{
			std::stringstream 	ss;
			std::string			port_server;

			ss << this->_servers[y].port_server;
			ss >> port_server;
			if (port_server == port_request)
				return (y);
			y++;
		}
		return (-1);

	}
	std::stringstream 	ss;
	std::string			port_server;

	ss << this->_servers[0].port_server;
	ss >> port_server;
	if (port_server == port_request)
		return (0);
	return (-1);
}



int		HttpServer::ft_redirection( void )
{
	// size_t i = 0;
	std::cout << GREEN << "Dans ft_redirection " << CLEAR << std::endl;
	if (this->_servers[this->_num_serv].nbr_location == 0)
	{
		std::cout << "pas de location, la requete est " << this->_header_requete[0].path <<  std::endl;
		if (this->_servers[this->_num_serv].return_server.empty() == false)
		{
			std::cout << "il y a une redirection, on setup 'this->_header_requete[0].return_used = true;'" << std::endl;
			
			std::cout << "requete= " << this->_header_requete[0].path << std::endl;
			if (this->_header_requete[0].path.find("/flavicon.ico") != std::string::npos)
			{
				std::cout << "flavicon donc pas de redirection" << std::endl;
				return (0);
			}
			this->_header_requete[0].return_used = true;
			return (1);
			exit(1);
		}
		else
		{
			std::cout << "pas de redirection dans le bloc server return 0" << std::endl;
			this->_header_requete[0].return_used = false;
			return (0);
		}
		
	}

	// std::cout << "On a setup this->_num_loc = " << this->_num_loc << std::endl;
	if (this->_servers[this->_num_serv].location[this->_num_loc].return_location.empty() == true)
	{
		std::cout << "On a pas de  redirection dans le bloc location : " << this->_servers[this->_num_serv].location[this->_num_loc].name_location << std::endl;
		this->_header_requete[0].return_used = false;
		return (0);
	}
	std::cout << "On a une redirection dans le bloc location : " << this->_servers[this->_num_serv].location[this->_num_loc].name_location << std::endl;
	this->_header_requete[0].return_used = true;
	this->_header_requete[0].path.erase(0, this->_servers[this->_num_serv].location[this->_num_loc].name_location.size());
	this->_header_requete[0].path.insert(0, this->_servers[this->_num_serv].location[this->_num_loc].return_location);
	return (1);

		// std::vector<std::string> all_location; // container qui va avoir le nom de tous les locations
		// for (std::vector<t_location>::iterator it = this->_servers[this->_num_serv].location.begin(); it != this->_servers[this->_num_serv].location.end(); it++)
		// 	all_location.push_back(it->name_location);
		// std::sort(all_location.begin(), all_location.end(), std::greater<std::string>()); // on trie les noms des locations
		
		// for (std::vector<std::string>::iterator it = all_location.begin(); it != all_location.end(); ++it)
		// {	
		// 	size_t i = 0;
		// 	while (i < this->_servers[this->_num_serv].nbr_location)
		// 	{
		// 		if (*it == this->_servers[this->_num_serv].location[i].name_location)
		// 		{
		// 			if (this->_header_requete[0].path.compare(0, it->size(), *it) == 0)
		// 			{
		// 				if (this->_servers[this->_num_serv].location[i].return_location.empty() == true)
		// 				{
		// 					std::cout << "On a pas de  redirection dans le bloc location : " << this->_servers[this->_num_serv].location[i].name_location << std::endl;
		// 					this->_header_requete[0].return_used = false;
		// 					return (0);
		// 				}
		// 				else
		// 				{
		// 					std::cout << "On a une redirection dans le bloc location : " << this->_servers[this->_num_serv].location[i].name_location << std::endl;
		// 					this->_header_requete[0].return_used = true;
		// 					// exit(1);
		// 					// test on supprime root et le nom de la location pour rempplacer par le return
		// 					this->_header_requete[0].path.erase(0, this->_servers[this->_num_serv].location[i].name_location.size());
		// 					this->_header_requete[0].path.insert(0, this->_servers[this->_num_serv].location[i].return_location);
		// 					return (1);
		// 				}
		// 			}
		// 		}
		// 		i++;
		// 	}
		// }
		// std::cout << "la requete ne correspond a aucun bloc location donc error 404 ?" << std::endl;
		// exit(1);
		// return (-1);
	// }
	// return (0);
}



size_t HttpServer::check_location( std::string path, std::string name_location )
{
	size_t 	i = 0;

	std::cout << "\nFonction check_location avec le path = -" << path << "-" << std::endl;
	// if 
	
	while ( i < this->_servers[this->_num_serv].location.size())
	{
		if (name_location == this->_servers[this->_num_serv].location[i].name_location)
		{
			std::cout << "bloc location = " << name_location << std::endl;
			this->_num_loc = i;   // a changer tu peux savoir l'index du loc bienavant ...

			std::cout << "Et la requete est : "<< this->_header_requete[0].path << std::endl;
			std::cout << "path = " << path << std::endl;
			 // Probleme avec les roots 
			if (path.size() - this->_servers[0].root_server.size() > 1)
			{
				struct stat buff;
				
				if (stat(path.c_str(), &buff) < 0)
				{
					
					if (name_location != "/")
					{
						if (this->_servers[this->_num_serv].location[i].root_location.empty() == true)
						{
							if (this->_servers[this->_num_serv].location[i].index_location.empty() == true)
							{
								if (this->_servers[this->_num_serv].location[i].autoindex_location == true)
								{
									if (this->_header_requete[0].path[this->_header_requete[0].path.size() - 1] == '/')
									{
										this->_header_requete[0].path = this->_servers[this->_num_serv].root_server;
										this->_header_requete[0].path.insert(0, "--AUTOINDEX--");
										std::cout << "Requete exsite pas mais location diff de / pas de root, pas index, autoindex ON donc on retourne ce qu'il y a dans root";
										return (0);
									}
									
								}
							}
						}
					}
					std::cout << "Le fichier demande n'existe pas on sort 404" << std::endl;
					std::cout << "car la requete est : " << this->_header_requete[0].path << std::endl;
					// exit(1);
					return (1); //404
				}
				// exit(1);
				if (S_ISDIR(buff.st_mode))
				{
					std::cout << "DOSSIER   i = " << i << std::endl;
					std::cout << "ICI requete[0].path = " << this->_header_requete[0].path << std::endl;
					if (this->_servers[this->_num_serv].location[i].root_location.empty() == false)		// root dans location est setup
					{
						// std::cout << "this->_header_requete[0].path[this->_header_requete[0].path.size() - 1] = " << this->_header_requete[0].path[this->_header_requete[0].path.size() - 1]<< std::endl;
						// exit(1);
						if (this->_header_requete[0].path[this->_header_requete[0].path.size() - 1] == '/')
						{
							if (this->_servers[this->_num_serv].location[i].index_location.empty() == false)	// si l'index est setup on le rajoute
							{
								this->_header_requete[0].path.append(this->_servers[this->_num_serv].location[i].index_location);
								return (0);
							}
							std::cout << "On demande un dossier, dans le bloc location / avec root sans index donc prend l'index du server" << std::endl;
							if (this->_servers[this->_num_serv].location[i].autoindex_location ==  true)
							{
								std::cout << "ON a AUTOINDEX exit" << std::endl;
								this->_header_requete[0].path.insert(0, "--AUTOINDEX--");
								
								return (0);
								exit(1);
							}
							this->_header_requete[0].path.append(this->_servers[this->_num_serv].index_server);
							return (0); //403
							
						}
						else
						{
							std::cout << "On demande un dossier, dans le bloc location / 403" << std::endl;
							return (2); // return 403/ demande first au lieu de first/
						}

						// exit(1);
					}
					else			// pas de root dans location
					{
						// on regarde si on demande un index.
						if (this->_header_requete[0].path[this->_header_requete[0].path.size() -1] == '/')
						{
							std::cout << "ON DEMANDE l'index d'un dossier" << std::endl;
							if (this->_servers[this->_num_serv].location[i].index_location.empty() == false)	// si l'index est setup on le rajoute
							{
								this->_header_requete[0].path.append(this->_servers[this->_num_serv].location[i].index_location);
								return (0);
							}
							if (this->_servers[this->_num_serv].location[i].autoindex_location == true)
							{
								this->_header_requete[0].path.insert(0, "--AUTOINDEX--");
								std::cout << "dans location, pas de root, pas de index, le dossier existe et il y a autoindex donc autoindex" << std::endl;
								return (0);
							}
							std::cout << "buf " << std::endl;
							exit(1);
							return (2); // 403
						}
						else
						{
							std::cout << "On demande un dossier sans /, dans le bloc location / 403" << std::endl;
							return (2); // return 403/ demande first au lieu de first/
						}
					}
				}
				else if (S_ISREG(buff.st_mode))
				{
					std::cout << "FICHIER donc bon" << std::endl;
					return (0);
				}
				else
					return (1); // 404
			}
			else	// path == / donc on return 0
			{
				if (this->_servers[this->_num_serv].location[i].root_location.empty() == true)
				{
					if (this->_servers[this->_num_serv].location[i].index_location.empty() == true)
					{
						std::cout << "On est dans le bloc location, on demande l'index, pas de root ni d'index dans location donc on retourne l'index du server" << std::endl;
						this->_header_requete[0].path.append(this->_servers[this->_num_serv].index_server);
						return (0);
					}
					this->_header_requete[0].path.append(this->_servers[this->_num_serv].location[i].index_location);
					std::cout << "On est dans le bloc location, on demande l'index, pas de root  dans location donc on retourne l'index du location" << std::endl;
					return (0);
					// return (1); //404
				}

				std::cout << "autre path = /" << std::endl;
				this->_header_requete[0].path_file = path;
				exit(1);
				return (0);
			}
		}
		i++;
	}
	std::cout << "MINCE " <<std::endl;
	exit(1);
	return (0);
}


size_t 	HttpServer::ft_check_access_path( void )
{
	std::cout << GREEN <<  "\n\nDans verifie le droit duchemin" << CLEAR << std::endl;

	if (this->_servers[this->_num_serv].nbr_location == 0)
	{
		std::cout << "pas de location donc on compare la demande avec l'acces de l'index" << std::endl;
		std::cout << "path = " << this->_header_requete[0].path << std::endl;
		
		if (this->_header_requete[0].path == "/")			// On demande l'index on retourne 0
		{
			this->_header_requete[0].path.insert(0, this->_servers[this->_num_serv].root_server);
			this->_header_requete[0].path.append(this->_servers[this->_num_serv].index_server);
			return (0);
		}
		std::string		path_index_server = this->_servers[this->_num_serv].index_server;
		struct stat		buff_index_server;
		struct stat		buff_path;

		path_index_server.insert(0, this->_servers[this->_num_serv].root_server);	
		stat(path_index_server.c_str(), &buff_index_server);

		this->_header_requete[0].path.insert(0, this->_servers[this->_num_serv].root_server);
		std::cout << "\n\n vant le dernier stat, header[path] = " << this->_header_requete[0].path << " et le path de l'index du erver = " << path_index_server << std::endl;

		if (stat(this->_header_requete[0].path.c_str(), &buff_path) < 0)
			return (1); // 404
		if (buff_path.st_dev == buff_index_server.st_dev && buff_path.st_ino == buff_index_server.st_ino)
			return (0);
		else
		{
			if (this->_header_requete[0].path.find("/flavicon.ico") != std::string::npos)
			{
				this->_header_requete[0].path.clear();
				this->_header_requete[0].path.insert(0, this->_servers[this->_num_serv].root_server);
				this->_header_requete[0].path.append("/flavicon.ico");
				std::cout << "this->_header_requete[0].path = " << this->_header_requete[0].path << std::endl;
				return (0);
			}
			return (2); // 403
		}
	}
	
	// std::cout << "Il y a plusieurs locations" << std::endl;
	std::cout << "path = " << this->_header_requete[0].path << std::endl;
	if (this->_header_requete[0].path.find("/flavicon.ico") != std::string::npos)
	{
		// this->_header_requete[0].path.erase(0, this->_servers[this->_num_serv].root_server.size());
		if (this->_header_requete[0].path.compare("/flavicon.ico") == 0)
		{
			// std::cout << "bingo" << std::endl;
			// exit(1);
			this->_header_requete[0].path.clear();
			this->_header_requete[0].path.insert(0, this->_servers[this->_num_serv].root_server);
			this->_header_requete[0].path.append("/flavicon.ico");
			std::cout << "merde this->_header_requete[0].path = " <<this->_header_requete[0].path << std::endl;
			// exit(1);	
			return (0);
		}
		std::cout << "404" << std::endl;
		return (1); // 404
	}


	std::vector<std::string> all_location; // container qui va avoir le nom de tous les locations
	for (std::vector<t_location>::iterator it = this->_servers[this->_num_serv].location.begin(); it != this->_servers[this->_num_serv].location.end(); it++)
		all_location.push_back(it->name_location);
	std::sort(all_location.begin(), all_location.end(), std::greater<std::string>()); // on trie les noms des locations
	
	for (std::vector<std::string>::iterator it = all_location.begin(); it != all_location.end(); ++it)
	{
		// std::cout << "bloc location = " << *it << std::endl;
		size_t 	i = 0;

		while ( i < this->_servers[this->_num_serv].location.size()) // a changer
		{
			if (*it == this->_servers[this->_num_serv].location[i].name_location)
			{
				std::cout << "On est dans le bloc du server " << *it << std::endl;
				if (this->_header_requete[0].path.compare(0, it->size(), *it) == 0)
				{
					std::cout << "la requete: " << this->_header_requete[0].path << " appartient au bloc location : " << *it << std::endl;
					
					
					// // si un root est setup on le met dans la requete
					if (this->_servers[this->_num_serv].location[i].root_location.empty() == false)
					{
						if (*it != "/") // on supprime le nom du bloc location et on met le root location
							this->_header_requete[0].path.erase(0, it->size());
						this->_header_requete[0].path.insert(0, this->_servers[this->_num_serv].location[i].root_location);
					}
					this->_header_requete[0].path.insert(0, this->_servers[this->_num_serv].root_server);

					if (*it == "/")
						return (this->check_location(this->_header_requete[0].path, "/"));

					std::cout << "avant check location =  " << this->_header_requete[0].path << std::endl;
					// exit(1);
					return (this->check_location(this->_header_requete[0].path, *it));
					
					exit(1);
				}
			}
			i++;
		}
	}
	std::cout << "404" << std::endl;
	return (1); // 404
}



/*
**	find_replace and ft_clean_path get request
**		on doit les deplacer ailleurs.
**	clean path get request est a ameliorer. c'est du brut force LOL
*/
std::string		find_replace( std::string to_search, std::string to_replace, std::string data )
{
	size_t pos = data.find(to_search);

	while (pos != std::string::npos)
	{
		data.replace(pos, to_search.size(), to_replace);
		pos = data.find(to_search, pos + to_replace.size());
	}
	return (data);
}
// URL ENCODING
std::string		HttpServer::ft_clean_path_get_request( std::string tmp )  // A TERMINER
{
	std::map<std::string, std::string> url_coding;

	// url_coding.insert(std::pair<std::string , std::string>("+", " ")); // space	
	url_coding.insert(std::pair<std::string , std::string>("%21", "!")); 	
	url_coding.insert(std::pair<std::string , std::string>("%22", "\"")); 	
	url_coding.insert(std::pair<std::string , std::string>("%23", "#")); 	
	url_coding.insert(std::pair<std::string , std::string>("%24", "$")); 
	url_coding.insert(std::pair<std::string , std::string>("%25", "%")); 	
	url_coding.insert(std::pair<std::string , std::string>("%26", "&")); 
	url_coding.insert(std::pair<std::string , std::string>("%27", "'")); 	
	url_coding.insert(std::pair<std::string , std::string>("%28", "(")); 	
	url_coding.insert(std::pair<std::string , std::string>("%29", ")")); 	
	url_coding.insert(std::pair<std::string , std::string>("%2A", "*"));
	url_coding.insert(std::pair<std::string , std::string>("%2B", "+"));
	url_coding.insert(std::pair<std::string , std::string>("%2C", ","));
	url_coding.insert(std::pair<std::string , std::string>("%2D", "-"));
	url_coding.insert(std::pair<std::string , std::string>("%2E", "."));
	url_coding.insert(std::pair<std::string , std::string>("%2F", "/"));
	url_coding.insert(std::pair<std::string , std::string>("%3A", ":"));
	url_coding.insert(std::pair<std::string , std::string>("%3B", ";"));
	url_coding.insert(std::pair<std::string , std::string>("%3C", "<"));
	url_coding.insert(std::pair<std::string , std::string>("%3D", "="));
	url_coding.insert(std::pair<std::string , std::string>("%3E", ">"));
	url_coding.insert(std::pair<std::string , std::string>("%3F", "?"));
	url_coding.insert(std::pair<std::string , std::string>("%40", "@"));
	url_coding.insert(std::pair<std::string , std::string>("%5B", "["));
	url_coding.insert(std::pair<std::string , std::string>("%5C", "\\"));
	url_coding.insert(std::pair<std::string , std::string>("%5D", "]"));
	url_coding.insert(std::pair<std::string , std::string>("%5E", "^"));
	url_coding.insert(std::pair<std::string , std::string>("%5F", "_"));
	url_coding.insert(std::pair<std::string , std::string>("%E2", "€"));
	url_coding.insert(std::pair<std::string , std::string>("%60", "`"));

	std::map<std::string, std::string>::iterator it_find = url_coding.begin();
	for (; it_find != url_coding.end(); it_find++)
	{
		tmp = find_replace(it_find->first, it_find->second, tmp);
	}
	return (tmp);

	return (tmp);
}