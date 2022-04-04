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

#include "../Headers/CGI_exec.hpp"

Cgi_exec::Cgi_exec( void ) : _env_cgi()
{
	return ;
}

Cgi_exec::Cgi_exec( const Cgi_exec &copy ) : _env_cgi(copy._env_cgi)
{
	return ;
}

Cgi_exec::~Cgi_exec( void )
{
	if (this->_env_cgi.empty() == 0)
	{
		this->_env_cgi.clear();
	}
	return ;
}

Cgi_exec		&Cgi_exec::operator=( const Cgi_exec &element )
{
	if (this != &element)
	{
		this->_env_cgi = element._env_cgi;
	}
	return (*this);
}

size_t			Cgi_exec::ft_setup_env_cgi( void )
{
	// setup all data...
	//https://fr.wikipedia.org/wiki/Variables_d%27environnement_CGI
	this->_env_cgi.insert(std::pair<std::string, std::string>("SERVER_SOFTWARE", "NULL"));
	this->_env_cgi.insert(std::pair<std::string, std::string>("SERVER_NAME", "NULL"));
	this->_env_cgi.insert(std::pair<std::string, std::string>("GATEWAY_INTERFACE", "NULL"));
	this->_env_cgi.insert(std::pair<std::string, std::string>("SERVER_PROTOCOL", "NULL"));
	this->_env_cgi.insert(std::pair<std::string, std::string>("SERVER_PORT", "NULL"));
	this->_env_cgi.insert(std::pair<std::string, std::string>("REQUEST_METHOD", "NULL"));
	this->_env_cgi.insert(std::pair<std::string, std::string>("PATH_INFO", "NULL"));
	this->_env_cgi.insert(std::pair<std::string, std::string>("PATH_TRANSLATED", "NULL"));
	this->_env_cgi.insert(std::pair<std::string, std::string>("SCRIPT_NAME", "NULL"));
	this->_env_cgi.insert(std::pair<std::string, std::string>("QUERY_STRING", "NULL"));
	// PAS OBLIGE JE CROIS... DOIT METTRE VIDE CAD LE SUPPRIMER
	this->_env_cgi.insert(std::pair<std::string, std::string>("REMOTE_HOST", "NULL"));
	
	this->_env_cgi.insert(std::pair<std::string, std::string>("REMOTE_ADDR", "NULL"));
	this->_env_cgi.insert(std::pair<std::string, std::string>("AUTH_TYPE", "NULL"));
	this->_env_cgi.insert(std::pair<std::string, std::string>("REMOTE_USER", "NULL"));
	this->_env_cgi.insert(std::pair<std::string, std::string>("REMOTE_IDENT", "NULL"));
	this->_env_cgi.insert(std::pair<std::string, std::string>("CONTENT_TYPES", "NULL"));
	this->_env_cgi.insert(std::pair<std::string, std::string>("CONTENT_LENGTH", "NULL"));

	// PAS NECESSAIRE JE CROIS
	this->_env_cgi.insert(std::pair<std::string, std::string>("HTTP_ACCEPT", "NULL"));
	this->_env_cgi.insert(std::pair<std::string, std::string>("HTTP_ACCEPT_LANGUAGE", "NULL"));
	this->_env_cgi.insert(std::pair<std::string, std::string>("HTTP_USER_AGENT", "NULL"));
	this->_env_cgi.insert(std::pair<std::string, std::string>("HTTP_COOKIE", "NULL"));
	this->_env_cgi.insert(std::pair<std::string, std::string>("HTTP_REFERER", "NULL"));



	std::cout << "this->_env_cgi contains : " << std::endl;
	std::map<std::string, std::string>::iterator		it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::iterator		it_e = this->_env_cgi.end();
	for ( ; it_b != it_e; it_b++)
	{
		std::cout << "it_b->first = " << it_b->first << " and it_b->second = " << it_b->second << std::endl;
	}
	return (0);
}

/*
**	SETTERS
*/
void	Cgi_exec::setServerSoftware( std::string const software )
{
	this->_env_cgi["SERVER_SOFTWARE"] = software;
	return ;
}

void	Cgi_exec::setServerName( std::string const name )
{
	this->_env_cgi["SERVER_NAME"] = name;
	return ;
}
void	Cgi_exec::setGatewayInterface( std::string const gateway )
{
	this->_env_cgi["GATEWAY_INTERFACE"] = gateway;
	return ;
}
void	Cgi_exec::setServerProtocol( std::string const protocol )
{
	this->_env_cgi["SERVER_PROTOCOL"] = protocol;
	return ;
}
void	Cgi_exec::setServerPort( std::string const port )
{
	this->_env_cgi["SERVER_PORT"] = port;
	return ;
}
void	Cgi_exec::setRequestMethod( std::string const method )
{
	this->_env_cgi["REQUEST_METHOD"] = method;
	return ;
}
void	Cgi_exec::setPathInfo( std::string const path_info )
{
	this->_env_cgi["PATH_INFO"] = path_info;
	return ;
}
void	Cgi_exec::setPathTranslated( std::string const path_trans )
{
	this->_env_cgi["PATH_TRANSLATED"] = path_trans;
	return ;
}
void	Cgi_exec::setScriptName( std::string const script_name )
{
	this->_env_cgi["SCRIPT_NAME"] = script_name;
	return ;
}
void	Cgi_exec::setQueryString( std::string const query_string )
{
	this->_env_cgi["QUERY_STRING"] = query_string;
	return ;
}

/*
**	GETTERS
*/
std::string	Cgi_exec::getServerSoftware( void )
{
	std::map<std::string, std::string>::iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "SERVER_SOFTWARE")
			return (it_b->second);
	}
	return ("");
}


void	Cgi_exec::ft_display_all_variable_env( void )
{
	std::map<std::string, std::string>::iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		std::cout << it_b->first << " = " << it_b->second << std::endl;
	}
	return ;
}