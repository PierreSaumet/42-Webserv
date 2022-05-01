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
**	HttpServer Empty Constructor.
*/
HttpServer::HttpServer( void ) { // a terminer, initialiser toutes les variables

	std::cout << "Dans HttpServer constructor" << std::endl;
	std::cout << "trying to catch the data form Parsing. " << std::endl;
	return ;
}

// je ne suis pas sur que cette fonction soit necessaire
// l'idee est de mettre les roots sur tous les index et ou les upload store et ou les errors
void HttpServer::ft_put_root( void )
{	
	std::cout << "\n\n\n" << std::endl;
	std::cout << GREEN << "Dans ft_put_root" << CLEAR << std::endl;

	size_t i = 0;
	while (i < this->_servers.size())
	{
		if (this->_servers[i].root_server.empty() == false)
		{
			std::cout << " root last = " << this->_servers[i].root_server[this->_servers[i].root_server.size() - 1] << std::endl;
			if (this->_servers[i].root_server[this->_servers[i].root_server.size() - 1] == '/')
			{
				this->_servers[i].index_server.insert(0, this->_servers[i].root_server);
				this->_servers[i].upload_store_server.insert(0, this->_servers[i].root_server);
			}
			else
			{
				this->_servers[i].index_server.insert(0, "/");
				this->_servers[i].index_server.insert(0, this->_servers[i].root_server);
				if (this->_servers[i].upload_store_server.empty() == false)
				{
					this->_servers[i].upload_store_server.insert(0, "/");
					this->_servers[i].upload_store_server.insert(0, this->_servers[i].root_server);
				}
			}
		}
		
		i++;
	}

}


void HttpServer::DISPLAY_ALL_DATA_PARSING( void )
{
	std::cout << "\n\n\n" << std::endl;
	std::cout << GREEN << "Dans DISPLAY_ALL_DATA_PARSING" << CLEAR << std::endl;

	std::cout << "Nombre de servers: " << this->_servers.size() << std::endl;
	size_t i = 0;
	while (i < this->_servers.size())
	{
		std::cout << "Server numero : " << i << std::endl;
		std::cout << "nom server = " << this->_servers[i].name_server << std::endl;
		std::cout << "host server = " << this->_servers[i].host_server << std::endl;
		std::cout << "port server = " << this->_servers[i].port_server << std::endl;
		std::cout << "root server = " << this->_servers[i].root_server << std::endl;
		std::cout << "index server = " << this->_servers[i].index_server << std::endl;
		std::cout << "return server = " << this->_servers[i].return_server << std::endl;
		std::cout << "autoindex server = " << this->_servers[i].autoindex_server << std::endl;
		std::cout << "buffer size server = " << this->_servers[i].buffer_size_server << std::endl;
		std::cout << "cgi path server = " << this->_servers[i].cgi_path_server << std::endl;
		std::cout << "upload store  server = " << this->_servers[i].upload_store_server << std::endl;
		

		std::cout << "Method accepted server ";
		for (std::vector<std::string>::iterator it = this->_servers[i].methods_server.begin(); it != this->_servers[i].methods_server.end(); it++)
			std::cout << *it << "\t";
		std::cout << "\n";

		std::cout << "folder_error server = " << this->_servers[i].folder_error << std::endl;
		std::cout << "Error accepted server ";
		for (std::map<int, std::string>::iterator it = this->_servers[i].error_server.begin(); it != this->_servers[i].error_server.end(); it++)
			std::cout << it->first << " et " << it->second << "\n";

		
		std::cout << "Nbr location server =" << this->_servers[i].nbr_location << std::endl;
		std::cout << " test = " << this->_servers[i].location.empty() << std::endl;
		size_t y = 0;
		while (y < this->_servers[i].nbr_location)
		{
			std::cout << "Loction numero : " << y << std::endl;
			std::cout << "nom location = " << this->_servers[i].location[y].name_location << std::endl;
			std::cout << "root location = " << this->_servers[i].location[y].root_location << std::endl;
			std::cout << "index location = " << this->_servers[i].location[y].index_location << std::endl;
			std::cout << "autoindex location = " << this->_servers[i].location[y].autoindex_location << std::endl;
			std::cout << "upload store location = " << this->_servers[i].location[y].upload_store_location << std::endl;
			std::cout << "buffer size location = " << this->_servers[i].location[y].buffer_size_location << std::endl;
			std::cout << "folder error location = " << this->_servers[i].location[y].folder_error << std::endl;
			std::cout << "Error accepted location ";
			for (std::map<int, std::string>::iterator it = this->_servers[i].location[y].error_location.begin(); it != this->_servers[i].location[y].error_location.end(); it++)
				std::cout << it->first << " et " << it->second << "\n";
			std::cout << "\nMethod accepted location ";
			for (std::vector<std::string>::iterator it = this->_servers[i].location[y].methods_location.begin(); it != this->_servers[i].location[y].methods_location.end(); it++)
				std::cout << *it << "\t";
			std::cout << "\n";

			std::cout << "\n";
			y++;
		}
		std::cout << "\n\n";
		i++;
	}

	// std::cout << "autre = " << 
}

