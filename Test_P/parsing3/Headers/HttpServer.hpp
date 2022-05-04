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
#include <dirent.h>

#include "CGI_exec.hpp"

class Cgi_exec;

class HttpServer {

	public:

		// This structure contains sockets necessary for the creation of the server
		typedef struct			s_http_server {

			int					enable;
			struct sockaddr_in	svr_addr;
			int					sock;
		}						t_http_server;

		// This structure contains sockets from the clients to be used by the server
		typedef struct			s_client_socket {

			int					client_socket;
			struct sockaddr_in	client_addr;

		}						t_client_socket;

		// This structure is used to check if all data from the client have been recieved
		typedef struct			s_recv_data {

			bool				chunked;			// Recieved chunked data from POST method
			std::string			boundary;			// Recieved data from POST with boundaries.
		}						t_recv_data;

		// This structure is the main structure used to parse the data send by the client.
		typedef struct 			s_header_request {

			std::string			method;				// GET, POST or DELETE
			std::string			path;				// Le path c'est a dire l'URL transmise avec les donnees pour Get
			std::string			protocol;
			std::string			host;				// Contains Host and Port: localhost:8080
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
			

			std::string			path_file;

			bool				return_used;
			
			bool				upload;
			bool				cgi;
			bool				error;
			size_t				num_error;
			std::string			body_error;			// si cgi body_error contient la response du cgi sans le header
			std::map<std::string, std::string>	data;	// Contient les information transmise a Get via un formulaire					// contient les informations en get.
		}						t_header_request;
		

		/*
		**	Canonical Form in HttpServer.cpp
		*/
		HttpServer( std::string &configfile );
		~HttpServer( void);
		HttpServer			&operator=( const HttpServer &element );


		/*
		**	Functions in HttpServer.cpp, used to create servers.
		*/
		int					ft_create_servers( void );
		int					ft_main_loop( void );
		void				ft_handle_connections( void );
		void				ft_check_isset( void );
		int					ft_write( void );
		int					ft_reading( void );
		size_t				ft_check_recv_complete(std::string tt_buffer);


		/*
		**	Functions in HttpServerRequest.cpp, used to apply the corresponding method
		*/
		void				ft_parser_requete( int len_msg, std::string msg );
		size_t				ft_get(std::string request_http, int len_msg);
		size_t				ft_post(std::string request_http, int len_msg);
		size_t				ft_delete(std::string request_http, int len_msg);

		
		/*
		**	Functions in HttpServerResponse.cpp, used to setup the header and the body response to send to the client
		*/
		std::string			ft_setup_response_to_send( void );
		std::string			ft_setup_header( void );


		/*
		**
		*/





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
		

		size_t			ft_check_access_location( std::string path );

		// std::string::iterator ft_find_end_header( std::string request_http );


		

		// std::string		ft_where_to_upload( std::string path);
		size_t			ft_upload_file( std::string request_http );
		
		void			ft_exec_cgi_test( std::string request_http, int len_msg );

		size_t		ft_parsing_path_get_request( void );
		std::string		ft_clean_path_get_request( std::string tmp );

		std::string		ft_get_content_length( struct stat buff, size_t len, size_t len_header ) const;
		std::string		ft_get_end_header( void ) const;
		std::string		ft_get_content_type( size_t binary ) const;
		std::string		ft_get_charset( void ) const;
		std::string		ft_get_server_name( void ) const;
		std::string		ft_get_status( bool x ) const;
		std::string		ft_get_date( void ) const;
		std::string		ft_get_allow( void ) const;
		std::string    	ft_get_code_redirection( void ) const;
		std::string 	ft_get_return_location( void ) const;

		std::string		ft_get_file( std::string path ) const;

		int				ft_check_method_allowed_header( std::string request_http, std::string method );
		/*
		**	Function used if an error occurs in the request
		*/
		void			ft_setup_error_header( void );
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



		size_t ft_check_access_path( void );

		int ft_find_index_server( void );

		size_t check_location( std::string path, std::string name_location );
	
		std::string ft_create_autoindex( void );


		int ft_redirection( void );

		int ft_continue_send( std::vector<t_client_socket>::iterator it_client );
	private:

		HttpServer( const HttpServer &copy );					// Copy constructor
		HttpServer( void );										// Empty constructor

		Cgi_exec						*_cgi;					// This variable is used to get data from Cgi's class
		Parsing							*_data;					// This variable is used to get data from Parsing's class.

		std::vector<t_http_server>		_http_servers;			// This variable contains sockets for the server.
		int								_max_connections;		// This variable is used setup a maximum of connection.
		fd_set							_read_fs;				// This variable is used by the server to read on a fd.
		fd_set							_write_fs;				// This variable is used by the server to write on a fd.
		std::vector<t_client_socket>	_all_client_socket;		// This variable contains all client's file descriptor.
		int								_return_select;			// This variable is the return of select().

		std::vector<t_server>           _servers;				// This variable contains all server's informations.
		std::string						_response_to_send;		// This variable contains all data to send to the client.
		t_recv_data						_recv_complete;			// This variable is used to see if the server has recieved everything from the client.
		std::string						_tmp_buffer;			// This variable is a string used to collect the client's request.
		std::vector<t_header_request>	_header_requete;		// This variable is used to collect data from the header or the body of the client's request

		size_t _num_serv;										// This variable is the index of the bloc server being currently used.												
		size_t  _num_loc;										// This variable is the index of the bloc location being currently used.
	

		size_t   _DATA;
		unsigned long still_to_send;
		unsigned long total_send;
};

#endif
