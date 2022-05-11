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
	// setup all data to null ... 
	//https://fr.wikipedia.org/wiki/Variables_d%27environnement_CGI
	this->_env_cgi.insert(std::pair<std::string, std::string>("SERVER_SOFTWARE", ""));
	this->_env_cgi.insert(std::pair<std::string, std::string>("SERVER_NAME", ""));
	this->_env_cgi.insert(std::pair<std::string, std::string>("GATEWAY_INTERFACE", ""));
	this->_env_cgi.insert(std::pair<std::string, std::string>("SERVER_PROTOCOL", ""));
	this->_env_cgi.insert(std::pair<std::string, std::string>("SERVER_PORT", ""));
	this->_env_cgi.insert(std::pair<std::string, std::string>("REQUEST_METHOD", ""));
	this->_env_cgi.insert(std::pair<std::string, std::string>("PATH_INFO", ""));
	this->_env_cgi.insert(std::pair<std::string, std::string>("PATH_TRANSLATED", ""));
	this->_env_cgi.insert(std::pair<std::string, std::string>("SCRIPT_NAME", ""));
	this->_env_cgi.insert(std::pair<std::string, std::string>("SCRIPT_FILENAME", ""));
	this->_env_cgi.insert(std::pair<std::string, std::string>("QUERY_STRING", ""));
	this->_env_cgi.insert(std::pair<std::string, std::string>("REMOTE_ADDR", ""));
	// this->_env_cgi.insert(std::pair<std::string, std::string>("AUTH_TYPE", "")); // inutile ?
	this->_env_cgi.insert(std::pair<std::string, std::string>("REMOTE_USER", ""));
	this->_env_cgi.insert(std::pair<std::string, std::string>("REMOTE_IDENT", ""));
	this->_env_cgi.insert(std::pair<std::string, std::string>("CONTENT_TYPE", ""));
	this->_env_cgi.insert(std::pair<std::string, std::string>("CONTENT_LENGTH", ""));
	this->_env_cgi.insert(std::pair<std::string, std::string>("REQUEST_URI", ""));
	this->_env_cgi.insert(std::pair<std::string, std::string>("STATUS_CODE", ""));
	this->_env_cgi.insert(std::pair<std::string, std::string>("REDIRECT_STATUS", ""));
	this->_env_cgi.insert(std::pair<std::string, std::string>("HTTP_ACCEPT", ""));


	// PAS OBLIGE JE CROIS... DOIT METTRE VIDE CAD LE SUPPRIMER
	this->_env_cgi.insert(std::pair<std::string, std::string>("REMOTE_HOST", ""));
	

	// PAS NECESSAIRE JE CROIS
	
	this->_env_cgi.insert(std::pair<std::string, std::string>("HTTP_ACCEPT_LANGUAGE", ""));
	this->_env_cgi.insert(std::pair<std::string, std::string>("HTTP_USER_AGENT", ""));
	this->_env_cgi.insert(std::pair<std::string, std::string>("HTTP_COOKIE", ""));
	this->_env_cgi.insert(std::pair<std::string, std::string>("HTTP_REFERER", ""));

	//	A rajouter un status code ? je ne sais pas si ca existe mais Jerome m'a dit que oui
	//this->_env_cgi.insert(std::pair<std::string, std::string>("REDIRECT_STATUS", "NULL"));

	// std::cout << "this->_env_cgi contains : " << std::endl;
	// std::map<std::string, std::string>::const_iterator		it_b = this->_env_cgi.begin();
	// std::map<std::string, std::string>::const_iterator		it_e = this->_env_cgi.end();
	// for ( ; it_b != it_e; it_b++)
	// {
	// 	std::cout << "it_b->first = " << it_b->first << " and it_b->second = " << it_b->second << std::endl;
	// }
	return (0);
}

