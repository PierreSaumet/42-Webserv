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

size_t HttpServer::ft_choose_wich_server( std::string header, int num )
{
	std::cout << GREEN << "Dans ft_choose_wich_server( std::string header );" << CLEAR << std::endl;

	std::cout << "HEADER = " << header << std::endl;
	// si http/1.1 absent = erreur
	size_t pos = 0;
	if ((pos = header.find("HTTP/1.1\r\n")) == std::string::npos)
	{
		std::cout << "Erreur pas http/1/" << std::endl;
		exit(1);
	}
	if ((pos = header.find("Host: ")) == std::string::npos)
	{
		std::cout << "Erreur pas host" << std::endl;
		exit(1);
	}
	if ((pos = header.find("Accept: ")) == std::string::npos)
	{
		std::cout << "Erreur pas Accept" << std::endl;
		exit(1);
	}

	// on parcourt les servers pour voir lequel choisir en fonction du HOST
	// on cherche le host;
	pos = header.find("Host: ");
	size_t end = header.find("\r\n", pos);
	std::string tmp(header, pos + 6, end - (pos + 6));
	std::cout << "on veut le server = " << tmp << std::endl;
	std::cout << "le num du server du client = " << num << std::endl;
	std::cout << " il y a :" << this->_data->ft_get_nbr_servers() << " server " << std::endl;
	size_t i = 0;
	if (this->_data->ft_get_nbr_servers() == 1)
		return (0);
	while ( i < this->_data->ft_get_nbr_servers())
	{
		std::cout << " host + port = " << this->_servers[i].host_server << " et " << this->_servers[i].port_server << std::endl;
		if (this->_servers[i].host_server == this->_servers[num].host_server)
		{
			if (this->_servers[i].port_server == this->_servers[num].port_server)
			{
				if (tmp == this->_servers[i].name_server)
				{
					std::cout << "BINGO on utilise le server numero : = " << i << std::endl;
					exit(1);
				}
			}
		}
		i++;
		
		// if (tmp == this->_servers[i].name_server && num == (int)i)
		// {
		// 	// if (num == (int)i)
		// 	// {
		// 		std::cout << "Bingo = " << tmp << " et server = "  << i << std::endl;
		// 		exit(1);
		// 	// }
		// }
		// i++;
	}
	std::cout << "On trouve pas le nom qui correspond  on retourne num :" << num << std::endl;
	return (num);

}



HttpServer::t_header_request	HttpServer::ft_parser_requete( int port_client, int len_msg, std::string msg )
{
	std::cout << "le client : " << port_client << " est dans parser requete" << std::endl;
	// this->_all_client_socket.client_socket
	if (this->_header_requete.empty() == false)
	{
		std::cout << "ERTRREUR impossible " << std::endl;
		exit(1);
	}
	size_t pos = 0;
	if ((pos = msg.find("\r\n\r\n")) == std::string::npos)
	{
		std::cout << "ERREURURURUR ";
		exit(1);
	}
	std::string tmp(msg, 0, pos);
	this->ft_choose_wich_server(tmp, port_client);

	// std::cout << "MSG = " << msg << std::endl;
	exit(1);


	std::string request_http(msg);  // useless
	if (request_http.compare(0, 4, "GET ") == 0)
	{
		this->ft_get(request_http, len_msg);
		return (this->_header_requete[0]);
	}
	else if (request_http.compare(0, 5, "POST ") == 0)
		this->ft_post(request_http, len_msg);
	else if (request_http.compare(0, 7, "DELETE ") == 0)
		this->ft_delete(request_http, len_msg);
	else
	{
		std::cout << "ERRUER NI GET NI POST NI DELETE = " << msg << std::endl;
		if (this->_header_requete.empty() == true)    // a changer
		{
			this->_header_requete.push_back(t_header_request());
			this->ft_do_error(405);
		}
		else
		{
			this->_header_requete.push_back(t_header_request());
			this->ft_do_error(500);
		}
	}
	std::cout << "aie aie " << std::endl;
	exit(1);
	// return ;
	return (this->_header_requete[0]);
}


