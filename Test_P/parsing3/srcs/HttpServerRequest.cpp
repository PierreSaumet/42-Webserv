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
	std::cout << "Dans parser requete " << std::endl;

	std::string request_http(msg);
	std::cout << "msg = " << request_http << std::endl;
	std::cout << "len = " << request_http.length() << std::endl;
	std::cout << "len = " << len_msg << std::endl;



	if (request_http.compare(0, 4, "GET ") == 0)
	{
		this->ft_get(request_http, len_msg);
		std::cout << "fin de ft_get(), on return " << std::endl;
		return ;
	}
	else if (request_http.compare(0, 5, "POST ") == 0)
		this->ft_post(request_http, len_msg);
	else if (request_http.compare(0, 7, "DELETE ") == 0)
		this->ft_delete(request_http, len_msg);
	else
	{
		std::cout << "ERROR dans la method error 405" << std::endl;
	}

	// TOUT CE QUI EST EN COMMENTAIRE DESSOUS DOIT ETRE SUPPRIME ET IMPLEMENTE 
	//DANS LES AUTRES FONCTIONS.

	// on regarder si c'est get et la taille sup a 1023 si oui erreur 431
	// if (request_http.compare(0, 4, "GET ") == 0 && len_msg > 1023)
	// {
	// 	std::cout << RED << "PUTAIN ERREUR 431 car get et trop grand " << CLEAR << std::endl;
	// 	this->_header_requete[0].error = true;
	// 	this->_header_requete[0].num_error = 431; 
	// }

	// on trouve la fin du header de la requete HTTP.
	// std::string::iterator	it_b = request_http.begin();
	// std::string::iterator	it_end_request;
	// int i =0;
	// for(; it_b != request_http.end(); it_b++)
	// {
	// 	std::string end_request(it_b, it_b + 4);
	// 	if (end_request == "\r\n\r\n")
	// 	{
	// 		it_end_request = it_b;
	// 		break ;
	// 	}
	// 	// std::cout << "*it = " << *it_b << std::endl;
	// 	i++;
	// }
	// std::cout << RED << "ICI " << CLEAR << std::endl;
	// std::string size_header(request_http.begin(), it_end_request);
	// std::cout << RED << "ICI =)" << CLEAR << std::endl;
	// if (size_header.size() >= 1024)		// server apache 8000 max
	// {
	// 	std::cout << RED << "ICI 2" << CLEAR << std::endl;
	// 	// throw error ou affiche la page 413 ?
	// 	// throw Error(10, "Error, in recieved header, the header size is greater than 8000.", 2);
	// 	std::cout << "ERREUR size du header trop grand return une erreur 413 entity too large" << std::endl;

	// 	// alors si get return 431
	// }
	// else
	// {
	// 	std::cout << RED << "ICI 3" << CLEAR << std::endl;
	// 	std::cout << "Taille du header bon = " << size_header.size() << std::endl;
	// 	// test savoir quelle method
	// 	if (this->_header_requete.empty() == true)
	// 	{
	// 		std::cout << "Notre container header est vide, on en cree un et on recupere les informations" << std::endl;
	// 		this->_header_requete.push_back(t_header_request());

	// 		this->_header_requete[0].method = this->ft_check_methods_header(size_header);
	// 		if (this->_header_requete[0].method.empty() == true)
	// 			throw Error(11, "Error, in recieved header, the method used is not correct.", 2);
	// 		std::cout << "On a la requete :" << this->_header_requete[0].method << "-" <<  std::endl;
		
	// 		this->_header_requete[0].path = this->ft_check_path_header(size_header);
	// 		if (this->_header_requete[0].path.empty() == true)
	// 			throw Error(12, "Error, in recieved header, the path is not correct.", 2);;
	// 		std::cout << "le path = " << this->_header_requete[0].path << std::endl;
			
	// 		// erruer avec le /flavicon.ico
	// 		if (this->_header_requete[0].path == "/flavicon.ico ")
	// 		{
	// 			std::cout << "merde flavicon " << std::endl;
	// 			this->_header_requete[0].path = "/ ";
	// 		}


	// 		std::cout << " Taille DU PATH = " << this->_header_requete[0].path.length() << std::endl;

	// 		// donc si la taille du path est superieur a 1024 on  va dire
	// 		if (this->_header_requete[0].path.length() > 1024)
	// 		{
	// 			// doit setup error 414 si GET
	// 			std::cout << "path superierur a 1024" << std::endl;
	// 		}
	// 		else
	// 		{
	// 			std::cout << "taille path ok" << std::endl;
	// 		}
	// 		// EUH a quoi ca sert ca ?
	// 		// if (this->_header_requete[0].path == "/page2.html")
	// 		// {
	// 		// 	std::cout << "YES" << std::endl;
	// 		// 	exit(EXIT_SUCCESS);
	// 		// }

	// 		this->_header_requete[0].protocol = this->ft_check_protocol_header(size_header);
	// 		if (this->_header_requete[0].protocol.empty() == true)
	// 			throw Error(13, "Error, in recieved header, the protocol is not correct.", 2);
	// 		std::cout << "le protocol = " << this->_header_requete[0].protocol << "-" << std::endl;

	// 		this->_header_requete[0].host = this->ft_check_host_header(size_header);
	// 		if (this->_header_requete[0].host.empty() == true)
	// 			throw Error(14, "Error, in recieved header, the host is not correct.", 2);			
	// 		std::cout << "le host = " << this->_header_requete[0].host << "-" << std::endl;

	// 		std::cout << GREEN << "On a bien recu une demande " << CLEAR << std::endl;
	// 	}
	// 	else
	// 	{
	// 		std::cout << "Probleme le container qui recupere la header de la requete n'est pas vide. " << std::endl;
	// 		std::cout << "Il faut le supprimer apres avoir fait traite une demande." << std::endl;
	// 	}

	// }
	// std::cout << "i = " << i << std::endl;
	return ;
}

