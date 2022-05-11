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
**	void	
**
**	<!> This function is used if there is an error in the header or any error.
**	IT IS NOT FINISHED.
*/
void			HttpServer::ft_setup_error_header_response( t_header_request *requete) 
{
	std::cout << GREEN << "Dans ft_setup_error_header_response" << CLEAR << std::endl;
	std::string tmp = requete->body_error;
	if (this->_servers[this->_num_serv].nbr_location > 0)
	{
		requete->body_error.append(this->_servers[this->_num_serv].location[this->_num_loc].root_location);
		requete->body_error.insert(0, this->_servers[this->_num_serv].root_server);
		
		std::map<int, std::string>::iterator it_loc = this->_servers[this->_num_serv].location[this->_num_loc].error_location.begin();
		for (; it_loc != this->_servers[this->_num_serv].location[this->_num_loc].error_location.end(); ++it_loc)
		{
			if (it_loc->first >= 0 && requete->num_error == (size_t)it_loc->first)
			{
				std::cout << "ici = requete->body_error = " << requete->body_error << std::endl;
				requete->body_error.append(it_loc->second);
				std::cout << "ici = requete->body_error = " << requete->body_error << std::endl;
				struct stat buff;
				stat(requete->body_error.c_str(), &buff);
				if (S_ISDIR(buff.st_mode) && requete->body_error[requete->body_error.size() -1 ] != '/')
					requete->body_error.append("/");
				std::cout << "ici = requete->body_error = " << requete->body_error << std::endl;
				std::cout << "\nplusieurs bloc location sbody error contient = " << requete->body_error << std::endl;
				return ;
			}
		}
		requete->body_error = tmp;

	}
	// else
	// {
		requete->body_error.append(this->_servers[this->_num_serv].root_server);
		
		std::map<int, std::string>::iterator it = this->_servers[this->_num_serv].error_server.begin();
		for (; it != this->_servers[this->_num_serv].error_server.end(); it++)
		{
			if (it->first >= 0 && requete->num_error == (size_t)it->first)
			{
				requete->body_error.append(it->second);
				struct stat buff;
				stat(requete->body_error.c_str(), &buff);
				if (S_ISDIR(buff.st_mode) && requete->body_error[requete->body_error.size() -1 ] != '/')
					requete->body_error.append("/");
				std::cout << "\n 0 bloc location body error contient = " << requete->body_error << std::endl;
				return ;
			}
		}
	// }
	// on met le body error a 0
	requete->body_error.clear();
	std::cout << " body error = " << requete->body_error << std::endl;
	return ;
}

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
// We setup an error before goint to get or other
void				HttpServer::ft_setup_error_header_2( void)
{
	std::string tmp = this->_header_requete[0].body_error;
	if (this->_servers[this->_num_serv].nbr_location > 0)
	{
		std::vector<std::string> all_location; // container qui va avoir le nom de tous les locations
		for (std::vector<t_location>::iterator it = this->_servers[this->_num_serv].location.begin(); it != this->_servers[this->_num_serv].location.end(); it++)
			all_location.push_back(it->name_location);
		
		std::sort(all_location.begin(), all_location.end(), std::greater<std::string>()); // on trie les noms des locations
		
		size_t i = 0;
		for (std::vector<std::string>::iterator it = all_location.begin(); it != all_location.end(); ++it)
		{
			if (*it == "/")
			{
				this->_num_loc = i;
				this->put_error_in_body();
				return ;
			}
			else
			{
				std::cout << "*it = " << *it << std::endl;
				std::cout << "comp = " << this->_header_requete[0].path.compare(0, it->size(), *it) << std::endl;
				std::cout << " root = " << this->_header_requete[0].path << std::endl;
				if (this->_header_requete[0].path.compare(0, it->size(), *it) == 0)
				{
					if (this->_header_requete[0].path.size() == it->size() || this->_header_requete[0].path[it->size()] == '/')
					{
						this->_num_loc = i;
						this->put_error_in_body();
						return ;
					}

				}
			}
			i++;
		}
		
	}
	this->_header_requete[0].body_error.append(this->_servers[this->_num_serv].root_server);
	
	std::map<int, std::string>::iterator it = this->_servers[this->_num_serv].error_server.begin();
	for (; it != this->_servers[this->_num_serv].error_server.end(); it++)
	{
		if (it->first >= 0 && this->_header_requete[0].num_error == (size_t)it->first)
		{
			this->_header_requete[0].body_error.append(it->second);
			struct stat buff;
			stat(this->_header_requete[0].body_error.c_str(), &buff);
			if (S_ISDIR(buff.st_mode) && this->_header_requete[0].body_error[this->_header_requete[0].body_error.size() -1 ] != '/')
				this->_header_requete[0].body_error.append("/");
			return ;
		}
	}
	this->_header_requete[0].body_error.clear();
	return ;
}

