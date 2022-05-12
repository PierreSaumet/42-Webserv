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

int HttpServer::ft_check_basic( std::string header )
{
	size_t 		pos = 0;
	if ((pos = header.find("HTTP/1.1\r\n")) == std::string::npos)
		return (-1);
	if ((pos = header.find("Host: ")) == std::string::npos)
		return (-1);
	if ((pos = header.find("Accept: ")) == std::string::npos)
		return (-1);
	return (0);
}

int			HttpServer::ft_choose_wich_location( std::string header )
{
	this->_header_requete[0].path = ft_check_path_header(header);
	if (this->_servers[this->_num_serv].nbr_location == 0)
	{
		this->_header_requete[0].location = false;
		return (0);
	}
	if (this->_header_requete[0].path == "")
		return (-1);

	size_t pos = this->_header_requete[0].path.find("/", 1);
	std::string tmp;
	if (pos == std::string::npos)
	{
		tmp = this->_header_requete[0].path;
		tmp.append("/");
	}
	else
	{
		tmp = this->_header_requete[0].path;
		tmp.erase(pos + 1, this->_header_requete[0].path.size() - pos + 1);
	}		
	
	std::vector<std::string> all_location; // container qui va avoir le nom de tous les locations
	for (std::vector<t_location>::iterator it = this->_servers[this->_num_serv].location.begin(); it != this->_servers[this->_num_serv].location.end(); it++)
		all_location.push_back(it->name_location);
		
	std::sort(all_location.begin(), all_location.end(), std::greater<std::string>()); // on trie les noms des locations
	
	for (std::vector<std::string>::iterator it = all_location.begin(); it != all_location.end(); ++it)
	{
		size_t i = 0;
		while (i < this->_servers[this->_num_serv].nbr_location)
		{
			if (*it == this->_servers[this->_num_serv].location[i].name_location)
			{
				if (this->_servers[this->_num_serv].location[i].name_location[this->_servers[this->_num_serv].location[i].name_location.size() - 1] != '/')
				{
					if (tmp.compare(0, tmp.size() -1, this->_servers[this->_num_serv].location[i].name_location) == 0)
					{
						this->_header_requete[0].location = true;
						return (i);
					}
				}
				else
				{
					if (tmp.compare(0, tmp.size(), this->_servers[this->_num_serv].location[i].name_location) == 0)
					{
						this->_header_requete[0].location = true;
						return (i);
					}
				}
				if (*it == "/")
				{
					this->_header_requete[0].location = true;
					return (i);
				}
			}
			i++;
		}
	}
	return (-1);
}

int			HttpServer::ft_choose_wich_server( std::string header, int num )
{
	size_t 		pos = 0;
	pos = header.find("Host: ");

	size_t		end = header.find("\r\n", pos);
	std::string tmp(header, pos + 6, end - (pos + 6));
	size_t		i = 0;
	if (this->_data->ft_get_nbr_servers() == 1)
		return (0);
	while ( i < this->_data->ft_get_nbr_servers())
	{
		if (this->_servers[i].host_server == this->_servers[num].host_server)
			if (this->_servers[i].port_server == this->_servers[num].port_server)
				if (tmp == this->_servers[i].name_server)
					return (i);
		i++;
	}
	return (num);
}

size_t HttpServer::ft_post_2( t_header_request data, std::string body)
{
	std::cout << "bingo " << std::endl;
	std::cout << "method = " << data.method << std::endl;
	std::cout << "path = " << data.path << std::endl;

	std::cout << "dans le body il y a un /r/n/r/n ?: " << body.find("\r\n\r\n") << std::endl;
		
	
	this->_header_requete[0] = data;
	if (this->ft_check_cgi_or_php(this->_header_requete[0].path) == 1)  // verifier cette fonction
	{
		this->_num_serv = data.num_server;
		if (this->_servers[this->_num_serv].cgi_path_server.empty() == true)
		{
			return (ft_do_error(500));
		}
		if (body.size() > this->_servers[this->_num_serv].buffer_size_server)
		{
			return (ft_do_error(413));
		}
		
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
		this->_header_requete[0].body_post = body;
		this->ft_exec_cgi_test(); //request_http, len_msg);
		std::cout << "fin de cgi" << std::endl;
		this->_header_requete[0].expect = false;
		this->_header_requete[0].error = false;
		this->_header_requete[0].connection_close = true;
		this->_header_requete[0].cgi = true;
		return (0);
	}
	std::cout << RED << "Erreur ft_post_2" << CLEAR << std::endl;
	this->_header_requete[0].error = true;
	this->_header_requete[0].num_error = 400;
	this->ft_setup_error_header();
	return (0);
}


