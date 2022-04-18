/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServerRequest.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psaumet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/01 11:07:34 by psaumet           #+#    #+#             */
/*   Updated: 2021/06/01 11:07:42 by psaumet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Headers/HttpServer.hpp"

/*
**	INFORMATIONS SUR CE FICHIER !
**
**		Ce fichier contient toutes les fonctions utilisees lorsqu'un client
**		fait une requete au server. Il peut demander a afficher un page avec
**		la methode GET, ou il peut poster un formulaire avec GET ou POST et
**		supprimer quelque chose avec DELETE.
**
**		Toutes les fonctions ici permettent de parser, le header et le body (
**		seulement pour la fonction POST). Elles verifient que toutes les informations
**		concordent et genere un vector (this->_header_requete) avec ces informations.

**		std::vector<t_header_request> 	_header_requete contient:
			std::string method  	= get ou post ou delete
			std::string path		= qui est le path demande ex: /qtest.php?name=pierre
			std::string protocol	= HTTP /1.1
			std::string	host		= 127.0.0.1:8080 par exempe
			bool		cgi			= true il y a cgi dans le path ou php
			bool		error		= true s'il y a une erreur dans le header de la requete
									ou s'il y a une erreur quelconque.
			size_t		num_error	= le numero de l'erreur si bool error = true.
			std::string body_error	= euh alors la c'est le foure tout. Dedans on peut mettre
			ce qu'on veut pour l'instant
*/

/*
**	void	ft_parser_requete( int len_msg, const char  *msg )
**		This function checks that there is either a GET, a POST or DELETE request in
**		the client request path.
**		Then choose the corresponding function.
*/
void	HttpServer::ft_parser_requete( int len_msg, std::string msg )
{
	std::cout << "\nDans ft_parser_requete: " << std::endl;

	std::string request_http(msg);
	std::cout << "Contenu de la requete = " << request_http << std::endl;
	std::cout << "taille de la requete = " << request_http.length() << std::endl;
	std::cout << "\n" << std::endl;

	if (request_http.compare(0, 4, "GET ") == 0)
	{
		this->ft_get(request_http, len_msg);
		std::cout << "fin de ft_get(), on return " << std::endl;
		return ;
	}
	else if (request_http.compare(0, 5, "POST ") == 0)
	{
		this->ft_post(request_http, len_msg);
		std::cout << "fin de ft_post(), on return ;" << std::endl;
		return ;
	}
	else if (request_http.compare(0, 7, "DELETE ") == 0)
		this->ft_delete(request_http, len_msg);
	else
	{
		if (this->_header_requete.empty() == true)
		{
			this->_header_requete.push_back(t_header_request());
			this->_header_requete[0].error = true;
			this->_header_requete[0].num_error = 405;
			if (this->ft_setup_error_header(request_http, len_msg) == 0)
				return ;
		}
	}
	return ;
}


std::string::iterator	HttpServer::ft_find_end_header( std::string request_http )
{
	std::string::iterator	it_b = request_http.begin();
	std::string::iterator	it_end_request;
	// int i =0;
	for(; it_b != request_http.end(); it_b++)
	{
		std::string end_request(it_b, it_b + 4);
		if (end_request == "\r\n\r\n")
		{
			it_end_request = it_b;
			
			std::cout << "on atrouve la fin du header" << std::endl;
			return (it_b);
			break ;
		}
	}
	std::cout << "ERROR on a pas la fin du header ??? " << std::endl;
	// exit(1);
	return (it_b);
}