/*
**	void		pour gerer les requete
*/
void			HttpServer::ft_setup_error_header( void) 
{
	std::cout << GREEN << "Dans ft_setup_error_header" << CLEAR << std::endl;
	std::string tmp = this->_header_requete[0].body_error;
	std::cout << "ici ? " << std::endl;

	if (this->_num_loc == 666)
	{
		std::cout << "_num_loc = 666 " << std::endl;
		this->ft_setup_error_header_2();
		return ;
		exit(1);
	}

	if (this->_servers[this->_num_serv].nbr_location > 0)
	{
		std::cout << "on a des locations " << std::endl;
		this->_header_requete[0].body_error.append(this->_servers[this->_num_serv].location[this->_num_loc].root_location);
		this->_header_requete[0].body_error.insert(0, this->_servers[this->_num_serv].root_server);
		
		std::map<int, std::string>::iterator it_loc = this->_servers[this->_num_serv].location[this->_num_loc].error_location.begin();
		for (; it_loc != this->_servers[this->_num_serv].location[this->_num_loc].error_location.end(); ++it_loc)
		{
			if (it_loc->first >= 0 && this->_header_requete[0].num_error == (size_t)it_loc->first)
			{
				std::cout << "ici = this->_header_requete[0].body_error = " << this->_header_requete[0].body_error << std::endl;
				this->_header_requete[0].body_error.append(it_loc->second);
				std::cout << "ici = this->_header_requete[0].body_error = " << this->_header_requete[0].body_error << std::endl;
				struct stat buff;
				stat(this->_header_requete[0].body_error.c_str(), &buff);
				if (S_ISDIR(buff.st_mode) && this->_header_requete[0].body_error[this->_header_requete[0].body_error.size() -1 ] != '/')
					this->_header_requete[0].body_error.append("/");
				std::cout << "ici = this->_header_requete[0].body_error = " << this->_header_requete[0].body_error << std::endl;
				std::cout << "\nplusieurs bloc location sbody error contient = " << this->_header_requete[0].body_error << std::endl;
				return ;
			}
		}
		this->_header_requete[0].body_error = tmp;

	}
	// else
	// {
	this->_header_requete[0].body_error.append(this->_servers[this->_num_serv].root_server);
	
	std::map<int, std::string>::iterator it = this->_servers[this->_num_serv].error_server.begin();
	for (; it != this->_servers[this->_num_serv].error_server.end(); it++)
	{
		if (it->first >= 0 && this->_header_requete[0].num_error == (size_t)it->first)
		{
			this->_header_requete[0].body_error.append(it->second);
			struct stat buff;
			stat(this->_header_requete[0].body_error.c_str(), &buff);
			if (S_ISDIR(buff.st_mode) && this->_header_requete[0].body_error[this->_header_requete[0].body_error.size() -1 ] != '/')
				this->_header_requete[0].body_error.append("/");
			std::cout << "\n 0 bloc location body error contient = " << this->_header_requete[0].body_error << std::endl;
			return ;
		}
	}
	this->_header_requete[0].body_error.clear();
	std::cout << " body error = " << this->_header_requete[0].body_error << std::endl;
	return ;
}



std::string		HttpServer::ft_find_error_html( t_header_request *requete )
{
	std::cout << GREEN << "\n On est dans ft_find_error_html " <<  CLEAR << std::endl;
	std::stringstream 	ss;
	std::string			path_error;
	struct stat 		buff;
	DIR 				*dir = NULL;
	struct dirent  		*file = NULL;

	std::cout << "body error = " << requete->body_error << std::endl;
	
	if (stat(requete->body_error.c_str(), &buff) < 0)
	{
		std::cout << "pas normal " << std::endl;
		exit(1);
		return (ft_create_error(requete));
	}

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
					std::cout << "fichier = " << file->d_name << std::endl;
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
					path_error.insert(0, this->ft_get_content_type(requete, 0)); // a changer
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
		path_error =  ft_get_file(requete->body_error);
		path_error.insert(0, this->ft_get_end_header());
		path_error.insert(0, this->ft_get_content_length(buff, 0, 0));
		path_error.insert(0, this->ft_get_server_name());
		path_error.insert(0, this->ft_get_date());
		path_error.insert(0, this->ft_get_charset());
		path_error.insert(0, this->ft_get_content_type(requete, 0)); // a changer
		if(requete->num_error == 405)
			path_error.insert(0, this->ft_get_allow());
		path_error.insert(0, this->ft_get_status(requete, true));
		return (path_error);
	}
// std::cout << "mince " << std::endl;
// 	exit(1);
	return (ft_create_error(requete)); // test
}

/*
**	std::string	ft_create_error( void )
**		This function is used if there is an error and if there is no error page setup.
**
**	It will creates a body and a header into a std::string and return it.
*/


std::string		HttpServer::ft_create_error( t_header_request *requete )
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
	;
	
	pos = error_string.find("by Pierre");
	error_string.erase(pos - 2, 2);
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
	error_string.insert(0, this->ft_get_content_type(requete, 0)); // a changer
	if(requete->num_error == 405)
		error_string.insert(0, this->ft_get_allow());
	error_string.insert(0, this->ft_get_status(requete, true));
	error_string.append("\0");

	
	// exit(1);
	if (requete->num_error == 100)
	{
		error_string.clear();
		// error_string.insert(0, this->ft_get_end_header());
		error_string.insert(0, this->ft_get_server_name());
		error_string.insert(0, this->ft_get_date());
		error_string.insert(0, this->ft_get_status(requete, true));
		// error_string.append("\0");

	}
	std::cout << "error_string = " << error_string << std::endl;
	// exit(1);
	return (error_string);
}

// HTTP/1.1 400 Bad Request
// Content-Type: text/html; charset=UTF-8
// Date: Thu, 05 May 2022 14:49:58 GMT+02
// Server: Webserv1
// Content-Length: 368


// <!DOCTYPE html><html><head><title>400</title><style type=text/css>body {color: red;font-weight: 900;font-size: 20px;font-family: Arial, Helvetica, sans-serif; }</style><link rel="icon" type="image/x-con" href="/flavicon.ico"/><link rel="shortcut icon" type="image/x-con" href="/flavicon.ico" /></head><body><h1></h1><p>400 Bad Request
// by Pierre.</p></body></html>