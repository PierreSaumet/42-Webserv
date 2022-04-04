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
		Cgi_exec( void );
		Cgi_exec( const Cgi_exec &copy );
		~Cgi_exec( void );

		Cgi_exec		&operator=( const Cgi_exec &element );

		size_t			ft_setup_env_cgi( void );

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

		std::string		getServerSoftware( void );
		std::string		getServerName( void );
		std::string		getGatewayInterface( void );
		std::string		getServerProtocol( void );
		std::string		getServerPort( void );
		std::string		getRequestMethod( void );
		std::string		getPathInfo( void );
		std::string		getPathTranslated( void );
		std::string		getScriptName( void );
		std::string		getQueryString( void );
		std::string		getRemoteHost( void );
		std::string		getRemoteAddr( void );
		std::string		getAuthType( void );
		std::string		getRemoteUser( void );
		std::string		getRemoteIdent( void );
		std::string		getContentTypes( void );
		std::string		getContentLength( void );
		std::string		getHttpAccept( void );
		std::string		getHttpAcceptLanguage( void );
		std::string		getHttpUserAgent( void );
		std::string		getCookie( void );
		std::string		getReferer( void );

		void			ft_display_all_variable_env( void );

	private:
		std::map<std::string, std::string>		_env_cgi;

};

#endif