size_t			HttpServer::ft_get( std::string request_http, int len_msg)
{
	std::cout << GREEN << "Dans get : " << CLEAR <<  std::endl;

	// On verifie que this->_header_requete est vide, apres chaque requete il doit etre vide.
	if (this->_header_requete.empty() == true)
	{
		this->_header_requete.push_back(t_header_request());
		if (ft_check_method_allowed_header(request_http, "GET") == 1)				// On verifie que la methode est autorisee
		{
			std::cout << RED << "La methode GET est interdite donc on sort une erreur 405" << CLEAR << std::endl;
			this->_header_requete[0].error = true;
			this->_header_requete[0].num_error = 405;
			if (this->ft_setup_error_header(request_http, len_msg) == 0)
				return (0);
			else
			{
				std::cout << "ft_setup_erro_header return 1, ce qui est pas normal." << std::endl;
				std::cout << "on doit sortir une erreur 500" << std::endl;
				sleep(2);
				return (1);
			}
		}
		std::cout << BLUE << "La methode GET est autorisee, on continue." << CLEAR << std::endl;

		if (len_msg > 1023)													// on verifie que le header ne soit pas trop long
		{

			std::cout << RED << "On a une  ERREUR 431 car GET method et donnees trop grandes " << CLEAR << std::endl;
			this->_header_requete[0].error = true;
			this->_header_requete[0].num_error = 431; 
			if (this->ft_setup_error_header(request_http, len_msg) == 0)
				return (0);
			else
			{
				std::cout << "ft_setup_erro_header return 1, ce qui est pas normal." << std::endl;
				std::cout << "on doit sortir une erreur 500" << std::endl;
				sleep(2);
				return (1);
			}

		}
		
		std::cout << BLUE << "Ok pas d'erreur 431 ou d'erreur 405 donc on continue." << CLEAR <<  std::endl;

		// A partir d'ici, on va parser la requete et recuperer toutes les informations disponibles.
		
		// si pas de fin de header faire une erreur
		size_t pos_hea = request_http.find("\r\n\r\n");
		std::string size_header(request_http, 0, pos_hea);
		// std::string::iterator	it_end_request = ft_find_end_header( request_http );
		







		// On recupere la methode
		this->_header_requete[0].method = "GET";
		std::cout << "On a la requete : " << this->_header_requete[0].method << "-" <<  std::endl;
	
		// On recupere le path contenant des donnees s'il y en a. Et on y a rajoute le root
		this->_header_requete[0].path = this->ft_check_path_header(size_header);
		if (this->_header_requete[0].path.empty() == true)
			throw Error(12, "Error, in recieved header, the path is not correct.", 2);;
		std::cout << "On a le path : " << this->_header_requete[0].path << std::endl;
		
		this->ft_parsing_path_get_request();
		std::cout << "On a la query_string : " << this->_header_requete[0].query_string << std::endl;

		this->_header_requete[0].protocol = this->ft_check_protocol_header(size_header);
		if (this->_header_requete[0].protocol.empty() == true)
			throw Error(13, "Error, in recieved header, the protocol is not correct.", 2);
		std::cout << "On a le protocol : " << this->_header_requete[0].protocol << "-" << std::endl;

		this->_header_requete[0].host = this->ft_check_host_header(size_header);
		if (this->_header_requete[0].host.empty() == true)
			throw Error(14, "Error, in recieved header, the host is not correct.", 2);			
		std::cout << "On a le host : " << this->_header_requete[0].host << "-" << std::endl;
		
		

		// RAJOUT VENDREDi
		this->_header_requete[0].accept = this->ft_check_accept_header(size_header);
		if (this->_header_requete[0].accept.empty() == true)
			throw Error(15, "Error, in recieved header, the accept is not correct.", 2);
		std::cout << "On a le accept = "<< this->_header_requete[0].accept << std::endl;
	
		
		this->_header_requete[0].path_http = this->ft_check_pathhttp_header(size_header);
		if (this->_header_requete[0].path_http.empty() == true)
			throw Error(16, "Error, in recieved header, the path of the file si not correct." , 2);
		std::cout << "le path = " << this->_header_requete[0].path_http << std::endl;
		


		
		
		if (this->ft_check_cgi_or_php(request_http) == 1)
		{
			// on a du php ou du cgi ?
			// donc faut utiliser cgi
			this->ft_exec_cgi_test( request_http, len_msg);

			// this->_header_requete[0].cgi_return = this->_cgi->ft_execute_cgi();
		}
		std::cout << GREEN << "On a bien recu une demande " << CLEAR << std::endl;
		// exit(1);
	}
	else
	{
		std::cout << RED << "Probleme le container qui recupere la header de la requete n'est pas vide. " << std::endl;
		std::cout << "Il faut le supprimer apres avoir fait traite une demande." << CLEAR << std::endl;
	}
	return (0);
}


