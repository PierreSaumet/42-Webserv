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
#include "../Headers/CGI_exec.hpp"

/*
**	HttpServer Constructor with argument.
*/
HttpServer::HttpServer( std::string &configfile) : _http_servers(), _max_connections(1000), _read_fs(), _write_fs(),
_all_client_socket(), _return_select(), _servers(), _response_to_send(), _recv_complete(), _tmp_buffer(), _num_serv(),
_num_loc(), _DATA(), _still_to_send(), _total_send()
{
	this->_cgi = new Cgi_exec();
	try {
		this->_data = new Parsing(configfile);
		
		if (this->_data->ft_get_error() == 1)
			return ;
		this->_servers = this->_data->ft_get_servers();
		if (this->ft_create_servers() == 1)
			return ;
		this->ft_main_loop();
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	return ;
}


/*
**	HttpServer Destructor.
*/
HttpServer::~HttpServer( void ) {

	delete this->_cgi;
	if (this->_data)
		delete (this->_data);
	return ;
}

/*
**	int		HttpServer::ft_create_servers( void )
**		This function will create the servers. It will use fcntl, setsockopt, and it will bind the server
**	with the data given from the configuration file.
**
**	If an error occurs it will throw it.
*/
int					HttpServer::ft_create_servers( void )
{
	try
	{
		int			i;

		i = 0;
		for (size_t i = 0; i < this->_data->ft_get_nbr_servers(); i++)
		{
			this->_http_servers.push_back(t_http_server());
			this->_http_servers[i].num = i;
			this->_http_servers[i].enable = 1;
			this->_http_servers[i].sock = socket(AF_INET, SOCK_STREAM, 0);

			if (this->_http_servers[i].sock < 0)
				throw Error(0, "Error, 'creation of server', cannot create a socket.", 2);
			
			if (fcntl(this->_http_servers[i].sock, F_SETFL, O_NONBLOCK) < 0)
				throw Error(666, "Error, 'creation of server', cannot fcntl a socker server.", 2);

			if (setsockopt(this->_http_servers[i].sock, SOL_SOCKET, SO_REUSEADDR, &this->_http_servers[i].enable, sizeof(this->_http_servers[i].enable)) < 0)
			{
				if (close(this->_http_servers[i].sock) < 0)
				{
					close(this->_http_servers[i].sock);
					throw Error(1, "Error, 'creation of server', cannot close socket.", 2);
				}
				throw Error(2, "Error, 'creation of server', cannot set up the socket options.", 2);
			}

			memset((char *)&this->_http_servers[i].svr_addr, 0, sizeof(this->_http_servers[i].svr_addr));

			this->_http_servers[i].svr_addr.sin_family = AF_INET;
			if (this->_servers[i].host_server.empty() == true)
				this->_http_servers[i].svr_addr.sin_addr.s_addr = INADDR_ANY;
			else
				this->_http_servers[i].svr_addr.sin_addr.s_addr = inet_addr(this->_servers[i].host_server.c_str());
			this->_http_servers[i].svr_addr.sin_port = htons(this->_servers[i].port_server);
			size_t 		y = 0;
			size_t		binded = 0;
			
			while (y < i)
			{
				if (this->_servers[i].port_server == this->_servers[y].port_server && this->_servers[y].host_server == this->_servers[i].host_server)
					binded = 1;
				if (this->_servers[i].host_server.empty() == true)
				{
					if (this->_servers[y].host_server == "127.0.0.1")
						if (this->_servers[i].port_server == this->_servers[y].port_server)
							binded = 1;
				}
				y++;
			}

			if (binded == 0)
			{
				if (bind(this->_http_servers[i].sock, (struct sockaddr *) &this->_http_servers[i].svr_addr, sizeof(this->_http_servers[i].svr_addr)) < 0)
				{
					close(this->_http_servers[i].sock);
					throw Error(4, "Error, 'creation of server', cannot bind socket.", 2);
				}
				if (listen(this->_http_servers[i].sock, this->_max_connections) < 0)
				{
					if (close(this->_http_servers[i].sock) < 0)
						throw Error(1, "Error, 'creation of server', cannot close socket.", 2);
					throw Error(5, "Error, 'creation of server', cannot listen.", 2);
				}
				this->_http_servers[i].binded = true;
				std::cout << GREEN << "The -"<< this->_servers[i].name_server << "- is running with : -" << this->_servers[i].host_server << "- on the port: -" << this->_servers[i].port_server << "-" << CLEAR << std::endl;
				std::cout << std::endl;
			}
		}
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}


/*
**	void	HttpServer::ft_handle_connections( void )
**		This functions setup the server socket with a fd for reading and setup the client socket with
**	a reading and writing socket.
*/
void	HttpServer::ft_handle_connections( void )
{
	FD_ZERO(&this->_read_fs);
	FD_ZERO(&this->_write_fs);

	std::vector<t_http_server>::iterator it_b = this->_http_servers.begin();
	std::vector<t_http_server>::iterator it_e = this->_http_servers.end();

	for (it_b = this->_http_servers.begin(); it_b != it_e; it_b++)
		FD_SET(it_b->sock, &this->_read_fs);

	std::vector<t_client_socket>::iterator it_b_client = this->_all_client_socket.begin();
	std::vector<t_client_socket>::iterator it_e_client = this->_all_client_socket.end();
	for (it_b_client = this->_all_client_socket.begin(); it_b_client != it_e_client; it_b_client++)
	{
		FD_SET(it_b_client->client_socket, &this->_read_fs);
		FD_SET(it_b_client->client_socket, &this->_write_fs);
	}
	
	if ((this->_return_select = select(FD_SETSIZE, &this->_read_fs, &this->_write_fs, NULL, NULL)) < 0 && int_signal == 0)
	{
		
		this->ft_clean_socket_clients();
		this->ft_clean_socket_servers();
		throw Error(5, "Error, 'main loop server', cannot select().", 2);
	}
	return ;
}

/*
**	void	HttpServer::ft_check_isset( void )
**		This function will check if a server can accept a client's socket.
**	If yes, put it in 'this->_all_clients_sockets.
*/
void		HttpServer::ft_check_isset( void )
{
	std::vector<t_http_server>::iterator it_b = this->_http_servers.begin();
	std::vector<t_http_server>::iterator it_e = this->_http_servers.end();

	for (; it_b != it_e; it_b++)
	{
		int		socket_new_client;
		struct sockaddr_in	addr_new_client;
		socklen_t	size_addr_new_client = sizeof(addr_new_client);
		memset((char*)&addr_new_client, 0, (int)size_addr_new_client);


		if (it_b->binded == true)
		{
			if (FD_ISSET(it_b->sock, &this->_read_fs))
			{
				socket_new_client = accept(it_b->sock, (struct sockaddr *)&addr_new_client, &size_addr_new_client);		
				// std::cerr << strerror(errno) << std::endl;
				if (socket_new_client < 0 && int_signal == 0)
					throw Error(6, "Error, 'main loop server', server cannot accept() a client, maybe DDOS.", 2);
				else
				{
					
					if (fcntl(socket_new_client, F_SETFL, O_NONBLOCK) < 0)
						throw Error(7, "Error, 'in main loop', server cannot change FD client with fcntl().", 2);
					else
					{
						t_client_socket new_client;

						new_client.client_socket = socket_new_client;
						new_client.client_addr = addr_new_client;
						new_client.server_socket = it_b->sock;
					
						new_client.num = it_b->num;
						new_client.recv = false;
						new_client.request = t_header_request();
						this->_all_client_socket.push_back(new_client);
					}		
				}
			}
		}
	}
}

/*
**	HttpServer::ft_continue_send( std::vector<t_client_socket>::iterator it_client )
**		This function will call ft_setup_response_to_send() and then use send() function, to
**	reply to the client.
**	It will mostly check if there is some data to send to the client or not.
**	If send return -1, we should disconnect the client.
*/
void 			HttpServer::ft_continue_send( std::vector<t_client_socket>::iterator it_client )
{
	long long 	ret = 0;

	_response_to_send = ft_setup_response_to_send(&it_client->request);
	if ((ret = send(it_client->client_socket, _response_to_send.c_str(),  _response_to_send.size(), 0)) < 0)
	{
		if (_response_to_send.empty() == false)
			_response_to_send.erase(_response_to_send.begin(), _response_to_send.end());
		close(it_client->client_socket);
		it_client = this->_all_client_socket.erase(it_client);
		_DATA = 0;
		it_client->recv = false;
		return ;
	}
	else
	{
		if (ret == 0)
		{
			if (_response_to_send.empty() == false)
				_response_to_send.erase(_response_to_send.begin(), _response_to_send.end());
			_DATA = 0;
			_total_send = 0;
			_still_to_send = 0;
			it_client->recv = false;
			return ;
		}
		if ((unsigned long)ret != _response_to_send.size())
		{
			_total_send = _response_to_send.size();
			_still_to_send = _response_to_send.size() - ret;
			it_client->recv = true;
			return ;
		}		
	}
	return ;
}

/*
**	int 	HttpServer::ft_write( void )
*		This function will check if the client need to recieve something or if there is 
**	still data to send.
**	If yes, use ft_continue_send(); 
*/
int 		HttpServer::ft_write( void )
{
	std::vector<t_client_socket>::iterator it_b_client = this->_all_client_socket.begin();
	std::vector<t_client_socket>::iterator it_e_client = this->_all_client_socket.end();

	for (; it_b_client != it_e_client; it_b_client++)
	{
		if (FD_ISSET(it_b_client->client_socket, &this->_write_fs))
		{
			if (it_b_client->recv == true || _still_to_send > 0)
			{
				this->ft_continue_send(it_b_client);
				it_b_client->recv = false;
				std::cout <<  BLUE << "We have send data." << CLEAR << std::endl;
			}
		}
	}
	return (0);
}

/*
**	size_t	HttpServer::ft_check_recv_complete( std::string tt_buffer )
**		This function is mostly used for the POST request. It will check
**	what kind of data is send and if the recieved all the data.
**	It return 1 if everything is good, otherwise return 0.
*/
size_t		HttpServer::ft_check_recv_complete( std::string tt_buffer )
{
	size_t pos = 0;
	if (tt_buffer.compare(0, 5, "POST ") == 0)
	{
		if (this->_recv_complete.chunked == false)
		{
			pos = tt_buffer.find("Transfer-Encoding: chunked");
			if (pos == std::string::npos)
			{
				pos = tt_buffer.find("Content-Type: application/x-www-form-urlencoded\r\n");	
				if (pos == std::string::npos)
				{
					if (this->_recv_complete.boundary.empty() == true)
					{
						pos = tt_buffer.find("Content-Type: multipart/form-data;");
						if (pos == std::string::npos)
							return (1);
						else
						{
							size_t		pos_end = tt_buffer.find("\r\n", pos);
							std::string tmp(tt_buffer, pos + 34, pos_end - pos - 34);
							pos = tmp.find("boundary=");
							if (pos == std::string::npos)
								return (1);
							else
							{
								if (tmp[0] == ' ')
									tmp.erase(0, 1);
								tmp.erase(0, 9);
								this->_recv_complete.boundary = tmp;
								this->_recv_complete.boundary.append("--");
								if (tt_buffer.find(this->_recv_complete.boundary) != std::string::npos)
									return (1);
								return (0);
							}
						}
					}
					else
					{
						if (tt_buffer.find(this->_recv_complete.boundary) != std::string::npos)
							return (1);
						return (0);
					}		
				}
				else
					return (1);
			}
			else
			{
				this->_recv_complete.chunked = true;
				pos = tt_buffer.find("0\r\n\r\n");
				if (pos == std::string::npos)
					return (0);
				else
					return (1);
			}
		}
		else
		{
			if (tt_buffer.find("\r\n\r\n") == tt_buffer.size() - 4)
				return (1);
			std::string tmp(tt_buffer.end() - 5, tt_buffer.end());
			if (tmp == "0\r\n\r\n")
				return (1);
			return (0);	
		}
		return (1);
	}
	else if (tt_buffer.compare(0, 4, "GET ") == 0)
		return (1);
	else if (tt_buffer.compare(0, 7, "DELETE ") == 0)
		return (1);
	else
		return (1);
	return (0);
}

/*
**	int		HttpServer::ft_reading( void )
**		This function is used to read data from the client.
**	If all data are recieved it will proceed to ft_parser_requete.
**
**	If an error occurs with recv return, disconnect the client.	
*/
int		HttpServer::ft_reading( void )
{
	std::vector<t_client_socket>::iterator it_b_client = this->_all_client_socket.begin();
	std::vector<t_client_socket>::iterator it_e_client = this->_all_client_socket.end();

	char buffer[10240 + 1];
	std::string	tt_buffer;

	for (; it_b_client != it_e_client; it_b_client++)
	{
		if (FD_ISSET(it_b_client->client_socket, &this->_read_fs))
		{
			memset((char *)buffer, 0, 10240 + 1);
			int		request_length = 0;
			if ((request_length = recv(it_b_client->client_socket, buffer, sizeof(buffer), 0)) <= 0)
			{ // Useless but needed it for the correction
				if (request_length == 0) 
				{				
					FD_CLR(it_b_client->client_socket, &this->_read_fs);
					FD_CLR(it_b_client->client_socket, &this->_write_fs);
					close(it_b_client->client_socket);
					it_b_client = this->_all_client_socket.erase(it_b_client);
					it_b_client->recv = false;
					break ;
				}
				FD_CLR(it_b_client->client_socket, &this->_read_fs);
				FD_CLR(it_b_client->client_socket, &this->_write_fs);
				close(it_b_client->client_socket);
				it_b_client = this->_all_client_socket.erase(it_b_client);
				it_b_client->recv = false;
				break ;
			}
			_tmp_buffer.append(buffer, request_length);

			if (ft_check_recv_complete(_tmp_buffer) == 1)
			{
				std::cout <<  BLUE << "We recieved data." << CLEAR << std::endl;
				_still_to_send = 0;
				it_b_client->request = this->ft_parser_requete(it_b_client->num ,_tmp_buffer.size() , _tmp_buffer, it_b_client->request);
				it_b_client->recv = true;
				this->_header_requete.erase(this->_header_requete.begin(), this->_header_requete.end());
				this->_recv_complete.chunked = false;
				this->_recv_complete.boundary.clear();
				_tmp_buffer.clear();
				_DATA = 1;
				return (0);
			}
			else
				_DATA = 0;	
		}
	}
	return (0);
}

/*
**	int	HttpServer::ft_main_loop( void )
**		This function is the main loop
*/
int		HttpServer::ft_main_loop( void )
{
	this->_recv_complete.chunked = false;
	_DATA = 0;
	_still_to_send = 0;
	while (int_signal == 0)
	{
		try {
			
			this->ft_handle_connections();
			if (this->_return_select != 0 && int_signal == 0)
			{
				this->ft_check_isset();
				this->ft_write();
				this->ft_reading();
			}		    
		}
		catch (std::exception &e)
		{
			std::cerr << e.what() << std::endl;
			break ;
		}
	}
	this->ft_clean_socket_clients();
	this->ft_clean_socket_servers();
	return (0);
}