void		HttpServer::ft_init_general_structure( void )
{
	this->_header_requete[0].port_client = 0;
	this->_header_requete[0].method = "";
	this->_header_requete[0].path = "";
	this->_header_requete[0].protocol = "";
	this->_header_requete[0].host = "";
	this->_header_requete[0].accept = "";
	this->_header_requete[0].path_http = "";
	this->_header_requete[0].query_string = "";
	this->_header_requete[0].cgi_return = "";
	this->_header_requete[0].referer = "";
	this->_header_requete[0].request_uri = "";
	this->_header_requete[0].content_type = "";
	this->_header_requete[0].content_length = "";
	this->_header_requete[0].body_post = "";
	this->_header_requete[0].script_file_name = "";
	this->_header_requete[0].path_file = "";
	this->_header_requete[0].return_used = false;
	this->_header_requete[0].upload = false;
	this->_header_requete[0].cgi = false;
	this->_header_requete[0].error = false;
	this->_header_requete[0].num_error = 0;
	this->_header_requete[0].body_error = "";
	this->_header_requete[0].num_server = 0;
	this->_header_requete[0].expect = false;
	this->_header_requete[0].connection_close = false;
}

HttpServer::t_header_request	HttpServer::ft_parser_requete( int port_client, int len_msg, std::string request, t_header_request data)
{
	this->_num_serv = port_client;
	this->_num_loc = 0;
	size_t			pos = request.find("\r\n\r\n");

	this->_header_requete.push_back(t_header_request());
	if (data.expect == true)		// a refaire
	{
		std::cout << "exepct 100 exit" << std::endl;
		// exit(1);
		if (data.method == "POST")
		{
			this->ft_post_2(data, request);
			return (this->_header_requete[0]);
		}
		else
		{
			std::cout << "here exit" << std::endl;
			exit(1);
		}
	}
	
	
	if (pos == std::string::npos)
	{
		this->_header_requete[0].path = "/";
		this->ft_do_error(400);
		return (this->_header_requete[0]);
	}

	std::string 	header(request, 0, pos);
	

	this->ft_init_general_structure();
	this->_num_serv = this->ft_choose_wich_server(header, port_client);
	if ((this->_num_loc = this->ft_choose_wich_location(header)) == -1) // on a deja recuperer le path
	{
		this->ft_do_error(404);
		return (this->_header_requete[0]);
	}
	std::cout << "on a un num server = " << this->_num_serv << " et  loc " <<  this->_header_requete[0].location << " duc oup num = " << this->_num_loc << std::endl;
	if (ft_check_basic(header) < 0)
	{
		this->ft_do_error(400);
		return (this->_header_requete[0]);
	}
	if (header.compare(0, 4, "GET ") == 0)
	{
		this->ft_get(request, len_msg);
		return (this->_header_requete[0]);
	}
	else if (header.compare(0, 5, "POST ") == 0)
	{
		this->ft_post(request, data);
		return (this->_header_requete[0]);
	}
	else if (header.compare(0, 7, "DELETE ") == 0)
		this->ft_delete(request, len_msg);

	std::cout << "ERRUER NI GET NI POST NI DELETE = " <<  std::endl;
	this->_header_requete.push_back(t_header_request());
	this->_header_requete[0].path = this->ft_check_path_header(header);
	this->ft_do_error(405);
	return (this->_header_requete[0]);
}


size_t			HttpServer::ft_get(std::string request_http, int len_msg)
{
	std::cout << GREEN << "\nDans GET : " << CLEAR <<  std::endl;
	
	std::cout << "request = " << request_http << std::endl;
	
	
	if (len_msg > 1023)
		return (ft_do_error(431));

	size_t 			pos_header = request_http.find("\r\n\r\n");
	std::string 	size_header(request_http, 0, pos_header);
	int 			ret = 0;

	this->_header_requete[0].method = "GET";									// on a deja verifier ca avant
	this->_header_requete[0].host = this->ft_check_host_header(size_header);				
	this->_header_requete[0].query_string = this->ft_parsing_path_get_request();
	this->_header_requete[0].protocol = "HTTP/1.1";
	this->_header_requete[0].accept = this->ft_check_accept_header(size_header);
	if ((ret = ft_check_method_allowed_header("GET")) > 0)
		return (ft_do_error(405));
	std::cout << "path = " << this->_header_requete[0].path << std::endl;

	if (this->ft_check_cgi_or_php(request_http) == 1)
	{
		if (this->_servers[this->_num_serv].cgi_path_server.empty() == true)
			return (ft_do_error(500));

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
		// We get the total path for the varriable path_http
		char	cwd[256];
		if (getcwd(cwd, sizeof(cwd)) == NULL)
		{
			std::cerr << "Error getcwd" << std::endl;
			exit(1);
		}
		
		std::string tt(cwd);
		this->_header_requete[0].path_http = tt;
		this->_header_requete[0].path_http.append("/");
		this->_header_requete[0].path_http.append(this->_header_requete[0].script_file_name);
		
		this->ft_exec_cgi_test();
		this->_header_requete[0].cgi = true;
		return (0);
	}

	if ((ret = this->ft_redirection()) == 1)	// a deplacer avant cgi je pense
	{
		std::cout << "On a une redirection " << std::endl;
		return (0);
	}
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
	this->_header_requete[0].num_loc = this->_num_loc;
	this->_header_requete[0].num_server = this->_num_serv;
	return (0);
}