size_t			HttpServer::ft_parsing_path_get_request( void )
{
	std::cout << GREEN << "Dans ft_parsing_path_get_request : " << CLEAR << std::endl;
	size_t		pos_cursor = this->_header_requete[0].path.find("?");
	if (pos_cursor == std::string::npos)
	{
		std::cout << "\tIl n'y a pas de donnees a parser dans parsing ft_parsing_path_get_request." << std::endl;
		this->_header_requete[0].query_string = "";
		return (0);
	}
	else
	{
		std::cout << "\tIl y a des donnees a parser dans la requete GET." << std::endl;
		std::string tmp;
		pos_cursor++;
		size_t len = this->_header_requete[0].path.length();
		while (this->_header_requete[0].path[pos_cursor])
		{
			if (this->_header_requete[0].path.compare(pos_cursor, 1, "=") == 0)
			{
				this->_header_requete[0].data.insert(std::pair<std::string, std::string>(tmp, "NULL"));
				tmp.clear();
			}
			else if (this->_header_requete[0].path.compare(pos_cursor, 1, "&") == 0 || pos_cursor == len - 1)
			{
				if (pos_cursor == len - 1)
					tmp.append(this->_header_requete[0].path, pos_cursor, 1);
				std::map<std::string, std::string>::iterator it_b = this->_header_requete[0].data.begin();
				for (; it_b != this->_header_requete[0].data.end(); it_b++)
				{
					if (it_b->second == "NULL")
					{
						// On change les characteres speciaux
						tmp = ft_clean_path_get_request(tmp);
						it_b->second = tmp;

						// On ajoute les donnees a this->_header_requete[0].query_string
						this->_header_requete[0].query_string.append(it_b->first);
						this->_header_requete[0].query_string.append("=");
						this->_header_requete[0].query_string.append(it_b->second);
						this->_header_requete[0].query_string.append("&");
					}
				}
				tmp.clear();
			}
			else
				tmp.append(this->_header_requete[0].path, pos_cursor, 1);
			pos_cursor++;
		}
	}
	// std::map<std::string, std::string>::iterator it_b = this->_header_requete[0].data.begin();
	// for (; it_b != this->_header_requete[0].data.end(); it_b++)
	// {
	// 	std::cout << it_b->first << " = " << it_b->second << std::endl;
	// }
	this->_header_requete[0].query_string.erase(this->_header_requete[0].query_string.end() - 1);
	return (0);
}

