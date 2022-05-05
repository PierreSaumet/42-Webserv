/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI_exec.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psaumet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/01 11:07:34 by psaumet           #+#    #+#             */
/*   Updated: 2021/06/01 11:07:42 by psaumet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "HttpServer.hpp"

class Cgi_exec
{
	public:
		/*
		**	Canonical form
		*/
		Cgi_exec( void );
		Cgi_exec( const Cgi_exec &copy );
		~Cgi_exec( void );

		Cgi_exec		&operator=( const Cgi_exec &element );

		/*
		**	This function setup all variables to "NULL" string
		*/
		size_t			ft_setup_env_cgi( void );

		/*
		**	This function execute the cgi from the path
		*/
		std::string		ft_execute_cgi( std::string path_cgi, std::string path_file );

		std::string		ft_return_data_from_cgi( void );

		std::vector<std::string>		ft_convert_map_to_vector( void );

		void					ft_add_to_cgi_string( unsigned char *buffer, int ret );

		std::string				ft_return_string_cgi( void );
		/*
		**	Setters
		*/
		void			set_body_string_post( std::string const body_post );

		void			setScriptFileName( std::string const script_filename );
		void			setRedirectStatus( std::string const redirect_status );
		void			setStatusCode( std::string const status_code );
		void			setServerSoftware( std::string const software );
		void			setServerName( std::string const name );
		void			setGatewayInterface( std::string const gateway );
		void			setServerProtocol( std::string const protocol );
		void			setServerPort( std::string const port );
		void			setRequestMethod( std::string const method );
		void			setPathInfo( std::string const path_info );
		void			setPathTranslated( std::string const path_trans );
		void			setScriptName( std::string const script_name );
		void			setQueryString( std::string const query_string );
		void			setRemoteHost( std::string const remote_host );
		void			setRemoteADDR( std::string const remote_addr );
		void			setAuthType( std::string const auth_type );
		void			setRemoteUser( std::string const remote_user );
		void			setRemoteIdent( std::string const remote_ident );
		void			setContentType( std::string const content_type );
		void			setContentLength( std::string const content_length );
		void			setHttpAccept( std::string const http_accept );
		void			setHttpAcceptLanguage( std::string const http_languange );
		void			setHttpUserAgent( std::string const http_user );
		void			setHttpCookie( std::string const http_cookie );
		void			setHttpReferer( std::string const http_referer );
		void			setRequestUri( std::string const request_uri );

		/*
		**	Getters
		*/
		std::string		get_body_string_post( void ) const ;

		std::string		getScriptFilename( void ) const;
		std::string		getRedirectStatus( void ) const;
		std::string		getStatusCode( void ) const;
		std::string		getRequestUri( void ) const;
		std::string		getServerSoftware( void ) const;
		std::string		getServerName( void ) const;
		std::string		getGatewayInterface( void ) const;
		std::string		getServerProtocol( void ) const;
		std::string		getServerPort( void ) const;
		std::string		getRequestMethod( void ) const;
		std::string		getPathInfo( void ) const;
		std::string		getPathTranslated( void ) const;
		std::string		getScriptName( void ) const;
		std::string		getQueryString( void ) const;
		std::string		getRemoteHost( void ) const;
		std::string		getRemoteAddr( void ) const;
		std::string		getAuthType( void ) const;
		std::string		getRemoteUser( void ) const;
		std::string		getRemoteIdent( void ) const;
		std::string		getContentType( void ) const;
		std::string		getContentLength( void ) const;
		std::string		getHttpAccept( void ) const;
		std::string		getHttpAcceptLanguage( void ) const;
		std::string		getHttpUserAgent( void ) const;
		std::string		getCookie( void ) const;
		std::string		getReferer( void ) const;


		// a supprimer
		void			ft_test( void );
		void			ft_display_all_variable_env( void );

	private:
		std::map<std::string, std::string>		_env_cgi;

		std::string								_cgi_string; // c'est le retour, la string contenant toutes les donnees
		std::string								_body_string_post;

};

#endif