/*
**	pour une requete GET il nous faut
OK	CONTENT_LENGTH = vide ?
OK	CONTENT_TYPE = vide ?
OK	GATEWAY_INTERFACE = CGI/1.1
OK	HTTP_accept = valeur du header:
		exemple = text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*//*;q=0.8
OK	PATH_INFO = le chemin du fichier php:
		exemple = /home/pierre/dossier/webser/test/parsing3/root/Hello/fichier.php
OK	PATH_TRANSLATED = pareil que path_info
		exemple = /home/pierre/dossier/webser/test/parsing3/root/Hello/fichier.php
OK	QUERY_STRING = les valeurs qui proviennet de l'url car get
		exemple = name=SAUMET&prenom=PIERRE
OK	STATUS_CODE = 200 comme redirect status	
OK	SERVER_PROTOCOl = HTTP/1.1
OK	SERVER_SOFTWARE = webserv/1.0 enfin notre server


OK	REDIRECT_STATUS = 200  ==> verifier avant que les fichiers sont bons et les donnees le sont egalement
OK	REQUEST_METHOD = GET


OK	REQUEST_URI = l'addresse du fichier + les info
		exemple = /Hello/query_get_test.php?name=SAUMET
	SCRIPT_FILENAME = nom du fichier
		exemple = query_test_get.php
OK	SCRIPT_NAME = pareil que script_filemane
		exemple = query_test_get.php

 pas obligatoire ?
OK	SERVER_NAME = le nom du server
	SERVER_PORT = le port du server ou on ecoute



	et le write on lui donne une string vide. je pense
*/

/*
**	pour une requete POST il nous faut
	CONTENT_LENGTH = la valeur du body recu enfin la taille 
		exemple = 6 pour pierre
	CONTENT_TYPE = valeur qu'on trovue dans le header de la requete
		exemple = application/x-www-form-urlencoded
	GATEWAY_INTERFACE = CGI/1.1
	HTTP_ACCEPT = valeur qu'on trouve dans le header de la requete
		exemple = text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*//*;q=0.8
	PATH_INFO = pareil que pour GET
	PATH_TRANSLATED = pareil que pour GEt
	REDIRECT_STTUS = 200
	REQUEST_METHOD = POST
	REQUEST_URO = nom du fichier suelement
		exemple = /fichier.php
	SCRIPT_NAME = nom du fichier
		exemple = /fichier.php
	SERVER_NAME	= le nom du server
	SERVEr_PORt = le ort du server
	SERVER_PROTOCOL = HTTP/1.1
	SERVER_SOFTWARE = bah server webserv
	STATTUS _CODE = 200

	et pour le write, il faut lui transmettre les donnees qu'on a recupere
	genre = name=pierre?prenom=paindemie
*/