/*
**	find_replace and ft_clean_path get request
**		on doit les deplacer ailleurs.
**	clean path get request est a ameliorer. c'est du brut force LOL
*/
std::string		find_replace( std::string to_search, std::string to_replace, std::string data )
{
	size_t pos = data.find(to_search);

	while (pos != std::string::npos)
	{
		data.replace(pos, to_search.size(), to_replace);
		pos = data.find(to_search, pos + to_replace.size());
	}
	return (data);
}
// URL ENCODING
std::string		HttpServer::ft_clean_path_get_request( std::string tmp )
{
	std::map<std::string, std::string> url_coding;

	// url_coding.insert(std::pair<std::string , std::string>("+", " ")); // space	
	url_coding.insert(std::pair<std::string , std::string>("%21", "!")); 	
	url_coding.insert(std::pair<std::string , std::string>("%22", "\"")); 	
	url_coding.insert(std::pair<std::string , std::string>("%23", "#")); 	
	url_coding.insert(std::pair<std::string , std::string>("%24", "$")); 
	url_coding.insert(std::pair<std::string , std::string>("%25", "%")); 	
	url_coding.insert(std::pair<std::string , std::string>("%26", "&")); 
	url_coding.insert(std::pair<std::string , std::string>("%27", "'")); 	
	url_coding.insert(std::pair<std::string , std::string>("%28", "(")); 	
	url_coding.insert(std::pair<std::string , std::string>("%29", ")")); 	
	url_coding.insert(std::pair<std::string , std::string>("%2A", "*"));
	url_coding.insert(std::pair<std::string , std::string>("%2B", "+"));
	url_coding.insert(std::pair<std::string , std::string>("%2C", ","));
	url_coding.insert(std::pair<std::string , std::string>("%2D", "-"));
	url_coding.insert(std::pair<std::string , std::string>("%2E", "."));
	url_coding.insert(std::pair<std::string , std::string>("%2F", "/"));
	url_coding.insert(std::pair<std::string , std::string>("%3A", ":"));
	url_coding.insert(std::pair<std::string , std::string>("%3B", ";"));
	url_coding.insert(std::pair<std::string , std::string>("%3C", "<"));
	url_coding.insert(std::pair<std::string , std::string>("%3D", "="));
	url_coding.insert(std::pair<std::string , std::string>("%3E", ">"));
	url_coding.insert(std::pair<std::string , std::string>("%3F", "?"));
	url_coding.insert(std::pair<std::string , std::string>("%40", "@"));
	url_coding.insert(std::pair<std::string , std::string>("%5B", "["));
	url_coding.insert(std::pair<std::string , std::string>("%5C", "\\"));
	url_coding.insert(std::pair<std::string , std::string>("%5D", "]"));
	url_coding.insert(std::pair<std::string , std::string>("%5E", "^"));
	url_coding.insert(std::pair<std::string , std::string>("%5F", "_"));
	url_coding.insert(std::pair<std::string , std::string>("%E2", "€"));
	url_coding.insert(std::pair<std::string , std::string>("%60", "`"));

	std::map<std::string, std::string>::iterator it_find = url_coding.begin();
	for (; it_find != url_coding.end(); it_find++)
	{
		tmp = find_replace(it_find->first, it_find->second, tmp);
	}
	return (tmp);

	return (tmp);
}

void			HttpServer::ft_exec_cgi_test( std::string request_http, int len_msg )
{
	std::cout << GREEN << "\n\nDANS exec CGI ... " << CLEAR << std::endl;
	std::cout << "request_http = " << request_http << std::endl;
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
		tmp_2.insert(0, this->_servers[0].root_server);
		std::cout << "\n tmp_2 = " << tmp_2 << std::endl;
		this->_cgi->setScriptName(tmp_2);
		this->_cgi->setScriptFileName(tmp_2);

		tmp_2 = this->_header_requete[0].request_uri;
		tmp_2.insert(0, this->_servers[0].root_server);
		this->_cgi->setRequestUri(tmp_2);


		// probleme si plusieurs servers ...
		this->_cgi->setServerName(this->_servers[0].name_server);
		
		

		// std::cout << GREEN << "\n\nOn a tout setup, on va afficher pour verifier " << CLEAR << std::endl;
		// this->_cgi->ft_display_all_variable_env();

		// exit(1);
		std::cout << GREEN << "\n\nMaintenant on utilise le CGI avec les donnees " << CLEAR << std::endl;
		
		// std::cout << "address cgi = " << this->_servers[0].cgi_path_server << std::endl;
		std::string tmp = this->_header_requete[0].script_file_name;
		tmp.insert(0, this->_servers[0].root_server);
		// std::cout << "tmp = " << tmp << std::endl;

		// std::string BINGO = "";
		this->_header_requete[0].body_error = this->_cgi->ft_execute_cgi(this->_servers[0].cgi_path_server, tmp);
		
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
		if (this->_header_requete[0].upload == true)
		{
			this->ft_upload_file();
			exit(1);
			// this->_cgi->setScriptName("");
			// this->_cgi->setScriptFileName("");
			// this->_cgi->setRequestUri("/");			// a changer
		}
// name=Pierre&message=COUCOU1
		// a setup

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

size_t			HttpServer::ft_upload_file( void )
{
	/*
	**	1 ) regarder si on a recu un fichier
				si non, pas de fichier
					erreur afficher une page, il faut mettre un fichier
		2) verifier les droits
		2) regarder si le fichier existe
				si oui, retourner une erreur, file deja uplooad
		3) 	on upload le fichier
			1) open
			2) write
			3) close
			4) code sortie html 201 created file
			afficher une page avec le bon code
				
	*/
	// TMP tout a changer
	std::string filename = "./root/test.txt";
	if (filename.empty() == true)
	{
		// erreur pas de fichier
	}
	struct stat buff;
	if (lstat(filename.c_str(), &buff) == 0)
	{
		// erruer deja upload
	}
	else
	{
		FILE *fp;
		fp = fopen(filename.c_str(), "a");
		if (fp == NULL)
		{
			// erreur 500
		}
		


		fclose(fp);
		// int fd = open(filename.c_str(), O_WRONLY | O_CREAT, 00755);
		// if (fd < 0)
		// {
		// 	// erreur 500 on ne peut pas creer le fichier
		// }
		// close(fd);
	}
	return (0);
}

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
			std::cout << "IL Y A DU PHP ET IL FAUT UPLOADER" << std::endl;
			this->_header_requete[0].upload = true;
			return (true);
		}
		std::cout << "non on a pas de php ou de cgi" << std::endl;
		return (false);
	}
	return (false);
}


