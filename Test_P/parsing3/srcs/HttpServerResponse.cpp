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



std::string		HttpServer::ft_setup_header( void )
{
	// cette fonction doit voir si le path de la requete est valide et existe.
	// ouvrir le fichier, verifier qu'il ne soit pas vide.
	// setup up le header avec le numero de la reponse du server.
	std::string filename(this->_servers[0].index_server.c_str());
	FILE *input_file = NULL;
	std::string res;
	std::string file_contents;
	std::string the_header;
	//							 on verifie le path
	std::cout << GREEN <<  "\nFonction ft_setup_header" << CLEAR << std::endl;

	if (this->_header_requete[0].error == true)
	{
		// une erreur a ete setup donc on doit deja avoir une reposne a display;
		std::cout << "On a une erreur : " << this->_header_requete[0].num_error << std::endl;
		if (this->_header_requete[0].body_error.empty() == false)
		{
			// fonction qui va chercher l'erreur dans les differents dossiers
			// et renvoie tout header + body
			the_header = ft_find_error_html( );
			std::cout << "return de ft_setup_header : " << the_header << std::endl;
			return (the_header);
		}
		else
		{
			std::cout << "putain le header requet ebody erro n'est est pas vide " << std::endl;
			return (NULL);
		}
		return (NULL);
		// std::string the_header;

		std::cout << RED << " On arrive jamais la ???? " << std::endl;
		the_header.insert(0, this->ft_get_end_header());

		// struct stat buff2;
		// if (stat()) 

		// the_header.insert(0, this->ft_get_content_length(buff2));
		the_header.insert(0, this->ft_get_server_name());
		the_header.insert(0, this->ft_get_charset());
		the_header.insert(0, this->ft_get_content_type());
		the_header.insert(0, this->ft_get_status(true));
		return (NULL);
	}
	// tout marche bien pour l'instant


	//	checker si ? et si get car du coup on a des donnees a recuperer.
	if (this->_header_requete[0].path.find(".php") != std::string::npos)
	{
		this->_header_requete[0].path = "./root/query_get_test.php";
	}
	else
	{
	// std::cout << "le path = -" << this->_header_requete[0].path << "-" << std::endl;
	// on insere le root directory
	// std::cout << "le root = " << this->_servers[0].root_server << std::endl;
	this->_header_requete[0].path.insert(0, this->_servers[0].root_server);
	this->_header_requete[0].path.erase(this->_header_requete[0].path.end() - 1, this->_header_requete[0].path.end()); // on supprime l'espace de la fin.
	// std::cout << "le path avec le root = -" << this->_header_requete[0].path << "-" << std::endl;
	}
	
	std::cout << "on doit avoir le fichier : " << this->_header_requete[0].path << std::endl;
	sleep(1);
	struct stat buff2;
	if (stat(this->_header_requete[0].path.c_str(), &buff2) < 0)	// le fichier existe pas on return 404
	{
		// on setup une erreur 404
		std::cout << RED << "on doit setup 404" << CLEAR << std::endl;
		this->_header_requete[0].error = true;
		this->_header_requete[0].num_error = 404;

		// on verifie si l'erreur existe
		int ret = ft_setup_error_header(this->_header_requete[0].path, this->_header_requete[0].path.size());
		sleep(3);
		std::cout << "\n\n fin du test " << std::endl;
		size_t pos;
		if (ret == 0)
		{
			std::cout << "alors erreur dans le root server " << std::endl;
			pos = this->_header_requete[0].body_error.find(this->_servers[0].folder_error);
		}
		else if (ret > 0 || ret == -1)
		{
			std::cout << "alors erreur dans le location server " << std::endl;
			// A TERMINER LA POSITION 2
			if (ret == -1)
				ret = 0;
			std::cout << "kek " << this->_servers[0].location[ret].folder_error << std::endl;
			std::cout << "bur " << this->_header_requete[0].body_error << std::endl;
			pos = this->_header_requete[0].body_error.find(this->_servers[0].location[ret].folder_error);
		}
		else
		{
			std::cout << "tu fais de la merde pierre avril " << std::endl;
			exit(1);
		}
		if (pos == std::string::npos)
		{
			// c'est que le dossier contentn les erreurs n'a pas ete trouve
			// doncfaut creer une erreur
			std::cout << "merde" << std::endl;
			sleep(2);
			return (ft_create_error());
		}
		else
		{
			// faut renvoyer l'erreur
			std::cout << "merde 2 " << std::endl;
			sleep(2);
			struct stat buff;
			if (stat(this->_header_requete[0].body_error.c_str(), &buff) < 0)
				return (ft_create_error());
			std::string tmp = ft_get_file(this->_header_requete[0].body_error);
			tmp.insert(0, this->ft_get_end_header());
			tmp.insert(0, this->ft_get_content_length(buff));
			tmp.insert(0, this->ft_get_server_name());
			tmp.insert(0, this->ft_get_date());
			tmp.insert(0, this->ft_get_charset());
			tmp.insert(0, this->ft_get_content_type());
			tmp.insert(0, this->ft_get_status(true));
			std::cout << " il a trouve " << std::endl;
			return (tmp);
		}
	}
	if (buff2.st_size == 0)
	{
		std::cout << "Error, la page demande est vide. " << std::endl;
		return (NULL);
	}

	input_file = fopen(this->_header_requete[0].path.c_str(), "r");
	if (input_file == NULL)
	{
		std::cout << "Error, pour ouvrir la page demande avec fopen." << std::endl;
		return (NULL);
	}
	fclose(input_file);



	std::cout << "OK donc tout est bon ici la page demandee existe on va mettre rendre le header\n" << std::endl;
	the_header.insert(0, this->ft_get_end_header());
	the_header.insert(0, this->ft_get_content_length(buff2));
	the_header.insert(0, this->ft_get_server_name());
	the_header.insert(0, this->ft_get_date());
	the_header.insert(0, this->ft_get_charset());
	the_header.insert(0, this->ft_get_content_type());
	the_header.insert(0, this->ft_get_status(true));
	return (the_header);

}