std::string	Cgi_exec::ft_execute_cgi( std::string path_cgi, std::string path_file )
{
	std::cout << GREEN << "\n On est dans ft_execute_cgi : " << CLEAR << std::endl;
	std::cout << "path_Cgi = " << path_cgi << std::endl;
	std::cout << "pathc file = " << path_file << std::endl;
	char	**sysCline = NULL;
	char	**sysEnv = NULL;
	std::vector<std::string>		aArgs;
	std::vector<std::string>		aEnv;

	aArgs.push_back(path_cgi);
	aArgs.push_back(path_file);

	sysCline = new char*[aArgs.size() + 1];
	for (unsigned long i = 0; i < aArgs.size(); i++)
	{
		sysCline[i] = new char[aArgs[i].size() + 1];
		strncpy(sysCline[i], aArgs[i].c_str(), aArgs[i].size() + 1);
		std::cout << "SYScline = " << sysCline[i] << std::endl;
	}
	sysCline[aArgs.size()] = NULL;

	aEnv = this->ft_convert_map_to_vector();

	sysEnv = new char*[aEnv.size() + 1];
	for (unsigned long i = 0; i < aEnv.size(); i++)
	{
		sysEnv[i] = new char[aEnv[i].size() + 1];
		strncpy(sysEnv[i], aEnv[i].c_str(), aEnv[i].size() + 1);
		std::cout << "Sysenv = " << sysEnv[i] << std::endl;
	}
	sysEnv[aEnv.size()] = NULL;

	// sleep(5);

	pid_t pid;
	int stdin_tmp = dup(STDIN_FILENO);
	int stdout_tmp = dup(STDOUT_FILENO);

	FILE *file_in = tmpfile();		// cree un fichier temporaire dans le cas de grosse donnees je pense
	if (file_in == NULL)
		exit(1);
	FILE *file_out = tmpfile();
	if (file_out == NULL)
		exit(1);
	long fd_in = fileno(file_in);
	if (fd_in == -1)
		exit(1);
	long fd_out = fileno(file_out);
	if (fd_out == -1)
		exit(1);

	write(fd_in, this->_body_string_post.c_str(), this->_body_string_post.size());
	
	//	On replace le curseur de lecture de fd_in au debut
	lseek(fd_in, 0, SEEK_SET);

	pid = fork();
	if (pid < 0)
		exit(1);
	else if (pid == 0)
	{
		if (dup2(fd_in, STDIN_FILENO) == -1)
			exit(1);
		if (dup2(fd_out, STDOUT_FILENO) == -1)
			exit(1);
		// on execute le cgi avec execve
		if (execve(sysCline[0], sysCline, sysEnv) == -1)
			return ("");
	}
	else // parenmt
	{
		unsigned char		buffer[2000] = {0};
		int 				ret = 1;
		
		waitpid(-1, NULL, 0);
		lseek(fd_out, 0, SEEK_SET);
		while (ret > 0)
		{
			memset(buffer, 0, 2000);
			ret = read(fd_out, buffer, 2000 - 1);
			buffer[ret] = 0;
			if (ret > 0)
			{
				this->ft_add_to_cgi_string(buffer, ret );
			}
		}

	}

	dup2(stdin_tmp, STDIN_FILENO);
	dup2(stdout_tmp, STDOUT_FILENO);
	close(stdin_tmp);
	close(stdout_tmp);
	fclose(file_in);
	fclose(file_out);

	for (unsigned long i = 0; i < aEnv.size(); i++)
	{
		delete [] sysEnv[i];
	}
	delete [] sysEnv;

	for (unsigned long i = 0; i < aArgs.size(); i++)
	{
		delete [] sysCline[i];
	}
	delete [] sysCline;
	return (this->ft_return_string_cgi());
}

/*
**	Return this->_cgi_string which contains the string of the cgi
**	and clear it.
*/
std::string		Cgi_exec::ft_return_string_cgi( void )
{
	std::string tmp = this->_cgi_string;

	this->_cgi_string.clear();
	return (tmp);
}


/*
**	This function add he buffer from the CGI to this->_cgi_string
*/
void		Cgi_exec::ft_add_to_cgi_string( unsigned char *buffer, int ret )
{

	int i = 0;

	while (i < ret)
	{
		this->_cgi_string += buffer[i];
		i++;
	}
	return ;
}


std::vector<std::string>		Cgi_exec::ft_convert_map_to_vector( void )
{
	std::vector<std::string>	tmp;

	std::map<std::string, std::string>::iterator 	it = this->_env_cgi.begin();
	for (; it != this->_env_cgi.end(); it++)
	{
		std::string string = "";
		string = it->first;
		string.append("=");
		string.append(it->second);
		std::cout << "aEnv = " << string << std::endl;
		tmp.push_back(string);

	}
	return (tmp);
}

/*
**	SETTERS
*/
void	Cgi_exec::set_body_string_post( std::string const body_post )
{
	this->_body_string_post = body_post;
}


void	Cgi_exec::setScriptFileName( std::string const script_filename )
{
	this->_env_cgi["SCRIPT_FILENAME"] = script_filename;
	return ;
}

void	Cgi_exec::setRedirectStatus( std::string const redirect_status )
{
	this->_env_cgi["REDIRECT_STATUS"] = redirect_status;
	return ;
}

void	Cgi_exec::setStatusCode( std::string const status_code )
{
	this->_env_cgi["STATUS_CODE"] = status_code;
	return ;
}

void	Cgi_exec::setRequestUri( std::string const request_uri )
{
	this->_env_cgi["REQUEST_URI"] = request_uri;
	return ;
}
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