/*
**	size_t		ft_setup_error_header( std::string request_http, int len_msg )
**
**	<!> This function is used if there is an error in the header or any error.
**	IT IS NOT FINISHED.
*/
int			HttpServer::ft_setup_error_header( std::string request_http, int len_msg )
{
	(void)request_http;
	(void)len_msg;
	// donc on a une erreur.
	// 1) il faut regarder ou on se trouve dans la requete si on est dans du root ou dans du location
	// 2) regarder si l'erreur donne est deja configurer dans la directive error_page
	// 3) si oui, on setup le header avec le fichier
	// 4) si non, on regarde si on a le fichier de l'erreur en question.
	// 5) si le fichier existe on setup le header et on revoit
	// 6) si non, on setup le bon header

	// 1
	//	1: il y a des location si oui, on cherche si on est dedans...
	if (this->_servers[0].nbr_location > 0)
	{
		std::cout << "il y a des loctions : " << this->_servers[0].nbr_location << std::endl;
		size_t i = 0;
		while ( i < this->_servers[0].nbr_location)
		{
			std::cout << "affiche le nom de la location : " << this->_servers[0].location[i].name_location << std::endl;
			
			size_t found = request_http.find("/");
			// size_t size_name_location = this->_servers[0].location[i].name_location.length();
			if (found == std::string::npos)
			{
				std::cout << "ERREUR euh pas normal ne trouve pas / dans la requete a traiter "<< std::endl;
				exit(EXIT_FAILURE);
			}
			else
			{
				// std::cout << "request http = " << request_http << std::endl;
				// std::cout << "nom lcoation = " << this->_servers[0].location[i].name_location << std::endl;
				// std::cout << "found = " << found << std::endl;
				std::string tmp = this->_servers[0].location[i].name_location;
				if (tmp[tmp.size() - 1] != '/')
					tmp.append("/");
				// std::cout << "tmp = " << tmp << std::endl;
				if (request_http.find(tmp) != std::string::npos)
				{
					std::cout << "OUI on se trouve dans un dossier location a continuer   11 " << std::endl;
					
					// go checker si l'erreur est setup
					if (this->_header_requete[0].error == true)
					{
						if (this->_header_requete[0].num_error)
						{

							std::map<int, std::string>::iterator it = this->_servers[0].location[i].error_location.begin();
							// on cherche si l'erreur a ete idique dans une directive error_page
							for (; it != this->_servers[0].location[i].error_location.end(); it++)
							{
								std::cout << "on display les erreur dans location... " << it->first << std::endl;
								if (it->first >= 0 && this->_header_requete[0].num_error == (size_t)it->first)
								{
									std::cout << "OUI ils sont egaux, le num_error et l'erreur qui a ete setup dans le fichier de conf" << std::endl;
									std::cout << "euh second = " << it->second << std::endl;
									// on met dans le body le fichier a recuperer
									this->_header_requete[0].body_error = it->second;
									std::cout << " on a mis le fichier a recuperer pour l'error." << std::endl;
									std::cout << "\nbody error = " << this->_header_requete[0].body_error;
									if (i == 0)
										return (-1);
									else
										return (i);
								}
								else
								{
									std::cout << "euh non " << std::endl;
								}
							}
							std::cout << RED << "l'erreur n'a pas ete trouve dans error_page " << CLEAR << std::endl;
							this->_header_requete[0].body_error = "/";
							this->_header_requete[0].body_error.insert(0, this->_servers[0].location[i].name_location);
							// this->_header_requete[0].body_error = this->_servers[0].location[i].name_location;
							this->_header_requete[0].body_error.insert(0, this->_servers[0].root_server);
							std::cout << "euh le body error = " << this->_header_requete[0].body_error << std::endl;
							// this->_header_requete[0].body_error = this->_servers[0].root_server;
							// this->_header_requete[0].body_error = "NULL";
						}
						else
						{
							std::cout << "pas normal on ne doit pas etre la num error pas setup = false" << std::endl;
							exit(EXIT_FAILURE);
						}
					}
					else
					{
						std::cout << "pas normal on ne doit pas etre la error = false" << std::endl;
						exit(EXIT_FAILURE);
					}

					return (0);
				}
			}
			i++;
		}
		std:: cout << RED << " la pierre ca ne correspond pas des location  " << CLEAR << std::endl;

		std::map<int, std::string>::iterator it = this->_servers[0].error_server.begin();
		// on cherche si l'erreur a ete idique dans une directive error_page
		for (; it != this->_servers[0].error_server.end(); it++)
		{
			std::cout << "on display les erreur setup du server... " << it->first << std::endl;
			if (it->first >= 0 && this->_header_requete[0].num_error == (size_t)it->first)
			{
				std::cout << "OUI ils sont egaux, le num_error et l'erreur qui a ete setup dans le fichier de conf" << std::endl;
				std::cout << "euh second = " << it->second << std::endl;
				// on met dans le body le fichier a recuperer
				this->_header_requete[0].body_error = it->second;
				std::cout << " on a mis le fichier a recuperer pour l'error." << std::endl;
				std::cout << "\nbody error = " << this->_header_requete[0].body_error;
				return (0);
			}
			else
			{
				std::cout << "euh non " << std::endl;
			}
		}
		std:: cout << RED << " la pierre que faire ?  " << CLEAR << std::endl;
		this->_header_requete[0].body_error = "/";
		
		// this->_header_requete[0].body_error = this->_servers[0].location[i].name_location;
		this->_header_requete[0].body_error.insert(0, this->_servers[0].root_server);
		// this->_servers[index_server].location[index_location].name_location
	}
	else
	{
		std::cout << "il y a pas de location" << std::endl;
	}
	return (0);
}