/*
**	HttpServer Constructor with argument.
*/
HttpServer::HttpServer( std::string &configfile) : _max_connections(1000) { // a terminer, initialiser toutes les variables

	// debut CGI/////////////////////////////////////////////
	// (void)configfile;
	this->_cgi = new Cgi_exec();
	// // this->_cgi->ft_setup_env_cgi();
	// // // std::cout << "test getsoft = " << this->_cgi->getServerSoftware() << std::endl;
	// // // this->_cgi->setServerSoftware("Bonjour");
	// // // std::cout << "test setsoft = " << this->_cgi->getServerSoftware() << std::endl;

	// // // std::cout << "\n\n\n display all = " << std::endl;
	// // // this->_cgi->ft_display_all_variable_env();
	// // std::cout << "\n\n" << std::endl;

	// this->_cgi->ft_test();
	// // // // delete this->_cgi;
	
	// return ;


	std::cout << "Constructor avec argument "<< std::endl;
	try {
		this->_data = new Parsing(configfile);
		

		if (this->_data->ft_get_error() == 1)
			return ;
		this->_servers = this->_data->ft_get_servers();											// on recupere les informations provenant de la class parsing
		// ft_put_root();
		// this->DISPLAY_ALL_DATA_PARSING();

		// exit(1);
		// std::cout << "display un truc = " << this->_servers[0].host_server << std::endl;
		if (this->ft_create_servers() == 1)
			return ;
		this->ft_test_main_loop_server();
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
HttpServer::HttpServer( const HttpServer &copy ) {			// a terminer, initialiser toutes les variables

	this->_data = copy._data;
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
HttpServer			&HttpServer::operator=( const HttpServer &element ) { // a terminer, initialiser toutes les variables

	if (this != &element)
	{
		this->_data = element._data;
	}
	return (*this);
}

int					HttpServer::ft_create_servers( void )
{
	try
	{
		int			i;

		i = 0;
		for (size_t i = 0; i < this->_data->ft_get_nbr_servers(); i++)
		{
			this->_http_servers.push_back(t_http_server());
			this->_http_servers[i].enable = 1;

			this->_http_servers[i].sock = socket(AF_INET, SOCK_STREAM, 0);
			if (this->_http_servers[i].sock < 0)
				throw Error(0, "Error, 'creation of server', cannot create a socket.", 2);
			
			if (fcntl(this->_http_servers[i].sock, F_SETFL, O_NONBLOCK) < 0)
				throw Error(666, "Error, 'creation of server', cannot fcntl a socker server.", 2);

			if (setsockopt(this->_http_servers[i].sock, SOL_SOCKET, SO_REUSEADDR, &this->_http_servers[i].enable, sizeof(this->_http_servers[i].enable)) < 0)
			{
				// il faut fermer les socket ? tous les sockets < ==========================================
				if (close(this->_http_servers[i].sock) < 0)
				{
					close(this->_http_servers[i].sock);
					throw Error(1, "Error, 'creation of server', cannot close socket.", 2);
				}
				throw Error(2, "Error, 'creation of server', cannot set up the socket options.", 2);
			}

			memset((char *)&this->_http_servers[i].svr_addr, 0, sizeof(this->_http_servers[i].svr_addr));
			this->_http_servers[i].svr_addr.sin_family = AF_INET;
			// this->_http_servers[i].svr_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
			this->_http_servers[i].svr_addr.sin_addr.s_addr = INADDR_ANY;
			this->_http_servers[i].svr_addr.sin_port = htons(this->_servers[i].port_server);

			if (bind(this->_http_servers[i].sock, (struct sockaddr *) &this->_http_servers[i].svr_addr, sizeof(this->_http_servers[i].svr_addr)) < 0)
			{
				// il faut fermer les socket ? tous les sockets < ==========================================
				std::cerr << "error bind = " << strerror(errno) << std::endl;
				if (close(this->_http_servers[i].sock) < 0)
				{
					close(this->_http_servers[i].sock);
					throw Error(1, "Error, 'creation of server', cannot close socket.", 2);
				}
				std::cout << "cannot bind this->_http_servers[i].sock = " << this->_http_servers[i].sock << std::endl;
				throw Error(4, "Error, 'creation of server', cannot bind socket.", 2);
			}

			if (listen(this->_http_servers[i].sock, this->_max_connections) < 0)
			{
				// il faut fermer les socket ? tous les sockets < ==========================================
				if (close(this->_http_servers[i].sock) < 0)
				{
					close(this->_http_servers[i].sock);
					throw Error(1, "Error, 'creation of server', cannot close socket.", 2);
				}
				throw Error(5, "Error, 'creation of server', cannot listen.", 2);
			}
			std::cout << GREEN << " Le server: "<< this->_servers[i].name_server << " tourne sur le port : " << this->_servers[i].port_server << CLEAR << std::endl;
			std::cout << std::endl;
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
**	A deplacer ailleurs
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
	// 1) vider l'ensemble de lecture et d'ecriture
	FD_ZERO(&this->_read_fs);
	FD_ZERO(&this->_write_fs);

	/// 2) on ajoute le l'ensemble de lecture a l'ensemble des servers
	std::vector<t_http_server>::iterator it_b = this->_http_servers.begin();
	std::vector<t_http_server>::iterator it_e = this->_http_servers.end();

	for (it_b = this->_http_servers.begin(); it_b != it_e; it_b++)
	{
		FD_SET(it_b->sock, &this->_read_fs);
	}

	// 3) on ajoute l'ensemble de lecture et ecriture aux clients? 
	std::vector<t_client_socket>::iterator it_b_client = this->_all_client_socket.begin();
	std::vector<t_client_socket>::iterator it_e_client = this->_all_client_socket.end();
	for (it_b_client = this->_all_client_socket.begin(); it_b_client != it_e_client; it_b_client++)
	{
		FD_SET(it_b_client->client_socket, &this->_read_fs);
		FD_SET(it_b_client->client_socket, &this->_write_fs);
	}
	// 4 ) select ? 
	// std::cout << "FD_SETSIZE = " << FD_SETSIZE << std::endl;
	if ((this->_return_select = select(FD_SETSIZE, &this->_read_fs, &this->_write_fs, NULL, NULL)) < 0 && int_signal == 0)
	{
		// il faut fermer les socket ? tous les sockets < ==========================================
		std::cout << "dans erreur de select" << std::endl;
		this->ft_clean_socket_servers();
		this->ft_clean_socket_clients();
		exit(EXIT_FAILURE);
		//throw Error(5, "Error, 'main loop server', cannot select().", 2);
	}
	return ;
}

/*
**	Test apres select il faut verifier si le fd est compris dans l'ensemble
**	avec fd_ISSET
*/
void		HttpServer::ft_verifier_ensemble_isset( void )
{
	std::vector<t_http_server>::iterator it_b = this->_http_servers.begin();
	std::vector<t_http_server>::iterator it_e = this->_http_servers.end();

	for (; it_b != it_e; it_b++)
	{
		int		socket_new_client;
		struct sockaddr_in	addr_new_client;
		socklen_t	size_addr_new_client = sizeof(addr_new_client);
		memset((char*)&addr_new_client, 0, (int)size_addr_new_client);

		if (FD_ISSET(it_b->sock, &this->_read_fs))
		{
			// on recoit une demande d'un cliebt
			socket_new_client = accept(it_b->sock, (struct sockaddr *)&addr_new_client, &size_addr_new_client);		
			if (socket_new_client < 0 && int_signal == 0)
				throw Error(6, "Error, 'main loop server', server cannot accept() a client.", 2);
			else
			{
				std::cout << GREEN << "nouvelle connection client numero: " << socket_new_client << " avec le server " << CLEAR << std::endl;
				// maintenant on modifi l'etat du fd avec fcntl pour le mettre en non bloquand
				if (fcntl(socket_new_client, F_SETFL, O_NONBLOCK) < 0)
					throw Error(7, "Error, 'in main loop', server cannot change FD client with fcntl().", 2);
				else
				{
					// on ajoute le nouveau client.
					t_client_socket new_client;

					new_client.client_socket = socket_new_client;
					// std::cout << "nouveau client = " << new_client.client_socket << std::endl;
					new_client.client_addr = addr_new_client;
					this->_all_client_socket.push_back(new_client);
					std::cout << "Un nouveau client a ete ajoute, total de client = " << this->_all_client_socket.size() << std::endl;
					// std::cout << "taille clien = " << this->_all_client_socket.size() << std::endl;
				}		
			}
		}
	}
}

/*
**	Cette fonction recupere le header dans une string via ft_setup_header().
**	Si on doit afficher une erreur ou si on doit utiliser le CGi
**	alors on peut retourner directement la string car elle contient le header et le body
**	Sinon on ouvre le fichier et on le rajoute a header qu'on termine par retourner.
*/
std::string		HttpServer::ft_settup_http_response( void )				
{
	std::cout << GREEN << "\n\n ft_settup_http_response " << CLEAR << std::endl;
	std::string filename(this->_servers[0].index_server.c_str());				// a changer, car la ca affiche seuelemtn l'index.html
	std::string file_contents;
	struct stat sb;
	std::string res;
	FILE *input_file = NULL;


	std::string header = ft_setup_header();
	std::cout << "header = " << header << std::endl;
	std::cout << "le fichier demande = -" << this->_header_requete[0].path << "-" << std::endl;
	
	// Si on a une erreur ou si on a un fichier qui utilise le cgi, alors on peut retourner directement car on a setup le header et le body
	if (this->_header_requete[0].error == true)	
		return (header);
	if (this->_header_requete[0].cgi == true)
		return (header);
	// TEST REDIRECTIOn
	if (this->_header_requete[0].return_used == true)
		return (header);
	// Sinon, on recupere le fichier demande et on ajoute header qui correspond au header.
	input_file = fopen(this->_header_requete[0].path.c_str(), "r");
	stat(this->_header_requete[0].path.c_str(), &sb);
	res.resize(sb.st_size + 100);											// a changer
	fread(const_cast<char*>(res.data()), sb.st_size, 1, input_file);
	fclose(input_file);
	file_contents = res;
	file_contents.insert(0, header.c_str());
	return (file_contents);
}

/*
**	Function ft_test_writing: used send().
**	But before need to setup the http header and response
*/
int 		HttpServer::ft_test_writing( void )
{
	std::vector<t_client_socket>::iterator it_b_client = this->_all_client_socket.begin();
	std::vector<t_client_socket>::iterator it_e_client = this->_all_client_socket.end();

	for (; it_b_client != it_e_client; it_b_client++)
	{
		int ret_send;
		if (FD_ISSET(it_b_client->client_socket, &this->_write_fs))
		{
			if (this->_header_requete.empty() == true)		// on a pas de header_requete donc on a pas eu de demande
			{
				break;
			}

			_HTTP_RESPONSE = ft_settup_http_response();
			if (_HTTP_RESPONSE.empty())
			{
				std::cout << "La reponse a envoyer est null a cause de setttup http response() = ERROR / on ne doit jamais etre la " << std::endl;
				return (0);
			}
			std::cout << " HTTP_RESPONSE = -" << _HTTP_RESPONSE << "-" << std::endl;

			// test  erreur 431
		

			ret_send = send(it_b_client->client_socket, _HTTP_RESPONSE.c_str(),  _HTTP_RESPONSE.size(), 0);
			// sleep(10);
			if (ret_send < 0)
			{
				std::cout << "dans retour de send < 0 , cad send n'a pas marche " << std::endl;
				if (_HTTP_RESPONSE.empty())
					_HTTP_RESPONSE.erase(_HTTP_RESPONSE.begin(), _HTTP_RESPONSE.end());
				close(it_b_client->client_socket);
				it_b_client = this->_all_client_socket.erase(it_b_client);

				// std::cout << "On a retournee une reponse, on ferme le socket du client." << std::endl;
			}
			else
			{
				std::cout << "send a fonctionne ret_send = " << ret_send << std::endl;
				if (_HTTP_RESPONSE.empty())
					_HTTP_RESPONSE.erase(_HTTP_RESPONSE.begin(), _HTTP_RESPONSE.end());
				std::cout << "\nle client = " << it_b_client->client_socket << std::endl;
				close(it_b_client->client_socket);
				it_b_client = this->_all_client_socket.erase(it_b_client);
				std::cout << "On a retournee une reponse, on ferme le socket du client." << std::endl;
				this->_header_requete.erase(this->_header_requete.begin(), this->_header_requete.end());
				continue ;
			}
		}
	}
	return (0);
}


size_t		HttpServer::ft_check_recv_complete( std::string tt_buffer )
{
	std::cout << GREEN << "Dans ft_check_recv_complete : " << CLEAR << std::endl;

	// std::cout << "BUFFER = " << tt_buffer << "\n\n\n\n" << std::endl;

	size_t pos = 0;
	if (tt_buffer.compare(0, 5, "POST ") == 0)
	{
		this->_recv_complete.method = "POST";	// useless
		if (this->_recv_complete.chunked == false)	//n'a pas ete setup ou pas de chunked
		{
			pos = tt_buffer.find("Transfer-Encoding: chunked");
			if (pos == std::string::npos)	// Les data ne sont pas Chunked
			{
				pos = tt_buffer.find("Content-Type: application/x-www-form-urlencoded\r\n");	
				if (pos == std::string::npos)		// envoie formulaire via multipart
				{
					if (this->_recv_complete.boundary.empty() == true)
					{
						pos = tt_buffer.find("Content-Type: multipart/form-data;");
						if (pos == std::string::npos)
						{
							std::cout << "Erreur requete post content-type " << std::endl;
							exit(1);
						}
						else
						{
							size_t pos_end = tt_buffer.find("\r\n", pos);
							std::string tmp(tt_buffer, pos + 34, pos_end - pos - 34);
							pos = tmp.find("boundary=");
							if (pos == std::string::npos)
							{
								std::cout << "Erreur requete post pas de boundary dans le formulaire multipart" << std::endl;
								exit(1);
							}
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
				else				// envoie de formulaire par default
				{
					pos = tt_buffer.find("Content-Length: ");
					if (pos == std::string::npos)
					{
						std::cout << "Erreur requete post content-lenght" << std::endl;
						exit(1);
					}
					else
					{
						std::cout << "Ok good envoie de data via un formulaire" << std::endl;
						return (1);
					}
				}
			}
			else
			{
				std::cout << " On a du data chunked" << std::endl;
				this->_recv_complete.chunked = true;
				pos = tt_buffer.find("0\r\n\r\n");				// On cherche la fin du transfer encoding
				if (pos == std::string::npos)					// On n'a pas la fin, on continue de recuperer de la data
				{
					std::cout << "On n'a pas la fin du chunked" << std::endl;
					return (0);
				}
				else
					return (1);								// On a toute la requete
			}
		}
		else
		{
			std::string tmp(tt_buffer.end() - 5, tt_buffer.end());
			if (tmp == "0\r\n\r\n")
			{
				std::cout << "Les toutes les donnees chunked sont transmises." << std::endl;
				return (1);
			}
			return (0);	
		}
		// impossible d'etre la
		exit(1);
	}
	else if (tt_buffer.compare(0, 4, "GET ") == 0)
	{
		// std::cout << "YES GET " << std::endl;
		pos = tt_buffer.find("\r\n\r\n");
		if (pos == std::string::npos)
		{
			std::cout << "ERREUR GET NE TROUVE PAS de header" << std::endl;
			exit(1);
		}
		return (1);
	}
	else if (tt_buffer.compare(0, 7, "DELETE ") == 0)
	{
		std::cout << "YES DELETE" << std::endl;
		return (1);
	}
	else
	{
		std::cout << "NI GET NI POST NI DELETE doit sortir une erreur" << std::endl;
		return (1);
	}
	return (0);
}

/*
*	**	Testing writing
*/
int		HttpServer::ft_test_reading( void )
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

			if ((request_length = recv(it_b_client->client_socket, buffer, sizeof(buffer), 0)) <= 0)
			{
				if (request_length == -1)
				{
					std::cout << "ERREUR RECV = -1 \t: " << request_length << std::endl;
					std::cout << " on close le client et on continue" << std::endl;
					FD_CLR(it_b_client->client_socket, &this->_read_fs);
					close(it_b_client->client_socket);
					it_b_client = this->_all_client_socket.erase(it_b_client);
					std::cerr << strerror(errno) << std::endl;
					exit(1);
				}
				else
				{
					std::cout << "La connection est fermee avc le client dans recv" << std::endl;
					FD_CLR(it_b_client->client_socket, &this->_read_fs);
					close(it_b_client->client_socket);
					it_b_client = this->_all_client_socket.erase(it_b_client);
					std::cerr << strerror(errno) << std::endl;
					continue ;
				}
			}
			_TOTAL_BUFFER.append(buffer, request_length);

			if (ft_check_recv_complete(_TOTAL_BUFFER) == 1)
			{
				// exit(1);
				this->ft_parser_requete(_TOTAL_BUFFER.size() , _TOTAL_BUFFER);
				this->_recv_complete.chunked = false;
				this->_recv_complete.boundary.clear();
				this->_recv_complete.method = "";
				_TOTAL_BUFFER.clear();
			}
			else
				std::cout << "ft_check_recv == 0" << std::endl;
			this->_recv_complete.method = "";	// useless
		}
	}
	return (0);
}

/*
**	Test de la boucle principal qui va tout faire.
*/
int		HttpServer::ft_test_main_loop_server( void )
{
	std::cout << "Dans la boucle principale" << std::endl;
	std::cout << "signal = " << int_signal << std::endl;
	while (int_signal == 0)
	{
		try {
			
			this->ft_handle_connections();
			if (this->_return_select != 0 && int_signal == 0) // cas ou on obtient quelque chose, genre un mec se connecte et bah envoie des donnes ?
			{
				this->ft_verifier_ensemble_isset();
				if (this->ft_test_reading() == 1)
				{
					std::cout << "test_reading return -1" << std::endl;			// Erreur a changer 
					return (1);
				}
				if (this->ft_test_writing() == 1)								// Erreur a changer
				{
					std::cout << "test_writing return -1" << std::endl;
					return (1);
				}
			}		    
		}
		catch (std::exception &e)
		{
			std::cout << " on capte une erreur ici dans la boucle principale." << std::endl;
			std::cerr << e.what() << std::endl;
			std::cout << "dans catch error main loop " << std::endl;

			/* A FAIRE LOL */

			break ;
		}
	}
	this->ft_clean_socket_servers();
	this->ft_clean_socket_clients();

	std::cout << "FIN DU PROGRAMME " << std::endl;
	return (0);
}