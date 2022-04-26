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
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#include "Parsing.hpp"
#include <algorithm>
#include <map>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

#include "CGI_exec.hpp"

class Cgi_exec;

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

		typedef struct			s_recv_data {

			std::string			method;			// post ou get
			size_t				pos_end_header;	// post ou get
			std::string			content_length;
			std::string			content_type;
			std::string			boundary;
			std::string			size_body;
			std::string			size_header;
		}						t_recv_data;


		typedef struct 			s_header_request {

			std::string			method;				// Get Delete ou Post
			std::string			path;				// Le path c'est a dire l'URL transmise avec les donnees pour Get
			std::string			protocol;
			std::string			host;
			std::string			accept;				// le Accept: de la requete
			std::string			path_http;			// le path total du fichier demande.
			std::string			query_string;		// les valeur donnees dans l'url pour une requete get
			std::string			cgi_return;			// string contenant les donnees provenant du cgi
			std::string			referer;
			std::string			request_uri;		// path du file php plus les donnes de get  ( GET SEUELENT);
			std::string			content_type;		// suelement post
			std::string			content_length;		// seulement post
			std::string			body_post;				//seulement post
			std::string			script_file_name;	// nom du fichier
			bool				upload;
			bool				cgi;
			bool				error;
			size_t				num_error;
			std::string			body_error;			// si cgi body_error contient la response du cgi sans le header
			std::map<std::string, std::string>	data;	// Contient les information transmise a Get via un formulaire					// contient les informations en get.
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
		void				ft_handle_connections( void );
		void				ft_verifier_ensemble_isset( void );
		int					ft_test_writing( void );
		int					ft_test_reading( void );

		void				ft_parser_requete( int len_msg, std::string msg );
		std::string				ft_settup_http_response( void );

		/*	Functions used to get information from the header of a request.
		*/
		std::string		ft_check_referer( std::string request_http );
		std::string		ft_check_path_header( std::string header) ;
		std::string		ft_check_protocol_header( std::string header );
		std::string		ft_check_host_header( std::string header );
		std::string		ft_check_pathhttp_header( std::string header );
		std::string		ft_check_accept_header( std::string header );
		bool			ft_check_cgi_or_php( std::string request_http );
		std::string		ft_check_content_length( std::string request_http );
		std::string		ft_check_content_type( std::string request_http );
		std::string		ft_check_body_post( std::string request_http );
		std::string		ft_setup_header( void );

		size_t			ft_check_access_location( std::string path );

		std::string::iterator ft_find_end_header( std::string request_http );


		
		/*
		**	Functions used to set up the headers for the response
		*/
		size_t			ft_get(std::string request_http, int len_msg);
		size_t			ft_post(std::string request_http, int len_msg);
		void			ft_delete(std::string request_http, int len_msg);

		std::string		ft_where_to_upload( std::string path);
		size_t			ft_upload_file( std::string request_http );
		
		void			ft_exec_cgi_test( std::string request_http, int len_msg );

		size_t		ft_parsing_path_get_request( void );
		std::string		ft_clean_path_get_request( std::string tmp );

		std::string		ft_get_content_length( struct stat buff, size_t len ) const;
		std::string		ft_get_end_header( void ) const;
		std::string		ft_get_content_type( void ) const;
		std::string		ft_get_charset( void ) const;
		std::string		ft_get_server_name( void ) const;
		std::string		ft_get_status( bool x ) const;
		std::string		ft_get_date( void ) const;
		std::string		ft_get_allow( void ) const;

		std::string		ft_get_file( std::string path ) const;

		int				ft_check_method_allowed_header( std::string request_http, std::string method );
		/*
		**	Function used if an error occurs in the request
		*/
		int			ft_setup_error_header( std::string request_http, int len_msg );
		std::string			ft_find_error_html( void );
		std::string			ft_create_error( void );
		std::string			ft_return_error( void );

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

		// ICI nouveau lyundi 24 
		size_t				ft_check_recv_complete(std::string tt_buffer);
		void				ft_test_display_recv( void );


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

		t_recv_data						_recv_complete;
		std::string						_TOTAL_BUFFER;

		std::vector<t_header_request>		_header_requete;

		Cgi_exec			*_cgi;

	protected:
};

#endif