std::string		HttpServer::ft_find_error_html( void )
{
	std::cout << "\n On est dans ft_find_error_html " << std::endl;
	std::stringstream 	ss;
	std::string			path_error;

	// path_error = ./root/Hello/errors/431.html431.html
	// path_error = ./root/Hello/431.html

	size_t pos = this->_header_requete[0].body_error.find("html");

	if (pos == std::string::npos)
	{
			// il n'y a pas d'html
		std::cout << "il y a pas  un html " << std::endl;
		ss << this->_header_requete[0].num_error;
		ss >> path_error;
		path_error.resize(4);
		path_error.insert(4, ".html");
		path_error.insert(0, this->_header_requete[0].body_error);
	}
	else
	{
		std::cout << "il y a un html " << std::endl;
		path_error = this->_header_requete[0].body_error;
	}

	std::cout << "path_error = -" << path_error << "-" << std::endl;
	struct stat buff;
	if (stat(path_error.c_str(), &buff) < 0)
	{
		// le fichier n'a pas ete trouve on renvoie une string avec toute l'erreur.
		return (ft_create_error()); // test
	}
	else
	{
		if (buff.st_size == 0)	// fichier vide donc on ft_create_error()
			return (ft_create_error());
		else	// on renvoie le fichier de l'erreur
		{
			std::string tmp = ft_get_file(path_error);
			std::cout << "dans erreur on recupere le fichier" << std::endl;


			tmp.insert(0, this->ft_get_end_header());
			tmp.insert(0, this->ft_get_content_length(buff));
			tmp.insert(0, this->ft_get_server_name());
			tmp.insert(0, this->ft_get_date());
			tmp.insert(0, this->ft_get_charset());
			tmp.insert(0, this->ft_get_content_type());

			if(this->_header_requete[0].num_error == 405)
				tmp.insert(0, this->ft_get_allow());
			tmp.insert(0, this->ft_get_status(true));
			return (tmp);
		}
		// std::cerr << strerror(errno) << std::endl;
		// exit(1);
	}
	std::cout << RED << "IMPOSSIBLE D'arriver la il y a un probleme dans la creation de l'erreur" << CLEAR << std::endl;
	return ("");
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
	if (this->_servers[0].nbr_location > 0)
	{
		//std::cout << "il y a des locations : " << this->_servers[0].nbr_location << std::endl;
		size_t i = 0;
		while ( i < this->_servers[0].nbr_location)
		{
			size_t found = this->_header_requete[0].body_error.find("/");
			if (found == std::string::npos)
			{
				std::cout << "ERREUR euh pas normal ne trouve pas / dans la requete a traiter "<< std::endl;
				exit(EXIT_FAILURE);
			}
			else
			{
				std::string tmp_name = this->_servers[0].location[i].name_location;
				if (tmp_name[tmp_name.size() - 1] != '/')
					tmp_name.append("/");
				if (this->_header_requete[0].body_error.find(tmp_name) != std::string::npos)
				{
					std::vector<std::string>::const_iterator	it_b = this->_servers[0].location[i].methods_location.begin();
					for (; it_b != this->_servers[0].location[i].methods_location.end(); it_b++)
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
	std::vector<std::string>::const_iterator  it_b = this->_servers[0].methods_server.begin();
	for (; it_b != this->_servers[0].methods_server.end(); it_b++)
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

// std::string		HttpServer::ft_return_error( void )
// {
// 	std::map<size_t, std::string> list_error;
// 	list_error.insert(std::pair<size_t, std::string>(400, "Bad Request"));
// 	list_error.insert(std::pair<size_t, std::string>(401, "Unauthorized (RFC 7235)"));
// 	list_error.insert(std::pair<size_t, std::string>(402, "Payment Required"));
// 	list_error.insert(std::pair<size_t, std::string>(403, "Forbidden"));
// 	list_error.insert(std::pair<size_t, std::string>(404, "Not Found"));
// 	list_error.insert(std::pair<size_t, std::string>(405, "Method Not Allowed"));
// 	list_error.insert(std::pair<size_t, std::string>(406, "Not Acceptable"));
// 	list_error.insert(std::pair<size_t, std::string>(407, "Proxy Authentication Required"));
// 	list_error.insert(std::pair<size_t, std::string>(408, "Request Timeout"));
// 	list_error.insert(std::pair<size_t, std::string>(409, "Conflict"));
// 	list_error.insert(std::pair<size_t, std::string>(410, "Gone"));
// 	list_error.insert(std::pair<size_t, std::string>(411, "Length Required"));
// 	list_error.insert(std::pair<size_t, std::string>(412, "Precondition Failed"));
// 	list_error.insert(std::pair<size_t, std::string>(413, "Payload Too Large"));
// 	list_error.insert(std::pair<size_t, std::string>(414, "URI Too Long"));
// 	list_error.insert(std::pair<size_t, std::string>(415, "Unsupported Media Type"));
// 	list_error.insert(std::pair<size_t, std::string>(416, "Range Not Satisfiable"));
// 	list_error.insert(std::pair<size_t, std::string>(417, "Expectation Failed"));
// 	list_error.insert(std::pair<size_t, std::string>(421, "Misdirected Request"));
// 	list_error.insert(std::pair<size_t, std::string>(422, "Unprocessable Entity"));
// 	list_error.insert(std::pair<size_t, std::string>(423, "Locked"));
// 	list_error.insert(std::pair<size_t, std::string>(424, "Failed Dependency"));
// 	list_error.insert(std::pair<size_t, std::string>(425, "Too Early"));
// 	list_error.insert(std::pair<size_t, std::string>(426, "Upgrade Required"));
// 	list_error.insert(std::pair<size_t, std::string>(428, "Precondition Required"));
// 	list_error.insert(std::pair<size_t, std::string>(429, "Too Many Requests"));
// 	list_error.insert(std::pair<size_t, std::string>(431, "Request Header Fields Too Large"));
// 	list_error.insert(std::pair<size_t, std::string>(451, "Unavailable For Legal Reasons"));
// 	list_error.insert(std::pair<size_t, std::string>(500, "Internal Server Error"));
// 	list_error.insert(std::pair<size_t, std::string>(501, "Not Implemented"));
// 	list_error.insert(std::pair<size_t, std::string>(502, "Bad Gateway"));
// 	list_error.insert(std::pair<size_t, std::string>(503, "Service Unavailable"));
// 	list_error.insert(std::pair<size_t, std::string>(504, "Gateway Timeout"));
// 	list_error.insert(std::pair<size_t, std::string>(505, "HTTP Version Not Supported"));
// 	list_error.insert(std::pair<size_t, std::string>(506, "Variant Also Negotiates"));
// 	list_error.insert(std::pair<size_t, std::string>(507, "Insufficient Storage"));
// 	list_error.insert(std::pair<size_t, std::string>(508, "Loop Detected"));
// 	list_error.insert(std::pair<size_t, std::string>(510, "Not Extended"));
// 	list_error.insert(std::pair<size_t, std::string>(511, "Network Authentification Required"));

// 	std::map<size_t, std::string>::iterator it_b = list_error.begin();
// 	std::string tmp;
// 	for (; it_b != list_error.end(); it_b++)
// 	{
// 		if (it_b->first == this->_header_requete[0].num_error)
// 		{
// 			tmp.insert(0, it_b->second);
// 			tmp.insert(0, " ");
// 			std::stringstream ss;
// 			std::string tmp_num;
// 			ss << it_b->first;
// 			ss >> tmp_num;
// 			tmp.insert(0, tmp_num);
// 			return (tmp);
// 		}
// 	}
// 	return (tmp);

// }

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
	else
		return ("HTTP/1.1 200 OK\r\n");
	return ("HTTP/1.1 200 OK\r\n");
}


std::string		HttpServer::ft_get_server_name( void ) const
{
	// a changer en fonctin du server qu'on utilise KEK...
	std::string		tmp;

	// std::cout << " je dois prendre ca = " << this->_servers[0].name_server << std::endl;
	tmp.insert(0, "\r\n");
	tmp.insert(0, this->_servers[0].name_server);
	tmp.insert(0, "Server: ");
	return (tmp);
}


std::string		HttpServer::ft_get_charset( void ) const
{
	return ("charset=UTF-8\r\n");
}

std::string		HttpServer::ft_get_content_type( void ) const
{
	// peut etre a changer en fonction de ce qu'on affiche ? 
	return ("Content-Type: text/html; ");
}

std::string		HttpServer::ft_get_content_length( struct stat buff ) const
{
	std::string tmp_size;
	std::stringstream ss_tmp;

	std::cout << "dans get content lengtjh size = " << buff.st_size << std::endl;
	sleep(1);
	ss_tmp << buff.st_size + 100;
	ss_tmp >> tmp_size;

	tmp_size.insert(0, "Content-Length: ");
	return (tmp_size);
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