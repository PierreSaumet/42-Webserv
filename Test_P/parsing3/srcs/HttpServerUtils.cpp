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