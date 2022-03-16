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
		// throw error
		std::cout << "ERREUR size du header trop grand return une erreur 413 entity too large" << std::endl;
	}
	else
	{
		std::cout << "Taille du header bon = " << size_header.size() << std::endl;
		// test savoir quelle method
		if (this->_header_requete.empty() == true)
		{
			std::cout << "ehader request epty " << std::endl;
			this->_header_requete.push_back(t_header_request());


			this->_header_requete[0].method = this->ft_check_methods(size_header);
			if (this->_header_requete[0].method.empty() == true)
			{
				std::cout << "PLS DANS LE HEADER" << std::endl;
			}
			std::cout << "On a la requete : " << this->_header_requete[0].method << std::endl;
		}
		else
		{
			std::cout << " keke _header_requete est pas vide ? " << std::endl;
		}

	}
	std::cout << "i = " << i << std::endl;
}

std::string		HttpServer::ft_check_methods( std::string header )
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