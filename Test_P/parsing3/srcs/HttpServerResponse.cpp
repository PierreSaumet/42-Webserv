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
	std::cout << "on doit verifier le path ..." << std::endl;

	if (this->_header_requete[0].error == true)
	{
		// une erreur a ete setup donc on doit deja avoir une reposne a display;
		std::cout << "GO SETUP HEADER POUR ERROR " << std::endl;
		std::cout << "le body c'est quoi deja = " << this->_header_requete[0].body_error << std::endl;
		if (this->_header_requete[0].body_error.empty() == false)
		{
			// fonction qui va chercher l'erreur dans les differents dossiers
			// si ne trouve pas alors renvoie juste le header
			the_header = ft_find_error_html( );
			return (the_header);
		}
		else
		{
			std::cout << "putain le header requet ebody erro n'est est pas vide " << std::endl;
			return (NULL);
		}
		return (NULL);
		// std::string the_header;
		the_header.insert(0, this->ft_get_end_header());

		// struct stat buff2;
		// if (stat()) 

		// the_header.insert(0, this->ft_get_content_length(buff2));
		the_header.insert(0, this->ft_get_server_name());
		the_header.insert(0, this->ft_get_charset());
		the_header.insert(0, this->ft_get_content_type());
		the_header.insert(0, this->ft_get_status());
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
	
	struct stat buff2;
	if (stat(this->_header_requete[0].path.c_str(), &buff2) < 0)
	{
		std::cout << "Error, la page demande n'exite pas. ici" << std::endl;
		return (NULL);
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

	// std::string the_header;


	the_header.insert(0, this->ft_get_end_header());
	the_header.insert(0, this->ft_get_content_length(buff2));
	the_header.insert(0, this->ft_get_server_name());
	the_header.insert(0, this->ft_get_charset());
	the_header.insert(0, this->ft_get_content_type());
	the_header.insert(0, this->ft_get_status());
	return (the_header);

	// // on verifier si le path est l'index
	// if (this->_header_requete[0].path == "/ ")
	// {
	// 	std::cout << "display index" << std::endl;	
	// 	struct stat buff;
	// 	if (stat(filename.c_str(), &buff) < 0)
	// 	{
	// 		std::cout << "Error, la page demande n'exite pas." << std::endl;
	// 		return (NULL);
	// 	}
	// 	if (buff.st_size == 0)
	// 	{
	// 		std::cout << "Error, la page demande est vide. " << std::endl;
	// 		return (NULL);
	// 	}
	// 	input_file = fopen(filename.c_str(), "r");
	// 	if (input_file == NULL)
	// 	{
	// 		std::cout << "Error, pour ouvrir la page demande avec fopen." << std::endl;
	// 		return (NULL);
	// 	}

	// 	// stat(filename.c_str(), &sb);
	// 	res.resize(buff.st_size + 100);
	// 	std::cout << "taille buffer = " << buff.st_size << std::endl;
		
	// 	fread(const_cast<char*>(res.data()), buff.st_size, 1, input_file);

	// 	fclose(input_file);
	// 	file_contents = res;
	// 	std::cout << "la taille putain = " << file_contents.size() << std::endl;

	// 	// file_contents.insert(0, file_contents.c_str());
	// 	file_contents.insert(0, this->ft_get_end_header());
	// 	file_contents.insert(0, this->ft_get_content_length(buff));
	// 	file_contents.insert(0, this->ft_get_server_name());
	// 	file_contents.insert(0, this->ft_get_charset());
	// 	file_contents.insert(0, this->ft_get_content_type());
	// 	file_contents.insert(0, this->ft_get_status());
		
		
	// 	// std::cout << "KEK = " << ft_get_server_name() << std::endl;
	// 	std::cout << "\nfile contents = " << file_contents << std::endl;
	// 	sleep(1);
	// }
	// return (NULL);
}

std::string		HttpServer::ft_find_error_html( void )
{
	// (void)request_html;
	// (void)len_msg;

	std::stringstream 	ss;
	std::string			path_error;
	// path_error.insert(0, ".html");
	ss << this->_header_requete[0].num_error;
	ss >> path_error;
	path_error.resize(4);
	path_error.insert(4, ".html");
	path_error.insert(0, this->_header_requete[0].body_error);
	std::cout << "path_error = " << path_error << std::endl;

	std::string the_header;

	struct stat buff;
	if (stat(path_error.c_str(), &buff) < 0)
	{
		// le fichier n'a pas ete trouve
		// on setup le juste le header avec le bon numero d'erreur
		std::cout << "size buff = " << buff.st_size << std::endl;
		buff.st_size = 0;

		
		std::string tmp = ft_create_fake_error();
		return (tmp); // test
		buff.st_size = tmp.length();
		std::cout << "buff st size = " << buff.st_size << std::endl;
		sleep(1);
		the_header = tmp;
		the_header.insert(0, this->ft_get_end_header());
		the_header.insert(0, this->ft_get_content_length(buff));
		the_header.insert(0, this->ft_get_server_name());
		the_header.insert(0, this->ft_get_charset());
		the_header.insert(0, this->ft_get_content_type());
		the_header.insert(0, this->ft_get_status());
		std::cout << "\n\nDU COUP LE HEADER DE L:ERREUR =" << the_header << std::endl;
		return (the_header);
	}
	// struct stat buff2;
	// if (stat(this->_header_requete[0].path.c_str(), &buff2) < 0)
	// {
	// 	std::cout << "Error, la page demande n'exite pas. ici" << std::endl;
	// 	return (NULL);
	// }
	// if (buff2.st_size == 0)
	// {
	// 	std::cout << "Error, la page demande est vide. " << std::endl;
	// 	return (NULL);
	// }

	// input_file = fopen(this->_header_requete[0].path.c_str(), "r");
	// if (input_file == NULL)
	// {
	// 	std::cout << "Error, pour ouvrir la page demande avec fopen." << std::endl;
	// 	return (NULL);
	// }
	// fclose(input_file);

	std::cout << "kek pas cool problem find error_page" << std::endl;
	return (NULL);



}

std::string		HttpServer::ft_create_fake_error( void )
{
	std::string the_header = "</h1></body></html>\r\n";			// c'est la find
																// faut mettre le message de l'erreur et le numero

	the_header.insert(0, ft_return_error());
	the_header.insert(0, "<!DOCTYPE html><html><head><title>WebServ Error</title><style>body { background-color: #111 }h1 { font-size:4cm; text-align: center; color: black; text-shadow: 0 0 2mm red}</style></head><body><h1>");
	
	std::string tmp;
	// tmp.insert(0, "<!DOCTYPE html><html><head><title>Hello World</title></head><body><h1>TEST Webserv</h1></body></html>\r\n");

	// tmp = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n<!DOCTYPE html><html><head><title>Hello World</title><style>body { background-color: #111 }h1 { font-size:4cm; text-align: center; color: black; text-shadow: 0 0 2mm red}</style></head><body><h1>TEST Webserv</h1></body></html>\r\n";
	
	
	tmp = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n<!DOCTYPE html><html><head><title>GET Query String</title><style>body { background-color: rgb(105, 140, 238) }h1 { font-size:2cm; text-align: center; color: rgb(121, 219, 56);text-shadow: 0 0 2mm red}</style></head><body><h1> Im'gonne say hello to you using the query string in the URL ! </h1><h2><?php echo 'Hello ' . htmlspecialchars($_GET['name']) . '!';?></h2></body></html>\r\n";
	
	return (tmp);
}


std::string		HttpServer::ft_get_end_header( void ) const
{
	return ("\r\n\r\n");
}

std::string		HttpServer::ft_return_error( void )
{
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

	std::map<size_t, std::string>::iterator it_b = list_error.begin();
	std::string tmp;
	for (; it_b != list_error.end(); it_b++)
	{
		if (it_b->first == this->_header_requete[0].num_error)
		{
			tmp.insert(0, it_b->second);
			tmp.insert(0, " ");
			std::stringstream ss;
			std::string tmp_num;
			ss << it_b->first;
			ss >> tmp_num;
			tmp.insert(0, tmp_num);
			return (tmp);
		}
	}
	return (tmp);

}

std::string		HttpServer::ft_get_status( void ) const 
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
				status_str.insert(0, "HTTP/1.1 ");
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
	return ("Charset=UTF-8\r\n");
}

std::string		HttpServer::ft_get_content_type( void ) const
{
	// peut etre a changer en fonction de ce qu'on affiche ? 
	return ("Content-Type: text/html\r\n");
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