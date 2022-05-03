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

std::string		HttpServer::ft_create_autoindex( void )
{
	std::string string;
	std::stringstream ss;


	

	// <p>by Pierre.</p></body></html>";
	DIR *dir = NULL;
	struct dirent  *file = NULL;

	this->_header_requete[0].path.erase(0, 13);
	std::string actual_folder = this->_header_requete[0].path;
	actual_folder.erase(0, this->_servers[this->_num_serv].root_server.size());


	std::cout << " file path = " << this->_header_requete[0].path << std::endl;
	std::cout << " actual folder = " << actual_folder << std::endl;
	string = "<!DOCTYPE html><html><head><title>AutoIndex</title><style type=text/css>body {color: lightred;font-weight: 900;font-size: 20px;font-family: Arial, Helvetica, sans-serif; }</style><link rel=\"icon\" type=\"image/x-con\" href=\"/flavicon.ico\"/><link rel=\"shortcut icon\" type=\"image/x-con\" href=\"/flavicon.ico\" /></head><body><h1>Auto-Index</h1>/";

	if ((dir = opendir(this->_header_requete[0].path.c_str())) != NULL)
	{
		while ((file = readdir(dir)) != NULL)
		{
			// std::cout << "dossier = " << file->d_name << std::endl;
			std::string file_name = file->d_name;

			if (is_directory(this->_header_requete[0].path + "/" + file_name ))
			{
				std::cout << "dossier = " << file_name << std::endl;
				file_name += '/';
			}
			// if (is_directory(actual_folder) && actual_folder[actual_folder.size() - 1] != '/')

			// else
			// 	std::cout << "fichier = " << file->d_name << std::endl;
			string.append("<p><a href=\"");
			string.append(file_name + "\" class=\"active\">" + file_name + "</a></p>\n");
		}
		closedir(dir);
	}
	else
	{
		closedir(dir);
		exit(1);
	}
	string.append("<p>by Pierre.</p></body></html>");
	// std::cout << "\nTOTAL = " << string << std::endl;
	return (string);
}

std::string HttpServer::ft_get_return_location( void ) const
{
	if (this->_servers[this->_num_serv].nbr_location == 0)
	{
		return ("Location: " + this->_servers[this->_num_serv].return_server + "\r\n");
	}
	else
	{
		return ("Location: " + this->_servers[this->_num_serv].location[this->_num_loc].return_location + "\r\n");
	}
}


/*
**
*/
std::string	HttpServer::ft_get_file( std::string path ) const
{
	FILE *input_file = NULL;
	struct stat sb;
	std::string	file;

	// path.erase(0, 1);
	// std::cout << ""
	input_file = fopen(path.c_str(), "r");
	if (input_file == NULL)
	{
		std::cout << "Error, pour ouvrir la page demande avec fopen." << std::endl;
		std::cerr << strerror(errno) << std::endl;
		exit(1);
		return (NULL);
	}
	stat(path.c_str(), &sb);
	file.resize(sb.st_size + 100);
	fread(const_cast<char*>(file.data()), sb.st_size, 1, input_file);
	fclose(input_file);
	// std::cout << "file = " << file << std::endl;
	return (file);
}

/*
**	std::string	ft_create_error( void )
**		This function is used if there is an error and if there is no error page setup.
**
**	It will creates a body and a header into a std::string and return it.
*/
std::string		HttpServer::ft_create_error( void )
{
	std::cout << "\n Dans ft_create_error " << std::endl;

	std::string content_length;
	std::string error_string;
	std::stringstream ss;
	std::string tmp;
	size_t pos;

	error_string = "<!DOCTYPE html><html><head><title></title><style type=text/css>body {color: red;font-weight: 900;font-size: 20px;font-family: Arial, Helvetica, sans-serif; }</style><link rel=\"icon\" type=\"image/x-con\" href=\"/flavicon.ico\"/><link rel=\"shortcut icon\" type=\"image/x-con\" href=\"/flavicon.ico\" /></head><body><h1></h1><p>by Pierre.</p></body></html>";
	ss << this->_header_requete[0].num_error;
	ss >> tmp;
	pos = error_string.find("</title>");
	error_string.insert(pos, tmp);
	ss.str("");
	ss.clear();
	pos = error_string.find("by");
	error_string.insert(pos, ft_get_status(false));
	error_string.insert(0, this->ft_get_end_header());
	ss << error_string.size();
	ss >> content_length;
	content_length.insert(0, "Content-Length: ");
	content_length.append("\r\n");
	error_string.insert(0, content_length);
	error_string.insert(0, this->ft_get_server_name());
	error_string.insert(0, this->ft_get_date());
	error_string.insert(0, this->ft_get_charset());
	error_string.insert(0, this->ft_get_content_type());
	if(this->_header_requete[0].num_error == 405)
		tmp.insert(0, this->ft_get_allow());
	error_string.insert(0, this->ft_get_status(true));

	// std::cout << "dans create error" << std::endl;
	// std::cout << "retour ft_get_allox() = " << this->ft_get_allow() << std::endl;

	// exit(1);
	return (error_string);
}

