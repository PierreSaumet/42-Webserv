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
	
	std::vector<std::string> all_location;
	for (std::vector<t_location>::iterator it = this->_servers[this->_num_serv].location.begin(); it != this->_servers[this->_num_serv].location.end(); it++)
		all_location.push_back(it->name_location);
		
	std::sort(all_location.begin(), all_location.end(), std::greater<std::string>());
	
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
		if (this->_servers[i].host_server.empty() == true)
			this->_servers[i].host_server = "127.0.0.1";
		if (this->_servers[i].host_server == this->_servers[num].host_server)
			if (this->_servers[i].port_server == this->_servers[num].port_server)
				if (tmp == this->_servers[i].name_server)
					return (i);
		i++;
	}
	return (num);
}

size_t HttpServer::ft_post_continue( t_header_request data, std::string body)
{
	this->_header_requete[0] = data;
	if (this->ft_check_cgi_or_php() == 1)
	{
		this->_num_serv = data.num_server;
		if (this->_servers[this->_num_serv].cgi_path_server.empty() == true)
			return (ft_do_error(500));
		if (body.size() > this->_servers[this->_num_serv].buffer_size_server)
			return (ft_do_error(413));

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
		this->ft_exec_cgi_test();
		this->_header_requete[0].expect = false;
		this->_header_requete[0].error = false;
		this->_header_requete[0].connection_close = true;
		this->_header_requete[0].cgi = true;
		return (0);
	}
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
	if (data.expect == true)
	{
		if (data.method == "POST")
		{
			this->ft_post_continue(data, request);
			return (this->_header_requete[0]);
		}
		else
		{
			ft_do_error(500);
			return (this->_header_requete[0]);
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
	if ((this->_num_loc = this->ft_choose_wich_location(header)) == -1)
	{
		this->ft_do_error(404);
		return (this->_header_requete[0]);
	}
	this->_header_requete[0].num_loc = this->_num_loc;
	this->_header_requete[0].num_server = this->_num_serv;
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
	{
		this->ft_delete(request, len_msg);
		return (this->_header_requete[0]);
	}
	this->ft_do_error(405);
	return (this->_header_requete[0]);
}


size_t			HttpServer::ft_get(std::string request_http, int len_msg)
{
	if (len_msg > 1023)
		return (ft_do_error(431));

	size_t 			pos_header = request_http.find("\r\n\r\n");
	std::string 	size_header(request_http, 0, pos_header);
	int 			ret = 0;

	this->_header_requete[0].method = "GET";
	this->_header_requete[0].host = this->ft_check_host_header(size_header);				
	this->_header_requete[0].query_string = this->ft_parsing_path_get_request();
	this->_header_requete[0].protocol = "HTTP/1.1";
	this->_header_requete[0].accept = this->ft_check_accept_header(size_header);
	if ((ret = ft_check_method_allowed_header("GET")) > 0)
		return (ft_do_error(405));
	if (this->ft_check_cgi_or_php() == 1)
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

	if ((ret = this->ft_redirection()) == 1)
		return (0);
	if ((ret = this->ft_check_access_path()) > 0)
	{
		this->_header_requete[0].error = true;
		if (ret == 2)
			this->_header_requete[0].num_error = 403;
		if (ret == 1)
			this->_header_requete[0].num_error = 404;
		this->ft_setup_error_header();
		return (0);
	}
	this->_header_requete[0].num_loc = this->_num_loc;
	this->_header_requete[0].num_server = this->_num_serv;
	return (0);
}

size_t			HttpServer::ft_post(std::string request_http, t_header_request data)
{
	(void)data;
	int 					res = 0;

	size_t 					pos_header = request_http.find("\r\n\r\n");
	std::string 			size_header(request_http, 0, pos_header);
	std::string 			size_body(request_http, size_header.size(), request_http.size());
	std::string 			chunked_string = "";

	this->_header_requete[0].num_loc = this->_num_loc;
	this->_header_requete[0].num_server = this->_num_serv;

	if (size_header.size() > 1023)
		return (ft_do_error(431));

	if (this->_recv_complete.chunked == true)
	{
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
	this->_header_requete[0].path_http = this->ft_check_pathhttp_header(size_header);
	if (this->_header_requete[0].path_http.empty() == true)
		return (ft_do_error(400));
	this->_header_requete[0].content_length = this->ft_check_content_length(size_header);
	if (this->_header_requete[0].content_length.empty() == true && this->_recv_complete.chunked == false)
		return (ft_do_error(411));
	this->_header_requete[0].content_type = this->ft_check_content_type(size_header);
	if (this->_header_requete[0].content_type.empty() == true)
		return (ft_do_error(400));
	this->_header_requete[0].body_post = this->ft_check_body_post(size_body);
	if (this->_header_requete[0].body_post == "nothing")
		this->_header_requete[0].body_post = size_body;
	if (this->_recv_complete.chunked == true)
		this->_header_requete[0].body_post = chunked_string;
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

	std::stringstream ss;
	size_t ss_size;
	ss << this->_header_requete[0].content_length;
	ss >> ss_size;

	if (size_body.size() -4 != ss_size && this->_recv_complete.chunked == false)
	{

		std::stringstream ss;
		size_t ss_size;
		ss << this->_header_requete[0].content_length;
		ss >> ss_size;
		size_t pos_expect = size_header.find("Expect: ");
		if (pos_expect == std::string::npos)
			return (ft_do_error(403));
		if (ss_size > this->_servers[this->_num_serv].buffer_size_server)
			return (ft_do_error(413));
		
		this->_header_requete[0].expect = true;
		return (ft_do_error(100));
	}

	if (this->ft_check_cgi_or_php() == 1)
	{
		if (this->_servers[this->_num_serv].cgi_path_server.empty() == true)
			return (ft_do_error(500));

		char	cwd[256];
		if (getcwd(cwd, sizeof(cwd)) == NULL)
			throw Error(4, "Error, getcwd doesn't work", 2);
		std::string tt(cwd);
		this->_header_requete[0].path_http = tt;
		this->_header_requete[0].path_http.append("/");
		this->_header_requete[0].path_http.append(this->_header_requete[0].script_file_name);
		this->_header_requete[0].request_uri = this->_header_requete[0].path;
		this->_header_requete[0].script_file_name = this->_header_requete[0].path;
		this->ft_exec_cgi_test();
		this->_header_requete[0].cgi = true;
		this->_header_requete[0].num_loc = this->_num_loc;
		this->_header_requete[0].num_server = this->_num_serv;
		return (0);
	}
	this->_header_requete[0].num_loc = this->_num_loc;
	this->_header_requete[0].num_server = this->_num_serv;
	this->_header_requete[0].error = true;
	this->_header_requete[0].num_error = 400;
	this->ft_setup_error_header();
	return (0);
}

#include "sys/stat.h"
size_t			HttpServer::ft_delete(std::string request_http, int len_msg)
{
	(void)len_msg;
	size_t pos_header = request_http.find("\r\n\r\n");
	std::string size_header(request_http, 0, pos_header);

	this->_header_requete[0].method = "DELETE";
	this->_header_requete[0].protocol = "HTTP/1.1";
	this->_header_requete[0].host = this->ft_check_host_header(size_header);
	if (this->_header_requete[0].host.empty() == true)
		return (ft_do_error(400));
	if (ft_check_method_allowed_header("DELETE") == 1)
		return (ft_do_error(405));
	int res = 0;
	if ((res = this->ft_redirection()) == 1)
		return (0);

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

	struct stat buff;
	if (stat(this->_header_requete[0].path.c_str(), &buff) != 0)
		return (ft_do_error(404));
	else
	{
		std::stringstream ss;
		ss << buff.st_size;
		ss >> this->_header_requete[0].body_error;
		if (remove(this->_header_requete[0].path.c_str()) != 0)
			return (ft_do_error(202));
		else
		{
			this->_header_requete[0].cgi = true;
			if (stat(this->_header_requete[0].path.c_str(), &buff) != 0)
				this->_header_requete[0].num_error = 204;
			else
			{
				this->_header_requete[0].body_error = "\r\n\r\n<!DOCTYPE html><html><head><title>204</title><style type=text/css>body {color: blue;font-weight: 900;font-size: 20px;font-family: Arial, Helvetica, sans-serif; }</style><link rel=\"icon\" type=\"image/x-con\" href=\"/flavicon.ico\"/><link rel=\"shortcut icon\" type=\"image/x-con\" href=\"/flavicon.ico\" /></head><body><h1>accepted but no =)</h1><p>by Pierre.</p></body></html>";				
				this->_header_requete[0].num_error = 202;
			}
			return (0);
		}
	}
}


