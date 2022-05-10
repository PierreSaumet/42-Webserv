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






size_t			HttpServer::ft_check_access_location( std::string path )
{
	std::cout << GREEN << "Dans ft_check_access location " << CLEAR << std::endl;
	std::cout << "path = " << path << std::endl;

	return (0);
}


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
				std::string 	tmp = this->_servers[this->_num_serv].root_server;

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
	size_t	pos_end = header.find("\r\n", pos);
	if (pos_end == std::string::npos)
	{
		std::string tmp(header, pos + 8, header.size() - pos - 8);
		return (tmp);
	}
	else
	{
		std::string tmp(header, pos + 8, pos_end - (pos + 8));
		return (tmp); 
	}
}

// surement useless du coup
std::string		HttpServer::ft_check_referer( std::string request_http )
{
	size_t pos = request_http.find("Referer: ");
	if (pos == std::string::npos)
		return ("");
	size_t pos_end = request_http.find("\r\n", pos);
	if (pos_end == std::string::npos)
		return ("");
	std::string tmp(request_http, pos + 9, pos_end - (pos + 9));
	
	pos_end = tmp.find_last_of("/");
	tmp.erase(0, pos_end);
	
	return (tmp);
}


int				HttpServer::ft_check_method_allowed_header( std::string method )
{
	// std::cout << GREEN << "\tDans ft_check_method_allowed_header " << CLEAR << std::endl;
		
	if (this->_servers[this->_num_serv].nbr_location > 0)
	{
		std::vector<std::string> all_location; // container qui va avoir le nom de tous les locations
		for (std::vector<t_location>::iterator it = this->_servers[this->_num_serv].location.begin(); it != this->_servers[this->_num_serv].location.end(); it++)
			all_location.push_back(it->name_location);
		
		std::sort(all_location.begin(), all_location.end(), std::greater<std::string>()); // on trie les noms des locations
		
		for (std::vector<std::string>::iterator it = all_location.begin(); it != all_location.end(); ++it)
		{
			size_t pos_slash = 0;
			if (*it == "/")
				pos_slash = 1; // on cherche le deuxieme / pour avoir le premier dossier de la requete
			else
				pos_slash = this->_header_requete[0].path.find("/", 1); 
			if (this->_header_requete[0].path.compare(0, pos_slash , *it) == 0)  // on a un dossier location qui correspond
			{
				size_t count = 0;
				for (std::vector<t_location>::iterator it_loc = this->_servers[this->_num_serv].location.begin(); it_loc != this->_servers[this->_num_serv].location.end(); it_loc++)
				{
					if (it_loc->name_location == *it)
					{
						this->_num_loc = count;
						for (std::vector<std::string>::iterator it_method = it_loc->methods_location.begin(); it_method != it_loc->methods_location.end(); it_method++)
						{
							if (*it_method == method) // la method est autorisee
								return (0);
						}
						return (1); // pas de method on return 1
					}
					count++;
				}
			}
		}
	}
	std::vector<std::string>::iterator  it_b = this->_servers[this->_num_serv].methods_server.begin();
	for (; it_b != this->_servers[this->_num_serv].methods_server.end(); it_b++)
	{
		if (*it_b == method)
			return (0);		// oui ca marche
	}
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
	size_t		pos = header.find_first_of("/", 0);
	size_t		pos_http = header.find("HTTP/");

	if (pos > pos_http)
		return ("");
	std::string 	tmp(header, pos, pos_http - pos - 1);
	// on setup aussi le request_uri pour le cgi
	this->_header_requete[0].request_uri = tmp;
	// on setup aussi scriptfilename pour le cgi
	size_t 			pos_tmp = tmp.find("?");
	std::string 	tmp2(tmp, 0, pos_tmp);
	this->_header_requete[0].script_file_name = tmp2;
	return (tmp);

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
	std::cout << GREEN << "Dans ft_check_host_header " << CLEAR << std::endl;
	size_t 		pos = header.find("Host: ");
	size_t 		pos_end = header.find("\r\n", pos);
	std::string host(header, pos + 6, pos_end - (pos + 6));

	if (host == this->_servers[this->_num_serv].name_server)
		return (host);	
	std::stringstream ss;
	ss << this->_servers[this->_num_serv].port_server;
	ss >> host;
	host.insert(0, ":");
	host.insert(0, this->_servers[this->_num_serv].host_server);
	return (host);
}

std::string		HttpServer::ft_check_content_type( std::string request_http )
{
	size_t pos = request_http.find("Content-Type: ");
	if (pos == std::string::npos)
	{
		pos = request_http.find("Content-type: ");
		if (pos == std::string::npos)
		{
			return ("");
		}
	}
	size_t pos_end = request_http.find("\r\n", pos);
	std::string tmp(request_http, pos + 14, pos_end - (pos + 14));
	return (tmp);
}

std::string		HttpServer::ft_check_content_length( std::string request_http )
{
	size_t pos = request_http.find("Content-Length: ");
	if (pos == std::string::npos)
	{
		pos = request_http.find("Content-length: ");
		if (pos == std::string::npos)
		{
			// std::cout << "Erreur dans POST le header n'a pas Content-Length" << std::endl;
			// std::cout << "requet t= " << request_http << std::endl;
			// std::cout << "what the fuck" << std::endl;
			return ("");
		}
	}
	size_t pos_end = request_http.find("\r\n", pos);
	std::string tmp(request_http, pos + 16, pos_end - (pos + 16));
		
	return (tmp);
}

std::string		HttpServer::ft_check_body_post( std::string request_http )
{
	if (this->_recv_complete.chunked == true)
		return ("nothing");
	size_t pos = request_http.find("\r\n\r\n"); // on cherche la fin
	if (pos == std::string::npos)
	{
		std::cout << "Erreur dans POST le header n'a pas /r/n/r/n" << std::endl;
		exit(1);
	}
	else
	{
		size_t pos_end = request_http.find("\r\n", pos);
		std::string tmp(request_http, pos + 4, pos_end - (pos + 4));
		// std::cout << "tmp = -" << tmp << "-" << std::endl;

		// on compare avec content_lenght
		//	 on a une erreur si j'envoie un fichier genre mp3		
		if ((long)tmp.size() == std::strtol(this->_header_requete[0].content_length.c_str(), NULL, 10))
			return (tmp);
		else if (std::strtol(this->_header_requete[0].content_length.c_str(), NULL, 10) < (long)tmp.size())	// pour le cas de mp3
		{
			std::cout << "tmp.size() = " << tmp.size() << std::endl;
			std::cout << "content_length = " << this->_header_requete[0].content_length << std::endl;
			std::cout << "ERROR size body differe de content length" << std::endl;
			if (this->_recv_complete.chunked == true)		// chunked
				return (tmp);
			exit(1);
		}
		else
		{
			std::cout << "yolo" << std::endl;
			return (tmp);
		}
		exit(1);
		return (tmp); 
	}
}