/*
**	size_t	ft_get( std::string request_http, int len_msg)
**		Cette fonction gere toutes les demandes GET.
**		1) elle verifie que notre variable _header_requete est vide.
**		2)	si la longueur de la requete (len_msg) est sup a 1023 = error 431  ==> peut etre a supprimer
**		3) sinon verifie si la requete a besoin de cgi ou php
**		4) si oui, execute CGI
**		4) si non, recupere les informations de la requete et les mets dans this->_header_request
*/
size_t			HttpServer::ft_get( std::string request_http, int len_msg)
{
	std::cout << BLUE <<  "Dans get : " << CLEAR <<  std::endl;
	std::cout << "taille request = " << request_http.length() << std::endl;
	std::cout << "taille request = " << request_http.size() << std::endl;
	std::cout << "len msg = " << len_msg << std::endl;
	//sleep(5);
	if (this->_header_requete.empty() == true)
	{
		this->_header_requete.push_back(t_header_request());

		if (len_msg > 1023)
		{

			std::cout << RED << "On a une  ERREUR 431 car GET method et donnees trop grandes " << CLEAR << std::endl;
			//sleep(5);
			this->_header_requete[0].error = true;
			this->_header_requete[0].num_error = 431; 

			if (this->ft_setup_error_header(request_http, len_msg) == 0)
				return (0);
			else
			{
				std::cout << "ft_setup_erro_header return 1, ce qui est pas normal." << std::endl;
				sleep(2);
				return (1);
			}

		}
		else
		{
			std::cout << "Ok pas d'erreur 431 donc on continue" << std::endl;
			std::cout << "request = " << request_http << std::endl;

			// peut etre a changer l'ordre
			//	On a une requete get, il faut verifier si on a du php et donc du cgi
			// if (ft_find_cgi_or_php(request_http, len_msg) == 1)
			// {
			// 	// on a du php ou du cgi ?
			// 	// donc faut utiliser cgi
			// 	ft_exec_cgi_test( request_http, len_msg);
			// }

			// on trouve la fin du header de la requete HTTP.
			std::string::iterator	it_b = request_http.begin();
			std::string::iterator	it_end_request;
			int i =0;
			for(; it_b != request_http.end(); it_b++)
			{
				std::string end_request(it_b, it_b + 4);
				if (end_request == "\r\n\r\n")
				{
					it_end_request = it_b;
					break ;
				}
				i++;
			}
			std::string size_header(request_http.begin(), it_end_request);
			std::cout << RED << "ICI 3" << CLEAR << std::endl;
			std::cout << "Taille du header bon = " << size_header.size() << std::endl;
			// test savoir quelle method
			if (this->_header_requete.empty() == false)
			{
				std::cout << "Notre container header est vide, on en cree un et on recupere les informations" << std::endl;
				this->_header_requete.push_back(t_header_request());

				this->_header_requete[0].method = this->ft_check_methods_header(size_header);
				if (this->_header_requete[0].method.empty() == true)
					throw Error(11, "Error, in recieved header, the method used is not correct.", 2);
				std::cout << "On a la requete :" << this->_header_requete[0].method << "-" <<  std::endl;
			
				this->_header_requete[0].path = this->ft_check_path_header(size_header);
				if (this->_header_requete[0].path.empty() == true)
					throw Error(12, "Error, in recieved header, the path is not correct.", 2);;
				std::cout << "le path = " << this->_header_requete[0].path << std::endl;
				
				if (ft_parsing_path_get_request() == 1)
				{
					std::cout << RED << "erreur dans le parsing d u path de get " << CLEAR << std::endl;
				}
				else
					std::cout << GREEN << "le parsing du path de get est OK =) " << CLEAR << std::endl;
				// exit(1); //(1);
				///////////////////////////////



				// erruer avec le /flavicon.ico
				if (this->_header_requete[0].path == "/flavicon.ico ")
				{
					std::cout << "merde flavicon " << std::endl;
					this->_header_requete[0].path = "/ ";
				}


				std::cout << " Taille DU PATH = " << this->_header_requete[0].path.length() << std::endl;

				// donc si la taille du path est superieur a 1024 on  va dire
				if (this->_header_requete[0].path.length() > 1024)
				{
					// doit setup error 431
					std::cout << "path superierur a 1024" << std::endl;
				}
				else
				{
					std::cout << "taille path ok" << std::endl;
				}

				this->_header_requete[0].protocol = this->ft_check_protocol_header(size_header);
				if (this->_header_requete[0].protocol.empty() == true)
					throw Error(13, "Error, in recieved header, the protocol is not correct.", 2);
				std::cout << "le protocol = " << this->_header_requete[0].protocol << "-" << std::endl;

				this->_header_requete[0].host = this->ft_check_host_header(size_header);
				if (this->_header_requete[0].host.empty() == true)
					throw Error(14, "Error, in recieved header, the host is not correct.", 2);			
				std::cout << "le host = " << this->_header_requete[0].host << "-" << std::endl;

				std::cout << GREEN << "On a bien recu une demande " << CLEAR << std::endl;
			}
			else
			{
				std::cout << "Probleme le container qui recupere la header de la requete est vide " << std::endl;
				std::cout << "Il faut le supprimer apres avoir fait traite une demande." << std::endl;
			}
			if (ft_find_cgi_or_php(request_http, len_msg) == 1)
			{
				// on a du php ou du cgi ?
				// donc faut utiliser cgi
				ft_exec_cgi_test( request_http, len_msg);
			}
			
			std::cout << "i = " << i << std::endl;

		}
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
	std::cout << "PATH = " << this->_header_requete[0].path << std::endl;
	size_t		pos_cursor = this->_header_requete[0].path.find("?");
	if (pos_cursor == std::string::npos)
	{
		std::cout << "pas de donnee a parser dans parsing path get request" << std::endl;
		return (0);
	}
	else
	{
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
				std::map<std::string, std::string>::iterator it_b = this->_header_requete[0].data.begin();
				for (; it_b != this->_header_requete[0].data.end(); it_b++)
				{
					if (it_b->second == "NULL")
					{
						// need to change characters
						tmp = ft_clean_path_get_request(tmp);
						it_b->second = tmp;
					}
				}
				tmp.clear();
			}
			else
				tmp.append(this->_header_requete[0].path, pos_cursor, 1);
			pos_cursor++;
		}
	}
	// std::cout << "\n\nDISPLAY ALL MAP DATA " << std::endl;
	// std::map<std::string, std::string>::iterator it_b = this->_header_requete[0].data.begin();
	// for (; it_b != this->_header_requete[0].data.end(); it_b++)
	// {
	// 	std::cout << it_b->first << " = " << it_b->second << std::endl;
	// }
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

	url_coding.insert(std::pair<std::string , std::string>("+", " ")); // space	
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
	std::cout << "DANS exec CGI ... " << std::endl;
	(void)request_http;
	(void)len_msg;
	return ;
}


