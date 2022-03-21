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

#include "Headers/HttpServer.hpp"


/*
**	DONC on parse la requete et on recupere les info
*/
void	HttpServer::ft_parser_requete( int len_msg, const char  *msg )
{
	std::cout << "Dans parser requete " << std::endl;

	std::string request_http(msg);
	// std::stringstream ss;
	// ss << msg;
	// ss >> request_http;
	std::cout << "msg = " << request_http << std::endl;
	std::cout << "len = " << request_http.length() << std::endl;
	std::cout << "len = " << len_msg << std::endl;

	
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

	std::string size_header(request_http.begin(), it_end_request);
	if (size_header.size() > 8000)		// server apache 8000 max
	{
		// throw error ou affiche la page 413 ?
		// throw Error(10, "Error, in recieved header, the header size is greater than 8000.", 2);
		std::cout << "ERREUR size du header trop grand return une erreur 413 entity too large" << std::endl;
	}
	else
	{
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

			// EUH a quoi ca sert ca ?
			if (this->_header_requete[0].path == "/page2.html")
			{
				std::cout << "YES" << std::endl;
				exit(EXIT_SUCCESS);
			}

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
			std::cout << " keke _header_requete est pas vide ? " << std::endl;
		}

	}
	std::cout << "i = " << i << std::endl;
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
				std::cout << "this->_servers[i].host_server =" << this->_servers[i].host_server <<std::endl;
				std::cout << "tmp =" << tmp << std::endl;
				std::cout << "i" << std::endl;
				if (tmp.compare(0, 9, this->_servers[i].host_server) == 0)
				{
					std::cout << "ILS SONT EGAUX les host" << std::endl;

					std::stringstream ss;
					ss << this->_servers[i].port_server;
					std::string port;
					ss >> port;
					if (tmp.compare(10, 2, port) == 0)
					{
						std::cout<< "Ils sont egaux les port " << std::endl;
						return (tmp);
					}
					else
						throw Error(666, "Erreur test lol, ", 666);
				}
				else
					std::cout << "les host ne sont pas egaux" << std::endl;
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