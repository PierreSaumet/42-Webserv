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


/*
**	Functions a deplacer dans un autre fichier
*/
void	HttpServer::ft_clean_socket_clients( void )
{
	std::vector<t_client_socket>::iterator it_b;
	std::vector<t_client_socket>::iterator it_e = this->_all_client_socket.end();

	for (it_b = this->_all_client_socket.begin(); it_b != it_e; it_b++)
	{
		FD_CLR(it_b->client_socket, &this->_write_fs);
		FD_CLR(it_b->client_socket, &this->_read_fs);
		close(it_b->client_socket);
	}
	return ;
}

void	HttpServer::ft_clean_socket_servers( void )
{
	std::vector<t_http_server>::iterator it_b;
	std::vector<t_http_server>::iterator it_e = this->_http_servers.end();

	for (it_b = this->_http_servers.begin(); it_b != it_e; it_b++)
		close(it_b->sock);
	return ;
}


std::string  HttpServer::get_string( size_t pos_end, std::string body)
{
	std::string tmp = "";

	tmp.append(body, 0, pos_end);
	return (tmp);
}

long long int  HttpServer::convert_hex_to_dec( std::string const string )
{
	return (std::strtol(string.c_str(), NULL, 16));
}

std::string HttpServer::ft_decode_chunked( std::string body)
{
	// std::cout << GREEN << "Dans ft_decode chunked " << CLEAR << std::endl;

	std::string 		total_body = "";
	std::string 		tmp_2 = "";
	
	body.erase(0, 4);
	while (body != "0\r\n\r\n")
	{
		size_t 			pos_end_number = 0;
		size_t 			pos_end_string = 0;

		pos_end_number = body.find("\r\n");
		
		std::string 	tmp = get_string(pos_end_number, body);
		long long int 	length = convert_hex_to_dec(tmp);

		body.erase(0, tmp.size() + 2);
		pos_end_string = body.find("\r\n");
		if (body.compare(pos_end_string, pos_end_string + 4, "\r\n\r\n") == 0)
			pos_end_string += 2;
		tmp_2 = get_string(pos_end_string, body);
		if (tmp_2.size() != (size_t)length)
			return ("");

		total_body.append(tmp_2);
		body.erase(0, tmp_2.size() + 2);
		tmp_2.clear();
	}

	if (body == "0\r\n\r\n")
	{
		// std::cout << "on a termine on retourne total_bdy"  << std::endl;
		return (total_body);
	}
	return ("");
}

size_t HttpServer::ft_do_error( size_t num_error )
{
	this->_header_requete[0].error = true;
	this->_header_requete[0].num_error = num_error; 
	this->ft_setup_error_header();
	return (1);
}