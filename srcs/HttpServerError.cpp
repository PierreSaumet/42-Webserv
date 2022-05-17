/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServerError.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psaumet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/01 11:07:34 by psaumet           #+#    #+#             */
/*   Updated: 2021/06/01 11:07:42 by psaumet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Headers/HttpServer.hpp"

/*
**	HttpServer::ft_setup_error_header_response( t_header_request *requete ) 
**		This function is mostly used in the 'HttpServerResponse' file, when the request was parsed
**	and we need to send an answer. But If there is an error at this step, before sending data.
**	We will use this function.
*/
void			HttpServer::ft_setup_error_header_response( t_header_request *requete ) 
{
	std::string tmp = requete->body_error;

	if (requete->location == true)
	{
		requete->body_error.append(this->_servers[requete->num_server].location[requete->num_loc].root_location);
		requete->body_error.insert(0, this->_servers[requete->num_server].root_server);

		std::map<int, std::string>::iterator it_loc = this->_servers[requete->num_server].location[requete->num_loc].error_location.begin();
		for (; it_loc != this->_servers[requete->num_server].location[requete->num_loc].error_location.end(); ++it_loc)
		{
			if (it_loc->first >= 0 && requete->num_error == (size_t)it_loc->first)
			{
				requete->body_error.append(it_loc->second);
				struct stat buff;
				stat(requete->body_error.c_str(), &buff);
				if (S_ISDIR(buff.st_mode) && requete->body_error[requete->body_error.size() -1 ] != '/')
					requete->body_error.append("/");
				return ;
			}
		}
		requete->body_error.clear();
		return ;
	}
	else
	{
		requete->body_error.append(this->_servers[requete->num_server].root_server);
		
		std::map<int, std::string>::iterator it = this->_servers[requete->num_server].error_server.begin();
		for (; it != this->_servers[requete->num_server].error_server.end(); it++)
		{
			if (it->first >= 0 && requete->num_error == (size_t)it->first)
			{
				requete->body_error.append(it->second);
				struct stat buff;
				stat(requete->body_error.c_str(), &buff);
				if (S_ISDIR(buff.st_mode) && requete->body_error[requete->body_error.size() -1 ] != '/')
					requete->body_error.append("/");
				return ;
			}
		}
	}
	requete->body_error.clear();
	return ;
}

/*
**	HttpServer::put_error_in_body( void )
**		This function is used to put the error body setup in the conf file to the body_error variable
*/
void HttpServer::put_error_in_body( void )
{
	this->_header_requete[0].body_error.append(this->_servers[this->_num_serv].location[this->_num_loc].root_location);
	this->_header_requete[0].body_error.insert(0, this->_servers[this->_num_serv].root_server);
	
	std::map<int, std::string>::iterator it_loc = this->_servers[this->_num_serv].location[this->_num_loc].error_location.begin();
	for (; it_loc != this->_servers[this->_num_serv].location[this->_num_loc].error_location.end(); ++it_loc)
	{
		if (it_loc->first >= 0 && this->_header_requete[0].num_error == (size_t)it_loc->first)
		{
			this->_header_requete[0].body_error.append(it_loc->second);
			struct stat buff;
			stat(this->_header_requete[0].body_error.c_str(), &buff);
			if (S_ISDIR(buff.st_mode) && this->_header_requete[0].body_error[this->_header_requete[0].body_error.size() -1 ] != '/')
				this->_header_requete[0].body_error.append("/");
			return ;
		}
	}
}

/*
**	SAME as HttpServer::ft_setup_error_header_response( t_header_request *requete ) 
**		but used during the process of parsing the request
*/
void			HttpServer::ft_setup_error_header( void) 
{
	std::string tmp = this->_header_requete[0].body_error;

	if (this->_header_requete[0].location == true)
	{
		this->_header_requete[0].body_error.append(this->_servers[this->_num_serv].location[this->_num_loc].root_location);
		this->_header_requete[0].body_error.insert(0, this->_servers[this->_num_serv].root_server);
		
		std::map<int, std::string>::iterator it_loc = this->_servers[this->_num_serv].location[this->_num_loc].error_location.begin();
		for (; it_loc != this->_servers[this->_num_serv].location[this->_num_loc].error_location.end(); ++it_loc)
		{
			if (it_loc->first >= 0 && this->_header_requete[0].num_error == (size_t)it_loc->first)
			{
				this->_header_requete[0].body_error.append(it_loc->second);
				
				struct stat		buff;
				stat(this->_header_requete[0].body_error.c_str(), &buff);
				if (S_ISDIR(buff.st_mode) && this->_header_requete[0].body_error[this->_header_requete[0].body_error.size() -1 ] != '/')
					this->_header_requete[0].body_error.append("/");
				return ;
			}
		}
		this->_header_requete[0].body_error = tmp;
	}
	else
	{
		this->_header_requete[0].body_error.append(this->_servers[this->_num_serv].root_server);
		std::map<int, std::string>::iterator it = this->_servers[this->_num_serv].error_server.begin();
		for (; it != this->_servers[this->_num_serv].error_server.end(); it++)
		{
			if (it->first >= 0 && this->_header_requete[0].num_error == (size_t)it->first)
			{
				this->_header_requete[0].body_error.append(it->second);
				struct stat		buff;
				stat(this->_header_requete[0].body_error.c_str(), &buff);
				if (S_ISDIR(buff.st_mode) && this->_header_requete[0].body_error[this->_header_requete[0].body_error.size() -1 ] != '/')
					this->_header_requete[0].body_error.append("/");
				return ;
			}
		}
		this->_header_requete[0].body_error.clear();
		return ;
	}
	return ;	
}

