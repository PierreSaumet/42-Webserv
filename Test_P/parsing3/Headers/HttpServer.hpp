/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psaumet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/01 11:07:34 by psaumet           #+#    #+#             */
/*   Updated: 2021/06/01 11:07:42 by psaumet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPSERVER_HPP
# define HTTPSERVER_HPP

#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <err.h>
#include "Parsing.hpp"
#include <algorithm>
#include <map>

#include <fcntl.h>


class HttpServer {

	public:

		// structure utilise pour les socket et informatons des servers genre port 8080 ou autre
		typedef struct			s_http_server {

			int					enable;
			struct sockaddr_in	svr_addr;
			int					sock;
		}						t_http_server;

		// structure utilise pour les socket et informations des clients genre port 8080 ou autre
		typedef struct			s_client_socket {

			int					client_socket;
			struct sockaddr_in	client_addr;

		}						t_client_socket;

		typedef struct 			s_header_request {

			std::string			method;
			std::string			path;
			std::string			protocol;
			std::string			host;
			bool				cgi;
			bool				error;
			size_t				num_error;
			std::string			body_error;
		}						t_header_request;
		

		/*
		**	Canonical Form
		*/
		HttpServer( void );
		HttpServer( std::string &configfile );
		HttpServer( const HttpServer &copy );
		~HttpServer( void);

		/*
		**	Overload operator '='
		*/
		HttpServer			&operator=( const HttpServer &element );

		/*
		**	Functions TESTING used to display pierre.
		*/
		int					ft_test( void );

		/*
		**	Functions used to creates server
		*/
		int					ft_create_servers( void );
		int					ft_test_main_loop_server( void );
		void				ft_gerer_les_connections_avec_select( void );
		void				ft_verifier_ensemble_isset( void );
		int					ft_test_writing( void );
		int					ft_test_reading( void );

		void				ft_parser_requete( int len_msg, const char *msg );
		std::string				ft_settup_http_response( void );
		std::string		ft_check_methods_header( std::string header );
		std::string		ft_check_path_header( std::string header) ;
		std::string		ft_check_protocol_header( std::string header );
		std::string		ft_check_host_header( std::string header );
		std::string		ft_setup_header( void );


		/*
		**	Functions used to set up the headers for the response
		*/
		size_t			ft_get(std::string request_http, int len_msg);
		void			ft_post(std::string request_http, int len_msg);
		void			ft_delete(std::string request_http, int len_msg);

		std::string		ft_get_content_length( struct stat buff ) const;
		std::string		ft_get_end_header( void ) const;
		std::string		ft_get_content_type( void ) const;
		std::string		ft_get_charset( void ) const;
		std::string		ft_get_server_name( void ) const;
		std::string		ft_get_status( void ) const;

		/*
		**	Function used if an error occurs in the request
		*/
		size_t			ft_setup_error_header( std::string request_http, int len_msg );
		std::string			ft_find_error_html( void );

		/*
		**	Function used to clean sockets from clients and servers.
		*/
		void				ft_clean_socket_clients( void );
		void				ft_clean_socket_servers( void );

		/*
		**	Functions used for the signal handler
		*/
		static int			int_signal;
		static void			handler_signal( int num );

		

	private:
		Parsing				*_data;
		std::vector<t_server>           _servers;


		// test
		// int	_sock;
		std::vector<t_http_server>		_http_servers;
		int								_max_connections;
		fd_set							_read_fs;
		fd_set							_write_fs;
		std::vector<t_client_socket>	_all_client_socket;	// va posseder toutes les conenctions. peut etre mettre en list
		int								_return_select;
		std::string						_HTTP_RESPONSE;

		std::vector<t_header_request>		_header_requete;

	protected:
};

#endif
