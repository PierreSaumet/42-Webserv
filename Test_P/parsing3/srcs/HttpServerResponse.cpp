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

	//							 on verifie le path
	std::cout << "on doit verifier le path ..." << std::endl;

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

	std::string the_header;


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

std::string		HttpServer::ft_get_end_header( void ) const
{
	return ("\r\n\r\n");
}

std::string		HttpServer::ft_get_status( void ) const 
{
	// a changer lol

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

	ss_tmp << buff.st_size + 100;
	ss_tmp >> tmp_size;

	tmp_size.insert(0, "Content-Length: ");
	return (tmp_size);
}