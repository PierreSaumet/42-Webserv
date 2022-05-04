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

// 4 fonctions
//	 1 pour choisir la methode
//	et les 3 methods
void	HttpServer::ft_parser_requete( int len_msg, std::string msg )
{
	std::string request_http(msg);
	if (request_http.compare(0, 4, "GET ") == 0)
		this->ft_get(request_http, len_msg);
	else if (request_http.compare(0, 5, "POST ") == 0)
		this->ft_post(request_http, len_msg);
	else if (request_http.compare(0, 7, "DELETE ") == 0)
		this->ft_delete(request_http, len_msg);
	else
	{
		if (this->_header_requete.empty() == true)
		{
			this->_header_requete.push_back(t_header_request());
			this->_header_requete[0].error = true;
			this->_header_requete[0].num_error = 405;
			this->ft_setup_error_header();
		}
		else
		{
			this->_header_requete.push_back(t_header_request());
			this->_header_requete[0].error = true;
			this->_header_requete[0].num_error = 500;
			this->ft_setup_error_header();
		}
	}
	return ;
}


size_t			HttpServer::ft_get( std::string request_http, int len_msg)
{
	std::cout << GREEN << "Dans get : " << CLEAR <<  std::endl;

	if (this->_header_requete.empty() == true)
	{
		this->_header_requete.push_back(t_header_request());
		if (len_msg > 1023)
		{
			std::cout << RED << "On a une  ERREUR 431 car GET method et donnees trop grandes " << CLEAR << std::endl;
			this->_header_requete[0].error = true;
			this->_header_requete[0].num_error = 431; 
			this->ft_setup_error_header();
			return (0);
		}
		std::cout << BLUE << "Ok pas d'erreur 431 donc on continue." << CLEAR <<  std::endl;

		size_t 			pos_header = request_http.find("\r\n\r\n"); // faire erreur si pas de fin de header
		std::string 	size_header(request_http, 0, pos_header);

		this->_header_requete[0].method = "GET";
		std::cout << "On a la method : " << this->_header_requete[0].method << "-" <<  std::endl;
	
		this->_header_requete[0].host = this->ft_check_host_header(size_header);
		if (this->_header_requete[0].host.empty() == true)
			throw Error(14, "Error, in recieved header, the host is not correct.", 2);			
		std::cout << "On a le host : " << this->_header_requete[0].host << "-" << std::endl;
		// exit(1);
		int ret_serv = 0;
		if ((ret_serv = this->ft_find_index_server()) == -1)
		{
			std::cout << "trouve pas le server exit" << std::endl;
			exit(1);
		}
		this->_num_serv = ret_serv;			// on utiliser le numero du server
		std::cout << "On est sur notre server numeor : " << ret_serv << std::endl;
		
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

		this->_header_requete[0].accept = this->ft_check_accept_header(size_header);
		if (this->_header_requete[0].accept.empty() == true)
			throw Error(15, "Error, in recieved header, the accept is not correct.", 2);
		std::cout << "On a le accept = "<< this->_header_requete[0].accept << std::endl;

		this->_header_requete[0].path_http = this->ft_check_pathhttp_header(size_header);
		if (this->_header_requete[0].path_http.empty() == true)
			throw Error(16, "Error, in recieved header, the path of the file si not correct." , 2);
		// std::cout << "le path = " << this->_header_requete[0].path_http << std::endl;
		

		size_t ret_method = 0;
		if ((ret_method = ft_check_method_allowed_header(request_http, "GET")) > 0)				// On verifie que la methode est autorisee
		{
			std::cout << RED << "La methode GET est interdite donc on sort une erreur 405" << CLEAR << std::endl;
			// exit(1);
			this->_header_requete[0].error = true;
			if (ret_method == 1)
				this->_header_requete[0].num_error = 405;
			if (ret_method == 2)
				this->_header_requete[0].num_error = 404;
			this->ft_setup_error_header();
			return (0);
		}
		std::cout << BLUE << "La methode GET est autorisee, on continue." << CLEAR << std::endl;
		
		
		if (this->ft_check_cgi_or_php(request_http) == 1)
		{
			this->ft_exec_cgi_test( request_http, len_msg);
			return (0);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		// TEST REDIRECTION AVEC RETURN
		size_t res = 0;
		if ((res = this->ft_redirection()) > 0)
		{
			// impossible
			this->_header_requete[0].error = true;
			if (res == 1)
				this->_header_requete[0].num_error = 404;
			if (res == 2)
				this->_header_requete[0].num_error = 403;
			this->ft_setup_error_header();
			return (0);

		}

		
		std::cout << "Avant ft_check_access_path, le header.path = " << this->_header_requete[0].path << std::endl;
		res = 0;
		if ((res = this->ft_check_access_path()) > 0)
		{
			std::cout << "erreur verifie le drot res = " << res << std::endl;
			sleep(2);
			this->_header_requete[0].error = true;
			if (res == 2)
				this->_header_requete[0].num_error = 403;
			if (res == 1)
				this->_header_requete[0].num_error = 404;
			this->ft_setup_error_header();
			return (0);
		}
		std::cout << GREEN << "On a bien recu une demande " << CLEAR << std::endl;
		std::cout << " la requete est = " << this->_header_requete[0].path << std::endl;
	}
	else
	{
		std::cout << RED << "Probleme le container qui recupere la header de la requete n'est pas vide. " << std::endl;
		std::cout << "Il faut le supprimer apres avoir fait traite une demande." << CLEAR << std::endl;
	}
	return (0);
}




/*
**		A FAIRE CAS DES REQUETE POST
*/
size_t			HttpServer::ft_post(std::string request_http, int len_msg)
{
	std::cout << GREEN  << "Dans ft_POST: " << CLEAR << std::endl;

	if (this->_header_requete.empty() == true)
	{
		this->_header_requete.push_back(t_header_request());

		size_t 			pos_hea = request_http.find("\r\n\r\n"); // si troue pas erreur
		std::string 	size_header(request_http, 0, pos_hea);
		std::string 			size_body(request_http, size_header.size(), request_http.size());		// on prend aussi le \r\n\r\n donc +4
		
		if (size_header.size() > 1023)	// on verifie que le header ne soit pas trop long
		{
			this->_header_requete[0].error = true;
			this->_header_requete[0].num_error = 431; 
			this->ft_setup_error_header();
			return (0);

		}
		std::cout << "header = " << size_header << std::endl;
		std::cout << "taille header = " << size_header.size() << std::endl;
		std::cout << "taille body = " << len_msg - size_header.size() << std::endl;
		std::cout << "et taille std::string body = " << size_body.size() << std::endl;
		std::cout << "taille total = " << len_msg << std::endl;
	
		std::cout << "il faut enelever 4 a la size_bodysize(0 pour avoi le body exacte" << std::endl;

		// test buffer size_ exclusivement dans le server
		//////////////////////////////////////////////////////////////////////////////////
		//				test size
		////////////////////////////////////////////////////////////

		this->_header_requete[0].host = this->ft_check_host_header(size_header);
		if (this->_header_requete[0].host.empty() == true)
			throw Error(14, "Error, in recieved header, the host is not correct.", 2);			
		std::cout << "\nOn a le host : " << this->_header_requete[0].host << "-" << std::endl;
		
		int ret_serv = 0;
		if ((ret_serv = this->ft_find_index_server()) == -1)
		{
			std::cout << "trouve pas le server exit" << std::endl;
			exit(1);
		}
		this->_num_serv = ret_serv;			// on utiliser le numero du servers
		std::cout << "le num du server est setup ? = " << this->_num_serv << std::endl;

		if (this->_servers[this->_num_serv].buffer_size_server == 0)
			this->_servers[this->_num_serv].buffer_size_server = 1000000;
		if (size_body.size() - 4 > this->_servers[this->_num_serv].buffer_size_server)
		{
			this->_header_requete[0].error = true;
			this->_header_requete[0].num_error = 431;
			this->ft_setup_error_header();
			return (0);
		}
		////////////////////////////////////////////////

		// On recupere la methode
		this->_header_requete[0].method = "POST";
		std::cout << "\nOn a la requete : " << this->_header_requete[0].method << "-" <<  std::endl;
	
		this->_header_requete[0].path = this->ft_check_path_header(size_header);
		if (this->_header_requete[0].path.empty() == true)
			throw Error(12, "Error, in recieved header, the path is not correct.", 2);;
		std::cout << "\nOn a le path : " << this->_header_requete[0].path << "\n" <<  std::endl;
		
		// Surement useless
		this->_header_requete[0].referer = this->ft_check_referer(size_header);
		if (this->_header_requete[0].referer.empty() == true && this->_recv_complete.chunked == false)
		{
			std::cout << "ERREUR " << std::endl;
			exit(1);
		}
		std::cout << "\nOn a le referer : " << this->_header_requete[0].referer << "\n" <<  std::endl;


	

		// des qu'on a le path on verifie la method
		if (ft_check_method_allowed_header(this->_header_requete[0].path, "POST") == 1)				// On verifie que la methode est autorisee
		{
			std::cout << RED << "La methode POST est interdite donc on sort une erreur 405" << CLEAR << std::endl;
			this->_header_requete[0].error = true;
			this->_header_requete[0].num_error = 405;
			this->ft_setup_error_header();
			return (0);
		}
		std::cout << BLUE << "La methode POST est autorisee, on continue." << CLEAR << std::endl;



		this->ft_parsing_path_get_request();
		std::cout << "\nOn a la query_string : " << this->_header_requete[0].query_string << std::endl;

		this->_header_requete[0].protocol = this->ft_check_protocol_header(size_header);
		if (this->_header_requete[0].protocol.empty() == true)
			throw Error(13, "Error, in recieved header, the protocol is not correct.", 2);
		std::cout << "\nOn a le protocol : " << this->_header_requete[0].protocol << "-" << std::endl;

		

		// RAJOUT VENDREDi
		this->_header_requete[0].accept = this->ft_check_accept_header(size_header);
		if (this->_header_requete[0].accept.empty() == true)
			throw Error(15, "Error, in recieved header, the accept is not correct.", 2);
		std::cout << "\nOn a le accept = "<< this->_header_requete[0].accept << std::endl;
	
		
		this->_header_requete[0].path_http = this->ft_check_pathhttp_header(size_header);
		if (this->_header_requete[0].path_http.empty() == true)
			throw Error(16, "Error, in recieved header, the path of the file is not correct." , 2);
		std::cout << "\nOn a le path = -" << this->_header_requete[0].path_http << "-" <<  std::endl;
		
		// exit(1);

		this->_header_requete[0].content_length = this->ft_check_content_length(size_header);
		if (this->_header_requete[0].content_length.empty() == true && this->_recv_complete.chunked == false)
		{
			this->_header_requete[0].error = true;
			this->_header_requete[0].num_error = 411; 
			this->ft_setup_error_header();
			return (0);
		}

		std::cout << "\nOn a le content_length = -" << this->_header_requete[0].content_length << "-" <<  std::endl;
		// sleep(5);
		// si body 0 genre un fichier avec 0 droit ex chmod 000
		if (this->_header_requete[0].content_length == "0")
		{
			std::cout << "OUI ERROR 400" << std::endl;
			this->_header_requete[0].error = true;
			this->_header_requete[0].num_error = 400; 
			this->ft_setup_error_header();
			return (0);

		}
		else
		{
			std::cout << "GOOD " << std::endl;
		}
		// sleep(10);

		this->_header_requete[0].content_type = this->ft_check_content_type(size_header);
		if (this->_header_requete[0].content_type.empty() == true)
			throw Error(16, "Error, in recieved header, the content_type is  not correct." , 2);
		std::cout << "\nOn a le content_type = -" << this->_header_requete[0].content_type << "-" <<  std::endl;
		

		this->_header_requete[0].body_post = this->ft_check_body_post(size_body);
		if (this->_header_requete[0].body_post.empty() == true)
			throw Error(16, "Error, in recieved header, the body_post is  not correct." , 2);
		std::cout << "\nOn a le body_post = -" << this->_header_requete[0].body_post << "-" <<  std::endl;
		


		

		if (this->ft_check_cgi_or_php(request_http) == 1)  // verifier cette fonction
		{
			// on a du php ou du cgi ?
			// donc faut utiliser cgi
			std::cout << "OUI " << std::endl;
			
			this->ft_exec_cgi_test( request_http, len_msg);
			// exit(1);
			return (0);
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





#include "sys/stat.h"
size_t			HttpServer::ft_delete(std::string request_http, int len_msg)
{
	std::cout << GREEN << "DANS FT_DELETE " << CLEAR << std::endl;
	std::cout << "REQUEST = " << request_http << std::endl;
	std::cout << "len_msg = " << len_msg << std::endl;



	if (this->_header_requete.empty() == true)
	{
		this->_header_requete.push_back(t_header_request());


		// si pas de fin de header faire une erreur
		size_t pos_header = request_http.find("\r\n\r\n");
		std::string size_header(request_http, 0, pos_header);

		// On recupere la methode
		this->_header_requete[0].method = "DELETE";
		std::cout << "On a la requete : " << this->_header_requete[0].method << "-" <<  std::endl;
	
		// On recupere le path contenant des donnees s'il y en a. Et on y a rajoute le root
		this->_header_requete[0].path = this->ft_check_path_header(size_header);
		if (this->_header_requete[0].path.empty() == true)
			throw Error(12, "Error, in recieved header, the path is not correct.", 2);;
		std::cout << "On a le path : " << this->_header_requete[0].path << std::endl;
		
		this->_header_requete[0].protocol = this->ft_check_protocol_header(size_header);
		if (this->_header_requete[0].protocol.empty() == true)
			throw Error(13, "Error, in recieved header, the protocol is not correct.", 2);
		std::cout << "On a le protocol : " << this->_header_requete[0].protocol << "-" << std::endl;

		this->_header_requete[0].host = this->ft_check_host_header(size_header);
		if (this->_header_requete[0].host.empty() == true)
			throw Error(14, "Error, in recieved header, the host is not correct.", 2);			
		std::cout << "On a le host : " << this->_header_requete[0].host << "-" << std::endl;

		// exit(1);

		if (ft_check_method_allowed_header(this->_header_requete[0].path, "DELETE") == 1)				// On verifie que la methode est autorisee
		{
			std::cout << RED << "La methode DELETE est interdite donc on sort une erreur 405" << CLEAR << std::endl;
			this->_header_requete[0].error = true;
			this->_header_requete[0].num_error = 405;
			exit(1);
			this->ft_setup_error_header();
			return (0);
		}
		std::cout << BLUE << "La methode DELETE est autorisee, on continue." << CLEAR << std::endl;



		// Donc on peut delete
		struct stat buff;

		if (stat(this->_header_requete[0].path.c_str(), &buff) != 0)		// chmod 000 677
		{
			// on a pas acces au fichier donc on sort une erreur 403
			this->_header_requete[0].error = true;
			this->_header_requete[0].num_error = 403;
			this->ft_setup_error_header();
			return (0);
		}
		else
		{
			if (remove(this->_header_requete[0].path.c_str()) != 0)
			{
				std::cout << "ON NE PEUT PAS SUPPRIMER" << std::endl;
				this->_header_requete[0].error = true;
				this->_header_requete[0].num_error = 403;
				this->ft_setup_error_header();
				return (0);
			}
			else
			{
				std::cout << "SI ON PEUT SUPPRIMER " << std::endl;
				this->_header_requete[0].cgi = true;
				this->_header_requete[0].body_error = "\r\n\r\n<!DOCTYPE html><html><head><title>204</title><style type=text/css>body {color: blue;font-weight: 900;font-size: 20px;font-family: Arial, Helvetica, sans-serif; }</style><link rel=\"icon\" type=\"image/x-con\" href=\"/flavicon.ico\"/><link rel=\"shortcut icon\" type=\"image/x-con\" href=\"/flavicon.ico\" /></head><body><h1>File delete so No Content =)</h1><p>by Pierre.</p></body></html>";				
				this->_header_requete[0].num_error = 204;
				return (0);
			}
		}

	}
	else
	{
		std::cout << RED << "DELETE Probleme le container qui recupere la header de la requete n'est pas vide. " << std::endl;
		std::cout << "Il faut le supprimer apres avoir fait traite une demande." << CLEAR << std::endl;
	}
	return (0);
	exit(1);
}