std::string		HttpServer::ft_get_allow( void ) const
{
	std::cout << "dans ft_get allow " << std::endl;
	std::string tmp = "Allow: ";
	
	// std::cout << "le body = " <<  this->_header_requete[0].body_error << std::endl;
	if (this->_servers[this->_num_serv].nbr_location > 0)
	{
		//std::cout << "il y a des locations : " << this->_servers[this->_num_serv].nbr_location << std::endl;
		size_t i = 0;
		while ( i < this->_servers[this->_num_serv].nbr_location)
		{
			size_t found = this->_header_requete[0].body_error.find("/");
			if (found == std::string::npos)
			{
				std::cout << "ERREUR euh pas normal ne trouve pas / dans la requete a traiter "<< std::endl;	// SORTIR ERREUR SERVER /
				exit(EXIT_FAILURE);
			}
			else
			{
				std::string tmp_name = this->_servers[this->_num_serv].location[i].name_location;
				if (tmp_name[tmp_name.size() - 1] != '/')
					tmp_name.append("/");
				if (this->_header_requete[0].body_error.find(tmp_name) != std::string::npos)
				{
					std::vector<std::string>::const_iterator	it_b = this->_servers[this->_num_serv].location[i].methods_location.begin();
					for (; it_b != this->_servers[this->_num_serv].location[i].methods_location.end(); it_b++)
					{
						tmp.append(*it_b);
						tmp.append(", ");
					}
					tmp.resize(tmp.size() - 2);
					tmp.append("\r\n");
					return (tmp);
				}
			}
			i++;
		}
	}
	std::cout << "on va chercher dans le root" << std::endl;
	std::vector<std::string>::const_iterator  it_b = this->_servers[this->_num_serv].methods_server.begin();
	for (; it_b != this->_servers[this->_num_serv].methods_server.end(); it_b++)
	{
		tmp.append(*it_b);
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


std::string  HttpServer::ft_get_code_redirection( void ) const
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
	if (this->_servers[this->_num_serv].nbr_location == 0)
	{
		std::cout << GREEN << "Dans t_get_code_redirection" << CLEAR << std::endl;
		std::map<std::string, std::string>::iterator it = num.begin();
		for ( ; it != num.end(); ++it)
		{
			if (it->first == this->_servers[this->_num_serv].code_return_server)
			{
				std::cout << "BIngo on a le bon code" << std::endl;
				ret = "\r\n";
				ret.insert(0, it->second);
				ret.insert(0, it->first + " ");
				ret.insert(0, "HTTP/1.1 ");
				std::cout << "RET = " << ret << std::endl;
				// exit(1);
				return (ret);
			}
		}
	}
	else
	{
		if (this->_servers[this->_num_serv].location[this->_num_loc].return_location.empty() == true)
		{
			std::cout << "normalement impossible d'etre la 2 " << std::endl;
			exit(1);
		}
		std::map<std::string, std::string>::iterator it = num.begin();
		for (; it != num.end(); ++it)
		{
			if (it->first == this->_servers[this->_num_serv].location[this->_num_loc].code_return_location)
			{
				std::cout << "BIngo on a le bon code" << std::endl;
				ret = "\r\n";
				ret.insert(0, it->second);
				ret.insert(0, it->first + " ");
				ret.insert(0, "HTTP/1.1 ");
				std::cout << "RET = " << ret << std::endl;
				// exit(1);
				return (ret);
			}
		}
	}
	std::cout << "normalement impossible d'etre la ";
	exit(1);
}

std::string		HttpServer::ft_get_status( bool x ) const 
{
	// a changer lol

	std::map<size_t, std::string> list_error;
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

	if (this->_header_requete[0].error == true)
	{
		std::string status_str;
		status_str = "\r\n";

		std::map<size_t, std::string>::iterator it_b = list_error.begin();
		for (; it_b != list_error.end(); it_b++)
		{
			if (it_b->first == this->_header_requete[0].num_error)
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
				// status_str.append("\r\n");
				return (status_str);
				break;
			}
		}
	}
	else if (this->_header_requete[0].return_used == true)
	{
		return (this->ft_get_code_redirection());
	}
	else
	{
		if (this->_header_requete[0].num_error == 201)
			return ("HTTP/1.1 201 Created\r\n");
		if (this->_header_requete[0].num_error == 204)
			return ("HTTP/1.1 204 No Content\r\n");
		if (this->_header_requete[0].num_error == 500)
			return ("HTTP/1.1 500 Internal Server Error\r\n");
	}
	// if (this->_servers[this->_num_serv].return_server.empty() == false)
	// 	return ("HTTP/1.1 301 Moved Permanently\r\n");
	return ("HTTP/1.1 200 OK\r\n");
}


std::string		HttpServer::ft_get_server_name( void ) const
{
	// a changer en fonctin du server qu'on utilise KEK...
	std::string		tmp;

	// std::cout << " je dois prendre ca = " << this->_servers[this->_num_serv].name_server << std::endl;
	tmp.insert(0, "\r\n");
	tmp.insert(0, this->_servers[this->_num_serv].name_server);
	tmp.insert(0, "Server: ");
	return (tmp);
}


std::string		HttpServer::ft_get_charset( void ) const			// peut etre a changer
{
	return ("charset=UTF-8\r\n");
}

std::string		HttpServer::ft_get_content_type( void ) const		// peut etre a changer 
{
	return ("Content-Type: text/html; ");
}

std::string		HttpServer::ft_get_content_length( struct stat buff, size_t len, size_t len_header ) const		// peut etre a changer
{
	std::string tmp_size;
	std::stringstream ss_tmp;

	(void)len_header;
	if (len == 0)
	{
		sleep(1);
		ss_tmp << buff.st_size + 100; 
		ss_tmp >> tmp_size;

		tmp_size.insert(0, "Content-Length: ");
		return (tmp_size);
	}
	else
	{
		ss_tmp << len - 38;		// - 38 = Content-type: text/html; charset=UTF-8
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