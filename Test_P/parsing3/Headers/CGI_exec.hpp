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
		std::string		ft_execute_cgi( void );

		/*
		**	Setters
		*/
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
		void			setContentTypes( std::string const content_type );
		void			setContentLength( std::string const content_length );
		void			setHttpAccept( std::string const http_accept );
		void			setHttpAcceptLanguage( std::string const http_languange );
		void			setHttpUserAgent( std::string const http_user );
		void			setHttpCookie( std::string const http_cookie );
		void			setHttpReferer( std::string const http_referer );

		/*
		**	Getters
		*/
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
		std::string		getContentTypes( void ) const;
		std::string		getContentLength( void ) const;
		std::string		getHttpAccept( void ) const;
		std::string		getHttpAcceptLanguage( void ) const;
		std::string		getHttpUserAgent( void ) const;
		std::string		getCookie( void ) const;
		std::string		getReferer( void ) const;

		void			ft_display_all_variable_env( void );

	private:
		std::map<std::string, std::string>		_env_cgi;

};

#endif