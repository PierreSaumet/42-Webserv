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

/*
**	Default's constructor
*/
Cgi_exec::Cgi_exec( void ) : _env_cgi(), _cgi_string(), _body_string_post()
{
	return ;
}

/*
**	Copy constructor
*/
Cgi_exec::Cgi_exec( const Cgi_exec &copy ) : _env_cgi(copy._env_cgi), _cgi_string(copy._cgi_string), _body_string_post(copy._body_string_post)
{
	return ;
}

/*
**	Destructor
*/
Cgi_exec::~Cgi_exec( void )
{
	if (this->_env_cgi.empty() == 0)
	{
		this->_env_cgi.clear();
	}
	return ;
}

/*
**	Overload operator =
*/
Cgi_exec		&Cgi_exec::operator=( const Cgi_exec &element )
{
	if (this != &element)
	{
		this->_env_cgi = element._env_cgi;
		this->_cgi_string = element._cgi_string;
		this->_body_string_post = element._body_string_post;
	}
	return (*this);
}

/*
**	size_t	Cgi_exec::ft_setup_env_cgi( void )
**			This function initializes 'this->_env_cg' which is a map's container with all variables
**			(std::string, std::string) needed for the cgi.
*/
void			Cgi_exec::ft_setup_env_cgi( void )
{
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
	this->_env_cgi.insert(std::pair<std::string, std::string>("REMOTE_USER", ""));
	this->_env_cgi.insert(std::pair<std::string, std::string>("REMOTE_IDENT", ""));
	this->_env_cgi.insert(std::pair<std::string, std::string>("CONTENT_TYPE", ""));
	this->_env_cgi.insert(std::pair<std::string, std::string>("CONTENT_LENGTH", ""));
	this->_env_cgi.insert(std::pair<std::string, std::string>("REQUEST_URI", ""));
	this->_env_cgi.insert(std::pair<std::string, std::string>("STATUS_CODE", ""));
	this->_env_cgi.insert(std::pair<std::string, std::string>("REDIRECT_STATUS", ""));
	this->_env_cgi.insert(std::pair<std::string, std::string>("HTTP_ACCEPT", ""));
	return ;
}

/*
**	std::string	Cgi_exec::ft_execute_cgi( std::string path_cgi, std::string path_file )
**			This function will use the CGI /usr/bin/php-cgi and return a string
**			which contains all the data given by the php-cgi binary.
**				
*/
std::string	Cgi_exec::ft_execute_cgi( std::string path_cgi, std::string path_file )
{
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
	}
	sysCline[aArgs.size()] = NULL;

	aEnv = this->ft_convert_map_to_vector();

	sysEnv = new char*[aEnv.size() + 1];
	for (unsigned long i = 0; i < aEnv.size(); i++)
	{
		sysEnv[i] = new char[aEnv[i].size() + 1];
		strncpy(sysEnv[i], aEnv[i].c_str(), aEnv[i].size() + 1);
	}
	sysEnv[aEnv.size()] = NULL;

	pid_t 					pid;
	int 					stdin_tmp = dup(STDIN_FILENO);
	int						stdout_tmp = dup(STDOUT_FILENO);

	FILE *file_in = tmpfile();
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
		if (execve(sysCline[0], sysCline, sysEnv) == -1)
			return ("");
	}
	else
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
**	std::string		Cgi_exec::ft_return_string_cgi( void )
**			Return 'this->_cgi_string' which contains the string of the cgi
**			and clear it.
*/
std::string		Cgi_exec::ft_return_string_cgi( void )
{
	std::string tmp = this->_cgi_string;

	this->_cgi_string.clear();
	return (tmp);
}


/*
**	void			Cgi_exec::ft_add_to_cgi_string( unsigned char *buffer, int ret )
**			This function add the buffer from the CGI to 'this->_cgi_string'
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

/*
**	std::vector<std::string>	Cgi_exec::ft_convert_map_to_vector( void )
**			This function will convert the data from the map container 'this->_env_cgi'
**			to a vector container and return it.
*/
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


/*
**	GETTERS
*/

std::string		Cgi_exec::get_body_string_post( void ) const 
{
	return (this->_body_string_post);
}