/*
**		A FAIRE CAS DES REQUETE POST
*/
size_t			HttpServer::ft_post(std::string request_http, int len_msg)
{
	std::cout << GREEN  << "Dans ft_POST: " << CLEAR << std::endl;
	std::cout << "request_http = " << request_http << std::endl;
	std::cout << "len_msg = " << len_msg << std::endl;



	if (this->_header_requete.empty() == true)
	{
		this->_header_requete.push_back(t_header_request());
		if (ft_check_method_allowed_header(request_http, "POST") == 1)				// On verifie que la methode est autorisee
		{
			std::cout << RED << "La methode POST est interdite donc on sort une erreur 405" << CLEAR << std::endl;
			this->_header_requete[0].error = true;
			this->_header_requete[0].num_error = 405;
			if (this->ft_setup_error_header(request_http, len_msg) == 0)
				return (0);
			else
			{
				std::cout << "ft_setup_erro_header return 1, ce qui est pas normal." << std::endl;
				std::cout << "on doit sortir une erreur 500" << std::endl;
				sleep(2);
				return (1);
			}
		}
		std::cout << BLUE << "La methode POST est autorisee, on continue." << CLEAR << std::endl;

		size_t pos_hea = request_http.find("\r\n\r\n");
		std::string size_header(request_http, 0, pos_hea);
		if (size_header.size() > 1023)	// on verifie que le header ne soit pas trop long
		{
			std::cout << RED << "On a une  ERREUR 431 car GET method et donnees trop grandes " << CLEAR << std::endl;
			this->_header_requete[0].error = true;
			this->_header_requete[0].num_error = 431; 
			if (this->ft_setup_error_header(request_http, len_msg) == 0)
				return (0);
			else
			{
				std::cout << "ft_setup_erro_header return 1, ce qui est pas normal." << std::endl;
				std::cout << "on doit sortir une erreur 500" << std::endl;
				sleep(2);
				return (1);
			}

		}
		// On regarde la taille du body si trop grand == erreur 413
		//	il faut comparer la capacity() de la string a la limite client_buffer_siuze()
		//	si c'est trop grand retourner une erreur 
		// a terminer
		std::string size_body(request_http, size_header.size(), request_http.size());
		std::cout << "euh size_body = -" << size_body << "-"<< std::endl;
		std::cout << "et taille body = " << size_body.size() << std::endl;
		
		// exit(1);
		// if (FT_FONCTION A FAIRE ())
		// REGARDE LE LOCATION ET OU SERVER
		//	SI CLIENT BUFFER SIZE < SIZE_BODY_CAPACITY()
		//	RETOURNE 413

		std::cout << "capacity en bite = " << size_body.capacity() << std::endl;
		std::cout << "length = " << size_body.length() << std::endl;
		std::cout << BLUE << "Ok pas d'erreur 431 ou d'erreur 405 donc on continue." << CLEAR <<  std::endl;
	
	
		// On recupere la methode
		this->_header_requete[0].method = "POST";
		std::cout << "\nOn a la requete : " << this->_header_requete[0].method << "-" <<  std::endl;
	
		// On recupere le path contenant des donnees s'il y en a. Et on y a rajoute le root
		this->_header_requete[0].path = this->ft_check_path_header(size_header);
		if (this->_header_requete[0].path.empty() == true)
			throw Error(12, "Error, in recieved header, the path is not correct.", 2);;
		std::cout << "\nOn a le path : " << this->_header_requete[0].path << std::endl;
		
		this->ft_parsing_path_get_request();
		std::cout << "\nOn a la query_string : " << this->_header_requete[0].query_string << std::endl;

		this->_header_requete[0].protocol = this->ft_check_protocol_header(size_header);
		if (this->_header_requete[0].protocol.empty() == true)
			throw Error(13, "Error, in recieved header, the protocol is not correct.", 2);
		std::cout << "\nOn a le protocol : " << this->_header_requete[0].protocol << "-" << std::endl;

		this->_header_requete[0].host = this->ft_check_host_header(size_header);
		if (this->_header_requete[0].host.empty() == true)
			throw Error(14, "Error, in recieved header, the host is not correct.", 2);			
		std::cout << "\nOn a le host : " << this->_header_requete[0].host << "-" << std::endl;
		
		

		// RAJOUT VENDREDi
		this->_header_requete[0].accept = this->ft_check_accept_header(size_header);
		if (this->_header_requete[0].accept.empty() == true)
			throw Error(15, "Error, in recieved header, the accept is not correct.", 2);
		std::cout << "\nOn a le accept = "<< this->_header_requete[0].accept << std::endl;
	
		
		this->_header_requete[0].path_http = this->ft_check_pathhttp_header(size_header);
		if (this->_header_requete[0].path_http.empty() == true)
			throw Error(16, "Error, in recieved header, the path of the file is not correct." , 2);
		std::cout << "\nOn a le path = -" << this->_header_requete[0].path_http << "-" <<  std::endl;
		

		this->_header_requete[0].content_length = this->ft_check_content_length(size_header);
		if (this->_header_requete[0].content_length.empty() == true)
			throw Error(16, "Error, in recieved header, the content_length is  not correct." , 2);
		std::cout << "\nOn a le content_length = -" << this->_header_requete[0].content_length << "-" <<  std::endl;
		
		this->_header_requete[0].content_type = this->ft_check_content_type(size_header);
		if (this->_header_requete[0].content_type.empty() == true)
			throw Error(16, "Error, in recieved header, the content_type is  not correct." , 2);
		std::cout << "\nOn a le content_type = -" << this->_header_requete[0].content_type << "-" <<  std::endl;
		

		this->_header_requete[0].body_post = this->ft_check_body_post(size_body);
		if (this->_header_requete[0].body_post.empty() == true)
			throw Error(16, "Error, in recieved header, the body_post is  not correct." , 2);
		std::cout << "\nOn a le body_post = -" << this->_header_requete[0].body_post << "-" <<  std::endl;
		


		

		if (this->ft_check_cgi_or_php(request_http) == 1)
		{
			// on a du php ou du cgi ?
			// donc faut utiliser cgi
			std::cout << "OUI " << std::endl;
			
			this->ft_exec_cgi_test( request_http, len_msg);

			exit(1);
			// this->_header_requete[0].cgi_return = this->_cgi->ft_execute_cgi();
		}
		std::cout << "NON " << std::endl;
		exit(1);
		std::cout << GREEN << "On a bien recu une demande " << CLEAR << std::endl;
	}
	std::cout << RED << "Tout est bon ? " << CLEAR << std::endl;
	exit(1);
	return (0);
	// exit(1);
}