size_t			HttpServer::ft_get(std::string request_http, int len_msg)
{
	std::cout << GREEN << "Dans get : " << CLEAR <<  std::endl;

	if (this->_header_requete.empty() == true)
	{
		this->_header_requete.push_back(t_header_request());
		// this->_header_requete
		if (len_msg > 1023)
			return (ft_do_error(431));

		size_t 			pos_header = request_http.find("\r\n\r\n"); // faire erreur si pas de fin de header
		std::string 	size_header(request_http, 0, pos_header);
		int 			ret = 0;

		this->_header_requete[0].method = "GET";
		// std::cout << "On a la method : " << this->_header_requete[0].method << "-" <<  std::endl;
	
		this->_header_requete[0].host = this->ft_check_host_header(size_header);
		if (this->_header_requete[0].host.empty() == true)
			return (ft_do_error(400));		

		
		if ((ret = this->ft_find_index_server()) == -1)
			return (ft_do_error(500));
		this->_num_serv = ret;
		
		
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


		// Check the methodd
		if ((ret = ft_check_method_allowed_header(request_http, "GET")) > 0)
		{
			this->_header_requete[0].error = true;
			if (ret == 1)
				this->_header_requete[0].num_error = 405;
			if (ret == 2)
				this->_header_requete[0].num_error = 404;
			this->ft_setup_error_header();
			return (1);
		}
		
		if (this->ft_check_cgi_or_php(request_http) == 1)
		{
			int 				res = 0;
			size_t 				pos = this->_header_requete[0].path.find("?");
			
			if (pos != std::string::npos)
			{
				std::string 	tmp = this->_header_requete[0].path;

				this->_header_requete[0].path.erase(pos, this->_header_requete[0].path.size());				
				if ((res = this->ft_check_access_path()) > 0)
				{
					this->_header_requete[0].error = true;
					if (res == 2)
						this->_header_requete[0].num_error = 403;
					if (res == 1)
						this->_header_requete[0].num_error = 404;
					this->ft_setup_error_header();
					return (0);
				}
				pos = tmp.find("?");
				tmp.erase(0, pos);
				this->_header_requete[0].path.append(tmp);
				this->_header_requete[0].request_uri = this->_header_requete[0].path;
				this->_header_requete[0].script_file_name = this->_header_requete[0].path;
				pos = this->_header_requete[0].script_file_name.find("?");
				this->_header_requete[0].script_file_name.erase(pos, this->_header_requete[0].script_file_name.size());
			}
			else
			{
				if ((res = this->ft_check_access_path()) > 0)
				{
					this->_header_requete[0].error = true;
					if (res == 2)
						this->_header_requete[0].num_error = 403;
					if (res == 1)
						this->_header_requete[0].num_error = 404;
					this->ft_setup_error_header();
					return (0);
				}
				this->_header_requete[0].request_uri = this->_header_requete[0].path;
				this->_header_requete[0].script_file_name = this->_header_requete[0].path;
			}
			std::cout << "fin = \n" << std::endl;
			std::cout << "this->_header_requete[0].path = " << this->_header_requete[0].path << std::endl;
			std::cout << "this->_header_requete[0].request_uri  = " << this->_header_requete[0].request_uri  << std::endl;
			std::cout << "this->_header_requete[0].script_file_name = " << this->_header_requete[0].script_file_name << std::endl;

			// We get the total path for the varriable path_http
			char	cwd[256];
			if (getcwd(cwd, sizeof(cwd)) == NULL)
				throw Error(666, "Error getcwd doesn't work.", 666);
			
			std::string tt(cwd);
			this->_header_requete[0].path_http = tt;
			this->_header_requete[0].path_http.append("/");
			this->_header_requete[0].path_http.append(this->_header_requete[0].script_file_name);

			this->ft_exec_cgi_test(); //request_http, len_msg);
			return (0);
		}

		// A TERMINER
		if ((ret = this->ft_redirection()) < 0)
		{
			// impossible
			this->_header_requete[0].error = true;
			if (ret == 1)
				this->_header_requete[0].num_error = 404;
			if (ret == 2)
				this->_header_requete[0].num_error = 403;
			this->ft_setup_error_header();
			return (0);
		}
		if (ret == 1)
			return (0);
		// std::cout << "apres redirection : " << std::endl;
		// std::cout << "path = " << this->_header_requete[0].path << std::endl;
		// std::cout << "this->_header_requete[0].return_used = " << this->_header_requete[0].return_used << std::endl;
		// std::cout << "this->_servers[this->_num_serv].return_server  = " << this->_servers[this->_num_serv].return_server << std::endl;
		// std::cout << "this->_servers[this->_num_serv].location[this->_num_loc].return_location = " << this->_servers[this->_num_serv].location[this->_num_loc].return_location << std::endl;

		// exit(1);

		// Check the uri and change it to the corresponding location
		if ((ret = this->ft_check_access_path()) > 0)
		{
			std::cout << "erreur verifie le drot ret = " << ret << std::endl;
			
			this->_header_requete[0].error = true;
			if (ret == 2)
				this->_header_requete[0].num_error = 403;
			if (ret == 1)
				this->_header_requete[0].num_error = 404;
			this->ft_setup_error_header();
			return (0);
		}
		std::cout << GREEN << "On a bien recu une demande " << CLEAR << std::endl;
		std::cout << " la requete est = " << this->_header_requete[0].path << std::endl;
		return (0);
	}
	else
		return (ft_do_error(500));
	
}


