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

// A changer
bool			HttpServer::ft_check_cgi_or_php( std::string request_http )
{
	std::cout << RED "dans la fonction find  php pour cgi : A REVOIR " << CLEAR << std::endl;
	// on trouve le premier /
	size_t 		find_backslash = request_http.find("/");
	// on cherche HTTP
	size_t		find_http = request_http.find("HTTP");
	//	on cherche .php?
	size_t		find_php;
	if (this->_header_requete[0].method == "GET")
	{
		find_php = request_http.find(".php?");
		if (find_php == std::string::npos)
			find_php = request_http.find(".php");
		if (find_php == std::string::npos)
			return (false);
	}
	if (this->_header_requete[0].method == "POST")
		find_php = request_http.find(".php");


	// si .php est entre / et HTTP c'est good, sinon erruer
	if (find_php > find_backslash && find_php < find_http)
	{
		std::cout << " good on a bien du php dans la requete qu'il faut utiliser avec cgi" << std::endl;
		this->_header_requete[0].cgi = true;
		return (true);
	}
	else
	{
		// on verifie si on a seulement du cgi
		size_t	find_cgi = request_http.find(".cgi");
		if (find_cgi > find_backslash && find_cgi < find_http)
		{
			std::cout << " good on a bien du cgi dans la requete qu'il faut utiliser avec cgi" << std::endl;
			this->_header_requete[0].cgi = true;
			return (true);
		}
		if (this->_header_requete[0].content_type.compare(0, 30, "multipart/form-data; boundary=") == 0)
		{
			std::cout << "IL Y A DU boundary ET IL FAUT UPLOADER" << std::endl;
			this->_header_requete[0].upload = true;
			return (true);
		}
		if (this->_recv_complete.chunked == true)
		{
			std::cout << "Transfer chunked" << std::endl;
			this->_header_requete[0].upload = true;
			return (true);
		}
		std::cout << "non on a pas de php ou de cgi" << std::endl;
		return (false);
	}
	return (false);
}



void			HttpServer::ft_exec_cgi_test( void )
{
	std::cout << GREEN << "\n\nDANS exec CGI ... " << CLEAR << std::endl;


	// std::cout << "method = " << this->_header_requete[0].method << std::endl;
	// std::cout << "path = " << this->_header_requete[0].path << std::endl;
	// std::cout << "protocol = " << this->_header_requete[0].protocol << std::endl;
	// std::cout << "host = " << this->_header_requete[0].host << std::endl;
	// std::cout << "request_uri = " << this->_header_requete[0].request_uri << std::endl;
	// std::cout << "script name = " << this->_header_requete[0].script_file_name << std::endl;
	// std::cout << "accept = " << this->_header_requete[0].accept << std::endl;
	// std::cout << "path_http = " << this->_header_requete[0].path_http << std::endl;
	// std::cout << "query_string = " << this->_header_requete[0].query_string << std::endl;
	// std::cout << "cgi_return = " << this->_header_requete[0].cgi_return << std::endl;
	// std::cout << "cgi = " << this->_header_requete[0].cgi << std::endl;
	// std::cout << "error = " << this->_header_requete[0].error << std::endl;
	// std::cout << "num_error = " << this->_header_requete[0].num_error << std::endl;
	// std::cout << "body_error = " << this->_header_requete[0].body_error << std::endl;

	// // uselss ????? 
	// std::map<std::string, std::string>::iterator it = this->_header_requete[0].data.begin();
	// std::cout << "data = " << std::endl;
	// for (; it != this->_header_requete[0].data.end(); it++)
	// {
	// 	std::cout << "first = " << it->first << " et = " << it->second << std::endl;

	// }
	// exit(1);

	// Setup CGI's variables to NULL
	std::cout << "\n ON SETUP les variables de _env_cgi " << std::endl;
	this->_cgi->ft_setup_env_cgi();

	// on rempli les valeurd de l'env en fonction de la method
	if (this->_header_requete[0].method == "GET")
	{
		std::cout << "On est dans get " << std::endl;
		this->_cgi->setContentLength("");
		this->_cgi->setContentType("");
		this->_cgi->setGatewayInterface("CGI/1.1");
		this->_cgi->setHttpAccept(this->_header_requete[0].accept);
		this->_cgi->setPathInfo(this->_header_requete[0].path_http);
		this->_cgi->setPathTranslated(this->_header_requete[0].path_http);
		std::cout << "query string = " << this->_header_requete[0].query_string << std::endl;
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
		
		
		std::cout << GREEN << "\n\nMaintenant on utilise le CGI avec les donnees " << CLEAR << std::endl;
		
		std::string 	tmp = this->_header_requete[0].script_file_name;

		// need to be delete
		// this->_cgi->ft_display_all_variable_env();
		
		this->_header_requete[0].body_error = this->_cgi->ft_execute_cgi(this->_servers[this->_num_serv].cgi_path_server, tmp);
		// std::cerr << "ici " << strerror(errno) << std::endl;
		// 	exit(1);
		// std::cout << "retourne cgi =  " << this->_header_requete[0].body_error  << std::endl;
		// exit(1);
		return ;
	}
	else if (this->_header_requete[0].method == "POST")
	{
		std::cout << "On est dans Post " << std::endl;
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

		// pas necessaire
		this->_cgi->ft_display_all_variable_env();
		
		std::cout << GREEN << "\n\nMaintenant on utilise le CGI avec les donnees " << CLEAR << std::endl;
		
		// A CCHANGE RSUREENT
		// std::cout << "address cgi = " << this->_servers[0].cgi_path_server << std::endl;
		std::string tmp_2 = this->_header_requete[0].script_file_name;
		tmp_2.insert(0, this->_servers[0].root_server);
		// std::cout << "tmp = " << tmp << std::endl;

		std::cout << " AVANT EXECUTE CGI : " << std::endl;
		std::cout << "first arg = -" << this->_servers[0].cgi_path_server << "-" << std::endl;
		

		std::cout << "test tmp_2 = / " << std::endl;
		tmp_2 = "/";
		std::cout << "second arg = -" << tmp_2 << "-" << std::endl;
		// exit(1);
		this->_header_requete[0].body_error = this->_cgi->ft_execute_cgi(this->_servers[0].cgi_path_server, tmp_2);
		// std::cout << "BINGO ? = " << this->_header_requete[0].body_error << std::endl;
		
	}
	else
	{
		std::cout << "cgi maais pas de post ou get" << std::endl;
		exit(1);
	} 
	return ;
}