size_t			HttpServer::ft_post(std::string request_http, t_header_request data)
{
	std::cout << GREEN  << "Dans ft_POST: " << CLEAR << std::endl;

	(void)data;
	int 					res = 0;

	size_t 					pos_header = request_http.find("\r\n\r\n");
	std::string 			size_header(request_http, 0, pos_header);
	std::string 			size_body(request_http, size_header.size(), request_http.size());		// on prend aussi le \r\n\r\n donc +4
	std::string 			chunked_string = "";

	// std::cout << "size_header = " << size_header << std::endl;
	// sleep(2);
	// std::cout << "size_body = " << size_body << std::endl;
	// sleep(2);



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

	this->_header_requete[0].method = "POST";
	this->_header_requete[0].host = this->ft_check_host_header(size_header);
	if (this->_header_requete[0].host.empty() == true)
		return (ft_do_error(400));

	if (this->_servers[this->_num_serv].buffer_size_server == 0)
		this->_servers[this->_num_serv].buffer_size_server = 1000000;
	if (size_body.size() - 4 > this->_servers[this->_num_serv].buffer_size_server)
		return (ft_do_error(413));

	this->_header_requete[0].query_string = this->ft_parsing_path_get_request();
	this->_header_requete[0].protocol = "HTTP/1.1";

	this->_header_requete[0].accept = this->ft_check_accept_header(size_header);
	if (this->_header_requete[0].accept.empty() == true)
		return (ft_do_error(400));

	if (ft_check_method_allowed_header("POST") == 1)
		return (ft_do_error(405));
	std::cout << BLUE << "La methode POST est autorisee, on continue." << CLEAR << std::endl;

	
	this->_header_requete[0].path_http = this->ft_check_pathhttp_header(size_header);
	if (this->_header_requete[0].path_http.empty() == true)
		return (ft_do_error(400));
	// std::cout << "\nOn a le path = -" << this->_header_requete[0].path_http << "-" <<  std::endl;

	this->_header_requete[0].content_length = this->ft_check_content_length(size_header);
	if (this->_header_requete[0].content_length.empty() == true && this->_recv_complete.chunked == false)
		return (ft_do_error(411));
	std::cout << "\nOn a le content_length = -" << this->_header_requete[0].content_length << "-" <<  std::endl;
	


	this->_header_requete[0].content_type = this->ft_check_content_type(size_header);
	if (this->_header_requete[0].content_type.empty() == true)
		return (ft_do_error(400));
	

	this->_header_requete[0].body_post = this->ft_check_body_post(size_body);
	// std::cout << "la" << std::endl;
	// if (this->_header_requete[0].body_post.empty() == true)
	// 	return (ft_do_error(400));
	// exit(1);
	if (this->_header_requete[0].body_post == "nothing")
		this->_header_requete[0].body_post = size_body;

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


	// test taille content length et le body
	std::stringstream ss;
	size_t ss_size;
	ss << this->_header_requete[0].content_length;
	ss >> ss_size;

	if (size_body.size() -4 != ss_size && this->_recv_complete.chunked == false)
	{
			// test taille content length et le body
		std::stringstream ss;
		size_t ss_size;
		ss << this->_header_requete[0].content_length;
		ss >> ss_size;
		std::cout << "taille requete = " << request_http.size() << std::endl;
		std::cout << "taille header = " << size_header.size() << std::endl;
		std::cout << "taille body = " << size_body.size() << std::endl;
		std::cout << "content lenght = " << ss_size << std::endl;


		std::cout << "content length est different de boy size" << std::endl;
		std::cout << "On cherche exepect 100" << std::endl;
		size_t pos_expect = size_header.find("Expect: ");
		if (pos_expect == std::string::npos)
		{
			std::cout << "corps body different content length = erruer " << std::endl;
			return (ft_do_error(403));
		}
		if (ss_size > this->_servers[this->_num_serv].buffer_size_server)
			return (ft_do_error(413));
		
		this->_header_requete[0].expect = true;
		return (ft_do_error(100));
	}
	// exit(1);


	if (this->ft_check_cgi_or_php(request_http) == 1)  // verifier cette fonction
	{
		if (this->_servers[this->_num_serv].cgi_path_server.empty() == true)
		{
			return (ft_do_error(500));
		}
		
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
		std::cout << "fin de cgi" << std::endl;
		this->_header_requete[0].cgi = true;
		return (0);
	}
	// std::cout << GREEN << "On a bien recu une demande " << CLEAR << std::endl;
	this->_header_requete[0].error = true;
	this->_header_requete[0].num_error = 400;
	this->ft_setup_error_header();
	return (0);
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

		if (ft_check_method_allowed_header("DELETE") == 1)				// On verifie que la methode est autorisee
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
			// sleep(2);
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