/*
**		A FAIRE CAS DES REQUETE POST
*/
size_t			HttpServer::ft_post(std::string request_http, int len_msg)
{
	std::cout << GREEN  << "Dans ft_POST: " << CLEAR << std::endl;
	(void)len_msg;
	if (this->_header_requete.empty() == true)
	{
		int 					res = 0;
		this->_header_requete.push_back(t_header_request());

		size_t 					pos_hea = request_http.find("\r\n\r\n"); // si troue pas erreur
		std::string 			size_header(request_http, 0, pos_hea);
		std::string 			size_body(request_http, size_header.size(), request_http.size());		// on prend aussi le \r\n\r\n donc +4
		std::string 			chunked_string = "";

		if (size_header.size() > 1023)
			return (ft_do_error(431));

		if (this->_recv_complete.chunked == true)
		{
			// We recieved chunked data, so we have to decode it
			if (( chunked_string = this->ft_decode_chunked( size_body )) == "")
				return (ft_do_error(400));
			chunked_string.append("\0");
			size_body = chunked_string;
		}
		
		// Now, we setup all variables need to display an answer
		// so we check all informations from the header.

		this->_header_requete[0].host = this->ft_check_host_header(size_header);
		if (this->_header_requete[0].host.empty() == true)
			return (ft_do_error(400));
		
		// Get the index server to know which one we should use
		if ((res = this->ft_find_index_server()) == -1)
			return (ft_do_error(500));
		this->_num_serv = res;
		// std::cout << "le num du server est setup ? = " << this->_num_serv << std::endl;

		// Check if a size of client buffer is setup and if the request's body is smaller
		if (this->_servers[this->_num_serv].buffer_size_server == 0)
			this->_servers[this->_num_serv].buffer_size_server = 1000000;
		if (size_body.size() - 4 > this->_servers[this->_num_serv].buffer_size_server)
			return (ft_do_error(413));

		this->_header_requete[0].method = "POST";
	
		this->_header_requete[0].path = this->ft_check_path_header(size_header);
		if (this->_header_requete[0].path.empty() == true)
			return (ft_do_error(400));

		// Check if the method used is allowed
		if (ft_check_method_allowed_header(this->_header_requete[0].path, "POST") == 1)
			return (ft_do_error(405));
		std::cout << BLUE << "La methode POST est autorisee, on continue." << CLEAR << std::endl;

		this->ft_parsing_path_get_request();
		// std::cout << "\nOn a la query_string : " << this->_header_requete[0].query_string << std::endl;

		this->_header_requete[0].protocol = this->ft_check_protocol_header(size_header);
		if (this->_header_requete[0].protocol.empty() == true)
			return (ft_do_error(400));
		// std::cout << "\nOn a le protocol : " << this->_header_requete[0].protocol << "-" << std::endl;


		this->_header_requete[0].accept = this->ft_check_accept_header(size_header);
		if (this->_header_requete[0].accept.empty() == true)
			return (ft_do_error(400));
		// std::cout << "\nOn a le accept = "<< this->_header_requete[0].accept << std::endl;
	
		
		this->_header_requete[0].path_http = this->ft_check_pathhttp_header(size_header);
		if (this->_header_requete[0].path_http.empty() == true)
			return (ft_do_error(400));
		// std::cout << "\nOn a le path = -" << this->_header_requete[0].path_http << "-" <<  std::endl;

		this->_header_requete[0].content_length = this->ft_check_content_length(size_header);
		if (this->_header_requete[0].content_length.empty() == true && this->_recv_complete.chunked == false)
			return (ft_do_error(411));
		// std::cout << "\nOn a le content_length = -" << this->_header_requete[0].content_length << "-" <<  std::endl;
		
		if (this->_header_requete[0].content_length == "0")  // maybe uselss
			return (ft_do_error(400));

		this->_header_requete[0].content_type = this->ft_check_content_type(size_header);
		if (this->_header_requete[0].content_type.empty() == true)
			return (ft_do_error(400));
		// std::cout << "\nOn a le content_type = -" << this->_header_requete[0].content_type << "-" <<  std::endl;
		

		this->_header_requete[0].body_post = this->ft_check_body_post(size_body);
		if (this->_header_requete[0].body_post.empty() == true)
			// throw Error(16, "Error, in recieved header, the body_post is  not correct." , 2);
		// std::cout << "\nOn a le body_post = -" << this->_header_requete[0].body_post << "-" <<  std::endl;
		
		// Put chunked decoded data to body_post
		if (this->_recv_complete.chunked == true)
			this->_header_requete[0].body_post = chunked_string;

		// Check the uri and change it to the corresponding location
		if ((res = this->ft_check_access_path()) > 0) 
		{
			this->_header_requete[0].error = true;
			if (res == 2)
				this->_header_requete[0].num_error = 403;
			if (res == 1)
				this->_header_requete[0].num_error = 404;
			this->ft_setup_error_header();
			return (0);
		}

		if (this->ft_check_cgi_or_php(request_http) == 1)  // verifier cette fonction
		{
			// There is cgi so we need to change some variables, otherwise it doesn't work
			char	cwd[256];
			if (getcwd(cwd, sizeof(cwd)) == NULL)
				throw Error(4, "Error, getcwd doesn't work", 2);
			std::string tt(cwd);
			this->_header_requete[0].path_http = tt;
			this->_header_requete[0].path_http.append("/");
			this->_header_requete[0].path_http.append(this->_header_requete[0].script_file_name);
			this->_header_requete[0].request_uri = this->_header_requete[0].path;
			this->_header_requete[0].script_file_name = this->_header_requete[0].path;
			this->ft_exec_cgi_test(); //request_http, len_msg);
			return (0);
		}
		std::cout << GREEN << "On a bien recu une demande " << CLEAR << std::endl;
		return (0);
	}
	return (ft_do_error(500));
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

		int ret_serv = 0;
		if ((ret_serv = this->ft_find_index_server()) == -1)
		{
			std::cout << "trouve pas le server exit" << std::endl;
			exit(1);
		}
		this->_num_serv = ret_serv;			// on utiliser le numero du servers
		std::cout << "le num du server est setup ? = " << this->_num_serv << std::endl;

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

		int res = 0;
		if ((res = this->ft_check_access_path()) > 0)
		{
			std::cout << "ERREUR ACCESS DELETE= " << res << std::endl;
			sleep(2);
			exit(1);
			this->_header_requete[0].error = true;
			if (res == 2)
				this->_header_requete[0].num_error = 403;
			if (res == 1)
				this->_header_requete[0].num_error = 404;
			this->ft_setup_error_header();
			return (0);
		}

		// Donc on peut delete
		struct stat buff;
		std::cout << "path this->_header_requete[0].path = " << this->_header_requete[0].path << std::endl;
		// exit(1); 
		if (stat(this->_header_requete[0].path.c_str(), &buff) != 0)		// chmod 000 677
		{
			// on a pas acces au fichier donc on sort une erreur 403
			this->_header_requete[0].error = true;
			this->_header_requete[0].num_error = 404;
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


