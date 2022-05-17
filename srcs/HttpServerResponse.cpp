/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServerSetResponse.cpp                           :+:      :+:    :+:  */
/*                                                    +:+ +:+         +:+     */
/*   By: psaumet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/01 11:07:34 by psaumet           #+#    #+#             */
/*   Updated: 2021/06/01 11:07:42 by psaumet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Headers/HttpServer.hpp"
#include "../Headers/CGI_exec.hpp"


std::string		HttpServer::ft_setup_response_to_send( t_header_request *requete )				
{
	std::string file_contents;
	struct stat buff;
	std::string res;
	FILE *input_file = NULL;

	if (_still_to_send > 0)
	{
		unsigned long truc = _total_send - _still_to_send;
		_response_to_send.erase(0, truc);
		return (_response_to_send);
	}

	std::string header = ft_setup_header(requete);

	if (requete->error == true || requete->cgi == true || requete->return_used == true)
		return (header);
	input_file = fopen(requete->path.c_str(), "rb");
	if (stat(requete->path.c_str(), &buff) < 0)
	{
		requete->error = true;
		requete->num_error = 500;
		return (this->ft_setup_response_to_send(requete));
	}
	res.resize(buff.st_size + 100);											
	fread(const_cast<char*>(res.data()), buff.st_size, 1, input_file);
	fclose(input_file);
	file_contents = res;
	file_contents.insert(0, header.c_str());
	return (file_contents);
}

int				ft_open_binary( std::string const path )
{
	std::vector<std::string>  extension;
	extension.push_back(".jpeg");
	extension.push_back(".jpg");
	extension.push_back(".png");
	extension.push_back(".gif");
	extension.push_back(".bmp");
	extension.push_back(".webp");
	extension.push_back(".ico");  
	extension.push_back(".midi");
	extension.push_back(".mpeg");
	extension.push_back(".mp3");
	extension.push_back(".mp4");
	extension.push_back(".avi");
	extension.push_back(".mov");
	extension.push_back(".mpeg4");
	extension.push_back(".webm");

	extension.push_back(".pdf");

	for (std::vector<std::string>::iterator it = extension.begin(); it != extension.end(); ++it)
	{
		size_t find = path.find(*it);
		if (find != std::string::npos)
		{
			if (path.size() == find + it->size())
				return (1);
		}
	}
	return (0);
}

std::string		HttpServer::ft_setup_header( t_header_request *requete )
{

	std::string filename(this->_servers[this->_num_serv].index_server.c_str());
	FILE *input_file = NULL;
	std::string res;
	std::string file_contents;
	std::string the_header;
	struct stat buff;

	if (requete->error == true)
	{
		if (requete->body_error.empty() == false)
			return (ft_find_error_html(requete));
		return (ft_create_error(requete));
	}
	if (requete->cgi == true)
	{
		std::string 	tmp = requete->body_error;
		size_t 			pos = tmp.find("\r\n\r\n");

		if (requete->num_error == 204)
		{
			tmp.clear();
			tmp.insert(0, "\r\n\r\n");
			tmp.insert(0, "Accept: " + requete->accept);
			tmp.insert(0, "Content-Length: " + requete->body_error + "\r\n");
			tmp.insert(0, this->ft_get_server_name());
			tmp.insert(0, this->ft_get_date());
			tmp.insert(0, "HTTP/1.1 204 No Content\r\n");
			return (tmp);
		}
		if (pos == std::string::npos)
		{
			requete->error = true;
			requete->num_error = 500;
			ft_setup_error_header_response(requete);
			if (requete->body_error.empty() == false)
				return (ft_find_error_html(requete));
			return (ft_create_error(requete));
		}
		tmp.insert(0, this->ft_get_content_length(buff, tmp.size(), 0));
		tmp.insert(0, this->ft_get_server_name());
		tmp.insert(0, this->ft_get_date());
		if (tmp.find("upload") != std::string::npos && tmp.find("Bingo") != std::string::npos)
			tmp.insert(0, "HTTP/1.1 201 Created\r\n");
		else
			tmp.insert(0, this->ft_get_status(requete, true));
		return (tmp);
	}
	if (requete->return_used == true)
	{
		the_header.insert(0, this->ft_get_end_header());
		the_header.insert(0, "Content-Length: 0\r\n");
		the_header.insert(0, this->ft_get_server_name());
		the_header.insert(0, this->ft_get_date());
		the_header.insert(0, "Location: " + requete->path + "\r\n");
		the_header.insert(0, this->ft_get_status(requete, true));
		return (the_header);
	}
	if (requete->autoindex == true)
	{
		the_header = this->ft_create_autoindex(requete);
		the_header.insert(0, this->ft_get_end_header());
		the_header.insert(0, this->ft_get_content_length(buff, the_header.size(), 0));
		the_header.insert(0, this->ft_get_server_name());
		the_header.insert(0, this->ft_get_date());
		the_header.insert(0, this->ft_get_status(requete, true));
		requete->error = true;
		return (the_header);
	}
	if (stat(requete->path.c_str(), &buff) < 0)
	{
		requete->error = true;
		requete->num_error = 404;
		ft_setup_error_header_response(requete);
		if (requete->body_error.empty() == false)
			return (ft_find_error_html(requete));
		return (ft_create_error(requete));
	}
	size_t binary = 0;
	if (ft_open_binary(requete->path) == 0)
		input_file = fopen(requete->path.c_str(), "r");
	else
	{
		input_file = fopen(requete->path.c_str(), "rb");
		binary = 1;
	}
	if (input_file == NULL)
	{
		requete->error = true;
		requete->num_error = 503;
		ft_setup_error_header_response(requete);
		if (requete->body_error.empty() == false)
			return (ft_find_error_html(requete));
		return (ft_create_error(requete));
	}
	fclose(input_file);
	the_header.insert(0, this->ft_get_end_header());
	the_header.insert(0, this->ft_get_content_length(buff, 0, 0));
	the_header.insert(0, this->ft_get_server_name());
	the_header.insert(0, this->ft_get_date());
	the_header.insert(0, this->ft_get_charset());
	the_header.insert(0, this->ft_get_content_type(requete, binary));
	the_header.insert(0, this->ft_get_status(requete, true));
	return (the_header);

}