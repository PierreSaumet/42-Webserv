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



class HttpServer {

	public:

		typedef struct			s_http_server {

			int					enable;
			struct sockaddr_in	svr_addr;
			int					sock;
		}						t_http_server;
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
		

	protected:
};

#endif
