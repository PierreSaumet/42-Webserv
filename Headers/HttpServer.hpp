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
			bool 				binded;
			size_t				num;
		}						t_http_server;

		// This structure is used to check if all data from the client have been recieved
		typedef struct			s_recv_data {

			bool				chunked;			// Recieved chunked data from POST method
			std::string			boundary;			// Recieved data from POST with boundaries.
		}						t_recv_data;

		// This structure is the main structure used to parse the data send by the client.
		typedef struct 			s_header_request {

			int 				port_client;
			std::string			method;					// GET, POST or DELETE
			std::string			path;					// URI with data for GET
			std::string			protocol;				// Protocol from the header
			std::string			host;					// Contains Host and Port: localhost:8080 from the header
			std::string			accept;					// Accept form the header
			std::string			path_http;				// Path of the file / binary used
			std::string			query_string;			// Data from a GET request
			std::string			cgi_return;				// String from CGI
			std::string			referer;				// Useless
			std::string			request_uri;			// Used onlu for GET, path of the cgi's file
			std::string			content_type;			// Only for POST
			std::string			content_length;			// Only for POST
			std::string			body_post;				// Only for POST
			std::string			script_file_name;		// File name
			std::string			path_file;				// Useless
			bool				return_used;			// Used if there is a return
			bool				upload;					// Used if there is an upload
			bool				cgi;					// Used if there is a cgi needed
			bool				error;					// Used if there is an error
			size_t				num_error;				// Number of error like 404/ 400
			std::string			body_error;				// Used to contains the error to display or data from the cgi
			std::map<std::string, std::string>	data;	// Contains data from GET
			bool				autoindex;				// Used if there is autoindex on
			bool 				location;				// Used if there is a location bloc
			int					num_loc;				// Location number
			int					num_server;				// Server number
			bool			 	expect;					// Used if there is an expect
			bool 				connection_close;		// Useless
		}						t_header_request;
		

		// This structure contains sockets from the clients to be used by the server
		typedef struct			s_client_socket {

			int					client_socket;
			struct sockaddr_in	client_addr;
			int  				server_socket;			//	Server's socket which used by the client.
			t_header_request 	request;				//	Contains all informations from the parsing of the request
			bool 				recv;					//	Used if there is still data to recv
			size_t  			num;					//	Server number
		}		t_client_socket;

		/*
		**	Canonical Form in HttpServer.cpp
		*/
		HttpServer( std::string &configfile );
		~HttpServer( void );

		/*
		**	Initialises the t_header_request structure
		*/
		void				ft_init_general_structure( void );

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
		t_header_request	ft_parser_requete( int port_client, int len_msg, std::string request, t_header_request test);
		size_t				ft_get(std::string request_http, int len_msg);
		size_t				ft_post(std::string request_http , t_header_request data);
		size_t				ft_delete(std::string request_http, int len_msg);
		size_t 				ft_post_continue(t_header_request data, std::string body);
		
		/*
		**	Functions in HttpServerResponse.cpp, used to setup the header and the body response to send to the client
		*/
		std::string			ft_setup_response_to_send( t_header_request *requete );
		std::string			ft_setup_header( t_header_request *requete );




		/*	Functions used to get information from the header of a request.
		*/
		std::string		ft_check_referer( std::string request_http );
		std::string		ft_check_path_header( std::string header) ;
		std::string		ft_check_protocol_header( std::string header );
		std::string		ft_check_host_header( std::string header );
		std::string		ft_check_pathhttp_header( std::string header );
		std::string		ft_check_accept_header( std::string header );
		bool			ft_check_cgi_or_php( void );
		std::string		ft_check_content_length( std::string request_http );
		std::string		ft_check_content_type( std::string request_http );
		std::string		ft_check_body_post( std::string request_http );
		

		size_t			ft_check_access_location( std::string path );

	


		
	void			ft_exec_cgi_test( void );
		


		std::string		ft_parsing_path_get_request( void );
		std::string		ft_clean_path_get_request( std::string tmp );

		std::string		ft_get_content_length( struct stat buff, size_t len, size_t len_header ) const;
		std::string		ft_get_end_header( void ) const;
		std::string		ft_get_content_type(t_header_request *requete, size_t binary ) const;
		std::string		ft_get_charset( void ) const;
		std::string		ft_get_server_name( void ) const;
		std::string		ft_get_status( t_header_request *requete, bool x ) const;
		std::string		ft_get_date( void ) const;
		std::string		ft_get_allow( void ) const;
		std::string    	ft_get_code_redirection( t_header_request *request ) const;
		std::string 	ft_get_return_location( void ) const;

		std::string		ft_get_file( std::string path ) const;

		int				ft_check_method_allowed_header( std::string method );
		/*
		**	Function used if an error occurs in the request
		*/
		void			ft_setup_error_header( void );

		void			ft_setup_error_header_response( t_header_request *requete );
		std::string			ft_find_error_html( t_header_request *requete );
		std::string			ft_create_error( t_header_request *requete );
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
	
		std::string ft_create_autoindex( t_header_request *requete );


		int ft_redirection( void );

		void ft_continue_send( std::vector<t_client_socket>::iterator it_client );

		std::string ft_decode_chunked( std::string body );

		size_t ft_do_error( size_t num_error );
		std::string  get_string( size_t pos_end, std::string body);
		long long int  convert_hex_to_dec( std::string const string );
	
		int ft_choose_wich_server( std::string header, int num);
		int ft_choose_wich_location( std::string header );

		int ft_check_basic( std::string header );

		void ft_setup_error_header_2( void );

		void put_error_in_body( void );

		std::string ft_last_modified( void );
		std::string	ft_expire( void );

	private:
		HttpServer( void );										// Empty constructor
		HttpServer( const HttpServer &copy );					// Copy constructor
		HttpServer						&operator=( const HttpServer &element );	//	Overload operator =

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
		size_t 							_num_serv;				// This variable is the index of the bloc server being currently used.												
		size_t  						_num_loc;				// This variable is the index of the bloc location being currently used.
		size_t   						_DATA;
		unsigned long 					_still_to_send;
		unsigned long 					_total_send;
};

#endif