std::string		HttpServer::ft_check_body_post( std::string request_http )
{
	size_t pos = request_http.find("\r\n\r\n"); // on cherche la fin
	if (pos == std::string::npos)
	{
		std::cout << "Erreur dans POST le header n'a pas /r/n/r/n" << std::endl;
		exit(1);
	}
	else
	{
		size_t pos_end = request_http.find("\r\n", pos);
		std::string tmp(request_http, pos + 4, pos_end - (pos + 4));
		std::cout << "tmp = -" << tmp << "-" << std::endl;
		// on compare avec contentlengt
		
		if ((long)tmp.size() == std::strtol(this->_header_requete[0].content_length.c_str(), NULL, 10))
			return (tmp);
		else
		{
			std::cout << "ERROR size body differe de content length" << std::endl;
			exit(1);
		}
		exit(1);
		return (tmp); 
	}
}

std::string		HttpServer::ft_check_content_type( std::string request_http )
{
	size_t pos = request_http.find("Content-Type: ");
	if (pos == std::string::npos)
	{
		std::cout << "Erreur dans POST le header n'a pas Content-TYPE" << std::endl;
		exit(1);
	}
	else
	{
		size_t pos_end = request_http.find("\r\n", pos);
		std::string tmp(request_http, pos + 14, pos_end - (pos + 14));
		return (tmp);
	}
}

std::string		HttpServer::ft_check_content_length( std::string request_http )
{
	size_t pos = request_http.find("Content-Length: ");
	if (pos == std::string::npos)
	{
		std::cout << "Erreur dans POST le header n'a pas Content-Length" << std::endl;
		exit(1);
	}
	else
	{
		size_t pos_end = request_http.find("\r\n", pos);
		std::string tmp(request_http, pos + 16, pos_end - (pos + 16));
		// std::cout << "tmp = -" << tmp << "-" << std::endl;
		return (tmp);
	}
}

/*
**	A FAIRE CAS DES REQUETES DELETE
*/
void			HttpServer::ft_delete(std::string request_http, int len_msg)
{
	(void)request_http;
	(void)len_msg;
}