void	Cgi_exec::setRemoteHost( std::string const remote_host )
{
	this->_env_cgi["REMOTE_HOST"] = remote_host;
	return ;
}
void	Cgi_exec::setRemoteADDR( std::string const remote_addr )
{
	this->_env_cgi["REMOTE_ADDR"] = remote_addr;
	return ;
}
void	Cgi_exec::setAuthType( std::string const auth_type )
{
	this->_env_cgi["AUTH_TYPE"] = auth_type;
	return ;
}
void	Cgi_exec::setRemoteUser( std::string const remote_user )
{
	this->_env_cgi["REMOTE_USER"] = remote_user;
	return ;
}
void	Cgi_exec::setRemoteIdent( std::string const remote_ident )
{
	this->_env_cgi["REMOTE_IDENT"] = remote_ident;
	return ;
}
void	Cgi_exec::setContentType( std::string const content_type )
{
	this->_env_cgi["CONTENT_TYPE"] = content_type;
	return ;
}
void	Cgi_exec::setContentLength( std::string const content_length )
{
	this->_env_cgi["CONTENT_LENGTH"] = content_length;
	return ;
}
void	Cgi_exec::setHttpAccept( std::string const http_accept )
{
	this->_env_cgi["HTTP_ACCEPT"] = http_accept;
	return ;
}
void	Cgi_exec::setHttpAcceptLanguage( std::string const http_languange )
{
	this->_env_cgi["HTTP_ACCEPT_LANGUAGE"] = http_languange;
	return ;
}
void	Cgi_exec::setHttpUserAgent( std::string const http_user )
{
	this->_env_cgi["HTTP_USER_AGENT"] = http_user;
	return ;
}
void	Cgi_exec::setHttpCookie( std::string const http_cookie )
{
	this->_env_cgi["HTTP_COOKIE"] = http_cookie;
	return ;
}
void	Cgi_exec::setHttpReferer( std::string const http_referer )
{
	this->_env_cgi["HTTP_REFERER"] = http_referer;
	return ;
}

/*
**	GETTERS
*/
std::string		Cgi_exec::get_body_string_post( void ) const 
{
	return (this->_body_string_post);
}


std::string		Cgi_exec::getScriptFilename( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "SCRIPT_FILENAME")
			return (it_b->second);
	}
	return ("");
}

std::string		Cgi_exec::getRedirectStatus( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "REDIRECT_STATUS")
			return (it_b->second);
	}
	return ("");
}

std::string		Cgi_exec::getStatusCode( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "STATUS_CODE")
			return (it_b->second);
	}
	return ("");
}

std::string Cgi_exec::getRequestUri( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "REQUEST_URI")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getServerSoftware( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "SERVER_SOFTWARE")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getServerName( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "SERVER_NAME")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getGatewayInterface( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "GATEWAY_INTERFACE")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getServerProtocol( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "SERVER_PROTOCOL")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getServerPort( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "SERVER_PORT")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getRequestMethod( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "REQUEST_METHOD")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getPathInfo( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "PATH_INFO")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getPathTranslated( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "PATH_TRANSLATED")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getScriptName( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "SCRIPT_NAME")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getQueryString( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "QUERY_STRING")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getRemoteHost( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "REMOTE_HOST")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getRemoteAddr( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "REMOTE_ADDR")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getAuthType( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "AUTH_TYPE")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getRemoteUser( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "REMOTE_USER")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getRemoteIdent( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "REMOTE_IDENT")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getContentType( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "CONTENT_TYPE")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getContentLength( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "CONTENT_LENGTH")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getHttpAccept( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "HTTP_ACCEPT")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getHttpAcceptLanguage( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "HTTP_ACCEPT_LANGUAGE")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getHttpUserAgent( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "HTTP_USER_AGENT")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getCookie( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "HTTP_COOKIE")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getReferer( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "HTTP_REFERER")
			return (it_b->second);
	}
	return ("");
}





void	Cgi_exec::ft_display_all_variable_env( void )
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		std::cout << it_b->first << " = " << it_b->second << std::endl;
	}
	return ;
}