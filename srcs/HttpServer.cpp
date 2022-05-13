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

// Ce fichier contient que la forme complien de HTTPSERVER et les fonctions necessaires pour initialiser le server

/*
**	HttpServer Empty Constructor.
*/
HttpServer::HttpServer( void ) : _max_connections(1000)
{
	return ;
}

/*
**	HttpServer Constructor with argument.
*/
HttpServer::HttpServer( std::string &configfile) : _max_connections(1000) { // A FAIRE , initialiser toutes les variables


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
**	HttpServer Copy Constructor
*/
HttpServer::HttpServer( const HttpServer &copy ): _cgi(copy._cgi), _data(copy._data), _http_servers(copy._http_servers), _max_connections(copy._max_connections),
	_read_fs(copy._read_fs), _write_fs(copy._write_fs), _all_client_socket(copy._all_client_socket), _return_select(copy._return_select),
	_servers(copy._servers), _response_to_send(copy._response_to_send), _recv_complete(copy._recv_complete), _tmp_buffer(copy._tmp_buffer),
	_header_requete(copy._header_requete), _num_serv(copy._num_serv), _num_loc(copy._num_loc)
{ 
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
**	HttpServer Overload Operator '='
*/
HttpServer			&HttpServer::operator=( const HttpServer &element ) { // A FAIRE , init toutes les variables

	if (this != &element)
	{
		this->_data = element._data;
	}
	return (*this);
}

int					HttpServer::ft_create_servers( void )		// A FAIRE, changer les erreurs et supprimer les std::cout ou std::cerr
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
			/// test 127.0.0.2
			// this->_http_servers[i].svr_addr.sin_addr.s_addr = INADDR_ANY;
			if (this->_servers[i].host_server.empty() == true)
				this->_http_servers[i].svr_addr.sin_addr.s_addr = INADDR_ANY;
			else
				this->_http_servers[i].svr_addr.sin_addr.s_addr = inet_addr(this->_servers[i].host_server.c_str());
			
			this->_http_servers[i].svr_addr.sin_port = htons(this->_servers[i].port_server);



			// test si plusieurs servers on la meme addresse
			size_t y= 0;
			size_t binded = 0;
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
			// std::cout << "le server = " << this->_servers[i].name_server << " son host = " << this->_servers[y].host_server << "est binded ? = " << binded << std::endl;
			if (binded == 0)
			{
				if (bind(this->_http_servers[i].sock, (struct sockaddr *) &this->_http_servers[i].svr_addr, sizeof(this->_http_servers[i].svr_addr)) < 0)
				{
					// std::cerr << "error bind = " << strerror(errno) << std::endl;
					close(this->_http_servers[i].sock);
					throw Error(4, "Error, 'creation of server', cannot bind socket.", 2);
				}
				// std::cout << "SERVER sock bind = " << this->_http_servers[i].sock << std::endl;
				if (listen(this->_http_servers[i].sock, this->_max_connections) < 0)
				{
					if (close(this->_http_servers[i].sock) < 0)
					{
						close(this->_http_servers[i].sock);
						throw Error(1, "Error, 'creation of server', cannot close socket.", 2);
					}
					throw Error(5, "Error, 'creation of server', cannot listen.", 2);
				}
				// this->_servers[i].binded = true;
				this->_http_servers[i].binded = true;
				std::cout << GREEN << "The -"<< this->_servers[i].name_server << "- is running with : -" << this->_servers[i].host_server << "- on the port: -" << this->_servers[i].port_server << "-" << CLEAR << std::endl;
				std::cout << std::endl;
			}
			// else
			// 	this->_http_servers.pop_back();
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
**	A DEPLACER DANS UN AUTRE FICHIERs
*/
int					HttpServer::int_signal = 0;
void				HttpServer::handler_signal( int num )
{
	int_signal = num;
	return ;
}

/*
**	This functions setup the server socket with a fd for reading and setup the client socket with
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


void		HttpServer::ft_check_isset( void )	// A FAIRE, supprimer les std::cout
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

void 			HttpServer::ft_continue_send( std::vector<t_client_socket>::iterator it_client )
{
	long long 	ret = 0;

	_response_to_send = ft_setup_response_to_send(&it_client->request);
	// std::cout << "response  = " << _response_to_send << std::endl;
	if ((ret = send(it_client->client_socket, _response_to_send.c_str(),  _response_to_send.size(), 0)) < 0)
	{
		// std::cout << "SEND retourn -1 erreur " << std::endl;
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
			// std::cout << "send a envoyer 100%" << std::endl;
			if (_response_to_send.empty() == false)
				_response_to_send.erase(_response_to_send.begin(), _response_to_send.end());
			_DATA = 0;
			total_send = 0;
			still_to_send = 0;
			it_client->recv = false;
			return ;
		}
		// std::cout << "ret  = " << ret << " et response to send = " << _response_to_send.size() << std::endl;
		if ((unsigned long)ret != _response_to_send.size())
		{
			// std::cout << RED << "on a pas tout envoyer exit " << CLEAR << std::endl;
			total_send = _response_to_send.size();
			still_to_send = _response_to_send.size() - ret;
			// std::cout << "IL RESTE A ENVOYER : " << still_to_send << " sur : " << total_send << std::endl;
			it_client->recv = true;
			return ;
		}		
	}
	return ;
}
int 		HttpServer::ft_write( void )
{
	std::vector<t_client_socket>::iterator it_b_client = this->_all_client_socket.begin();
	std::vector<t_client_socket>::iterator it_e_client = this->_all_client_socket.end();

	for (; it_b_client != it_e_client; it_b_client++)
	{
		if (FD_ISSET(it_b_client->client_socket, &this->_write_fs))
		{
			// std::cout << "le client est pret a ecrire ? = " << it_b_client->client_socket << std::endl;
			if (it_b_client->recv == true || still_to_send > 0)
			{
					
				// std::cout << "it_b_client->recv = " << it_b_client->recv << std::endl;
				// sleep(2);
				//if (_DATA == 0) // uselss?
				//	break ;
				// std::cout <<  BLUE << "C'est le client : " << it_b_client->client_socket << " qui va recevoir les donnes " << CLEAR << std::endl;
				this->ft_continue_send(it_b_client);
				it_b_client->recv = false;
				std::cout <<  BLUE << "We have send data." << CLEAR << std::endl;
				// std::cerr << strerror(errno) << std::endl;
				// std::cout << "\n\n";
			}
		}
	}
	return (0);
}


size_t		HttpServer::ft_check_recv_complete( std::string tt_buffer )
{
	// std::cout << GREEN << "Dans ft_check_recv_complete : " << CLEAR << std::endl;

	// std::cout << "BUFFER = " << tt_buffer << "\n\n\n\n" << std::endl;
	// sleep(3);

	size_t pos = 0;
	if (tt_buffer.compare(0, 5, "POST ") == 0)
	{
		// std::cout << "la " << std::endl;
		if (this->_recv_complete.chunked == false)	//n'a pas ete setup ou pas de chunked
		{
			// std::cout << "la 2" << std::endl;
			pos = tt_buffer.find("Transfer-Encoding: chunked");
			if (pos == std::string::npos)	// Les data ne sont pas Chunked
			{
				// std::cout << "la 3" << std::endl;
				pos = tt_buffer.find("Content-Type: application/x-www-form-urlencoded\r\n");	
				if (pos == std::string::npos)		// envoie formulaire via multipart
				{
					// std::cout << "On regarde les boundarys" << std::endl;
					if (this->_recv_complete.boundary.empty() == true)
					{
						// std::cout << "la 5" << std::endl;
						pos = tt_buffer.find("Content-Type: multipart/form-data;");
						if (pos == std::string::npos)
							return (1);
						else
						{
							// std::cout << "la 6" << std::endl;
							size_t pos_end = tt_buffer.find("\r\n", pos);
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
						{
							// std::cout << "On a trouve la boundary de fin" << std::endl;
							return (1);
						}
						return (0);
					}		
				}
				else				// envoie de formulaire par default
				{
					pos = tt_buffer.find("Content-Length: ");
					if (pos == std::string::npos)
					{
						// std::cout << "Erreur requete post content-lenght" << std::endl;
						return (1);
					}
					else
					{
						// std::cout << "Ok good envoie de data via un formulaire" << std::endl;
						return (1);
					}
				}
			}
			else
			{
				// std::cout << " On a du data chunked" << std::endl;
				// sleep(2);
				this->_recv_complete.chunked = true;
				pos = tt_buffer.find("0\r\n\r\n");				// On cherche la fin du transfer encoding
				if (pos == std::string::npos)					// On n'a pas la fin, on continue de recuperer de la data
				{
					// std::cout << "On n'a pas la fin du chunked" << std::endl;
					return (0);
				}
				else
					return (1);								// On a toute la requete
			}
		}
		else
		{
			// std::cout << "this->_recv_complete.chunked = " << this->_recv_complete.chunked <<std::endl;
			// std::cout << "tt_buffer " << tt_buffer << std::endl;
			// std::cout << "ici" <<std::endl;
			// std::cout << "size tt_buffer " << tt_buffer.size() << std::endl;
			// std::cout << "pos end header " << tt_buffer.find("\r\n\r\n");
			if (tt_buffer.find("\r\n\r\n") == tt_buffer.size() - 4)
			{
				return (1);
			}
			std::string tmp(tt_buffer.end() - 5, tt_buffer.end());
			if (tmp == "0\r\n\r\n")
			{
				// std::cout << "Les toutes les donnees chunked sont transmises." << std::endl;
				return (1);
			}
			// std::cout << "return 0" << std::endl;
			return (0);	
		}
		return (1);
	}
	else if (tt_buffer.compare(0, 4, "GET ") == 0)
	{
		// std::cout << "YES GET " << std::endl;
		pos = tt_buffer.find("\r\n\r\n");
		if (pos == std::string::npos)
		{
			// std::cout << "ERREUR GET NE TROUVE PAS de header" << std::endl;
			return (1);
		}
		return (1);
	}
	else if (tt_buffer.compare(0, 7, "DELETE ") == 0)
	{
		// std::cout << "YES DELETE" << std::endl;
		return (1);
	}
	else
	{
		// std::cout << "NI GET NI POST NI DELETE doit sortir une erreur" << std::endl;
		return (1);
	}
	return (0);
}

/*
*	**	Testing writing
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
			int request_length;
			// std::cout <<  BLUE << "Le client : " << it_b_client->client_socket << " nous envoie des donnees" << CLEAR << std::endl;
			if ((request_length = recv(it_b_client->client_socket, buffer, sizeof(buffer), 0)) <= 0)
			{
				// A CHANGER
				
				// std::cout << RED << "recv return 0 ou -1 donc on ferme le client: " <<  it_b_client->client_socket << " recv =" << request_length <<  CLEAR << std::endl;
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
				std::cout << RED << "Client disconnect" << CLEAR << std::endl;
				break ;
				
				// }
			}
			_tmp_buffer.append(buffer, request_length);

			if (ft_check_recv_complete(_tmp_buffer) == 1)
			{
				std::cout <<  BLUE << "We recieved data." << CLEAR << std::endl;
				// std::cerr << strerror(errno) << std::endl;
				still_to_send = 0;

				// std::cout << "chunk = " << this->_recv_complete.chunked << std::endl;
				
				// std::cout << " avant it_b_client request path = " << it_b_client->request.path << std::endl;
			
				// std::cout << "EXPECT = " << it_b_client->request.expect << std::endl;
				it_b_client->request = this->ft_parser_requete(it_b_client->num ,_tmp_buffer.size() , _tmp_buffer, it_b_client->request);
				// std::cout << " apresEXPECT = " << it_b_client->request.expect << std::endl;
				// sleep(2);
				
				it_b_client->recv = true;


				this->_header_requete.erase(this->_header_requete.begin(), this->_header_requete.end());
				
				this->_recv_complete.chunked = false;
				this->_recv_complete.boundary.clear();
				_tmp_buffer.clear();
				_DATA = 1;
				// std::cout << "fin du traitement de la requete" << std::endl;
				// sleep(2);
				return (0);
			}
			else
			{
				// std::cout << "ft_check_recv == 0" << std::endl;
				_DATA = 0;
			}		
		}
	}
	return (0);
}

/*
**	Test de la boucle principal qui va tout faire.
*/
int		HttpServer::ft_main_loop( void )
{
	this->_recv_complete.chunked = false;
	_DATA = 0;
	still_to_send = 0;
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
			// std::cout << " Error main loop" << std::endl;
			std::cerr << e.what() << std::endl;
			break ;
		}
	}
	this->ft_clean_socket_clients();
	this->ft_clean_socket_servers();
	return (0);
}