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
**	size_t		ft_check_cgi_or_php( std::string request_http, int len_msg )
**		This function will simply check the presence or absence in the request path
**		of 'cgi' or 'php'.
**
**		If there is, it returns true, otherwise returns false.
*/
bool			HttpServer::ft_check_cgi_or_php( std::string request_http )
{
	std::cout << "dans la fonction find cgi or php " << std::endl;
	// on trouve le premier /
	size_t 		find_backslash = request_http.find("/");
	// on cherche HTTP
	size_t		find_http = request_http.find("HTTP");
	//	on cherche .php?
	size_t		find_php;
	if (this->_header_requete[0].method == "GET")
		find_php = request_http.find(".php?");
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



void			HttpServer::ft_exec_cgi_test( std::string request_http, int len_msg )
{
	std::cout << GREEN << "\n\nDANS exec CGI ... " << CLEAR << std::endl;
	(void)request_http;
	// std::cout << "request_http = " << request_http << std::endl;
	std::cout << "longueur msg = " << len_msg << "\n\n" << std::endl;

	std::cout << "method = " << this->_header_requete[0].method << std::endl;
	std::cout << "path = " << this->_header_requete[0].path << std::endl;
	std::cout << "protocol = " << this->_header_requete[0].protocol << std::endl;
	std::cout << "host = " << this->_header_requete[0].host << std::endl;
	std::cout << "request_uri = " << this->_header_requete[0].request_uri << std::endl;
	std::cout << "script name = " << this->_header_requete[0].script_file_name << std::endl;
	std::cout << "accept = " << this->_header_requete[0].accept << std::endl;
	std::cout << "path_http = " << this->_header_requete[0].path_http << std::endl;
	std::cout << "query_string = " << this->_header_requete[0].query_string << std::endl;
	std::cout << "cgi_return = " << this->_header_requete[0].cgi_return << std::endl;
	std::cout << "cgi = " << this->_header_requete[0].cgi << std::endl;
	std::cout << "error = " << this->_header_requete[0].error << std::endl;
	std::cout << "num_error = " << this->_header_requete[0].num_error << std::endl;
	std::cout << "body_error = " << this->_header_requete[0].body_error << std::endl;

	std::map<std::string, std::string>::iterator it = this->_header_requete[0].data.begin();
	std::cout << "data = " << std::endl;
	for (; it != this->_header_requete[0].data.end(); it++)
	{
		std::cout << "first = " << it->first << " et = " << it->second << std::endl;

	}
	
	sleep(2);
	// on setup les variables a NULL
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
		this->_cgi->setQueryString(this->_header_requete[0].query_string);
		// techniquement pas d'erreur donc 200
		this->_cgi->setRedirectStatus("200");
		this->_cgi->setStatusCode("200");
		this->_cgi->setRequestMethod(this->_header_requete[0].method);
		this->_cgi->setServerSoftware("Webserv/1.0");
		this->_cgi->setServerProtocol("HTTP/1.1");
		
		
		// test en rajoutant root
		std::string tmp_2 = this->_header_requete[0].script_file_name;
		tmp_2.insert(0, this->_servers[this->_num_serv].root_server);
		std::cout << "\n tmp_2 = " << tmp_2 << std::endl;
		this->_cgi->setScriptName(tmp_2);
		this->_cgi->setScriptFileName(tmp_2);

		tmp_2 = this->_header_requete[0].request_uri;
		tmp_2.insert(0, this->_servers[this->_num_serv].root_server);
		this->_cgi->setRequestUri(tmp_2);


		// probleme si plusieurs servers ...
		this->_cgi->setServerName(this->_servers[this->_num_serv].name_server);
		
		

		// std::cout << GREEN << "\n\nOn a tout setup, on va afficher pour verifier " << CLEAR << std::endl;
		// this->_cgi->ft_display_all_variable_env();

		// exit(1);
		std::cout << GREEN << "\n\nMaintenant on utilise le CGI avec les donnees " << CLEAR << std::endl;
		
		// std::cout << "address cgi = " << this->_servers[this->_num_serv].cgi_path_server << std::endl;
		std::string tmp = this->_header_requete[0].script_file_name;
		tmp.insert(0, this->_servers[this->_num_serv].root_server);
		// std::cout << "tmp = " << tmp << std::endl;

		// std::string BINGO = "";
		this->_header_requete[0].body_error = this->_cgi->ft_execute_cgi(this->_servers[this->_num_serv].cgi_path_server, tmp);
		
		// sleep(1);
		// std::cout << "\n\n\n BINGO = \n" << this->_header_requete[0].body_error << std::endl;
		// exit(1);
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

		// probleme si plusieurs servers ...
		this->_cgi->setServerName(this->_servers[0].name_server);

		std::string tmp = this->_header_requete[0].script_file_name;
		tmp.insert(0, this->_servers[0].root_server);
		std::cout << "\n tmp = " << tmp << std::endl;
		this->_cgi->setScriptName(tmp);
		this->_cgi->setScriptFileName(tmp);
		this->_cgi->setRequestUri("");
		// if (this->_header_requete[0].upload == true)
		// {
		// 	size_t  ret = this->ft_upload_file(request_http);
		// 	if (ret == 201)
		// 	{
		// 		this->_header_requete[0].cgi = true;
		// 		this->_header_requete[0].body_error = "\r\n\r\n<!DOCTYPE html><html><head><title>201</title><style type=text/css>body {color: green;font-weight: 900;font-size: 20px;font-family: Arial, Helvetica, sans-serif; }</style><link rel=\"icon\" type=\"image/x-con\" href=\"/flavicon.ico\"/><link rel=\"shortcut icon\" type=\"image/x-con\" href=\"/flavicon.ico\" /></head><body><h1>201 Created, check it!</h1><p>by Pierre.</p></body></html>";
		// 		this->_header_requete[0].num_error = 201; // j'utilise error lol...
		// 		std::cout << "OUI 201 " << std::endl;
		// 		// sleep(2);
		// 	}
		// 	else if (ret == 200)
		// 	{
		// 		this->_header_requete[0].cgi = true;
		// 		this->_header_requete[0].body_error = "\r\n\r\n<!DOCTYPE html><html><head><title>200</title><style type=text/css>body {color: blue;font-weight: 900;font-size: 20px;font-family: Arial, Helvetica, sans-serif; }</style><link rel=\"icon\" type=\"image/x-con\" href=\"/flavicon.ico\"/><link rel=\"shortcut icon\" type=\"image/x-con\" href=\"/flavicon.ico\" /></head><body><h1>File Already uploaded =)</h1><p>by Pierre.</p></body></html>";				
		// 		this->_header_requete[0].num_error = 200;
		// 	}
		// 	else if (ret == 400)
		// 	{
		// 		this->_header_requete[0].error = true;
		// 		this->_header_requete[0].num_error = 400;
		// 		this->ft_setup_error_header();
		// 	}
		// 	else if (ret == 403)
		// 	{
		// 		this->_header_requete[0].error = true;
		// 		this->_header_requete[0].num_error = 403;
		// 		this->ft_setup_error_header();
		// 	}
		// 	else if (ret == 413)
		// 	{
		// 		this->_header_requete[0].error = true;
		// 		this->_header_requete[0].num_error = 413;
		// 		this->ft_setup_error_header();
		// 	}
		// 	else if (ret == 431)
		// 	{
		// 		this->_header_requete[0].error = true;
		// 		this->_header_requete[0].num_error = 431;
		// 		this->ft_setup_error_header();
		// 	}
		// 	else if (ret == 404)
		// 	{
		// 		this->_header_requete[0].error = true;
		// 		this->_header_requete[0].num_error = 404;
		// 		this->ft_setup_error_header();
		// 	}
		// 	else if (ret == 500)
		// 	{
		// 		this->_header_requete[0].cgi = true;
		// 		this->_header_requete[0].body_error = "\r\n\r\n<!DOCTYPE html><html><head><title>500</title><style type=text/css>body {color: red;font-weight: 900;font-size: 20px;font-family: Arial, Helvetica, sans-serif; }</style><link rel=\"icon\" type=\"image/x-con\" href=\"/flavicon.ico\"/><link rel=\"shortcut icon\" type=\"image/x-con\" href=\"/flavicon.ico\" /></head><body><h1>500 Internal Server Error</h1><p>by Pierre.</p></body></html>";				
		// 		this->_header_requete[0].num_error = 500;
		// 	}
		// 	return ;
			
		// }


		this->_cgi->setContentLength(this->_header_requete[0].content_length);
		this->_cgi->setContentType(this->_header_requete[0].content_type);
		this->_cgi->setRequestUri("");

		// il faut mettre le body dans la string privee body
		this->_cgi->set_body_string_post(this->_header_requete[0].body_post);

		this->_cgi->ft_display_all_variable_env();



		std::cout << GREEN << "\n\nMaintenant on utilise le CGI avec les donnees " << CLEAR << std::endl;
		
		// std::cout << "address cgi = " << this->_servers[0].cgi_path_server << std::endl;
		std::string tmp_2 = this->_header_requete[0].script_file_name;
		tmp_2.insert(0, this->_servers[0].root_server);
		// std::cout << "tmp = " << tmp << std::endl;

		std::cout << " AVANT EXECUTE CGI : " << std::endl;
		std::cout << "first arg = -" << this->_servers[0].cgi_path_server << "-" << std::endl;
		

		std::cout << "test tmp_2 = / " << std::endl;
		tmp_2 = "/";
		std::cout << "second arg = -" << tmp_2 << "-" << std::endl;
		this->_header_requete[0].body_error = this->_cgi->ft_execute_cgi(this->_servers[0].cgi_path_server, tmp_2);
		std::cout << "BINGO ? = " << this->_header_requete[0].body_error << std::endl;
		exit(1);
	}
	else
	{
		std::cout << "on est ailleurs" << std::endl;
		exit(1);
	}

	return ;
}

