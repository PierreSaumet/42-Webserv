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
HttpServer::HttpServer( std::string &configfile) : _max_connections(1000) {

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

		
		//this->ft_test();
		this->ft_create_servers();
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
"<!DOCTYPE html><html><head><title>Hello World</title>"
"<style>body { background-color: #111 }"
"h1 { font-size:4cm; text-align: center; color: black;"
" text-shadow: 0 0 2mm red}</style></head>"
"<body><h1>TEST Webserv</h1></body></html>\r\n";

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
			memset((char *)&this->_http_servers[i].svr_addr, 0, sizeof(this->_http_servers[i].svr_addr));
			this->_http_servers[i].svr_addr.sin_family = AF_INET;
			this->_http_servers[i].svr_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
			this->_http_servers[i].svr_addr.sin_port = htons(this->_servers[i].port_server);
			
			this->_http_servers[i].sock = socket(AF_INET, SOCK_STREAM, 0);
			if (this->_http_servers[i].sock < 0)
				throw Error(1, "Error, 'creation of server', cannot create a socket.", 2);
			if (setsockopt(this->_http_servers[i].sock, SOL_SOCKET, SO_REUSEADDR, &this->_http_servers[i].enable, sizeof(int)) < 0)
			{
				// il faut fermer les socket ? tous les sockets < ==========================================
				if (close(this->_http_servers[i].sock) < 0)
					throw Error(3, "Error, 'creation of server', cannot close socket.", 2);	throw Error(2, "Error, 'creation of server', cannot set up the socket options.", 2);
			}
			if (bind(this->_http_servers[i].sock, (struct sockaddr *) &this->_http_servers[i].svr_addr, sizeof(this->_http_servers[i].svr_addr)) < 0)
			{
				// il faut fermer les socket ? tous les sockets < ==========================================
				if (close(this->_http_servers[i].sock) < 0)
					throw Error(3, "Error, 'creation of server', cannot close socket.", 2);
				throw Error(4, "Error, 'creation of server', cannot bind socket.", 2);
			}
			if (listen(this->_http_servers[i].sock, this->_max_connections) < 0)
			{
				// il faut fermer les socket ? tous les sockets < ==========================================
				if (close(this->_http_servers[i].sock) < 0)
					throw Error(3, "Error, 'creation of server', cannot close socket.", 2);
				throw Error(5, "Error, 'creation of server', cannot listen.", 2);
			}
			std::cout << GREEN << " Le server: "<< this->_servers[i].name_server << " tourne sur le port : " << this->_servers[i].port_server << CLEAR << std::endl;
			std::cout << std::endl;
		}
		// while (1)
		// {

		// }
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





	// TEST pour ouvrir et afficher le fichier index.html



  while (1) {
    client_fd = accept(sock, (struct sockaddr *) &cli_addr, &sin_len);
    printf("got connection\n");
 
    if (client_fd == -1) {
      perror("Can't accept");
      continue;
	// sleep(10);
	// exit(-1);
    }
	
	//write(client_fd, this->_servers[0].index_server.c_str(), sizeof(this->_servers[0].index_server.c_str()) - 1);
   
	std::string filename(this->_servers[0].index_server.c_str());
	std::string file_contents;

	struct stat sb;
	std::string res;

	FILE *input_file = fopen(filename.c_str(), "r");
	if (input_file == NULL)
	{
		std::cout << "ECHEC open " << std::endl;
		return (1);
	}
	stat(filename.c_str(), &sb);
	res.resize(sb.st_size + 100);
	

	fread(const_cast<char*>(res.data()), sb.st_size, 1, input_file);
	fclose(input_file);

	
	// file_contents = res;
	// std::string *relou = std::remove(res.begin(), res.end(), "\t");
	// res.erase(relou.begin(), relou.end());

	// file_contents = std::strtok(&res[0], " \t\n\v\r\f");
	file_contents = res;
	file_contents.insert(0, "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n");
	// std::cout << "TEST = " << file_contents << std::endl;
	// std::cout << "sizeof file content " << file_contents.size() << std::endl;
	// std::cout << "sizeof file content .c_str()" << sizeof(file_contents.c_str()) << std::endl;
	// int lol2 = write(1, file_contents.c_str(), file_contents.size());
	// std::cout << "\nreturn write qui marche PAS  = " << lol2 << std::endl;
	// std::cout << "\n\n" << std::endl;

	int lol2 = write(client_fd, file_contents.c_str(), file_contents.size());
	std::cout << "\nreturn write qui marche PAS  = " << lol2 << std::endl;


	// lol2 = write(client_fd, file_contents.c_str(), sizeof(file_contents) -1);
	// std::cout << "\nreturn write qui marche PAS  = " << lol2 <<std::endl;
	// sleep(10);
	// ca marche
	// std::cout << "TEST = " << response << std::endl;
   	// int lol = write(client_fd, response, sizeof(response) - 1); /*-1:'\0'*/
	// write(1, response, sizeof(response) -1);
	// std::cout << "return write qui marche = " << lol <<std::endl;
    close(client_fd);
	// break;
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

void	HttpServer::ft_gerer_les_connections_avec_select( void )
{
	// 1) vider l'ensemble de lecture et d'ecriture
	FD_ZERO(&this->_read_fs);
	FD_ZERO(&this->_write_fs);

	// std::cout << "ca pete ici ? " << std::endl;
	/// 2) on ajoute le l'ensemble de lecture a l'ensemble des servers
	std::vector<t_http_server>::iterator it_b = this->_http_servers.begin();
	std::vector<t_http_server>::iterator it_e = this->_http_servers.end();

	std::cout << "la taille = " << this->_http_servers.size() << std::endl;
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


	// manque le fait d;ajouter des nouvelles connections?

	// 4 ) select ? 
	if ((this->_return_select = select(FD_SETSIZE, &this->_read_fs, &this->_write_fs, NULL, NULL) < 0))
	{
		// il faut fermer les socket ? tous les sockets < ==========================================
		throw Error(5, "Error, 'main loop server', cannot select().", 2);
	}
	return ;
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
			ft_gerer_les_connections_avec_select();
			std::cout << "euh return select = " << this->_return_select << std::endl;
			if (this->_return_select != 0) // cas ou on obtient quelque chose, genre un mec se connecte et bah envoie des donnes ?
			{
				
			}		
		}
		catch (std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
		std::cout << "ici " << std::endl;
		// fonction qui va gerer les connections avec select.
		//break;
	}
	return (0);
}