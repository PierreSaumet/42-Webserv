/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServerCGI.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psaumet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/01 11:07:34 by psaumet           #+#    #+#             */
/*   Updated: 2021/06/01 11:07:42 by psaumet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Headers/HttpServer.hpp"

/*
**	bool	HttpServer::ft_check_cgi_or_php( void )
**		This function will check if the path of the request contains .php.
**	If yes, we need to use the php-cgi and return true. Otherwise return false.
*/
bool			HttpServer::ft_check_cgi_or_php( void )
{
	size_t		find_php;
	if (this->_header_requete[0].method == "GET")
	{
		find_php = this->_header_requete[0].path.find(".php?");
		if (find_php != std::string::npos)
			return (true);
		find_php = this->_header_requete[0].path.find(".php");
		if (find_php != std::string::npos)
			return (true);
		return (false);
	}
	if (this->_header_requete[0].method == "POST")
	{
		find_php = this->_header_requete[0].path.find(".php");
		if (find_php == this->_header_requete[0].path.size() - 4)
			return (true);
		return (false);
	}
	return (false);
}

/*
**	HttpServer::ft_exec_cgi_test( void )
**		This function will setup all variables needed for the CGI in function of the method request (GET or POST)
**	and then it will use the cgi.
*/
void			HttpServer::ft_exec_cgi_test( void )
{
	this->_cgi->ft_setup_env_cgi();
	if (this->_header_requete[0].method == "GET")
	{
		this->_cgi->setContentLength("");
		this->_cgi->setContentType("");
		this->_cgi->setGatewayInterface("CGI/1.1");
		this->_cgi->setHttpAccept(this->_header_requete[0].accept);
		this->_cgi->setPathInfo(this->_header_requete[0].path_http);
		this->_cgi->setPathTranslated(this->_header_requete[0].path_http);
		this->_cgi->setQueryString(this->_header_requete[0].query_string);
		this->_cgi->setRedirectStatus("200");
		this->_cgi->setStatusCode("200");
		this->_cgi->setRequestMethod(this->_header_requete[0].method);
		this->_cgi->setServerSoftware("Webserv/1.0");
		this->_cgi->setServerProtocol("HTTP/1.1");
		this->_cgi->setScriptName( this->_header_requete[0].script_file_name);
		this->_cgi->setScriptFileName( this->_header_requete[0].script_file_name);
		this->_cgi->setRequestUri(this->_header_requete[0].request_uri);
		this->_cgi->setServerName(this->_servers[this->_num_serv].name_server);
		
		std::string 	tmp = this->_header_requete[0].script_file_name;
		this->_header_requete[0].body_error = this->_cgi->ft_execute_cgi(this->_servers[this->_num_serv].cgi_path_server, tmp);
		if (this->_header_requete[0].body_error == "")
		{
			this->_header_requete[0].error = true;
			this->_header_requete[0].num_error = 500;
		}
		return ;
	}
	else if (this->_header_requete[0].method == "POST")
	{
		this->_cgi->setGatewayInterface("CGI/1.1");
		this->_cgi->setHttpAccept(this->_header_requete[0].accept);
		this->_cgi->setPathInfo(this->_header_requete[0].path_http);
		this->_cgi->setPathTranslated(this->_header_requete[0].path_http);
		this->_cgi->setRedirectStatus("200");
		this->_cgi->setStatusCode("200");
		this->_cgi->setRequestMethod(this->_header_requete[0].method);
		this->_cgi->setServerSoftware("Webserv/1.0");
		this->_cgi->setServerProtocol("HTTP/1.1");
		this->_cgi->setServerName(this->_servers[this->_num_serv].name_server);
		this->_cgi->setScriptName(this->_header_requete[0].script_file_name);
		this->_cgi->setScriptFileName(this->_header_requete[0].script_file_name);
		this->_cgi->setRequestUri("");
		this->_cgi->setContentLength(this->_header_requete[0].content_length);
		this->_cgi->setContentType(this->_header_requete[0].content_type);
		this->_cgi->setRequestUri("");
		this->_cgi->set_body_string_post(this->_header_requete[0].body_post);
		if (this->_recv_complete.chunked == true)
		{
			std::stringstream ss;
			ss << this->_header_requete[0].body_post.size();
			std::string s = ss.str();
			this->_cgi->setContentLength(s);
		}
		std::string 	tmp = this->_header_requete[0].script_file_name;
		tmp.insert(0, this->_servers[this->_num_serv].root_server);
		tmp = "/";

		this->_header_requete[0].body_error = this->_cgi->ft_execute_cgi(this->_servers[this->_num_serv].cgi_path_server, tmp);
		if (this->_header_requete[0].body_error == "")
		{
			this->_header_requete[0].error = true;
			this->_header_requete[0].num_error = 500;
		}
	}
	else
	{
		this->_header_requete[0].error = true;
		this->_header_requete[0].num_error = 500;
	}
	return ;
}