/*
**	std::string		HttpServer::ft_find_error_html( t_header_request *requete )
**		This function is used to find the error page given in the parsing and return an answer with an header and a body
*/
std::string		HttpServer::ft_find_error_html( t_header_request *requete )
{
	std::stringstream 	ss;
	std::string			path_error;
	struct stat 		buff;
	DIR 				*dir = NULL;
	struct dirent  		*file = NULL;


	if (stat(requete->body_error.c_str(), &buff) < 0)
		return (ft_create_error(requete));

	ss << requete->num_error;
	ss >> path_error;
	path_error.resize(4);
	if ((dir = opendir(requete->body_error.c_str())) != NULL)
	{
		while ((file = readdir(dir)) != NULL)
		{
			if (file->d_type == DT_REG)
			{
				if (path_error.compare(0, 3, file->d_name, 0, 3) == 0)
				{
					if (requete->body_error[requete->body_error.size() - 1] != '/')
						requete->body_error.append("/");
					requete->body_error.append(file->d_name);
					
					if (stat(requete->body_error.c_str(), &buff) < 0)
						return (ft_create_error(requete));
					path_error.clear();
					path_error =  ft_get_file(requete->body_error);
					path_error.insert(0, this->ft_get_end_header());
					path_error.insert(0, this->ft_get_content_length(buff, 0, 0));
					path_error.insert(0, this->ft_get_server_name());
					path_error.insert(0, this->ft_get_date());
					path_error.insert(0, this->ft_get_charset());
					path_error.insert(0, this->ft_get_content_type(requete, 0));
					if(requete->num_error == 405)
						path_error.insert(0, this->ft_get_allow());
					path_error.insert(0, this->ft_get_status(requete, true));
					closedir(dir);
					return (path_error);
				}
			}
		}
	}
	closedir(dir);
	if (S_ISREG(buff.st_mode))
	{
		path_error.clear(); 
		if ((path_error =  ft_get_file(requete->body_error)) == "")
			return (ft_create_error(requete));
		path_error.insert(0, this->ft_get_end_header());
		path_error.insert(0, this->ft_get_content_length(buff, 0, 0));
		path_error.insert(0, this->ft_get_server_name());
		path_error.insert(0, this->ft_get_date());
		path_error.insert(0, this->ft_get_charset());
		path_error.insert(0, this->ft_get_content_type(requete, 0));
		if(requete->num_error == 405)
			path_error.insert(0, this->ft_get_allow());
		path_error.insert(0, this->ft_get_status(requete, true));
		return (path_error);
	}
	return (ft_create_error(requete));
}

/*
**	std::string	ft_create_error( void )
**		This function is used if there is an error and if there is no error page setup.
**
**	It will creates a body and a header into a std::string and return it.
*/
std::string		HttpServer::ft_create_error( t_header_request *requete )
{
	std::string content_length;
	std::string error_string;
	std::stringstream ss;
	std::string tmp;
	size_t pos;

	error_string = "<!DOCTYPE html><html><head><title></title><style type=text/css>body {color: red;font-weight: 900;font-size: 20px;font-family: Arial, Helvetica, sans-serif; }</style><link rel=\"icon\" type=\"image/x-con\" href=\"/flavicon.ico\"/><link rel=\"shortcut icon\" type=\"image/x-con\" href=\"/flavicon.ico\" /></head><body><h1></h1><p>by Pierre.</p></body></html>";
	ss << requete->num_error;
	ss >> tmp;
	pos = error_string.find("</title>");
	error_string.insert(pos, tmp);
	ss.str("");
	ss.clear();
	pos = error_string.find("by");
	pos = error_string.find("by Pierre");
	error_string.insert(pos, tmp);
	pos = error_string.find("by Pierre");
	error_string.insert(pos, " ");
	error_string.insert(0, this->ft_get_end_header());
	ss << error_string.size() - 2;
	ss >> content_length;
	content_length.insert(0, "Content-Length: ");
	content_length.append("\r\n");
	error_string.insert(0, content_length);
	error_string.insert(0, this->ft_get_server_name());
	error_string.insert(0, this->ft_get_date());
	error_string.insert(0, this->ft_get_charset());
	error_string.insert(0, this->ft_get_content_type(requete, 0));
	if(requete->num_error == 405)
		error_string.insert(0, this->ft_get_allow());
	error_string.insert(0, this->ft_get_status(requete, true));
	error_string.append("\0");

	if (requete->num_error == 100)
	{
		error_string.clear();
		error_string.insert(0, this->ft_get_server_name());
		error_string.insert(0, this->ft_get_date());
		error_string.insert(0, this->ft_get_status(requete, true));
	}
	return (error_string);
}