/*
**	size_t		ft_find_cgi_or_php( std::string request_http, int len_msg )
**		This function will simply check the presence or absence in the request path
**		of 'cgi' or 'php'.
**
**		If there is, it returns true, otherwise returns false.
*/
bool			HttpServer::ft_find_cgi_or_php( std::string request_http, int len_msg )
{
	(void)len_msg;
	std::cout << "dans la fonction find cgi or php " << std::endl;
	// on trouve le premier /
	size_t 		find_backslash = request_http.find("/");
	// on cherche HTTP
	size_t		find_http = request_http.find("HTTP");
	//	on cherche .php?
	size_t		find_php = request_http.find(".php?");

	// si .php est entre / et HTTP c'est good, sinon erruer
	if (find_php > find_backslash && find_php < find_http)
	{
		std::cout << " good on a bien du php dans la requete qu'il faut utiliser avec cgi" << std::endl;
		// rajouter debut query_string avec ?
		return (true);
	}
	else
	{
		// on verifie si on a seulement du cgi
		size_t	find_cgi = request_http.find(".cgi");
		if (find_cgi > find_backslash && find_cgi < find_http)
		{
			std::cout << " good on a bien du cgi dans la requete qu'il faut utiliser avec cgi" << std::endl;
			// rajouter debut query_string avec ?
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
			size_t size_name_location = this->_servers[0].location[i].name_location.length();
			if (found == std::string::npos)
			{
				std::cout << "ERREUR euh pas normal ne trouve pas / dans la requete a traiter "<< std::endl;
				exit(EXIT_FAILURE);
			}
			else
			{
				std::cout << "request http = " << request_http << std::endl;
				std::cout << "nom lcoation = " << this->_servers[0].location[i].name_location << std::endl;
				std::cout << "found = " << found << std::endl;
				sleep(3);
				if (request_http.compare(found, size_name_location, this->_servers[0].location[i].name_location) == 0)
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
				else
				{
					found = request_http.find(this->_servers[0].location[i].name_location);
					if (found == std::string::npos)
					{
						std::cout << "ERREUR 36363 euh pas normal ne trouve pas / dans la requete a traiter "<< std::endl;
						exit(EXIT_FAILURE);
					}
					else
					{
						if (request_http.compare(found, size_name_location, this->_servers[0].location[i].name_location) == 0)
						{
							std::cout << "OUI on se trouve dans un dossier location a continuer  22" << std::endl;
							
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
												return (-1);	// retourne -1 si location est le premier == 0
											else
												return (i); // sinon retour l'endroit de location
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
					std::cout << "euh non pas dans un location " << std::endl;
					std::cout << " REKT FRAGILE " << std::endl;
					exit(EXIT_FAILURE);
				}

				exit(1);
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
void			HttpServer::ft_post(std::string request_http, int len_msg)
{
	(void)request_http;
	(void)len_msg;
}

/*
**	A FAIRE CAS DES REQUETES DELETE
*/
void			HttpServer::ft_delete(std::string request_http, int len_msg)
{
	(void)request_http;
	(void)len_msg;
}



/*
**	std::string		ft_check_host_header( std::string header )
**		This function checks for the presence of 'Host' in the client request.
**
**		et retourne une string contenant le host et le port.
**		exmeple: 127.0.0.1:8080
**			==> ne marche pas si 8081 			A FAIRE
*/
std::string		HttpServer::ft_check_host_header( std::string header )
{
	size_t pos;

	pos = 0;
	if ((pos = header.find("Host: ", 0)) == std::string::npos)
	{
		// A FAIRE: creer une erreur propre.
		std::cout << "ERREUR NE TROUVE PAS LE HOST DANS LE HEADER" << std::endl;
		return (NULL);
	}
	else
	{
		size_t pos_end;
		if ((pos_end = header.find("\r\n", pos)) == std::string::npos)
		{
			// A FAIRE: creer une erreur propre.
			std::cout << "ERREUR NE TROUVE PAS LE USER-AGENT DANS LE HEADER" << std::endl;
			return ("");
		}
		else
		{
			// on verifie que la position de host est avant la position de user-agent.
			if (pos > pos_end)
			{
				// A FAIRE: creer une erreur propre.
				std::cout << "ERREUR HOST doit etre avant USER-AGENT" << std::endl;
				return (NULL);
			}
			// on recupere les informations apres Host et avant User-agent
			std::string tmp(header, pos + 6, pos_end - (pos + 6));
			for (size_t i = 0; i < this->_data->ft_get_nbr_servers(); i++)
			{
				// on parcourt nos servers pour verifier que le host de la requete est
				//	bien pour un de nos servers.
				if (tmp.compare(0, 9, this->_servers[i].host_server) == 0)
				{
					// les host de la requete et de notre server sont egaux
					//	ex: 127.0.0.1
					std::cout << "ILS SONT EGAUX les host" << std::endl;

					// on convertit le port (std::size_t) en std::string.
					std::stringstream ss;
					ss << this->_servers[i].port_server;
					std::string port;
					ss >> port;
					if (tmp.compare(10, 4, port) == 0)
					{
						// les ports sont bon, on retourne la string au complete
						//		ex : 127.0.0.1:8080
						std::cout<< "Ils sont egaux les port cas 1" << std::endl;
						return (tmp);
					}
					else
					{
						//	Les ports ne correspondent pas.
						if (this->_data->ft_get_nbr_servers() > 1)
						{
							// Cas ou on a plusieurs servers
							std::cout << "euh nbr serv = " << this->_data->ft_get_nbr_servers() << std::endl;
							for (size_t y = 0; y < this->_data->ft_get_nbr_servers(); y++)
							{
								// on parcourt chaque servers pour voir si on a un port
								//	qui correspond a la requete du client.
								std::stringstream ss2;
								std::string port2;
								std::cout << RED << "y = " << y << " et port server = " << this->_servers[y].port_server << CLEAR << std::endl;
								ss2 << this->_servers[y].port_server;
								ss2 >> port2;
								std::cout << "Punaise = " << port2 << " et y = " << y << std::endl;
								if (tmp.compare(10, 4, port2) == 0)
								{
									std::cout<< "Ils sont egaux les port cas 2" << std::endl;
									return (tmp);
								}
								ss2.str("");
								ss2.flush();
								port2 = "";
							}
						}
						std::cout << "port pas egaux ? " << std::endl;
						std::cout << " tmp = -" << tmp << "- et nous = -" << port << "-" << std::endl;
						throw Error(666, "Erreur test lol, ", 666);			// A FAIRE au propre
					}
				}
				else
				{
					std::cout << "les host ne sont pas egaux" << std::endl;
					throw Error(666, "Erreur test lol 2, ", 666);		// A FAIRE au propre
				}
				// if (this->_servers[i].host_server == tmp)
				// 	return (tmp);
			}
			std::cout << "ERREUR ICI " << std::endl;			// A FAIRE au propre
			exit(1);
			return (std::string(header, pos + 6, pos_end - (pos + 6)));
		}
	}

}

/*
**	std::string		ft_check_protocol_header( std::string header )
**		This function checks the version of the HTTP protocol used by the client's request.
**	

**		Attention ne epeut pas retour ner NULL
*/
std::string		HttpServer::ft_check_protocol_header( std::string header )
{
	size_t pos;

	if ((pos = header.find("HTTP/1.1")) == std::string::npos)
	{
		// A FAIRE: creer une erreur propre.
		std::cout << "ERREUR NE TROUBE PAS LE PROTOCOL DANS LE HEADER" << std::endl;
		return (NULL);

	}
	else
	{
		return (std::string(header, pos, 8));
	}
}


std::string		HttpServer::ft_check_path_header( std::string header )
{
	size_t	pos;
	if ((pos = header.find_first_of("/", 0)) == std::string::npos)
	{
		// A FAIRE: creer une erreur propre.
		std::cout << "ERREUR NE TROUVE PAS de / dans le path du HEADER DE LA REQUETE \n";
		return (NULL);
	}
	else
	{
		size_t pos_http;
		if ((pos_http = header.find("HTTP/1.1\r\n")) == std::string::npos)
		{
			std::cout << "ERREUR NE TROUVE PAS  HTTP protocol dans le PATH du HEADER DE LA REQUETE \n";
			return (NULL);
		}
		else
		{
			if (pos > pos_http)
			{
				// A FAIRE: creer une erreur propre.
				std::cout << "ERREUR le path doit etre avant le http version dans le header du client \n";
				return (NULL);
			}
			// tout est bon, on retour le path en entier.
			std::string tmp(header, pos, pos_http - pos);
			return (tmp);
		}
	}
}

/*
** std::string		ft_check_methods_header( std::string header )
**	fonction useless je pense
**	checks la method utilisee dans la requete du client
**	et setup retourne une string contennant la methode utilisee.
*/
std::string		HttpServer::ft_check_methods_header( std::string header )
{
	size_t pos;
	if ((pos = header.find("GET", 0)) == std::string::npos)
	{
		if ((pos = header.find("POST", 0)) == std::string::npos)
		{
			if ((pos = header.find("DELETE", 0)) == std::string::npos)
			{
				std::cout << "check methos header probleme return null rien a ete trouve" << std::endl;
				return (NULL);
			}
			else
				return (std::string(header, pos, 6));;
		}
		else
			return (std::string(header, pos, 4));

	}
	else
		return (std::string(header, pos, 3));

	return (NULL);
}