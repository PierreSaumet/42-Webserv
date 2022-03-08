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

#include "Headers/HttpServer.hpp"

/*
**	HttpServer Empty Constructor.
*/
HttpServer::HttpServer( void ) {

	std::cout << "Dans HttpServer constructor" << std::endl;
	std::cout << "trying to catch the data form Parsing. " << std::endl;
	// this->_data = new Parsing();
	return ;
}

/*
**	HttpServer Constructor with argument.
*/
HttpServer::HttpServer( std::string &configfile) {

	std::cout << "Constructor avec argument "<< std::endl;
	try {
		this->_data = new Parsing(configfile);
		if (this->_data->ft_get_error() == 1)
			return ;


		//std::cout << "display name of file = " << this->_data->_name_of_file << std::end;
		//std::cout << this->_data->display_all() << std::endl;

		std::cout << "name = " << this->_data->ft_get_name_of_file() << std::endl;

		this->_servers = this->_data->ft_get_servers();
		std::cout << "display un truc = " << this->_servers[0].host_server << std::endl;
		std::cout << "Ici" << std::endl;

		
		// this->ft_test();
		this->ft_create_servers();
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	
	
	return ;
}


/*
**	HttpServer Copy Constructor
*/
HttpServer::HttpServer( const HttpServer &copy ) {

	this->_data = copy._data;
	return ;
}

/*
**	HttpServer Destructor.
*/
HttpServer::~HttpServer( void ) {

	delete (this->_data);
	return ;
}

/*
**	HttpServer Overload Operator '='
*/
HttpServer			&HttpServer::operator=( const HttpServer &element ) {

	if (this != &element)
	{
		this->_data = element._data;
	}
	return (*this);
}



 /*
 **"HTTP/1.1 200 OK\r\n"
"Content-Type: text/html; charset=UTF-8\r\n\r\n"
 */
char response[] = "HTTP/1.1 200 OK\r\n"
"Content-Type: text/html; charset=UTF-8\r\n\r\n"
"<!DOCTYPE html><html><head><title>Bye-bye baby bye-bye</title>"
"<style>body { background-color: #111 }"
"h1 { font-size:4cm; text-align: center; color: black;"
" text-shadow: 0 0 2mm red}</style></head>"
"<body><h1>HELLO PIERRE</h1></body></html>\r\n";

#include <arpa/inet.h>
int					HttpServer::ft_create_servers( void ) {

	//	Cette fonction va creer les servers.
	try
	{
		int			i;

		i = 0;
		size_t nbr_server = this->_data->ft_get_nbr_servers();
		std::cout << "\n\nIL y a " << nbr_server << " servers" << std::endl;

		for (size_t i = 0; i < this->_data->ft_get_nbr_servers(); i++)
		{
			this->_http_servers.push_back(t_http_server());

			this->_http_servers[i].enable = 0;
			this->_http_servers[i].svr_addr.sin_family = AF_INET;
			this->_http_servers[i].svr_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
			this->_http_servers[i].svr_addr.sin_port = htons(this->_servers[i].port_server);
			
			this->_http_servers[i].sock = socket(AF_INET, SOCK_STREAM, 0);
			if (this->_http_servers[i].sock < 0)
				throw Error(1, "Error, 'creation of server', cannot create a socket.", 2);
			if (setsockopt(this->_http_servers[i].sock, SOL_SOCKET, SO_REUSEADDR, &this->_http_servers[i].enable, sizeof(int)) < 0)
				throw Error(2, "Error, 'creation of server', cannot set up the socket options.", 2);
			if (bind(this->_http_servers[i].sock, (struct sockaddr *) &this->_http_servers[i].svr_addr, sizeof(this->_http_servers[i].svr_addr)) < 0)
			{
				if (close(this->_http_servers[i].sock) < 0)
					throw Error(3, "Error, 'creation of server', cannot close socket.", 2);
				throw Error(4, "Error, 'creation of server'. cannot bind socket.", 2);
			}
			std::cout << GREEN << " Le server: "<< this->_servers[i].name_server << " tourne sur le port : " << this->_servers[i].port_server << CLEAR << std::endl;
			std::cout << std::endl;
		}
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}


	return (0);
}


/*
** function testing display pierre
*/
int		HttpServer::ft_test( void ) 
{
	std::cout << "dans test" << std::endl;

  int one = 1, client_fd;
  struct sockaddr_in svr_addr, cli_addr;
  socklen_t sin_len = sizeof(cli_addr);
 
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
    err(1, "can't open socket");
 
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));
 
  int port = 8080;
  svr_addr.sin_family = AF_INET;
  svr_addr.sin_addr.s_addr = INADDR_ANY;
  svr_addr.sin_port = htons(port);
 
  if (bind(sock, (struct sockaddr *) &svr_addr, sizeof(svr_addr)) == -1) {
    close(sock);
    err(1, "Can't bind");
  }
 
  listen(sock, 5);









  while (1) {
    client_fd = accept(sock, (struct sockaddr *) &cli_addr, &sin_len);
    printf("got connection\n");
 
    if (client_fd == -1) {
      perror("Can't accept");
      continue;
	sleep(10);
	exit(-1);
    }
 
    write(client_fd, response, sizeof(response) - 1); /*-1:'\0'*/
    close(client_fd);
  }


	return (0);
}





/*
**	
*/
int					HttpServer::int_signal = 0;
void				HttpServer::handler_signal( int num )
{
	int_signal = num;
	return ;
}