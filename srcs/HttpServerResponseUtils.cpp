/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServerResponse.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psaumet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/01 11:07:34 by psaumet           #+#    #+#             */
/*   Updated: 2021/06/01 11:07:42 by psaumet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Headers/HttpServer.hpp"

bool is_directory( std::string const &name_file )
{
	struct stat		buff;
	int				ret;

	ret = stat(name_file.c_str(), &buff);
	if (ret != 0)
		return (false);
	return (S_ISDIR(buff.st_mode) == 1);

}

std::string		HttpServer::ft_create_autoindex( t_header_request *requete )
{
	std::string 		string;
	std::stringstream 	ss;
	DIR 				*dir = NULL;
	struct dirent  		*file = NULL;
	std::string 		actual_folder = requete->path;
	actual_folder.erase(0, this->_servers[requete->num_server].root_server.size());

	string = "<!DOCTYPE html><html><head><title>AutoIndex</title><style type=text/css>body {color: lightred;font-weight: 900;font-size: 20px;font-family: Arial, Helvetica, sans-serif; }</style><link rel=\"icon\" type=\"image/x-con\" href=\"/flavicon.ico\"/><link rel=\"shortcut icon\" type=\"image/x-con\" href=\"/flavicon.ico\" /></head><body><h1>Auto-Index</h1>/";
	if ((dir = opendir(requete->path.c_str())) != NULL)
	{
		while ((file = readdir(dir)) != NULL)
		{
			std::string 	file_name = file->d_name;
			if (is_directory(requete->path + "/" + file_name ))
				file_name += '/';
			string.append("<p><a href=\"");
			string.append(file_name + "\" class=\"active\">" + file_name + "</a></p>\n");
		}
		closedir(dir);
	}
	else
	{
		closedir(dir);
		std::cerr << "Error opendir" << std::endl;
		exit(1);
	}
	string.append("<p>by Pierre.</p></body></html>");
	return (string);
}

std::string HttpServer::ft_get_return_location( void ) const
{
	if (this->_servers[this->_num_serv].nbr_location == 0)
		return ("Location: " + this->_servers[this->_num_serv].return_server + "\r\n");
	else
		return ("Location: " + this->_servers[this->_num_serv].location[this->_num_loc].return_location + "\r\n");
}


std::string	HttpServer::ft_get_file( std::string path ) const
{
	FILE *input_file = NULL;
	struct stat sb;
	std::string	file;

	input_file = fopen(path.c_str(), "r");
	if (input_file == NULL)
	{
		return ("");
	}
	stat(path.c_str(), &sb);
	file.resize(sb.st_size + 100);
	fread(const_cast<char*>(file.data()), sb.st_size, 1, input_file);
	fclose(input_file);
	return (file);
}



std::string		HttpServer::ft_get_allow( void ) const
{
	std::string tmp = "Allow: ";
	
	if (this->_servers[this->_num_serv].nbr_location > 0)
	{
		std::vector<std::string>::const_iterator it_loc = this->_servers[this->_num_serv].location[this->_num_loc].methods_location.begin();
		for (; it_loc != this->_servers[this->_num_serv].location[this->_num_loc].methods_location.end(); it_loc++)
		{
			tmp.append(*it_loc);
			tmp.append(", ");
		}
		tmp.resize(tmp.size() - 2);
		tmp.append("\r\n");
		return (tmp);
	}
	std::vector<std::string>::const_iterator  it_serv = this->_servers[this->_num_serv].methods_server.begin();
	for (; it_serv != this->_servers[this->_num_serv].methods_server.end(); it_serv++)
	{
		tmp.append(*it_serv);
		tmp.append(", ");
	}
	tmp.resize(tmp.size() - 2);
	tmp.append("\r\n");

	return (tmp);
	
}

std::string		HttpServer::ft_get_end_header( void ) const
{
	return ("\r\n\r\n");
}


std::string  HttpServer::ft_get_code_redirection( t_header_request *requete ) const
{
	std::string							ret;
	std::map<std::string, std::string>  num;
	num.insert(std::pair<std::string, std::string>("300", "Multiple Choices"));
	num.insert(std::pair<std::string, std::string>("301", "Moved Permanently"));
	num.insert(std::pair<std::string, std::string>("302", "Found"));
	num.insert(std::pair<std::string, std::string>("303", "See Other"));
	num.insert(std::pair<std::string, std::string>("304", "Not Modified"));
	num.insert(std::pair<std::string, std::string>("307", "Temporary Redirect"));
	num.insert(std::pair<std::string, std::string>("308", "Permanent Redirect"));
	if (this->_servers[requete->num_server].nbr_location == 0)
	{
		std::map<std::string, std::string>::iterator it = num.begin();
		for ( ; it != num.end(); ++it)
		{
			if (it->first == this->_servers[requete->num_server].code_return_server)
			{
				ret = "\r\n";
				ret.insert(0, it->second);
				ret.insert(0, it->first + " ");
				ret.insert(0, "HTTP/1.1 ");
				return (ret);
			}
		}
	}
	else
	{
		std::map<std::string, std::string>::iterator it = num.begin();
		for (; it != num.end(); ++it)
		{
			if (it->first == this->_servers[requete->num_server].location[requete->num_loc].code_return_location)
			{
				ret = "\r\n";
				ret.insert(0, it->second);
				ret.insert(0, it->first + " ");
				ret.insert(0, "HTTP/1.1 ");
				return (ret);
			}
		}
	}
	return ("");
}

std::string		HttpServer::ft_get_status( t_header_request *requete, bool x ) const 
{
	std::map<size_t, std::string> list_error;
	list_error.insert(std::pair<size_t, std::string>(100, "Continue"));
	list_error.insert(std::pair<size_t, std::string>(400, "Bad Request"));
	list_error.insert(std::pair<size_t, std::string>(401, "Unauthorized (RFC 7235)"));
	list_error.insert(std::pair<size_t, std::string>(402, "Payment Required"));
	list_error.insert(std::pair<size_t, std::string>(403, "Forbidden"));
	list_error.insert(std::pair<size_t, std::string>(404, "Not Found"));
	list_error.insert(std::pair<size_t, std::string>(405, "Method Not Allowed"));
	list_error.insert(std::pair<size_t, std::string>(406, "Not Acceptable"));
	list_error.insert(std::pair<size_t, std::string>(407, "Proxy Authentication Required"));
	list_error.insert(std::pair<size_t, std::string>(408, "Request Timeout"));
	list_error.insert(std::pair<size_t, std::string>(409, "Conflict"));
	list_error.insert(std::pair<size_t, std::string>(410, "Gone"));
	list_error.insert(std::pair<size_t, std::string>(411, "Length Required"));
	list_error.insert(std::pair<size_t, std::string>(412, "Precondition Failed"));
	list_error.insert(std::pair<size_t, std::string>(413, "Payload Too Large"));
	list_error.insert(std::pair<size_t, std::string>(414, "URI Too Long"));
	list_error.insert(std::pair<size_t, std::string>(415, "Unsupported Media Type"));
	list_error.insert(std::pair<size_t, std::string>(416, "Range Not Satisfiable"));
	list_error.insert(std::pair<size_t, std::string>(417, "Expectation Failed"));
	list_error.insert(std::pair<size_t, std::string>(421, "Misdirected Request"));
	list_error.insert(std::pair<size_t, std::string>(422, "Unprocessable Entity"));
	list_error.insert(std::pair<size_t, std::string>(423, "Locked"));
	list_error.insert(std::pair<size_t, std::string>(424, "Failed Dependency"));
	list_error.insert(std::pair<size_t, std::string>(425, "Too Early"));
	list_error.insert(std::pair<size_t, std::string>(426, "Upgrade Required"));
	list_error.insert(std::pair<size_t, std::string>(428, "Precondition Required"));
	list_error.insert(std::pair<size_t, std::string>(429, "Too Many Requests"));
	list_error.insert(std::pair<size_t, std::string>(431, "Request Header Fields Too Large"));
	list_error.insert(std::pair<size_t, std::string>(451, "Unavailable For Legal Reasons"));
	list_error.insert(std::pair<size_t, std::string>(500, "Internal Server Error"));
	list_error.insert(std::pair<size_t, std::string>(501, "Not Implemented"));
	list_error.insert(std::pair<size_t, std::string>(502, "Bad Gateway"));
	list_error.insert(std::pair<size_t, std::string>(503, "Service Unavailable"));
	list_error.insert(std::pair<size_t, std::string>(504, "Gateway Timeout"));
	list_error.insert(std::pair<size_t, std::string>(505, "HTTP Version Not Supported"));
	list_error.insert(std::pair<size_t, std::string>(506, "Variant Also Negotiates"));
	list_error.insert(std::pair<size_t, std::string>(507, "Insufficient Storage"));
	list_error.insert(std::pair<size_t, std::string>(508, "Loop Detected"));
	list_error.insert(std::pair<size_t, std::string>(510, "Not Extended"));
	list_error.insert(std::pair<size_t, std::string>(511, "Network Authentification Required"));

	if (requete->error == true)
	{
		std::string status_str;
		status_str = "\r\n";

		std::map<size_t, std::string>::iterator it_b = list_error.begin();
		for (; it_b != list_error.end(); it_b++)
		{
			if (it_b->first == requete->num_error)
			{
				status_str.insert(0, it_b->second);
				status_str.insert(0, " ");
				std::stringstream ss;
				std::string tmp;
				ss << it_b->first;
				ss >> tmp;
				status_str.insert(0, tmp);
				if (x == true)
					status_str.insert(0, "HTTP/1.1 ");
				return (status_str);
			}
		}
	}
	else if (requete->return_used == true)
	{
		std::string tmp = this->ft_get_code_redirection(requete);
		if (tmp.empty() == true)
			return ("HTTP/1.1 200 OK\r\n");
		else
			return (tmp);
	}
	else
	{
		if (requete->num_error == 200)
			return ("HTTP/1.1 200 OK\r\n");
		if (requete->num_error == 201)
			return ("HTTP/1.1 201 Created\r\n");
		if (requete->num_error == 204)
			return ("HTTP/1.1 204 No Content\r\n");
		if (requete->num_error == 500)
			return ("HTTP/1.1 500 Internal Server Error\r\n");
	}
	return ("HTTP/1.1 200 OK\r\n");
}


std::string		HttpServer::ft_get_server_name( void ) const
{
	std::string		tmp;

	tmp.insert(0, "\r\n");
	tmp.insert(0, this->_servers[this->_num_serv].name_server);
	tmp.insert(0, "Server: ");
	return (tmp);
}


std::string		HttpServer::ft_get_charset( void ) const
{
	return ("charset=UTF-8\r\n");
}

std::string		HttpServer::ft_get_content_type( t_header_request *requete, size_t binary ) const
{
	std::vector<std::string> image;
	image.push_back("gif");
	image.push_back("png");
	image.push_back("jpeg");
	image.push_back("jpg");
	image.push_back("bmp");
	image.push_back("webp");
	image.push_back("ico");
	std::vector<std::string> music;
	music.push_back("midi");
	music.push_back("mpeg");
	music.push_back("ogg");
	music.push_back("wav");
	music.push_back("mp3");
	std::vector<std::string> video;
	video.push_back("mp4");
	video.push_back("avi");
	video.push_back("mov");
	video.push_back("mpeg4");
	video.push_back("webm");
	std::vector<std::string> autre;
	autre.push_back("pdf");

	if (binary == 1)
	{
		for( std::vector<std::string>::iterator it = image.begin(); it != image.end(); ++it)
		{
			size_t find = requete->path.find(*it, requete->path.size() - 5);
			if (find != std::string::npos)
			{
				if (requete->path.size() == find + it->size())
					return ("Content-Type: image/" + *it + "; ");				
			}
		}
		for( std::vector<std::string>::iterator it = music.begin(); it != music.end(); ++it)
		{
			size_t find = requete->path.find(*it);
			if (find != std::string::npos)
			{
				if (requete->path.size() == find + it->size())
				{
					if (*it == "mp3")
						return ("Content-Type: audio/mpeg; ");
					return ("Content-Type: audio/" + *it + "; ");
				}
			}
		}
		for( std::vector<std::string>::iterator it = video.begin(); it != video.end(); ++it)
		{
			size_t find = requete->path.find(*it);
			if (find != std::string::npos)
			{
				if (requete->path.size() == find + it->size())
					return ("Content-Type: video/" + *it + "; ");
			}
		}
		for( std::vector<std::string>::iterator it = autre.begin(); it != autre.end(); ++it)
		{
			size_t find = requete->path.find(*it);
			if (find != std::string::npos)
			{
				if (requete->path.size() == find + it->size())
					return ("Content-Type: application/" + *it + "; ");
			}
		}
	}
	return ("Content-Type: text/html; ");
}

std::string		HttpServer::ft_get_content_length( struct stat buff, size_t len, size_t len_header ) const
{
	std::string tmp_size;
	std::stringstream ss_tmp;

	(void)len_header;
	if (len == 0)
	{
		ss_tmp << buff.st_size + 100; 
		ss_tmp >> tmp_size;
		tmp_size.insert(0, "Content-Length: ");
		return (tmp_size);
	}
	else
	{
		ss_tmp << len - 38 - 4;
		ss_tmp >> tmp_size;
		tmp_size.append("\r\n");
		tmp_size.insert(0, "Content-Length: ");
		return (tmp_size);
	}
}

std::string		HttpServer::ft_get_date( void ) const
{
	struct timeval	tv;
	time_t			t;
	struct tm		*info;
	char			buffer[64];

	gettimeofday(&tv, NULL);
	t = tv.tv_sec;
	info = localtime(&t);
	strftime(buffer, sizeof(buffer), "%a, %d %b %Y %T GMT+02", info);
	std::string		tmp(buffer);
	tmp.insert(0, "Date: ");
	tmp.append("\r\n");
	return (tmp);
}

std::string		HttpServer::ft_last_modified( void )
{
	struct timeval	tv;
	time_t			t;
	struct tm		*info;
	char			buffer[64];

	gettimeofday(&tv, NULL);
	t = tv.tv_sec;
	info = localtime(&t);
	strftime(buffer, sizeof(buffer), "%a, %d %b %Y %T GMT+02", info);
	std::string		tmp(buffer);
	tmp.insert(0, "Last-Modified: ");
	tmp.append("\r\n");
	return (tmp);
}

std::string		HttpServer::ft_expire( void )
{
	struct timeval	tv;
	time_t			t;
	struct tm		*info;
	char			buffer[64];

	gettimeofday(&tv, NULL);
	t = tv.tv_sec;
	info = localtime(&t);
	strftime(buffer, sizeof(buffer), "%a, %d %b %Y %T GMT+02", info);
	std::string		tmp(buffer);
	tmp.insert(0, "Expires: ");
	tmp.append("\r\n");
	return (tmp);
}